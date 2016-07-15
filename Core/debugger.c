#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "debugger.h"
#include "memory.h"
#include "z80_cpu.h"
#include "gb.h"


typedef struct {
    enum {
        LVALUE_MEMORY,
        LVALUE_REG16,
        LVALUE_REG_H,
        LVALUE_REG_L,
    } kind;
    union {
        uint16_t *register_address;
        uint16_t memory_address;
    };
} lvalue_t;

typedef struct {
    bool has_bank;
    uint16_t bank:9;
    uint16_t value;
} value_t;

#define VALUE_16(x) ((value_t){false, 0, (x)})

struct GB_breakpoint_s {
    union {
        struct {
        uint16_t addr;
        uint16_t bank; /* -1 = any bank*/
        };
        uint32_t key; /* For sorting and comparing */
    };
    char *condition;
};

#define BP_KEY(x) (((struct GB_breakpoint_s){.addr = ((x).value), .bank = (x).has_bank? (x).bank : -1 }).key)

#define GB_WATCHPOINT_R (1)
#define GB_WATCHPOINT_W (2)

struct GB_watchpoint_s {
    union {
        struct {
            uint16_t addr;
            uint16_t bank; /* -1 = any bank*/
        };
        uint32_t key; /* For sorting and comparing */
    };
    char *condition;
    uint8_t flags;
};

#define WP_KEY(x) (((struct GB_watchpoint_s){.addr = ((x).value), .bank = (x).has_bank? (x).bank : -1 }).key)

static uint16_t bank_for_addr(GB_gameboy_t *gb, uint16_t addr)
{
    if (addr < 0x4000) {
        return gb->mbc_rom0_bank;
    }

    if (addr < 0x8000) {
        return gb->mbc_rom_bank;
    }

    if (addr < 0xD000) {
        return 0;
    }

    if (addr < 0xE000) {
        return gb->cgb_ram_bank;
    }

    return 0;
}

static const char *value_to_string(GB_gameboy_t *gb, uint16_t value, bool prefer_name)
{
    static __thread char output[256];
    const GB_bank_symbol_t *symbol = GB_debugger_find_symbol(gb, value);

    if (symbol && (value - symbol->addr > 0x1000 || symbol->addr == 0) ) {
        symbol = NULL;
    }

    /* Avoid overflow */
    if (symbol && strlen(symbol->name) > 240) {
        symbol = NULL;
    }

    if (!symbol) {
        sprintf(output, "$%04x", value);
    }

    else if (symbol->addr == value) {
        if (prefer_name) {
            sprintf(output, "%s ($%04x)", symbol->name, value);
        }
        else {
            sprintf(output, "$%04x (%s)", value, symbol->name);
        }
    }

    else {
        if (prefer_name) {
            sprintf(output, "%s+$%03x ($%04x)", symbol->name, value - symbol->addr, value);
        }
        else {
            sprintf(output, "$%04x (%s+$%03x)", value, symbol->name, value - symbol->addr);
        }
    }
    return output;
}

static const char *debugger_value_to_string(GB_gameboy_t *gb, value_t value, bool prefer_name)
{
    if (!value.has_bank) return value_to_string(gb, value.value, prefer_name);

    static __thread char output[256];
    const GB_bank_symbol_t *symbol = GB_map_find_symbol(gb->bank_symbols[value.bank], value.value);

    if (symbol && (value.value - symbol->addr > 0x1000 || symbol->addr == 0) ) {
        symbol = NULL;
    }

    /* Avoid overflow */
    if (symbol && strlen(symbol->name) > 240) {
        symbol = NULL;
    }

    if (!symbol) {
        sprintf(output, "$%02x:$%04x", value.bank, value.value);
    }

    else if (symbol->addr == value.value) {
        if (prefer_name) {
            sprintf(output, "%s ($%02x:$%04x)", symbol->name, value.bank, value.value);
        }
        else {
            sprintf(output, "$%02x:$%04x (%s)", value.bank, value.value, symbol->name);
        }
    }

    else {
        if (prefer_name) {
            sprintf(output, "%s+$%03x ($%02x:$%04x)", symbol->name, value.value - symbol->addr, value.bank, value.value);
        }
        else {
            sprintf(output, "$%02x:$%04x (%s+$%03x)", value.bank, value.value, symbol->name, value.value - symbol->addr);
        }
    }
    return output;
}

static value_t read_lvalue(GB_gameboy_t *gb, lvalue_t lvalue)
{
    /* Not used until we add support for operators like += */
    switch (lvalue.kind) {
        case LVALUE_MEMORY:
            return VALUE_16(GB_read_memory(gb, lvalue.memory_address));

        case LVALUE_REG16:
            return VALUE_16(*lvalue.register_address);

        case LVALUE_REG_L:
            return VALUE_16(*lvalue.register_address & 0x00FF);

        case LVALUE_REG_H:
            return VALUE_16(*lvalue.register_address >> 8);
    }
}

static void write_lvalue(GB_gameboy_t *gb, lvalue_t lvalue, uint16_t value)
{
    switch (lvalue.kind) {
        case LVALUE_MEMORY:
            GB_write_memory(gb, lvalue.memory_address, value);
            return;

        case LVALUE_REG16:
            *lvalue.register_address = value;
            return;

        case LVALUE_REG_L:
            *lvalue.register_address &= 0xFF00;
            *lvalue.register_address |= value & 0xFF;
            return;

        case LVALUE_REG_H:
            *lvalue.register_address &= 0x00FF;
            *lvalue.register_address |= value << 8;
            return;
    }
}

/* 16 bit value   <op> 16 bit value   = 16 bit value
   25 bit address <op> 16 bit value   = 25 bit address
   16 bit value   <op> 25 bit address = 25 bit address
   25 bit address <op> 25 bit address = 16 bit value (since adding pointers, for examples, makes no sense)
 
   Boolean operators always return a 16-bit value
   */
#define FIX_BANK(x) ((value_t){a.has_bank ^ b.has_bank, a.has_bank? a.bank : b.bank, (x)})

static value_t add(value_t a, value_t b) {return FIX_BANK(a.value + b.value);}
static value_t sub(value_t a, value_t b) {return FIX_BANK(a.value - b.value);}
static value_t mul(value_t a, value_t b) {return FIX_BANK(a.value * b.value);}
static value_t _div(value_t a, value_t b) {
    if (b.value == 0) {
        return FIX_BANK(0);
    }
    return FIX_BANK(a.value / b.value);
};
static value_t mod(value_t a, value_t b) {
    if (b.value == 0) {
        return FIX_BANK(0);
    }
    return FIX_BANK(a.value % b.value);
};
static value_t and(value_t a, value_t b) {return FIX_BANK(a.value & b.value);}
static value_t or(value_t a, value_t b) {return FIX_BANK(a.value | b.value);}
static value_t xor(value_t a, value_t b) {return FIX_BANK(a.value ^ b.value);}
static value_t shleft(value_t a, value_t b) {return FIX_BANK(a.value << b.value);}
static value_t shright(value_t a, value_t b) {return FIX_BANK(a.value >> b.value);}
static value_t assign(GB_gameboy_t *gb, lvalue_t a, uint16_t b)
{
    write_lvalue(gb, a, b);
    return read_lvalue(gb, a);
}

static value_t bool_and(value_t a, value_t b) {return VALUE_16(a.value && b.value);}
static value_t bool_or(value_t a, value_t b) {return VALUE_16(a.value || b.value);}
static value_t equals(value_t a, value_t b) {return VALUE_16(a.value == b.value);}
static value_t different(value_t a, value_t b) {return VALUE_16(a.value != b.value);}
static value_t lower(value_t a, value_t b) {return VALUE_16(a.value < b.value);}
static value_t greater(value_t a, value_t b) {return VALUE_16(a.value > b.value);}
static value_t lower_equals(value_t a, value_t b) {return VALUE_16(a.value <= b.value);}
static value_t greater_equals(value_t a, value_t b) {return VALUE_16(a.value >= b.value);}
static value_t bank(value_t a, value_t b) {return (value_t) {true, a.value, b.value};}


static struct {
    const char *string;
    char priority;
    value_t (*operator)(value_t, value_t);
    value_t (*lvalue_operator)(GB_gameboy_t *, lvalue_t, uint16_t);
} operators[] =
{
    // Yes. This is not C-like. But it makes much more sense.
    // Deal with it.
    {"+", 0, add},
    {"-", 0, sub},
    {"||", 0, bool_or},
    {"|", 0, or},
    {"*", 1, mul},
    {"/", 1, _div},
    {"%", 1, mod},
    {"&&", 1, bool_and},
    {"&", 1, and},
    {"^", 1, xor},
    {"<<", 2, shleft},
    {"<=", 3, lower_equals},
    {"<", 3, lower},
    {">>", 2, shright},
    {">=", 3, greater_equals},
    {">", 3, greater},
    {"==", 3, equals},
    {"=", -1, NULL, assign},
    {"!=", 3, different},
    {":", 4, bank},
};

value_t debugger_evaluate(GB_gameboy_t *gb, const char *string,
                           unsigned int length, bool *error,
                           uint16_t *watchpoint_address, uint8_t *watchpoint_new_value);

static lvalue_t debugger_evaluate_lvalue(GB_gameboy_t *gb, const char *string,
                                         unsigned int length, bool *error,
                                         uint16_t *watchpoint_address, uint8_t *watchpoint_new_value)
{
    *error = false;
    // Strip whitespace
    while (length && (string[0] == ' ' || string[0] == '\n' || string[0] == '\r' || string[0] == '\t')) {
        string++;
        length--;
    }
    while (length && (string[length-1] == ' ' || string[length-1] == '\n' || string[length-1] == '\r' || string[length-1] == '\t')) {
        length--;
    }
    if (length == 0)
    {
        GB_log(gb, "Expected expression.\n");
        *error = true;
        return (lvalue_t){0,};
    }
    if (string[0] == '(' && string[length - 1] == ')') {
        // Attempt to strip parentheses
        signed int depth = 0;
        for (int i = 0; i < length; i++) {
            if (string[i] == '(') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == ')') depth--;
        }
        if (depth == 0) return debugger_evaluate_lvalue(gb, string + 1, length - 2, error, watchpoint_address, watchpoint_new_value);
    }
    else if (string[0] == '[' && string[length - 1] == ']') {
        // Attempt to strip square parentheses (memory dereference)
        signed int depth = 0;
        for (int i = 0; i < length; i++) {
            if (string[i] == '[') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == ']') depth--;
        }
        if (depth == 0) {
            /* Todo: Warn the user when dereferencing a specific bank, but it's not mapped */
            return (lvalue_t){LVALUE_MEMORY, .memory_address = debugger_evaluate(gb, string + 1, length - 2, error, watchpoint_address, watchpoint_new_value).value};
        }
    }

    // Registers
    if (string[0] != '$' && (string[0] < '0' || string[0] > '9')) {
        if (length == 1) {
            switch (string[0]) {
                case 'a': return (lvalue_t){LVALUE_REG_H, .register_address = &gb->registers[GB_REGISTER_AF]};
                case 'f': return (lvalue_t){LVALUE_REG_L, .register_address = &gb->registers[GB_REGISTER_AF]};
                case 'b': return (lvalue_t){LVALUE_REG_H, .register_address = &gb->registers[GB_REGISTER_BC]};
                case 'c': return (lvalue_t){LVALUE_REG_L, .register_address = &gb->registers[GB_REGISTER_BC]};
                case 'd': return (lvalue_t){LVALUE_REG_H, .register_address = &gb->registers[GB_REGISTER_DE]};
                case 'e': return (lvalue_t){LVALUE_REG_L, .register_address = &gb->registers[GB_REGISTER_DE]};
                case 'h': return (lvalue_t){LVALUE_REG_H, .register_address = &gb->registers[GB_REGISTER_HL]};
                case 'l': return (lvalue_t){LVALUE_REG_L, .register_address = &gb->registers[GB_REGISTER_HL]};
            }
        }
        else if (length == 2) {
            switch (string[0]) {
                case 'a': if (string[1] == 'f') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_AF]};
                case 'b': if (string[1] == 'c') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_BC]};
                case 'd': if (string[1] == 'e') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_DE]};
                case 'h': if (string[1] == 'l') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_HL]};
                case 's': if (string[1] == 'p') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_SP]};
                case 'p': if (string[1] == 'c') return (lvalue_t){LVALUE_REG16, .register_address = &gb->pc};
            }
        }
        GB_log(gb, "Unknown register: %.*s\n", length, string);
        *error = true;
        return (lvalue_t){0,};
    }

    GB_log(gb, "Expression is not an lvalue: %.*s\n", length, string);
    *error = true;
    return (lvalue_t){0,};
}

#define ERROR ((value_t){0,})
value_t debugger_evaluate(GB_gameboy_t *gb, const char *string,
                          unsigned int length, bool *error,
                          uint16_t *watchpoint_address, uint8_t *watchpoint_new_value)
{
    *error = false;
    // Strip whitespace
    while (length && (string[0] == ' ' || string[0] == '\n' || string[0] == '\r' || string[0] == '\t')) {
        string++;
        length--;
    }
    while (length && (string[length-1] == ' ' || string[length-1] == '\n' || string[length-1] == '\r' || string[length-1] == '\t')) {
        length--;
    }
    if (length == 0)
    {
        GB_log(gb, "Expected expression.\n");
        *error = true;
        return ERROR;
    }
    if (string[0] == '(' && string[length - 1] == ')') {
        // Attempt to strip parentheses
        signed int depth = 0;
        for (int i = 0; i < length; i++) {
            if (string[i] == '(') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == ')') depth--;
        }
        if (depth == 0) return debugger_evaluate(gb, string + 1, length - 2, error, watchpoint_address, watchpoint_new_value);
    }
    else if (string[0] == '[' && string[length - 1] == ']') {
        // Attempt to strip square parentheses (memory dereference)
        signed int depth = 0;
        for (int i = 0; i < length; i++) {
            if (string[i] == '[') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == ']') depth--;
        }
        /* Todo: Warn the user when dereferencing a specific bank, but it's not mapped */
        if (depth == 0)
            return VALUE_16(
                       GB_read_memory(gb,
                           debugger_evaluate(gb, string + 1, length - 2, error, watchpoint_address, watchpoint_new_value).value
                       )
                   );
    }
    // Search for lowest priority operator
    signed int depth = 0;
    unsigned int operator_index = -1;
    unsigned int operator_pos = 0;
    for (int i = 0; i < length; i++) {
        if (string[i] == '(') depth++;
        else if (string[i] == ')') depth--;
        else if (string[i] == '[') depth++;
        else if (string[i] == ']') depth--;
        else if (depth == 0) {
            for (int j = 0; j < sizeof(operators) / sizeof(operators[0]); j++) {
                if (strlen(operators[j].string) > length - i) continue; // Operator too big.
                 // Priority higher than what we already have.
                if (operator_index != -1 && operators[operator_index].priority < operators[j].priority) continue;
                unsigned long operator_length = strlen(operators[j].string);
                if (memcmp(string + i, operators[j].string, operator_length) == 0) {
                    // Found an operator!
                    operator_pos = i;
                    operator_index = j;
                    /* for supporting = vs ==, etc*/
                    i += operator_length - 1;
                    break;
                }
            }
        }
    }
    if (operator_index != -1) {
        unsigned int right_start = (unsigned int)(operator_pos + strlen(operators[operator_index].string));
        value_t right = debugger_evaluate(gb, string + right_start, length - right_start, error, watchpoint_address, watchpoint_new_value);
        if (*error) return ERROR;
        if (operators[operator_index].lvalue_operator) {
            lvalue_t left = debugger_evaluate_lvalue(gb, string, operator_pos, error, watchpoint_address, watchpoint_new_value);
            if (*error) return ERROR;
            return operators[operator_index].lvalue_operator(gb, left, right.value);
        }
        value_t left = debugger_evaluate(gb, string, operator_pos, error, watchpoint_address, watchpoint_new_value);
        if (*error) return ERROR;
        return operators[operator_index].operator(left, right);
    }

    // Not an expression - must be a register or a literal

    // Registers
    if (string[0] != '$' && (string[0] < '0' || string[0] > '9')) {
        if (length == 1) {
            switch (string[0]) {
                case 'a': return VALUE_16(gb->registers[GB_REGISTER_AF] >> 8);
                case 'f': return VALUE_16(gb->registers[GB_REGISTER_AF] & 0xFF);
                case 'b': return VALUE_16(gb->registers[GB_REGISTER_BC] >> 8);
                case 'c': return VALUE_16(gb->registers[GB_REGISTER_BC] & 0xFF);
                case 'd': return VALUE_16(gb->registers[GB_REGISTER_DE] >> 8);
                case 'e': return VALUE_16(gb->registers[GB_REGISTER_DE] & 0xFF);
                case 'h': return VALUE_16(gb->registers[GB_REGISTER_HL] >> 8);
                case 'l': return VALUE_16(gb->registers[GB_REGISTER_HL] & 0xFF);
            }
        }
        else if (length == 2) {
            switch (string[0]) {
                case 'a': if (string[1] == 'f') return VALUE_16(gb->registers[GB_REGISTER_AF]);
                case 'b': if (string[1] == 'c') return VALUE_16(gb->registers[GB_REGISTER_BC]);
                case 'd': if (string[1] == 'e') return VALUE_16(gb->registers[GB_REGISTER_DE]);
                case 'h': if (string[1] == 'l') return VALUE_16(gb->registers[GB_REGISTER_HL]);
                case 's': if (string[1] == 'p') return VALUE_16(gb->registers[GB_REGISTER_SP]);
                case 'p': if (string[1] == 'c') return (value_t){true, bank_for_addr(gb, gb->pc), gb->pc};
            }
        }
        else if (length == 3) {
            if (watchpoint_address && memcmp(string, "old", 3) == 0) {
                return VALUE_16(GB_read_memory(gb, *watchpoint_address));
            }

            if (watchpoint_new_value && memcmp(string, "new", 3) == 0) {
                return VALUE_16(*watchpoint_new_value);
            }

            /* $new is identical to $old in read conditions */
            if (watchpoint_address && memcmp(string, "new", 3) == 0) {
                return VALUE_16(GB_read_memory(gb, *watchpoint_address));
            }
        }

        char symbol_name[length + 1];
        memcpy(symbol_name, string, length);
        symbol_name[length] = 0;
        const GB_symbol_t *symbol = GB_reversed_map_find_symbol(&gb->reversed_symbol_map, symbol_name);
        if (symbol) {
            return (value_t){true, symbol->bank, symbol->addr};
        }

        GB_log(gb, "Unknown register or symbol: %.*s\n", length, string);
        *error = true;
        return ERROR;
    }

    char *end;
    int base = 10;
    if (string[0] == '$') {
        string++;
        base = 16;
        length--;
    }
    uint16_t literal = (uint16_t) (strtol(string, &end, base));
    if (end != string + length) {
        GB_log(gb, "Failed to parse: %.*s\n", length, string);
        *error = true;
        return ERROR;
    }
    return VALUE_16(literal);
}

typedef bool debugger_command_imp_t(GB_gameboy_t *gb, char *arguments);

typedef struct {
    const char *command;
    uint8_t min_length;
    debugger_command_imp_t *implementation;
    const char *help_string; // Null if should not appear in help
} debugger_command_t;

static const char *lstrip(const char *str)
{
    while (*str == ' ' || *str == '\t') {
        str++;
    }
    return str;
}

static bool cont(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        GB_log(gb, "Usage: continue\n");
        return true;
    }
    gb->debug_stopped = false;
    return false;
}

static bool next(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        GB_log(gb, "Usage: next\n");
        return true;
    }
    
    gb->debug_stopped = false;
    gb->debug_next_command = true;
    gb->debug_call_depth = 0;
    return false;
}

static bool step(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        GB_log(gb, "Usage: step\n");
        return true;
    }

    return false;
}

static bool finish(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        GB_log(gb, "Usage: finish\n");
        return true;
    }

    gb->debug_stopped = false;
    gb->debug_fin_command = true;
    gb->debug_call_depth = 0;
    return false;
}

static bool stack_leak_detection(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        GB_log(gb, "Usage: sld\n");
        return true;
    }

    gb->debug_stopped = false;
    gb->stack_leak_detection = true;
    gb->debug_call_depth = 0;
    return false;
}

static bool registers(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        GB_log(gb, "Usage: registers\n");
        return true;
    }

    GB_log(gb, "AF = $%04x\n", gb->registers[GB_REGISTER_AF]); /* AF can't really be an address */
    GB_log(gb, "BC = %s\n", value_to_string(gb, gb->registers[GB_REGISTER_BC], false));
    GB_log(gb, "DE = %s\n", value_to_string(gb, gb->registers[GB_REGISTER_DE], false));
    GB_log(gb, "HL = %s\n", value_to_string(gb, gb->registers[GB_REGISTER_HL], false));
    GB_log(gb, "SP = %s\n", value_to_string(gb, gb->registers[GB_REGISTER_SP], false));
    GB_log(gb, "PC = %s\n", value_to_string(gb, gb->pc, false));

    GB_log(gb, "TIMA = %d/%u\n", gb->io_registers[GB_IO_TIMA], gb->tima_cycles);
    GB_log(gb, "Display Controller: LY = %d/%u\n", gb->io_registers[GB_IO_LY], gb->display_cycles % 456);
    return true;
}

/* Find the index of the closest breakpoint equal or greater to addr */
static uint16_t find_breakpoint(GB_gameboy_t *gb, value_t addr)
{
    if (!gb->breakpoints) {
        return 0;
    }

    uint32_t key = BP_KEY(addr);

    int min = 0;
    int max = gb->n_breakpoints;
    while (min < max) {
        uint16_t pivot = (min + max) / 2;
        if (gb->breakpoints[pivot].key == key) return pivot;
        if (gb->breakpoints[pivot].key > key) {
            max = pivot;
        }
        else {
            min = pivot + 1;
        }
    }
    return (uint16_t) min;
}

static bool breakpoint(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        GB_log(gb, "Usage: breakpoint <expression>[ if <condition expression>]\n");
        return true;
    }

    if (gb->n_breakpoints == (typeof(gb->n_breakpoints)) -1) {
        GB_log(gb, "Too many breakpoints set\n");
        return true;
    }

    char *condition = NULL;
    if ((condition = strstr(arguments, " if "))) {
        *condition = 0;
        condition += strlen(" if ");
        /* Verify condition is sane (Todo: This might have side effects!) */
        bool error;
        debugger_evaluate(gb, condition, (unsigned int)strlen(condition), &error, NULL, NULL);
        if (error) return true;

    }

    bool error;
    value_t result = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error, NULL, NULL);
    uint32_t key = BP_KEY(result);

    if (error) return true;

    uint16_t index = find_breakpoint(gb, result);
    if (index < gb->n_breakpoints && gb->breakpoints[index].key == key) {
        GB_log(gb, "Breakpoint already set at %s\n", debugger_value_to_string(gb, result, true));
        if (!gb->breakpoints[index].condition && condition) {
            GB_log(gb, "Added condition to breakpoint\n");
            gb->breakpoints[index].condition = strdup(condition);
        }
        else if (gb->breakpoints[index].condition && condition) {
            GB_log(gb, "Replaced breakpoint condition\n");
            free(gb->breakpoints[index].condition);
            gb->breakpoints[index].condition = strdup(condition);
        }
        else if (gb->breakpoints[index].condition && !condition) {
            GB_log(gb, "Removed breakpoint condition\n");
            free(gb->breakpoints[index].condition);
            gb->breakpoints[index].condition = NULL;
        }
        return true;
    }

    gb->breakpoints = realloc(gb->breakpoints, (gb->n_breakpoints + 1) * sizeof(gb->breakpoints[0]));
    memmove(&gb->breakpoints[index + 1], &gb->breakpoints[index], (gb->n_breakpoints - index) * sizeof(gb->breakpoints[0]));
    gb->breakpoints[index].key = key;

    if (condition) {
        gb->breakpoints[index].condition = strdup(condition);
    }
    else {
        gb->breakpoints[index].condition = NULL;
    }
    gb->n_breakpoints++;

    GB_log(gb, "Breakpoint set at %s\n", debugger_value_to_string(gb, result, true));
    return true;
}

static bool delete(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        GB_log(gb, "Delete all breakpoints? ");
        char *answer = gb->input_callback(gb);
        if (answer[0] == 'Y' || answer[0] == 'y') {
            for (unsigned i = gb->n_breakpoints; i--;) {
                if (gb->breakpoints[i].condition) {
                    free(gb->breakpoints[i].condition);
                }
            }
            free(gb->breakpoints);
            gb->breakpoints = NULL;
            gb->n_breakpoints = 0;
        }
        return true;
    }

    bool error;
    value_t result = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error, NULL, NULL);
    uint32_t key = BP_KEY(result);

    if (error) return true;

    uint16_t index = find_breakpoint(gb, result);
    if (index >= gb->n_breakpoints || gb->breakpoints[index].key != key) {
        GB_log(gb, "No breakpoint set at %s\n", debugger_value_to_string(gb, result, true));
        return true;
    }

    if (gb->breakpoints[index].condition) {
        free(gb->breakpoints[index].condition);
    }

    memmove(&gb->breakpoints[index], &gb->breakpoints[index + 1], (gb->n_breakpoints - index - 1) * sizeof(gb->breakpoints[0]));
    gb->n_breakpoints--;
    gb->breakpoints = realloc(gb->breakpoints, gb->n_breakpoints * sizeof(gb->breakpoints[0]));

    GB_log(gb, "Breakpoint removed from %s\n", debugger_value_to_string(gb, result, true));
    return true;
}

/* Find the index of the closest watchpoint equal or greater to addr */
static uint16_t find_watchpoint(GB_gameboy_t *gb, value_t addr)
{
    if (!gb->watchpoints) {
        return 0;
    }
    uint32_t key = WP_KEY(addr);
    int min = 0;
    int max = gb->n_watchpoints;
    while (min < max) {
        uint16_t pivot = (min + max) / 2;
        if (gb->watchpoints[pivot].key == key) return pivot;
        if (gb->watchpoints[pivot].key > key) {
            max = pivot;
        }
        else {
            min = pivot + 1;
        }
    }
    return (uint16_t) min;
}

static bool watch(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
print_usage:
        GB_log(gb, "Usage: watch (r|w|rw) <expression>[ if <condition expression>]\n");
        return true;
    }

    if (gb->n_watchpoints == (typeof(gb->n_watchpoints)) -1) {
        GB_log(gb, "Too many watchpoints set\n");
        return true;
    }

    uint8_t flags = 0;
    while (*arguments != ' ' && *arguments) {
        switch (*arguments) {
            case 'r':
                flags |= GB_WATCHPOINT_R;
                break;
            case 'w':
                flags |= GB_WATCHPOINT_W;
                break;
            default:
                goto print_usage;
        }
        arguments++;
    }

    if (!flags) {
        goto print_usage;
    }

    char *condition = NULL;
    if ((condition = strstr(arguments, " if "))) {
        *condition = 0;
        condition += strlen(" if ");
        /* Verify condition is sane (Todo: This might have side effects!) */
        bool error;
        /* To make $new and $old legal */
        uint16_t dummy = 0;
        uint8_t dummy2 = 0;
        debugger_evaluate(gb, condition, (unsigned int)strlen(condition), &error, &dummy, &dummy2);
        if (error) return true;

    }

    bool error;
    value_t result = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error, NULL, NULL);
    uint32_t key = WP_KEY(result);

    if (error) return true;

    uint16_t index = find_watchpoint(gb, result);
    if (index < gb->n_watchpoints && gb->watchpoints[index].key == key) {
        GB_log(gb, "Watchpoint already set at %s\n", debugger_value_to_string(gb, result, true));
        if (!gb->watchpoints[index].flags != flags) {
            GB_log(gb, "Modified watchpoint type\n");
            gb->watchpoints[index].flags = flags;
        }
        if (!gb->watchpoints[index].condition && condition) {
            GB_log(gb, "Added condition to watchpoint\n");
            gb->watchpoints[index].condition = strdup(condition);
        }
        else if (gb->watchpoints[index].condition && condition) {
            GB_log(gb, "Replaced watchpoint condition\n");
            free(gb->watchpoints[index].condition);
            gb->watchpoints[index].condition = strdup(condition);
        }
        else if (gb->watchpoints[index].condition && !condition) {
            GB_log(gb, "Removed watchpoint condition\n");
            free(gb->watchpoints[index].condition);
            gb->watchpoints[index].condition = NULL;
        }
        return true;
    }

    gb->watchpoints = realloc(gb->watchpoints, (gb->n_watchpoints + 1) * sizeof(gb->watchpoints[0]));
    memmove(&gb->watchpoints[index + 1], &gb->watchpoints[index], (gb->n_watchpoints - index) * sizeof(gb->watchpoints[0]));
    gb->watchpoints[index].key = key;
    gb->watchpoints[index].flags = flags;
    if (condition) {
        gb->watchpoints[index].condition = strdup(condition);
    }
    else {
        gb->watchpoints[index].condition = NULL;
    }
    gb->n_watchpoints++;

    GB_log(gb, "Watchpoint set at %s\n", debugger_value_to_string(gb, result, true));
    return true;
}

static bool unwatch(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        GB_log(gb, "Delete all watchpoints? ");
        char *answer = gb->input_callback(gb);
        if (answer[0] == 'Y' || answer[0] == 'y') {
            for (unsigned i = gb->n_watchpoints; i--;) {
                if (gb->watchpoints[i].condition) {
                    free(gb->watchpoints[i].condition);
                }
            }
            free(gb->watchpoints);
            gb->watchpoints = NULL;
            gb->n_watchpoints = 0;
        }
        return true;
    }

    bool error;
    value_t result = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error, NULL, NULL);
    uint32_t key = WP_KEY(result);

    if (error) return true;

    uint16_t index = find_watchpoint(gb, result);
    if (index >= gb->n_watchpoints || gb->watchpoints[index].key != key) {
        GB_log(gb, "No watchpoint set at %s\n", debugger_value_to_string(gb, result, true));
        return true;
    }

    if (gb->watchpoints[index].condition) {
        free(gb->watchpoints[index].condition);
    }

    memmove(&gb->watchpoints[index], &gb->watchpoints[index + 1], (gb->n_watchpoints - index - 1) * sizeof(gb->watchpoints[0]));
    gb->n_watchpoints--;
    gb->watchpoints = realloc(gb->watchpoints, gb->n_watchpoints* sizeof(gb->watchpoints[0]));

    GB_log(gb, "Watchpoint removed from %s\n", debugger_value_to_string(gb, result, true));
    return true;
}

static bool list(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        GB_log(gb, "Usage: list\n");
        return true;
    }

    if (gb->n_breakpoints == 0) {
        GB_log(gb, "No breakpoints set.\n");
    }
    else {
        GB_log(gb, "%d breakpoint(s) set:\n", gb->n_breakpoints);
        for (uint16_t i = 0; i < gb->n_breakpoints; i++) {
            value_t addr = (value_t){gb->breakpoints[i].bank != (uint16_t)-1, gb->breakpoints[i].bank, gb->breakpoints[i].addr};
            if (gb->breakpoints[i].condition) {
                GB_log(gb, " %d. %s (Condition: %s)\n", i + 1,
                                                        debugger_value_to_string(gb, addr, addr.has_bank),
                                                        gb->breakpoints[i].condition);
            }
            else {
                GB_log(gb, " %d. %s\n", i + 1, debugger_value_to_string(gb, addr, addr.has_bank));
            }
        }
    }

    if (gb->n_watchpoints == 0) {
        GB_log(gb, "No watchpoints set.\n");
    }
    else {
        GB_log(gb, "%d watchpoint(s) set:\n", gb->n_watchpoints);
        for (uint16_t i = 0; i < gb->n_watchpoints; i++) {
            value_t addr = (value_t){gb->watchpoints[i].bank != (uint16_t)-1, gb->watchpoints[i].bank, gb->watchpoints[i].addr};
            if (gb->watchpoints[i].condition) {
                GB_log(gb, " %d. %s (%c%c, Condition: %s)\n", i + 1, debugger_value_to_string(gb, addr, addr.has_bank),
                                                              (gb->watchpoints[i].flags & GB_WATCHPOINT_R)? 'r' : '-',
                                                              (gb->watchpoints[i].flags & GB_WATCHPOINT_W)? 'w' : '-',
                                                              gb->watchpoints[i].condition);
            }
            else {
                GB_log(gb, " %d. %s (%c%c)\n", i + 1, debugger_value_to_string(gb,addr, addr.has_bank),
                                               (gb->watchpoints[i].flags & GB_WATCHPOINT_R)? 'r' : '-',
                                               (gb->watchpoints[i].flags & GB_WATCHPOINT_W)? 'w' : '-');
            }
        }
    }

    return true;
}

static bool _should_break(GB_gameboy_t *gb, value_t addr)
{
    uint16_t index = find_breakpoint(gb, addr);
    uint32_t key = BP_KEY(addr);

    if (index < gb->n_breakpoints && gb->breakpoints[index].key == key) {
        if (!gb->breakpoints[index].condition) {
            return true;
        }
        bool error;
        bool condition = debugger_evaluate(gb, gb->breakpoints[index].condition,
                                           (unsigned int)strlen(gb->breakpoints[index].condition), &error, NULL, NULL).value;
        if (error) {
            /* Should never happen */
            GB_log(gb, "An internal error has occured\n");
            return true;
        }
        return condition;
    }
    return false;
}

static bool should_break(GB_gameboy_t *gb, uint16_t addr)
{
    /* Try any-bank breakpoint */
    value_t full_addr = (VALUE_16(addr));
    if (_should_break(gb, full_addr)) return true;

    /* Try bank-specific breakpoint */
    full_addr.has_bank = true;
    full_addr.bank = bank_for_addr(gb, addr);
    return _should_break(gb, full_addr);
}

static bool print(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        GB_log(gb, "Usage: print <expression>\n");
        return true;
    }

    bool error;
    value_t result = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error, NULL, NULL);
    if (!error) {
        GB_log(gb, "=%s\n", debugger_value_to_string(gb, result, false));
    }
    return true;
}

static bool examine(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        GB_log(gb, "Usage: examine <expression>\n");
        return true;
    }

    bool error;
    value_t addr = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error, NULL, NULL);
    if (!error) {
        if (addr.has_bank) {
            uint16_t old_rom0_bank = gb->mbc_rom0_bank;
            uint16_t old_rom_bank = gb->mbc_rom_bank;
            uint8_t old_mbc_ram_bank = gb->mbc_ram_bank;
            bool old_mbc_ram_enable = gb->mbc_ram_enable;
            uint8_t old_ram_bank = gb->cgb_ram_bank;
            uint8_t old_vram_bank = gb->cgb_vram_bank;

            gb->mbc_rom0_bank = addr.bank;
            gb->mbc_rom_bank = addr.bank;
            gb->mbc_ram_bank = addr.bank;
            gb->mbc_ram_enable = true;
            if (gb->is_cgb) {
                gb->cgb_ram_bank = addr.bank & 7;
                gb->cgb_vram_bank = addr.bank & 1;
            }

            GB_log(gb, "%02x:%04x: ", addr.bank, addr.value);
            for (int i = 0; i < 16; i++) {
                GB_log(gb, "%02x ", GB_read_memory(gb, addr.value + i));
            }
            GB_log(gb, "\n");

            gb->mbc_rom0_bank = old_rom0_bank;
            gb->mbc_rom_bank = old_rom_bank;
            gb->mbc_ram_bank = old_mbc_ram_bank;
            gb->mbc_ram_enable = old_mbc_ram_enable;
            gb->cgb_ram_bank = old_ram_bank;
            gb->cgb_vram_bank = old_vram_bank;
        }
        else {
            GB_log(gb, "%04x: ", addr.value);
            for (int i = 0; i < 16; i++) {
                GB_log(gb, "%02x ", GB_read_memory(gb, addr.value + i));
            }
            GB_log(gb, "\n");
        }
    }
    return true;
}

static bool mbc(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        GB_log(gb, "Usage: mbc\n");
        return true;
    }

    const GB_cartridge_t *cartridge = gb->cartridge_type;

    if (cartridge->has_ram) {
        GB_log(gb, "Cartrdige includes%s RAM: %x\n", cartridge->has_battery? " battery-backed": "", gb->mbc_ram_size);
    }
    else {
        GB_log(gb, "No cartridge RAM\n");
    }

    if (cartridge->mbc_type) {
        GB_log(gb, "MBC%d\n", cartridge->mbc_type);
        GB_log(gb, "Current mapped ROM bank: %x\n", gb->mbc_rom_bank);
        if (cartridge->has_ram) {
            GB_log(gb, "Current mapped RAM bank: %x\n", gb->mbc_ram_bank);
            GB_log(gb, "RAM is curently %s\n", gb->mbc_ram_enable? "enabled" : "disabled");
        }
        if (cartridge->mbc_type == GB_MBC1 && gb->mbc1_wiring == GB_STANDARD_MBC1_WIRING) {
            GB_log(gb, "MBC1 banking mode is %s\n", gb->mbc1.mode == 1 ? "RAM" : "ROM");
        }
        if (cartridge->mbc_type == GB_MBC1 && gb->mbc1_wiring == GB_MBC1M_WIRING) {
            GB_log(gb, "MBC1 uses MBC1M wiring. \n");
            GB_log(gb, "Current mapped ROM0 bank: %x\n", gb->mbc_rom0_bank);
            GB_log(gb, "MBC1 multicart banking mode is %s\n", gb->mbc1.mode == 1 ? "enabled" : "disabled");
        }

    }
    else {
        GB_log(gb, "No MBC\n");
    }

    if (cartridge->has_rumble) {
        GB_log(gb, "Cart contains a rumble pak\n");
    }

    if (cartridge->has_rtc) {
        GB_log(gb, "Cart contains a real time clock\n");
    }

    return true;
}

static bool help(GB_gameboy_t *gb, char *arguments);
static const debugger_command_t commands[] = {
    {"continue", 1, cont, "Continue running until next stop"},
    {"next", 1, next, "Run the next instruction, skipping over function calls"},
    {"step", 1, step, "Run the next instruction, stepping into function calls"},
    {"finish", 1, finish, "Run until the current function returns"},
    {"sld", 3, stack_leak_detection, "Run until the current function returns, or a stack leak is detected (Experimental)"},
    {"registers", 1, registers, "Print values of processor registers and other important registers"},
    {"cartridge", 2, mbc, "Displays information about the MBC and cartridge"},
    {"mbc", 3, mbc, NULL},
    {"breakpoint", 1, breakpoint, "Add a new breakpoint at the specified address/expression. Can also modify the condition of existing breakpoints."},
    {"delete", 2, delete, "Delete a breakpoint by its address, or all breakpoints"},
    {"watch", 1, watch, "Add a new watchpoint at the specified address/expression. Can also modify the condition and type of existing watchpoints."},
    {"unwatch", 3, unwatch, "Delete a watchpoint by its address, or all watchpoints"},
    {"list", 1, list, "List all set breakpoints and watchpoints"},
    {"print", 1, print, "Evaluate and print an expression"},
    {"eval", 2, print, NULL},
    {"examine", 2, examine, "Examine values at address"},
    {"x", 1, examine, NULL},
    {"help", 1, help, "List available commands"},
};

static bool help(GB_gameboy_t *gb, char *arguments)
{
    /* Todo: command specific help */
    const debugger_command_t *command = commands;
    for (size_t i = sizeof(commands) / sizeof(*command); i--; command++) {
        if (command->help_string) {
            GB_attributed_log(gb, GB_LOG_BOLD, "%s", command->command);
            GB_log(gb, ": %s\n", command->help_string);
        }
    }
    return true;
}

static const debugger_command_t *find_command(const char *string)
{
    const debugger_command_t *command = commands;
    size_t length = strlen(string);
    for (size_t i = sizeof(commands) / sizeof(*command); i--; command++) {
        if (command->min_length > length) continue;
        if (memcmp(command->command, string, length) == 0) { /* Is a substring? */
            return command;
        }
    }

    return NULL;
}

void GB_debugger_call_hook(GB_gameboy_t *gb)
{
    /* Called just after the CPU calls a function/enters an interrupt/etc... */

    if (gb->stack_leak_detection) {
        if (gb->debug_call_depth >= sizeof(gb->sp_for_call_depth) / sizeof(gb->sp_for_call_depth[0])) {
            GB_log(gb, "Potential stack overflow detected (Functions nest too much). \n");
            gb->debug_stopped = true;
        }
        else {
            gb->sp_for_call_depth[gb->debug_call_depth] = gb->registers[GB_REGISTER_SP];
            gb->addr_for_call_depth[gb->debug_call_depth] = gb->pc;
        }
    }

    gb->debug_call_depth++;
}

void GB_debugger_ret_hook(GB_gameboy_t *gb)
{
    /* Called just before the CPU runs ret/reti */

    gb->debug_call_depth--;

    if (gb->stack_leak_detection) {
        if (gb->debug_call_depth < 0) {
            GB_log(gb, "Function finished without a stack leak.\n");
            gb->debug_stopped = true;
        }
        else {
            if (gb->registers[GB_REGISTER_SP] != gb->sp_for_call_depth[gb->debug_call_depth]) {
                GB_log(gb, "Stack leak detected for function %s!\n", value_to_string(gb, gb->addr_for_call_depth[gb->debug_call_depth], true));
                GB_log(gb, "SP is $%04x, should be $%04x.\n", gb->registers[GB_REGISTER_SP],
                                                            gb->sp_for_call_depth[gb->debug_call_depth]);
                gb->debug_stopped = true;
            }
        }
    }
}

static bool _GB_debugger_test_write_watchpoint(GB_gameboy_t *gb, value_t addr, uint8_t value)
{
    uint16_t index = find_watchpoint(gb, addr);
    uint32_t key = WP_KEY(addr);

    if (index < gb->n_watchpoints && gb->watchpoints[index].key == key) {
        if (!(gb->watchpoints[index].flags & GB_WATCHPOINT_W)) {
            return false;
        }
        if (!gb->watchpoints[index].condition) {
            gb->debug_stopped = true;
            GB_log(gb, "Watchpoint: [%s] = $%02x\n", debugger_value_to_string(gb, addr, true), value);
            return true;
        }
        bool error;
        bool condition = debugger_evaluate(gb, gb->watchpoints[index].condition,
                                           (unsigned int)strlen(gb->watchpoints[index].condition), &error, &addr.value, &value).value;
        if (error) {
            /* Should never happen */
            GB_log(gb, "An internal error has occured\n");
            return false;
        }
        if (condition) {
            gb->debug_stopped = true;
            GB_log(gb, "Watchpoint: [%s] = $%02x\n", debugger_value_to_string(gb, addr, true), value);
            return true;
        }
    }
    return false;
}

void GB_debugger_test_write_watchpoint(GB_gameboy_t *gb, uint16_t addr, uint8_t value)
{
    if (gb->debug_stopped) return;

    /* Try any-bank breakpoint */
    value_t full_addr = (VALUE_16(addr));
    if (_GB_debugger_test_write_watchpoint(gb, full_addr, value)) return;

    /* Try bank-specific breakpoint */
    full_addr.has_bank = true;
    full_addr.bank = bank_for_addr(gb, addr);
    _GB_debugger_test_write_watchpoint(gb, full_addr, value);
}

static bool _GB_debugger_test_read_watchpoint(GB_gameboy_t *gb, value_t addr)
{
    uint16_t index = find_breakpoint(gb, addr);
    uint32_t key = WP_KEY(addr);

    if (index < gb->n_watchpoints && gb->watchpoints[index].key == key) {
        if (!(gb->watchpoints[index].flags & GB_WATCHPOINT_R)) {
            return false;
        }
        if (!gb->watchpoints[index].condition) {
            gb->debug_stopped = true;
            GB_log(gb, "Watchpoint: [%s]\n", debugger_value_to_string(gb, addr, true));
            return true;
        }
        bool error;
        bool condition = debugger_evaluate(gb, gb->watchpoints[index].condition,
                                           (unsigned int)strlen(gb->watchpoints[index].condition), &error, &addr.value, NULL).value;
        if (error) {
            /* Should never happen */
            GB_log(gb, "An internal error has occured\n");
            return false;
        }
        if (condition) {
            gb->debug_stopped = true;
            GB_log(gb, "Watchpoint: [%s]\n", debugger_value_to_string(gb, addr, true));
            return true;
        }
    }
    return false;
}

void GB_debugger_test_read_watchpoint(GB_gameboy_t *gb, uint16_t addr)
{
    if (gb->debug_stopped) return;

    /* Try any-bank breakpoint */
    value_t full_addr = (VALUE_16(addr));
    if (_GB_debugger_test_read_watchpoint(gb, full_addr)) return;

    /* Try bank-specific breakpoint */
    full_addr.has_bank = true;
    full_addr.bank = bank_for_addr(gb, addr);
    _GB_debugger_test_read_watchpoint(gb, full_addr);
}

void GB_debugger_run(GB_gameboy_t *gb)
{
    char *input = NULL;
    if (gb->debug_next_command && gb->debug_call_depth <= 0) {
        gb->debug_stopped = true;
    }
    if (gb->debug_fin_command && gb->debug_call_depth == -1) {
        gb->debug_stopped = true;
    }
    if (gb->debug_stopped) {
        GB_cpu_disassemble(gb, gb->pc, 5);
    }
next_command:
    if (input) {
        free(input);
    }
    if (!gb->debug_stopped && should_break(gb, gb->pc)) {
        gb->debug_stopped = true;
        GB_log(gb, "Breakpoint: PC = %s\n", value_to_string(gb, gb->pc, true));
        GB_cpu_disassemble(gb, gb->pc, 5);
    }
    if (gb->debug_stopped) {
        gb->debug_next_command = false;
        gb->debug_fin_command = false;
        gb->stack_leak_detection = false;
        input = gb->input_callback(gb);
        if (!input[0]) {
            goto next_command;
        }

        char *command_string = input;
        char *arguments = strchr(input, ' ');
        if (arguments) {
            /* Actually "split" the string. */
            arguments[0] = 0;
            arguments++;
        }
        else {
            arguments = "";
        }

        const debugger_command_t *command = find_command(command_string);
        if (command) {
            if (command->implementation(gb, arguments)) {
                goto next_command;
            }
        }
        else {
            GB_log(gb, "%s: no such command.\n", command_string);
            goto next_command;
        }

        /* Split to arguments and command */

        free(input);
    }
}

void GB_debugger_load_symbol_file(GB_gameboy_t *gb, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return;

    char *line = NULL;
    size_t size = 0;
    size_t length = 0;
    while ((length = getline(&line, &size, f)) != -1) {
        for (unsigned i = 0; i < length; i++) {
            if (line[i] == ';' || line[i] == '\n' || line[i] == '\r') {
                line[i] = 0;
                length = i;
                break;
            }
        }
        if (length == 0) continue;

        unsigned int bank, address;
        char symbol[length];

        if (sscanf(line, "%02x:%04x %s", &bank, &address, symbol) == 3) {
            bank &= 0x1FF;
            if (!gb->bank_symbols[bank]) {
                gb->bank_symbols[bank] = GB_map_alloc();
            }
            GB_bank_symbol_t *allocated_symbol = GB_map_add_symbol(gb->bank_symbols[bank], address, symbol);
            if (allocated_symbol) {
                GB_reversed_map_add_symbol(&gb->reversed_symbol_map, bank, allocated_symbol);
            }
        }
    }
    free(line);
    fclose(f);
}

const GB_bank_symbol_t *GB_debugger_find_symbol(GB_gameboy_t *gb, uint16_t addr)
{
    uint16_t bank = bank_for_addr(gb, addr);

    const GB_bank_symbol_t *symbol = GB_map_find_symbol(gb->bank_symbols[bank], addr);
    if (symbol) return symbol;
    if (bank != 0) return GB_map_find_symbol(gb->bank_symbols[0], addr); /* Maybe the symbol incorrectly uses bank 0? */

    return NULL;
}

const char *GB_debugger_name_for_address(GB_gameboy_t *gb, uint16_t addr)
{
    const GB_bank_symbol_t *symbol = GB_debugger_find_symbol(gb, addr);
    if (symbol && symbol->addr == addr) return symbol->name;
    return NULL;
}