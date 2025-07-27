#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gb.h"

typedef struct {
    bool has_bank;
    uint16_t bank:9;
    uint16_t value;
} value_t;

typedef struct {
    enum {
        LVALUE_MEMORY,
        LVALUE_MEMORY16,
        LVALUE_REG16,
        LVALUE_REG_H,
        LVALUE_REG_L,
    } kind;
    union {
        uint16_t *register_address;
        value_t memory_address;
    };
} lvalue_t;

#define VALUE_16(x) ((value_t){false, 0, (x)})

struct GB_breakpoint_s {
    unsigned id;
    union {
        struct {
            uint16_t addr;
            uint16_t bank; /* -1 = any bank*/
        };
        uint32_t key; /* For sorting and comparing */
    };
    char *condition;
    bool is_jump_to;
    uint16_t length;
    bool inclusive;
};

#define BP_KEY(x) (((struct GB_breakpoint_s){.addr = ((x).value), .bank = (x).has_bank? (x).bank : -1 }).key)

#define WATCHPOINT_READ (1)
#define WATCHPOINT_WRITE (2)

struct GB_watchpoint_s {
    unsigned id;
    union {
        struct {
            uint16_t addr;
            uint16_t bank; /* -1 = any bank*/
        };
        uint32_t key; /* For sorting and comparing */
    };
    char *condition;
    uint8_t flags;
    uint16_t length;
    bool inclusive;
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

typedef struct {
    uint16_t rom0_bank;
    uint16_t rom_bank;
    uint8_t mbc_ram_bank;
    bool mbc_ram_enable;
    uint8_t ram_bank;
    uint8_t vram_bank;
} banking_state_t;

static inline void save_banking_state(GB_gameboy_t *gb, banking_state_t *state)
{
    state->rom0_bank = gb->mbc_rom0_bank;
    state->rom_bank = gb->mbc_rom_bank;
    state->mbc_ram_bank = gb->mbc_ram_bank;
    state->mbc_ram_enable = gb->mbc_ram_enable;
    state->ram_bank = gb->cgb_ram_bank;
    state->vram_bank = gb->cgb_vram_bank;
}

static inline void restore_banking_state(GB_gameboy_t *gb, banking_state_t *state)
{

    gb->mbc_rom0_bank = state->rom0_bank;
    gb->mbc_rom_bank = state->rom_bank;
    gb->mbc_ram_bank = state->mbc_ram_bank;
    gb->mbc_ram_enable = state->mbc_ram_enable;
    gb->cgb_ram_bank = state->ram_bank;
    gb->cgb_vram_bank = state->vram_bank;
}

static inline void switch_banking_state(GB_gameboy_t *gb, uint16_t bank)
{
    gb->mbc_rom0_bank = bank;
    gb->mbc_rom_bank = bank;
    gb->mbc_ram_bank = bank;
    gb->mbc_ram_enable = true;
    if (GB_is_cgb(gb)) {
        gb->cgb_ram_bank = bank & 7;
        gb->cgb_vram_bank = bank & 1;
        if (gb->cgb_ram_bank == 0) {
            gb->cgb_ram_bank = 1;
        }
    }
}

static const char *value_to_string(GB_gameboy_t *gb, uint16_t value, bool prefer_name, bool prefer_local)
{
    static __thread char output[256];
    const GB_bank_symbol_t *symbol = GB_debugger_find_symbol(gb, value, prefer_local);

    if (symbol && (value - symbol->addr > 0x1000 || symbol->addr == 0) ) {
        symbol = NULL;
    }

    if (!symbol) {
        snprintf(output, sizeof(output), "$%04x", value);
    }

    else if (symbol->addr == value) {
        if (prefer_name) {
            snprintf(output, sizeof(output), "%s ($%04x)", symbol->name, value);
        }
        else {
            snprintf(output, sizeof(output), "$%04x (%s)", value, symbol->name);
        }
    }

    else {
        if (prefer_name) {
            snprintf(output, sizeof(output), "%s+$%03x ($%04x)", symbol->name, value - symbol->addr, value);
        }
        else {
            snprintf(output, sizeof(output), "$%04x (%s+$%03x)", value, symbol->name, value - symbol->addr);
        }
    }
    return output;
}

static GB_symbol_map_t *get_symbol_map(GB_gameboy_t *gb, uint16_t bank)
{
    if (bank >= gb->n_symbol_maps) {
        return NULL;
    }
    return gb->bank_symbols[bank];
}

static const char *debugger_value_to_string(GB_gameboy_t *gb, value_t value, bool prefer_name, bool prefer_local)
{
    if (!value.has_bank) return value_to_string(gb, value.value, prefer_name, prefer_local);

    static __thread char output[256];
    const GB_bank_symbol_t *symbol = GB_map_find_symbol(get_symbol_map(gb, value.bank), value.value, prefer_local);

    if (symbol && (value.value - symbol->addr > 0x1000 || symbol->addr == 0) ) {
        symbol = NULL;
    }

    if (!symbol) {
        snprintf(output, sizeof(output), "$%02x:$%04x", value.bank, value.value);
    }

    else if (symbol->addr == value.value) {
        if (prefer_name) {
            snprintf(output, sizeof(output), "%s ($%02x:$%04x)", symbol->name, value.bank, value.value);
        }
        else {
            snprintf(output, sizeof(output), "$%02x:$%04x (%s)", value.bank, value.value, symbol->name);
        }
    }

    else {
        if (prefer_name) {
            snprintf(output, sizeof(output), "%s+$%03x ($%02x:$%04x)", symbol->name, value.value - symbol->addr, value.bank, value.value);
        }
        else {
            snprintf(output, sizeof(output), "$%02x:$%04x (%s+$%03x)", value.bank, value.value, symbol->name, value.value - symbol->addr);
        }
    }
    return output;
}

static value_t read_lvalue(GB_gameboy_t *gb, lvalue_t lvalue)
{
    /* Not used until we add support for operators like += */
    switch (lvalue.kind) {
        case LVALUE_MEMORY:
            if (lvalue.memory_address.has_bank) {
                banking_state_t state;
                save_banking_state(gb, &state);
                switch_banking_state(gb, lvalue.memory_address.bank);
                value_t r = VALUE_16(GB_read_memory(gb, lvalue.memory_address.value));
                restore_banking_state(gb, &state);
                return r;
            }
            return VALUE_16(GB_read_memory(gb, lvalue.memory_address.value));

        case LVALUE_MEMORY16:
            if (lvalue.memory_address.has_bank) {
                banking_state_t state;
                save_banking_state(gb, &state);
                switch_banking_state(gb, lvalue.memory_address.bank);
                value_t r = VALUE_16(GB_read_memory(gb, lvalue.memory_address.value) |
                                   (GB_read_memory(gb, lvalue.memory_address.value + 1) * 0x100));
                restore_banking_state(gb, &state);
                return r;
            }
            return VALUE_16(GB_read_memory(gb, lvalue.memory_address.value) |
                            (GB_read_memory(gb, lvalue.memory_address.value + 1) * 0x100));

        case LVALUE_REG16:
            return VALUE_16(*lvalue.register_address);

        case LVALUE_REG_L:
            return VALUE_16(*lvalue.register_address & 0x00FF);

        case LVALUE_REG_H:
            return VALUE_16(*lvalue.register_address >> 8);
    }

    return VALUE_16(0);
}

static void write_lvalue(GB_gameboy_t *gb, lvalue_t lvalue, uint16_t value)
{
    switch (lvalue.kind) {
        case LVALUE_MEMORY:
            if (lvalue.memory_address.has_bank) {
                banking_state_t state;
                save_banking_state(gb, &state);
                switch_banking_state(gb, lvalue.memory_address.bank);
                GB_write_memory(gb, lvalue.memory_address.value, value);
                restore_banking_state(gb, &state);
                return;
            }
            GB_write_memory(gb, lvalue.memory_address.value, value);
            return;

        case LVALUE_MEMORY16:
            if (lvalue.memory_address.has_bank) {
                banking_state_t state;
                save_banking_state(gb, &state);
                switch_banking_state(gb, lvalue.memory_address.bank);
                GB_write_memory(gb, lvalue.memory_address.value, value);
                GB_write_memory(gb, lvalue.memory_address.value + 1, value >> 8);
                restore_banking_state(gb, &state);
                return;
            }
            GB_write_memory(gb, lvalue.memory_address.value, value);
            GB_write_memory(gb, lvalue.memory_address.value + 1, value >> 8);
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
static value_t _div(value_t a, value_t b) 
{
    if (b.value == 0) {
        return FIX_BANK(0);
    }
    return FIX_BANK(a.value / b.value);
};
static value_t mod(value_t a, value_t b) 
{
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
    int8_t priority;
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
    {"<=", -2, lower_equals},
    {"<", -2, lower},
    {">>", 2, shright},
    {">=", -2, greater_equals},
    {">", -2, greater},
    {"==", -2, equals},
    {"=", -1, NULL, assign},
    {"!=", -2, different},
    {":", 3, bank},
};

typedef struct {
    union {
        uint16_t old_address;
        uint16_t old_value;
    };
    uint16_t new_value;
    bool old_as_value;
} evaluate_conf_t;

static value_t debugger_evaluate(GB_gameboy_t *gb, const char *string,
                                 size_t length, bool *error,
                                 const evaluate_conf_t *conf);

static lvalue_t debugger_evaluate_lvalue(GB_gameboy_t *gb, const char *string,
                                         size_t length, bool *error,
                                         const evaluate_conf_t *conf)
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
    if (length == 0) { 
        GB_log(gb, "Expected expression.\n");
        *error = true;
        return (lvalue_t){0,};
    }
    if (string[0] == '(' && string[length - 1] == ')') {
        // Attempt to strip parentheses
        signed depth = 0;
        for (unsigned i = 0; i < length; i++) {
            if (string[i] == '(') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == ')') depth--;
        }
        if (depth == 0) return debugger_evaluate_lvalue(gb, string + 1, length - 2, error, conf);
    }
    else if (string[0] == '[' && string[length - 1] == ']') {
        // Attempt to strip square parentheses (memory dereference)
        signed depth = 0;
        for (unsigned i = 0; i < length; i++) {
            if (string[i] == '[') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == ']') depth--;
        }
        if (depth == 0) {
            return (lvalue_t){LVALUE_MEMORY, .memory_address = debugger_evaluate(gb, string + 1, length - 2, error, conf)};
        }
    }
    else if (string[0] == '{' && string[length - 1] == '}') {
        // Attempt to strip curly parentheses (memory dereference)
        signed depth = 0;
        for (unsigned i = 0; i < length; i++) {
            if (string[i] == '{') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == '}') depth--;
        }
        if (depth == 0) {
            return (lvalue_t){LVALUE_MEMORY16, .memory_address = debugger_evaluate(gb, string + 1, length - 2, error, conf)};
        }
    }

    // Registers
    if (string[0] != '$' && (string[0] < '0' || string[0] > '9')) {
        if (length == 1) {
            switch (string[0]) {
                case 'a': return (lvalue_t){LVALUE_REG_H, .register_address = &gb->af};
                case 'f': return (lvalue_t){LVALUE_REG_L, .register_address = &gb->af};
                case 'b': return (lvalue_t){LVALUE_REG_H, .register_address = &gb->bc};
                case 'c': return (lvalue_t){LVALUE_REG_L, .register_address = &gb->bc};
                case 'd': return (lvalue_t){LVALUE_REG_H, .register_address = &gb->de};
                case 'e': return (lvalue_t){LVALUE_REG_L, .register_address = &gb->de};
                case 'h': return (lvalue_t){LVALUE_REG_H, .register_address = &gb->hl};
                case 'l': return (lvalue_t){LVALUE_REG_L, .register_address = &gb->hl};
            }
        }
        else if (length == 2) {
            switch (string[0]) {
                case 'a': if (string[1] == 'f') return (lvalue_t){LVALUE_REG16, .register_address = &gb->af};
                case 'b': if (string[1] == 'c') return (lvalue_t){LVALUE_REG16, .register_address = &gb->bc};
                case 'd': if (string[1] == 'e') return (lvalue_t){LVALUE_REG16, .register_address = &gb->de};
                case 'h': if (string[1] == 'l') return (lvalue_t){LVALUE_REG16, .register_address = &gb->hl};
                case 's': if (string[1] == 'p') return (lvalue_t){LVALUE_REG16, .register_address = &gb->sp};
                case 'p': if (string[1] == 'c') return (lvalue_t){LVALUE_REG16, .register_address = &gb->pc};
            }
        }
        GB_log(gb, "Unknown register: %.*s\n", (unsigned) length, string);
        *error = true;
        return (lvalue_t){0,};
    }

    GB_log(gb, "Expression is not an lvalue: %.*s\n", (unsigned) length, string);
    *error = true;
    return (lvalue_t){0,};
}

#define ERROR ((value_t){0,})
static value_t debugger_evaluate(GB_gameboy_t *gb, const char *string,
                                 size_t length, bool *error,
                                 const evaluate_conf_t *conf)
{
    /* Disable watchpoints while evaluating expressions */
    uint16_t n_watchpoints = gb->n_watchpoints;
    gb->n_watchpoints = 0;

    value_t ret = ERROR;

    *error = false;
    // Strip whitespace
    while (length && (string[0] == ' ' || string[0] == '\n' || string[0] == '\r' || string[0] == '\t')) {
        string++;
        length--;
    }
    while (length && (string[length-1] == ' ' || string[length-1] == '\n' || string[length-1] == '\r' || string[length-1] == '\t')) {
        length--;
    }
    if (length == 0) { 
        GB_log(gb, "Expected expression.\n");
        *error = true;
        goto exit;
    }
    if (string[0] == '(' && string[length - 1] == ')') {
        // Attempt to strip parentheses
        signed depth = 0;
        for (unsigned i = 0; i < length; i++) {
            if (string[i] == '(') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == ')') depth--;
        }
        if (depth == 0) {
            ret = debugger_evaluate(gb, string + 1, length - 2, error, conf);
            goto exit;
        }
    }
    else if (string[0] == '[' && string[length - 1] == ']') {
        // Attempt to strip square parentheses (memory dereference)
        signed depth = 0;
        for (unsigned i = 0; i < length; i++) {
            if (string[i] == '[') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == ']') depth--;
        }

        if (depth == 0) {
            value_t addr = debugger_evaluate(gb, string + 1, length - 2, error, conf);
            banking_state_t state;
            if (addr.bank) {
                save_banking_state(gb, &state);
                switch_banking_state(gb, addr.bank);
            }
            ret = VALUE_16(GB_read_memory(gb, addr.value));
            if (addr.bank) {
                restore_banking_state(gb, &state);
            }
            goto exit;
        }
    }
    else if (string[0] == '{' && string[length - 1] == '}') {
        // Attempt to strip curly parentheses (memory dereference)
        signed depth = 0;
        for (unsigned i = 0; i < length; i++) {
            if (string[i] == '{') depth++;
            if (depth == 0) {
                // First and last are not matching
                depth = 1;
                break;
            }
            if (string[i] == '}') depth--;
        }

        if (depth == 0) {
            value_t addr = debugger_evaluate(gb, string + 1, length - 2, error, conf);
            banking_state_t state;
            if (addr.bank) {
                save_banking_state(gb, &state);
                switch_banking_state(gb, addr.bank);
            }
            ret = VALUE_16(GB_read_memory(gb, addr.value) | (GB_read_memory(gb, addr.value + 1) * 0x100));
            if (addr.bank) {
                restore_banking_state(gb, &state);
            }
            goto exit;
        }
    }
    // Search for lowest priority operator
    signed depth = 0;
    unsigned operator_index = -1;
    unsigned operator_pos = 0;
    for (unsigned i = 0; i < length; i++) {
        if (string[i] == '(') depth++;
        else if (string[i] == ')') depth--;
        else if (string[i] == '[') depth++;
        else if (string[i] == ']') depth--;
        else if (depth == 0) {
            for (unsigned j = 0; j < sizeof(operators) / sizeof(operators[0]); j++) {
                unsigned operator_length = strlen(operators[j].string);
                if (operator_length > length - i) continue; // Operator too long
                
                if (memcmp(string + i, operators[j].string, operator_length) == 0) {
                    if (operator_index != -1 && operators[operator_index].priority < operators[j].priority) {
                        /* for supporting = vs ==, etc*/
                        i += operator_length - 1;
                        break;
                    }
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
        unsigned right_start = (unsigned)(operator_pos + strlen(operators[operator_index].string));
        value_t right = debugger_evaluate(gb, string + right_start, length - right_start, error, conf);
        if (*error) goto exit;
        if (operators[operator_index].lvalue_operator) {
            lvalue_t left = debugger_evaluate_lvalue(gb, string, operator_pos, error, conf);
            if (*error) goto exit;
            ret = operators[operator_index].lvalue_operator(gb, left, right.value);
            goto exit;
        }
        value_t left = debugger_evaluate(gb, string, operator_pos, error, conf);
        if (*error) goto exit;
        ret = operators[operator_index].operator(left, right);
        goto exit;
    }

    // Not an expression - must be a register or a literal

    // Registers
    if (string[0] != '$' && (string[0] < '0' || string[0] > '9')) {
        if (length == 1) {
            switch (string[0]) {
                case 'a': ret = VALUE_16(gb->af >> 8); goto exit;
                case 'f': ret = VALUE_16(gb->af & 0xFF); goto exit;
                case 'b': ret = VALUE_16(gb->bc >> 8); goto exit;
                case 'c': ret = VALUE_16(gb->bc & 0xFF); goto exit;
                case 'd': ret = VALUE_16(gb->de >> 8); goto exit;
                case 'e': ret = VALUE_16(gb->de & 0xFF); goto exit;
                case 'h': ret = VALUE_16(gb->hl >> 8); goto exit;
                case 'l': ret = VALUE_16(gb->hl & 0xFF); goto exit;
            }
        }
        else if (length == 2) {
            switch (string[0]) {
                case 'a': if (string[1] == 'f') {ret = VALUE_16(gb->af); goto exit;}
                case 'b': if (string[1] == 'c') {ret = VALUE_16(gb->bc); goto exit;}
                case 'd': if (string[1] == 'e') {ret = VALUE_16(gb->de); goto exit;}
                case 'h': if (string[1] == 'l') {ret = VALUE_16(gb->hl); goto exit;}
                case 's': if (string[1] == 'p') {ret = VALUE_16(gb->sp); goto exit;}
                case 'p': if (string[1] == 'c') {ret = (value_t){true, bank_for_addr(gb, gb->pc), gb->pc};  goto exit;}
            }
        }
        else if (length == 3 && conf) {
            if (memcmp(string, "old", 3) == 0) {
                if (conf->old_as_value) {
                    ret = VALUE_16(conf->old_value);
                }
                else {
                    ret = VALUE_16(GB_read_memory(gb, conf->old_address));
                }
                goto exit;
            }

            if (memcmp(string, "new", 3) == 0) {
                ret = VALUE_16(conf->new_value);
                goto exit;
            }

        }

        char symbol_name[length + 1];
        memcpy(symbol_name, string, length);
        symbol_name[length] = 0;
        const GB_symbol_t *symbol = GB_reversed_map_find_symbol(&gb->reversed_symbol_map, symbol_name);
        if (symbol) {
            ret = (value_t){true, symbol->bank, symbol->addr};
            goto exit;
        }

        GB_log(gb, "Unknown register or symbol: %.*s\n", (unsigned) length, string);
        *error = true;
        goto exit;
    }

    char *end;
    unsigned base = 10;
    if (string[0] == '$') {
        string++;
        base = 16;
        length--;
    }
    uint16_t literal = (uint16_t) (strtol(string, &end, base));
    if (end != string + length) {
        GB_log(gb, "Failed to parse: %.*s\n", (unsigned) length, string);
        *error = true;
        goto exit;
    }
    ret = VALUE_16(literal);
exit:
    gb->n_watchpoints = n_watchpoints;
    return ret;
}

static void update_debug_active(GB_gameboy_t *gb)
{
    gb->debug_active = !gb->debug_disable && (gb->debug_stopped || gb->debug_fin_command || gb->debug_next_command || gb->breakpoints);
}

struct debugger_command_s;
typedef bool debugger_command_imp_t(GB_gameboy_t *gb, char *arguments, char *modifiers, const struct debugger_command_s *command);
typedef char *debugger_completer_imp_t(GB_gameboy_t *gb, const char *string, uintptr_t *context);

typedef struct debugger_command_s {
    const char *command;
    uint8_t min_length;
    debugger_command_imp_t *implementation;
    const char *help_string; // Null if should not appear in help
    const char *arguments_format; // For usage message
    const char *modifiers_format; // For usage message
    debugger_completer_imp_t *argument_completer;
    debugger_completer_imp_t *modifiers_completer;
} debugger_command_t;

static const char *lstrip(const char *str)
{
    while (*str == ' ' || *str == '\t') {
        str++;
    }
    return str;
}

#define STOPPED_ONLY \
if (!gb->debug_stopped) { \
GB_log(gb, "Program is running, use 'interrupt' to stop execution.\n"); \
return false; \
}

#define NO_MODIFIERS \
if (modifiers) { \
print_usage(gb, command); \
return true; \
}

static void print_usage(GB_gameboy_t *gb, const debugger_command_t *command)
{
    GB_log(gb, "Usage: %s", command->command);

    if (command->modifiers_format) {
        GB_log(gb, "[/%s]", command->modifiers_format);
    }

    if (command->arguments_format) {
        GB_log(gb, " %s", command->arguments_format);
    }

    GB_log(gb, "\n");
}

static bool cont(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    STOPPED_ONLY

    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }

    gb->debug_stopped = false;
    return false;
}

static bool interrupt(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    
    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }
    
    if (gb->debug_stopped) {
        GB_log(gb, "Program already stopped.\n");
        return true;
    }
    
    GB_debugger_break(gb);
    return true;
}

static char *reset_completer(GB_gameboy_t *gb, const char *string, uintptr_t *context)
{
    size_t length = strlen(string);
    const char *suggestions[] = {"quick", "reload"};
    while (*context < sizeof(suggestions) / sizeof(suggestions[0])) {
        if (strncmp(string, suggestions[*context], length) == 0) {
            return strdup(suggestions[(*context)++] + length);
        }
        (*context)++;
    }
    return NULL;
}

static bool reset(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    
    const char *stripped_argument = lstrip(arguments);
    
    if (stripped_argument[0] == 0) {
        GB_reset(gb);
        if (gb->debug_stopped) {
            GB_cpu_disassemble(gb, gb->pc, 5);
        }
        return true;
    }
    
    if (strcmp(stripped_argument, "quick") == 0) {
        GB_quick_reset(gb);
        if (gb->debug_stopped) {
            GB_cpu_disassemble(gb, gb->pc, 5);
        }
        return true;
    }
    
    if (strcmp(stripped_argument, "reload") == 0) {
        if (gb->debugger_reload_callback) {
            gb->debugger_reload_callback(gb);
            if (gb->undo_state) {
                free(gb->undo_state);
                gb->undo_state = NULL;
            }
            if (gb->debug_stopped) {
                GB_cpu_disassemble(gb, gb->pc, 5);
            }
            return true;
        }
        GB_log(gb, "ROM reloading via the debugger is not supported in this frontend.\n");
        return true;
    }
    
    print_usage(gb, command);
    return true;
}

static bool next(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    STOPPED_ONLY

    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }

    gb->debug_stopped = false;
    gb->debug_next_command = true;
    gb->debug_call_depth = 0;
    return false;
}

static bool step(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    STOPPED_ONLY

    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }

    return false;
}

static bool finish(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    STOPPED_ONLY

    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }

    gb->debug_stopped = false;
    gb->debug_fin_command = true;
    gb->debug_call_depth = 0;
    return false;
}

static bool registers(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }


    GB_log(gb, "AF  = $%04x (%c%c%c%c)\n", gb->af, /* AF can't really be an address */
           (gb->f & GB_CARRY_FLAG)?      'C' : '-',
           (gb->f & GB_HALF_CARRY_FLAG)? 'H' : '-',
           (gb->f & GB_SUBTRACT_FLAG)?   'N' : '-',
           (gb->f & GB_ZERO_FLAG)?       'Z' : '-');
    GB_log(gb, "BC  = %s\n", value_to_string(gb, gb->bc, false, false));
    GB_log(gb, "DE  = %s\n", value_to_string(gb, gb->de, false, false));
    GB_log(gb, "HL  = %s\n", value_to_string(gb, gb->hl, false, false));
    GB_log(gb, "SP  = %s\n", value_to_string(gb, gb->sp, false, false));
    GB_log(gb, "PC  = %s\n", value_to_string(gb, gb->pc, false, false));
    GB_log(gb, "IME = %s\n", gb->ime? "Enabled" : "Disabled");
    return true;
}

static char *on_off_completer(GB_gameboy_t *gb, const char *string, uintptr_t *context)
{
    size_t length = strlen(string);
    const char *suggestions[] = {"on", "off"};
    while (*context < sizeof(suggestions) / sizeof(suggestions[0])) {
        if (strncmp(string, suggestions[*context], length) == 0) {
            return strdup(suggestions[(*context)++] + length);
        }
        (*context)++;
    }
    return NULL;
}

/* Enable or disable software breakpoints */
static bool softbreak(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    if (strcmp(lstrip(arguments), "on") == 0 || !strlen(lstrip(arguments))) {
        gb->has_software_breakpoints = true;
    }
    else if (strcmp(lstrip(arguments), "off") == 0) {
        gb->has_software_breakpoints = false;
    }
    else {
        print_usage(gb, command);
    }

    return true;
}

static inline bool is_legal_symbol_char(char c)
{
    if (c >= '0' && c <= '9') return true;
    if (c >= 'A' && c <= 'Z') return true;
    if (c >= 'a' && c <= 'z') return true;
    if (c == '_') return true;
    if (c == '.') return true;
    return false;
}

static char *symbol_completer(GB_gameboy_t *gb, const char *string, uintptr_t *_context)
{
    const char *symbol_prefix = string;
    while (*string) {
        if (!is_legal_symbol_char(*string)) {
            symbol_prefix = string + 1;
        }
        string++;
    }
    
    if (*symbol_prefix == '$') {
        return NULL;
    }
    
    struct {
        uint16_t bank;
        uint32_t symbol;
    } *context = (void *)_context;
    
    
    size_t length = strlen(symbol_prefix);
    while (context->bank < 0x200) {
        GB_symbol_map_t *map = get_symbol_map(gb, context->bank);
        if (map == NULL ||
            context->symbol >= map->n_symbols) {
            context->bank++;
            context->symbol = 0;
            continue;
        }
        const char *candidate = map->symbols[context->symbol++].name;
        if (strncmp(symbol_prefix, candidate, length) == 0) {
            return strdup(candidate + length);
        }
    }
    return NULL;
}

static char *j_completer(GB_gameboy_t *gb, const char *string, uintptr_t *context)
{
    size_t length = strlen(string);
    const char *suggestions[] = {"j"};
    while (*context < sizeof(suggestions) / sizeof(suggestions[0])) {
        if (strncmp(string, suggestions[*context], length) == 0) {
            return strdup(suggestions[(*context)++] + length);
        }
        (*context)++;
    }
    return NULL;
}

static bool check_inclusive(char *to)
{
    size_t length = strlen(to);
    while (length > strlen("inclusive")) {
        if (to[length - 1] == ' ') {
            to[length - 1] = 0;
            length--;
            continue;
        }
        if (strcmp(to + length - strlen("inclusive"), "inclusive") == 0) {
            to[length - strlen("inclusive")] = 0;
            return true;
        }
        return false;
    }
    return false;
}

static bool breakpoint(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    bool is_jump_to = true;
    if (!modifiers) {
        is_jump_to = false;
    }
    else if (strcmp(modifiers, "j") != 0) {
        print_usage(gb, command);
        return true;
    }

    if (strlen(lstrip(arguments)) == 0) {
        print_usage(gb, command);
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
        debugger_evaluate(gb, condition, (unsigned)strlen(condition), &error, NULL);
        if (error) return true;

    }
    
    char *to = NULL;
    bool inclusive = false;
    if ((to = strstr(arguments, " to "))) {
        *to = 0;
        to += strlen(" to ");
        inclusive = check_inclusive(to);
    }

    bool error;
    value_t result = debugger_evaluate(gb, arguments, (unsigned)strlen(arguments), &error, NULL);
    if (error) return true;

    uint16_t length = 0;
    value_t end = result;
    if (to) {
        end = debugger_evaluate(gb, to, (unsigned)strlen(to), &error, NULL);
        if (error) return true;
        if (end.has_bank && result.has_bank && end.bank != result.bank) {
            GB_log(gb, "Breakpoint range start and end points have different banks\n");
            return true;
        }
        if (end.value <= result.value) {
            GB_log(gb, "Breakpoint range end point must be grater than the start point\n");
            return true;
        }
        length = end.value - result.value - 1;
    }

    uint32_t key = BP_KEY(result);
    
    gb->breakpoints = realloc(gb->breakpoints, (gb->n_breakpoints + 1) * sizeof(gb->breakpoints[0]));
    unsigned id = 1;
    if (gb->n_breakpoints) {
        id = gb->breakpoints[gb->n_breakpoints - 1].id + 1;
    }
    
    gb->breakpoints[gb->n_breakpoints++] = (struct GB_breakpoint_s){
        .id = id,
        .key = key,
        .condition = condition? strdup(condition) : NULL,
        .is_jump_to = is_jump_to,
        .length = length,
        .inclusive = inclusive,
    };

    if (is_jump_to) {
        gb->has_jump_to_breakpoints = true;
    }

    GB_log(gb, "Breakpoint %u set at %s", id, debugger_value_to_string(gb, result, true, false));
    if (length) {
        GB_log(gb, " - %s%s\n", debugger_value_to_string(gb, end, true, true), inclusive? " (inclusive)" : "");
    }
    else {
        GB_log(gb, "\n");
    }
    return true;
}

static bool delete(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    if (strlen(lstrip(arguments)) == 0) {
        for (unsigned i = gb->n_breakpoints; i--;) {
            if (gb->breakpoints[i].condition) {
                free(gb->breakpoints[i].condition);
            }
        }
        free(gb->breakpoints);
        gb->breakpoints = NULL;
        gb->n_breakpoints = 0;
        gb->has_jump_to_breakpoints = false;
        return true;
    }

    char *end;
    unsigned id = strtol(arguments, &end, 10);
    if (*end) {
        print_usage(gb, command);
        return true;
    }
    
    for (unsigned i = 0; i < gb->n_breakpoints; i++) {
        if (gb->breakpoints[i].id != id) continue;
        value_t addr = (value_t){gb->breakpoints[i].bank != (uint16_t)-1, gb->breakpoints[i].bank, gb->breakpoints[i].addr};
        GB_log(gb, "Breakpoint %u removed from %s\n", id, debugger_value_to_string(gb, addr, addr.has_bank, false));

        if (gb->breakpoints[i].condition) {
            free(gb->breakpoints[i].condition);
        }
        
        if (gb->breakpoints[i].is_jump_to) {
            gb->has_jump_to_breakpoints = false;
            for (unsigned j = 0; j < gb->n_breakpoints; j++) {
                if (j == i) continue;
                if (gb->breakpoints[j].is_jump_to) {
                    gb->has_jump_to_breakpoints = true;
                    break;
                }
            }
        }
        
        memmove(&gb->breakpoints[i], &gb->breakpoints[i + 1], (gb->n_breakpoints - i - 1) * sizeof(gb->breakpoints[0]));
        gb->n_breakpoints--;
        gb->breakpoints = realloc(gb->breakpoints, gb->n_breakpoints * sizeof(gb->breakpoints[0]));
        
        return true;
    }

    GB_log(gb, "Breakpoint %u was not found\n", id);
    return true;
}

static char *rw_completer(GB_gameboy_t *gb, const char *string, uintptr_t *context)
{
    size_t length = strlen(string);
    const char *suggestions[] = {"r", "rw", "w"};
    while (*context < sizeof(suggestions) / sizeof(suggestions[0])) {
        if (strncmp(string, suggestions[*context], length) == 0) {
            return strdup(suggestions[(*context)++] + length);
        }
        (*context)++;
    }
    return NULL;
}

static bool watch(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    if (strlen(lstrip(arguments)) == 0) {
        print_usage(gb, command);
        return true;
    }

    if (gb->n_watchpoints == (typeof(gb->n_watchpoints)) -1) {
        GB_log(gb, "Too many watchpoints set\n");
        return true;
    }

    if (!modifiers) {
        modifiers = "w";
    }

    uint8_t flags = 0;
    while (*modifiers) {
        switch (*modifiers) {
            case 'r':
                flags |= WATCHPOINT_READ;
                break;
            case 'w':
                flags |= WATCHPOINT_WRITE;
                break;
            default:
                print_usage(gb, command);
                return true;
        }
        modifiers++;
    }

    if (!flags) {
        print_usage(gb, command);
        return true;
    }

    char *condition = NULL;
    if ((condition = strstr(arguments, " if "))) {
        *condition = 0;
        condition += strlen(" if ");
        /* Verify condition is sane (Todo: This might have side effects!) */
        bool error;
        /* To make new and old legal */
        static const evaluate_conf_t conf = {
            .old_as_value = true,
            .old_value = 0,
            .new_value = 0,
        };
        debugger_evaluate(gb, condition, (unsigned)strlen(condition), &error, &conf);
        if (error) return true;
    }
    
    char *to = NULL;
    bool inclusive = false;
    if ((to = strstr(arguments, " to "))) {
        *to = 0;
        to += strlen(" to ");
        inclusive = check_inclusive(to);
    }

    bool error;
    value_t result = debugger_evaluate(gb, arguments, (unsigned)strlen(arguments), &error, NULL);
    uint32_t key = WP_KEY(result);
    
    uint16_t length = 0;
    value_t end = result;
    if (to) {
        end = debugger_evaluate(gb, to, (unsigned)strlen(to), &error, NULL);
        if (error) return true;
        if (end.has_bank && result.has_bank && end.bank != result.bank) {
            GB_log(gb, "Watchpoint range start and end points have different banks\n");
            return true;
        }
        if (end.value <= result.value) {
            GB_log(gb, "Watchpoint range end point must be grater than the start point\n");
            return true;
        }
        length = end.value - result.value - 1;
    }

    if (error) return true;
    
    unsigned id = 1;
    if (gb->n_watchpoints) {
        id = gb->watchpoints[gb->n_watchpoints - 1].id + 1;
    }
    

    gb->watchpoints = realloc(gb->watchpoints, (gb->n_watchpoints + 1) * sizeof(gb->watchpoints[0]));
    
    gb->watchpoints[gb->n_watchpoints++] = (struct GB_watchpoint_s){
        .id = id,
        .key = key,
        .condition = condition? strdup(condition) : NULL,
        .flags = flags,
        .length = length,
        .inclusive = inclusive,
    };

    const char *flags_string = inline_const(const char *[], {
        [WATCHPOINT_READ] = "read-only",
        [WATCHPOINT_WRITE] = "write-only",
        [WATCHPOINT_READ | WATCHPOINT_WRITE] = "read-write",
    })[flags];
    
    GB_log(gb, "Watchpoint %u set at %s", id, debugger_value_to_string(gb, result, true, false));
    if (length) {
        GB_log(gb, " - %s%s, %s\n", debugger_value_to_string(gb, end, true, true), inclusive? " (inclusive)" : "", flags_string);
    }
    else {
        GB_log(gb, ", %s\n", flags_string);
    }
    return true;
}

static bool unwatch(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    if (strlen(lstrip(arguments)) == 0) {
        for (unsigned i = gb->n_watchpoints; i--;) {
            if (gb->watchpoints[i].condition) {
                free(gb->watchpoints[i].condition);
            }
        }
        free(gb->watchpoints);
        gb->watchpoints = NULL;
        gb->n_watchpoints = 0;
        return true;
    }
    
    char *end;
    unsigned id = strtol(arguments, &end, 10);
    if (*end) {
        print_usage(gb, command);
        return true;
    }
    
    for (unsigned i = 0; i < gb->n_watchpoints; i++) {
        if (gb->watchpoints[i].id != id) continue;
        
        value_t addr = (value_t){gb->watchpoints[i].bank != (uint16_t)-1, gb->watchpoints[i].bank, gb->watchpoints[i].addr};
        GB_log(gb, "Watchpoint %u removed from %s\n", id, debugger_value_to_string(gb, addr, addr.has_bank, false));
        
        if (gb->watchpoints[i].condition) {
            free(gb->watchpoints[i].condition);
        }
        
        memmove(&gb->watchpoints[i], &gb->watchpoints[i + 1], (gb->n_watchpoints - i - 1) * sizeof(gb->watchpoints[0]));
        gb->n_watchpoints--;
        gb->watchpoints = realloc(gb->watchpoints, gb->n_watchpoints * sizeof(gb->watchpoints[0]));
        
        return true;
    }
    
    GB_log(gb, "Watchpoint %u was not found\n", id);
    return true;
}

static bool list(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }

    if (gb->n_breakpoints == 0) {
        GB_log(gb, "No breakpoints set.\n");
    }
    else {
        GB_log(gb, "%d breakpoint(s) set:\n", gb->n_breakpoints);
        for (uint16_t i = 0; i < gb->n_breakpoints; i++) {
            value_t addr = (value_t){gb->breakpoints[i].bank != (uint16_t)-1, gb->breakpoints[i].bank, gb->breakpoints[i].addr};
            char *end_string = NULL;
            if (gb->breakpoints[i].length) {
                value_t end = addr;
                end.value += gb->breakpoints[i].length + 1;
                end_string = strdup(debugger_value_to_string(gb, end, addr.has_bank, true));
            }
            if (gb->breakpoints[i].condition) {
                GB_log(gb, " %d. %s%s%s%s (%sCondition: %s)\n", gb->breakpoints[i].id,
                                                        debugger_value_to_string(gb, addr, addr.has_bank, false),
                                                        end_string? " - " : "",
                                                        end_string ?: "",
                                                        gb->breakpoints[i].inclusive? " (inclusive)" : "",
                                                        gb->breakpoints[i].is_jump_to? "Jump to, ": "",
                                                        gb->breakpoints[i].condition);
            }
            else {
                GB_log(gb, " %d. %s%s%s%s%s\n", gb->breakpoints[i].id,
                                          debugger_value_to_string(gb, addr, addr.has_bank, false),
                                          end_string? " - " : "",
                                          end_string ?: "",
                                          gb->breakpoints[i].inclusive? " (inclusive)" : "",
                                          gb->breakpoints[i].is_jump_to? " (Jump to)" : "");
            }
            if (end_string) {
                free(end_string);
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
            char *end_string = NULL;
            if (gb->watchpoints[i].length) {
                value_t end = addr;
                end.value += gb->watchpoints[i].length + 1;
                end_string = strdup(debugger_value_to_string(gb, end, addr.has_bank, true));
            }
            if (gb->watchpoints[i].condition) {
                GB_log(gb, " %d. %s%s%s%s (%c%c, Condition: %s)\n", gb->watchpoints[i].id, debugger_value_to_string(gb, addr, addr.has_bank, false),
                                                                  end_string? " - " : "", end_string ?: "",
                                                                  gb->watchpoints[i].inclusive? " (inclusive)" : "",
                                                                  (gb->watchpoints[i].flags & WATCHPOINT_READ)? 'r' : '-',
                                                                  (gb->watchpoints[i].flags & WATCHPOINT_WRITE)? 'w' : '-',
                                                                  gb->watchpoints[i].condition);
            }
            else {
                GB_log(gb, " %d. %s%s%s%s (%c%c)\n", gb->watchpoints[i].id, debugger_value_to_string(gb, addr, addr.has_bank, false),
                                                   end_string? " - " : "", end_string ?: "",
                                                   gb->watchpoints[i].inclusive? " (inclusive)" : "",
                                                   (gb->watchpoints[i].flags & WATCHPOINT_READ)? 'r' : '-',
                                                   (gb->watchpoints[i].flags & WATCHPOINT_WRITE)? 'w' : '-');
            }
        }
    }

    return true;
}

// Returns the id or 0
static unsigned should_break(GB_gameboy_t *gb, uint16_t addr, bool jump_to)
{
    if (unlikely(gb->backstep_instructions)) return false;
    uint16_t bank = bank_for_addr(gb, addr);
    for (unsigned i = 0; i < gb->n_breakpoints; i++) {
        struct GB_breakpoint_s *breakpoint = &gb->breakpoints[i];
        if (breakpoint->bank != (uint16_t)-1) {
            if (breakpoint->bank != bank) continue;
            if (!gb->boot_rom_finished) continue;
        }
        if (breakpoint->is_jump_to != jump_to) continue;
        if (addr < breakpoint->addr) continue;
        if (addr > (uint32_t)breakpoint->addr + breakpoint->length + breakpoint->inclusive) continue;
        if (!breakpoint->condition) return breakpoint->id;
        bool error;
        bool condition = debugger_evaluate(gb, breakpoint->condition,
                                           (unsigned)strlen(breakpoint->condition),
                                           &error, NULL).value;
        if (error) {
            GB_log(gb, "The condition for breakpoint %u is no longer a valid expression\n", breakpoint->id);
            return breakpoint->id;
        }
        if (condition) return breakpoint->id;
    }
    
    return 0;
}

static char *format_completer(GB_gameboy_t *gb, const char *string, uintptr_t *context)
{
    size_t length = strlen(string);
    const char *suggestions[] = {"a", "b", "d", "o", "x"};
    while (*context < sizeof(suggestions) / sizeof(suggestions[0])) {
        if (strncmp(string, suggestions[*context], length) == 0) {
            return strdup(suggestions[(*context)++] + length);
        }
        (*context)++;
    }
    return NULL;
}

static bool print(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    if (strlen(lstrip(arguments)) == 0) {
        print_usage(gb, command);
        return true;
    }

    if (!modifiers || !modifiers[0]) {
        modifiers = "a";
    }
    else if (modifiers[1]) {
        print_usage(gb, command);
        return true;
    }

    bool error;
    value_t result = debugger_evaluate(gb, arguments, (unsigned)strlen(arguments), &error, NULL);
    if (!error) {
        switch (modifiers[0]) {
            case 'a':
                GB_log(gb, "=%s\n", debugger_value_to_string(gb, result, false, false));
                break;
            case 'd':
                GB_log(gb, "=%d\n", result.value);
                break;
            case 'x':
                GB_log(gb, "=$%x\n", result.value);
                break;
            case 'o':
                GB_log(gb, "=0%o\n", result.value);
                break;
            case 'b':
            {
                if (!result.value) {
                    GB_log(gb, "=%%0\n");
                    break;
                }
                char binary[17];
                binary[16] = 0;
                char *ptr = &binary[16];
                while (result.value) {
                    *(--ptr) = (result.value & 1)? '1' : '0';
                    result.value >>= 1;
                }
                GB_log(gb, "=%%%s\n", ptr);
                break;
            }
            default:
                break;
        }
    }
    return true;
}

static bool examine(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    if (strlen(lstrip(arguments)) == 0) {
        print_usage(gb, command);
        return true;
    }

    bool error;
    value_t addr = debugger_evaluate(gb, arguments, (unsigned)strlen(arguments), &error, NULL);
    uint16_t count = 32;

    if (modifiers) {
        char *end;
        count = (uint16_t) (strtol(modifiers, &end, 10));
        if (*end) {
            print_usage(gb, command);
            return true;
        }
    }

    if (!error) {
        if (addr.has_bank) {
            banking_state_t old_state;
            save_banking_state(gb, &old_state);
            switch_banking_state(gb, addr.bank);

            while (count) {
                GB_log(gb, "%02x:%04x: ", addr.bank, addr.value);
                for (unsigned i = 0; i < 16 && count; i++) {
                    GB_log(gb, "%02x ", GB_safe_read_memory(gb, addr.value + i));
                    count--;
                }
                addr.value += 16;
                GB_log(gb, "\n");
            }

            restore_banking_state(gb, &old_state);
        }
        else {
            while (count) {
                GB_log(gb, "%04x: ", addr.value);
                for (unsigned i = 0; i < 16 && count; i++) {
                    GB_log(gb, "%02x ", GB_safe_read_memory(gb, addr.value + i));
                    count--;
                }
                addr.value += 16;
                GB_log(gb, "\n");
            }
        }
    }
    return true;
}

static bool disassemble(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    if (strlen(lstrip(arguments)) == 0) {
        arguments = "pc";
    }

    bool error;
    value_t addr = debugger_evaluate(gb, arguments, (unsigned)strlen(arguments), &error, NULL);
    uint16_t count = 5;

    if (modifiers) {
        char *end;
        count = (uint16_t) (strtol(modifiers, &end, 10));
        if (*end) {
            print_usage(gb, command);
            return true;
        }
    }

    if (!error) {
        if (addr.has_bank) {
            banking_state_t old_state;
            save_banking_state(gb, &old_state);
            switch_banking_state(gb, addr.bank);

            GB_cpu_disassemble(gb, addr.value, count);

            restore_banking_state(gb, &old_state);
        }
        else {
            GB_cpu_disassemble(gb, addr.value, count);
        }
    }
    return true;
}

static bool mbc(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS

    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }

    const GB_cartridge_t *cartridge = gb->cartridge_type;

    if (cartridge->has_ram) {
        bool has_battery = gb->cartridge_type->has_battery &&
                           (gb->cartridge_type->mbc_type != GB_TPP1 || (gb->rom[0x153] & 8));
        GB_log(gb, "Cartridge includes%s RAM: $%x bytes\n", has_battery? " battery-backed": "", gb->mbc_ram_size);
    }
    else {
        GB_log(gb, "No cartridge RAM\n");
    }

    if (cartridge->mbc_type) {
        if (gb->is_mbc30) {
            GB_log(gb, "MBC30\n");
        }
        else {
            static const char *const mapper_names[] = {
                [GB_MBC1]   = "MBC1",
                [GB_MBC2]   = "MBC2",
                [GB_MBC3]   = "MBC3",
                [GB_MBC5]   = "MBC5",
                [GB_MBC7]   = "MBC7",
                [GB_MMM01]  = "MMM01",
                [GB_HUC1]   = "HUC-1",
                [GB_HUC3]   = "HUC-3",
                [GB_CAMERA] = "MAC-GBD",

            };
            GB_log(gb, "%s\n", mapper_names[cartridge->mbc_type]);
        }
        if (cartridge->mbc_type == GB_MMM01 || cartridge->mbc_type == GB_MBC1) {
            GB_log(gb, "Current mapped ROM0 bank: %x\n", gb->mbc_rom0_bank);
        }
        GB_log(gb, "Current mapped ROM bank: %x\n", gb->mbc_rom_bank);
        if (cartridge->has_ram) {
            GB_log(gb, "Current mapped RAM bank: %x\n", gb->mbc_ram_bank);
            if (gb->cartridge_type->mbc_type != GB_HUC1) {
                GB_log(gb, "RAM is currently %s\n", gb->mbc_ram_enable? "enabled" : "disabled");
            }
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

    if (gb->cartridge_type->has_rumble &&
       (gb->cartridge_type->mbc_type != GB_TPP1 || (gb->rom[0x153] & 1))) {
        GB_log(gb, "Cart contains a Rumble Pak\n");
    }

    if (cartridge->has_rtc) {
        GB_log(gb, "Cart contains a real time clock\n");
    }

    return true;
}

static bool backtrace(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS

    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }

    GB_log(gb, "  1. %s\n", debugger_value_to_string(gb, (value_t){true, bank_for_addr(gb, gb->pc), gb->pc}, true, false));
    for (unsigned i = gb->backtrace_size; i--;) {
        GB_log(gb, "%3d. %s\n", gb->backtrace_size - i + 1, debugger_value_to_string(gb, (value_t){true, gb->backtrace_returns[i].bank, gb->backtrace_returns[i].addr}, true, false));
    }

    return true;
}

static char *keep_completer(GB_gameboy_t *gb, const char *string, uintptr_t *context)
{
    size_t length = strlen(string);
    const char *suggestions[] = {"keep"};
    while (*context < sizeof(suggestions) / sizeof(suggestions[0])) {
        if (strncmp(string, suggestions[*context], length) == 0) {
            return strdup(suggestions[(*context)++] + length);
        }
        (*context)++;
    }
    return NULL;
}

static bool ticks(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    STOPPED_ONLY
    bool keep = false;
    if (strcmp(lstrip(arguments), "keep") == 0) {
        keep = true;
    }
    else if (lstrip(arguments)[0]) {
        print_usage(gb, command);
        return true;
    }

    GB_log(gb, "T-cycles: %llu\n", (unsigned long long)gb->debugger_ticks);
    GB_log(gb, "M-cycles: %llu\n", (unsigned long long)gb->debugger_ticks / 4);
    GB_log(gb, "Absolute 8MHz ticks: %llu\n", (unsigned long long)gb->absolute_debugger_ticks);
    if (!keep) {
        GB_log(gb, "Tick count reset.\n");
        gb->debugger_ticks = 0;
        gb->absolute_debugger_ticks = 0;
    }

    return true;
}


static bool palettes(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }

    if (!GB_is_cgb(gb)) {
        GB_log(gb, "Not available on a DMG.\n");
        return true;
    }

    GB_log(gb, "Background palettes: \n");
    for (unsigned i = 0; i < 32; i++) {
        GB_log(gb, "%04x ", ((uint16_t *)&gb->background_palettes_data)[i]);
        if (i % 4 == 3) {
            GB_log(gb, "\n");
        }
    }

    GB_log(gb, "Object palettes: \n");
    for (unsigned i = 0; i < 32; i++) {
        GB_log(gb, "%04x ", ((uint16_t *)&gb->object_palettes_data)[i]);
        if (i % 4 == 3) {
            GB_log(gb, "\n");
        }
    }

    return true;
}

static bool dma(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }
    
    if (!GB_is_dma_active(gb)) {
        GB_log(gb, "DMA is inactive\n");
        return true;
    }
    
    if (gb->dma_current_dest == 0xFF) {
        GB_log(gb, "DMA warming up\n"); // Shouldn't actually happen, as it only lasts 2 T-cycles
        return true;
    }
    
    GB_log(gb, "Next DMA write: [$FE%02X] = [$%04X]\n", gb->dma_current_dest, gb->dma_current_src);
    
    return true;
}

static bool lcd(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }
    GB_log(gb, "LCDC:\n");
    GB_log(gb, "    LCD enabled: %s\n",(gb->io_registers[GB_IO_LCDC] & GB_LCDC_ENABLE)? "Enabled" : "Disabled");
    GB_log(gb, "    %s: %s\n", (gb->cgb_mode? "Object priority flags" : "Background and Window"),
                               (gb->io_registers[GB_IO_LCDC] & GB_LCDC_BG_EN)? "Enabled" : "Disabled");
    GB_log(gb, "    Objects: %s\n", (gb->io_registers[GB_IO_LCDC] & GB_LCDC_OBJ_EN)? "Enabled" : "Disabled");
    GB_log(gb, "    Object size: %s\n", (gb->io_registers[GB_IO_LCDC] & GB_LCDC_OBJ_SIZE)? "8x16" : "8x8");
    GB_log(gb, "    Background tilemap: %s\n", (gb->io_registers[GB_IO_LCDC] & GB_LCDC_BG_MAP)? "$9C00" : "$9800");
    GB_log(gb, "    Background and Window Tileset: %s\n", (gb->io_registers[GB_IO_LCDC] & GB_LCDC_TILE_SEL)? "$8000" : "$8800");
    GB_log(gb, "    Window: %s\n", (gb->io_registers[GB_IO_LCDC] & GB_LCDC_WIN_ENABLE)? "Enabled" : "Disabled");
    GB_log(gb, "    Window tilemap: %s\n", (gb->io_registers[GB_IO_LCDC] & GB_LCDC_WIN_MAP)? "$9C00" : "$9800");

    GB_log(gb, "\nSTAT:\n");
    static const char *modes[] = {"Mode 0, H-Blank", "Mode 1, V-Blank", "Mode 2, OAM", "Mode 3, Rendering"};
    GB_log(gb, "    Current mode: %s\n", modes[gb->io_registers[GB_IO_STAT] & 3]);
    GB_log(gb, "    LYC flag: %s\n", (gb->io_registers[GB_IO_STAT] & 4)? "On" : "Off");
    GB_log(gb, "    H-Blank interrupt: %s\n", (gb->io_registers[GB_IO_STAT] & 8)? "Enabled" : "Disabled");
    GB_log(gb, "    V-Blank interrupt: %s\n", (gb->io_registers[GB_IO_STAT] & 16)? "Enabled" : "Disabled");
    GB_log(gb, "    OAM interrupt: %s\n", (gb->io_registers[GB_IO_STAT] & 32)? "Enabled" : "Disabled");
    GB_log(gb, "    LYC interrupt: %s\n", (gb->io_registers[GB_IO_STAT] & 64)? "Enabled" : "Disabled");



    GB_log(gb, "\nCurrent line: %d\n", gb->current_line);
    GB_log(gb, "Current state: ");
    if (!(gb->io_registers[GB_IO_LCDC] & GB_LCDC_ENABLE)) {
        GB_log(gb, "Off\n");
    }
    else if (gb->display_state == 7 || gb->display_state == 8) {
        GB_log(gb, "Reading OAM data (%d/40)\n", gb->display_state == 8? gb->oam_search_index : 0);
    }
    else if (gb->display_state <= 3 || gb->display_state == 24 || gb->display_state == 31) {
        GB_log(gb, "Glitched line 0 OAM mode (%d cycles to next event)\n", -gb->display_cycles / 2);
    }
    else if (gb->mode_for_interrupt == 3) {
        if (((uint8_t)(gb->position_in_line + 16) < 8)) {
            GB_log(gb, "Adjusting for scrolling (%d/%d)\n", gb->position_in_line & 7, gb->io_registers[GB_IO_SCX] & 7);
        }
        else {
            signed pixel = gb->position_in_line > 160? (int8_t) gb->position_in_line : gb->position_in_line;
            GB_log(gb, "Rendering pixel (%d/160)\n", pixel);
        }
    }
    else {
        GB_log(gb, "Sleeping (%d cycles to next event)\n", -gb->display_cycles / 2);
    }
    GB_log(gb, "LY: %d\n", gb->io_registers[GB_IO_LY]);
    GB_log(gb, "LYC: %d\n", gb->io_registers[GB_IO_LYC]);
    GB_log(gb, "Window position: %d, %d\n", (signed) gb->io_registers[GB_IO_WX] - 7, gb->io_registers[GB_IO_WY]);
    GB_log(gb, "Interrupt line: %s\n", gb->stat_interrupt_line? "On" : "Off");
    GB_log(gb, "Background shifter size: %d\n", gb->bg_fifo.size);
    GB_log(gb, "Background fetcher state: %s\n", inline_const(const char *[], {
        "Tile (1/2)",
        "Tile (2/2)",
        "Low data (1/2)",
        "Low data (2/2)",
        "High data (1/2)",
        "High data (2/2)",
        "Push (1/2)",
        "Push (2/2)",
    })[gb->fetcher_state & 7]);

    return true;
}

static bool apu(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    const char *stripped = lstrip(arguments);
    if (strlen(stripped)) {
        if (stripped[0] != 0 && (stripped[0] < '1' || stripped[0] > '5')) {
            print_usage(gb, command);
            return true;
        }
    }

    if (stripped[0] == 0 || stripped[0] == '5') {
        GB_log(gb, "Current state: ");
        if (!gb->apu.global_enable) {
            GB_log(gb, "Disabled\n");
        }
        else {
            GB_log(gb, "Enabled\n");
            for (uint8_t channel = 0; channel < GB_N_CHANNELS; channel++) {
                GB_log(gb, "CH%u is %s, DAC %s; current sample = 0x%x\n", channel + 1,
                    gb->apu.is_active[channel] ? "active  " : "inactive",
                    GB_apu_is_DAC_enabled(gb, channel) ? "active  " : "inactive",
                    gb->apu.samples[channel]);
            }
        }

        GB_log(gb, "SO1 (left output):  volume %u,", gb->io_registers[GB_IO_NR50] & 0x07);
        if (gb->io_registers[GB_IO_NR51] & 0x0F) {
            for (uint8_t channel = 0, mask = 0x01; channel < GB_N_CHANNELS; channel++, mask <<= 1) {
                if (gb->io_registers[GB_IO_NR51] & mask) {
                    GB_log(gb, " CH%u", channel + 1);
                }
            }
        }
        else {
            GB_log(gb, " no channels");
        }
        GB_log(gb, "%s\n", gb->io_registers[GB_IO_NR50] & 0x80 ? " VIN": "");

        GB_log(gb, "SO2 (right output): volume %u,", gb->io_registers[GB_IO_NR50] & 0x70 >> 4);
        if (gb->io_registers[GB_IO_NR51] & 0xF0) {
            for (uint8_t channel = 0, mask = 0x10; channel < GB_N_CHANNELS; channel++, mask <<= 1) {
                if (gb->io_registers[GB_IO_NR51] & mask) {
                    GB_log(gb, " CH%u", channel + 1);
                }
            }
        }
        else {
            GB_log(gb, " no channels");
        }
        GB_log(gb, "%s\n", gb->io_registers[GB_IO_NR50] & 0x80 ? " VIN": "");
    }


    for (uint8_t channel = GB_SQUARE_1; channel <= GB_SQUARE_2; channel++) {
        if (stripped[0] != 0 && stripped[0] != ('1') + channel) continue;
        
        GB_log(gb, "\nCH%u:\n", channel + 1);
        GB_log(gb, "    Current volume: %u, current sample length: %u APU ticks (next in %u ticks)\n",
             gb->apu.square_channels[channel].current_volume,
            (gb->apu.square_channels[channel].sample_length ^ 0x7FF) * 2 + 1,
             gb->apu.square_channels[channel].sample_countdown);

        uint8_t nrx2 = gb->io_registers[channel == GB_SQUARE_1? GB_IO_NR12 : GB_IO_NR22];
        GB_log(gb, "    %u 256 Hz ticks till next volume %screase (out of %u)\n",
            gb->apu.square_channels[channel].volume_countdown,
            nrx2 & 8 ? "in" : "de",
            nrx2 & 7);

        uint8_t duty = gb->io_registers[channel == GB_SQUARE_1? GB_IO_NR11 :GB_IO_NR21] >> 6;
        GB_log(gb, "    Duty cycle %s%% (%s), current index %u/8%s\n",
               duty > 3? "" : inline_const(const char *[], {"12.5", "  25", "  50", "  75"})[duty],
               duty > 3? "" : inline_const(const char *[], {"_______-", "-______-", "-____---", "_------_"})[duty],
               gb->apu.square_channels[channel].current_sample_index,
               gb->apu.square_channels[channel].sample_surpressed ? " (suppressed)" : "");

        if (channel == GB_SQUARE_1) {
            GB_log(gb, "    Frequency sweep %s and %s\n",
                   ((gb->io_registers[GB_IO_NR10] & 0x7) && (gb->io_registers[GB_IO_NR10] & 0x70))? "active" : "inactive",
                   (gb->io_registers[GB_IO_NR10] & 0x8) ? "decreasing" : "increasing");
            if (gb->apu.square_sweep_calculate_countdown) {
                GB_log(gb, "    On-going frequency calculation will be ready in %u APU ticks\n",
                       gb->apu.square_sweep_calculate_countdown * 2 + 1 - gb->apu.lf_div);
            }
            else {
                GB_log(gb, "    Shadow frequency register: 0x%03x\n", gb->apu.shadow_sweep_sample_length);
                GB_log(gb, "    Sweep addend register: 0x%03x\n", gb->apu.sweep_length_addend);
            }
        }

        if (gb->apu.square_channels[channel].length_enabled) {
            GB_log(gb, "    Channel will end in %u 256 Hz ticks\n",
                gb->apu.square_channels[channel].pulse_length);
        }
    }

    if (stripped[0] == 0 || stripped[0] == '3') {
        GB_log(gb, "\nCH3:\n");
        GB_log(gb, "    Wave:");
        for (uint8_t i = 0; i < 16; i++) {
            GB_log(gb, "%s%X", i % 2? "" : " ", gb->io_registers[GB_IO_WAV_START + i] >> 4);
            GB_log(gb, "%X", gb->io_registers[GB_IO_WAV_START + i] & 0xF);
        }
        GB_log(gb, "\n");
        GB_log(gb, "    Current position: %u\n", gb->apu.wave_channel.current_sample_index);

        GB_log(gb, "    Volume %s (right-shifted %u times)\n",
               gb->apu.wave_channel.shift > 4? "" : inline_const(const char *[], {"100%", "50%", "25%", "", "muted"})[gb->apu.wave_channel.shift],
               gb->apu.wave_channel.shift);

        GB_log(gb, "    Current sample length: %u APU ticks (next in %u ticks)\n",
            gb->apu.wave_channel.sample_length ^ 0x7FF,
            gb->apu.wave_channel.sample_countdown);

        if (gb->apu.wave_channel.length_enabled) {
            GB_log(gb, "    Channel will end in %u 256 Hz ticks\n",
                gb->apu.wave_channel.pulse_length);
        }
    }


    if (stripped[0] == 0 || stripped[0] == '4') {
        GB_log(gb, "\nCH4:\n");
        GB_log(gb, "    Current volume: %u, current internal counter: 0x%04x (next increase in %u ticks)\n",
            gb->apu.noise_channel.current_volume,
            gb->apu.noise_channel.counter,
            gb->apu.noise_channel.counter_countdown);

        GB_log(gb, "    %u 256 Hz ticks till next volume %screase (out of %u)\n",
            gb->apu.noise_channel.volume_countdown,
            gb->io_registers[GB_IO_NR42] & 8 ? "in" : "de",
            gb->io_registers[GB_IO_NR42] & 7);

        GB_log(gb, "    LFSR in %u-step mode, current value ",
            gb->apu.noise_channel.narrow? 7 : 15);
        nounroll for (uint16_t lfsr = gb->apu.noise_channel.lfsr, i = 15; i--; lfsr <<= 1) {
            GB_log(gb, "%u%s", (lfsr >> 14) & 1, i%4 ? "" : " ");
        }

        if (gb->apu.noise_channel.length_enabled) {
            GB_log(gb, "    Channel will end in %u 256 Hz ticks\n",
                gb->apu.noise_channel.pulse_length);
        }
    }


    GB_log(gb, "\n\nReminder: APU ticks are @ 2 MiHz\n");

    return true;
}

static char *wave_completer(GB_gameboy_t *gb, const char *string, uintptr_t *context)
{
    size_t length = strlen(string);
    const char *suggestions[] = {"c", "f", "l"};
    while (*context < sizeof(suggestions) / sizeof(suggestions[0])) {
        if (strncmp(string, suggestions[*context], length) == 0) {
            return strdup(suggestions[(*context)++] + length);
        }
        (*context)++;
    }
    return NULL;
}

static bool wave(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    if (strlen(lstrip(arguments)) || (modifiers && !strchr("fcl", modifiers[0]))) {
        print_usage(gb, command);
        return true;
    }

    uint8_t shift_amount = 1, mask;
    if (modifiers) {
        switch (modifiers[0]) {
            case 'c':
                shift_amount = 2;
                break;
            case 'l':
                shift_amount = 8;
                break;
        }
    }
    mask = (0xF << (shift_amount - 1)) & 0xF;

    for (int8_t cur_val = 0xF & mask; cur_val >= 0; cur_val -= shift_amount) {
        for (uint8_t i = 0; i < 32; i++) {
            uint8_t sample = i & 1?
            (gb->io_registers[GB_IO_WAV_START + i / 2] & 0xF) :
            (gb->io_registers[GB_IO_WAV_START + i / 2] >> 4);
            if ((sample & mask) == cur_val) {
                GB_log(gb, "%X", sample);
            }
            else {
                GB_log(gb, "%c", i % 4 == 2 ? '-' : ' ');
            }
        }
        GB_log(gb, "\n");
    }

    return true;
}

static bool undo(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    STOPPED_ONLY
    
    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }
    
    if (!gb->undo_label) {
        GB_log(gb, "No undo state available\n");
        return true;
    }
    uint16_t pc = gb->pc;
    GB_load_state_from_buffer(gb, gb->undo_state, GB_get_save_state_size_no_bess(gb));
    GB_log(gb, "Reverted a \"%s\" command.\n", gb->undo_label);
    if (pc != gb->pc) {
        GB_cpu_disassemble(gb, gb->pc, 5);
    }
    gb->undo_label = NULL;
    
    return true;
}

#ifndef DISABLE_REWIND
static bool backstep(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command)
{
    NO_MODIFIERS
    STOPPED_ONLY
    
    if (strlen(lstrip(arguments))) {
        print_usage(gb, command);
        return true;
    }
    
    bool didPop = false;
retry:;
    typeof(gb->rewind_sequences[0]) *sequence = &gb->rewind_sequences[gb->rewind_pos];
    if (!gb->rewind_sequences || !sequence->key_state) {
        if (gb->rewind_buffer_length  == 0) {
            GB_log(gb, "Backstepping requires enabling rewinding\n");
        }
        else {
            GB_log(gb, "Reached the end of the rewind buffer\n");
            if (didPop) {
                GB_rewind_push(gb);
                sequence = &gb->rewind_sequences[gb->rewind_pos];
                sequence->instruction_count[sequence->pos] = 1;
            }
        }
        return true;
    }
    
    gb->backstep_instructions = sequence->instruction_count[sequence->pos] - 2;
    if (gb->backstep_instructions == (uint32_t)-1) { // This frame was just pushed, pop it and try again
        GB_rewind_pop(gb);
        gb->backstep_instructions = 0;
        didPop = true;
        goto retry;
    }
    else if (gb->backstep_instructions > 0x20000) {
        GB_log(gb, "Backstepping is currently not available\n");
        gb->backstep_instructions = 0;
        return true;
    }
    GB_rewind_pop(gb);
    GB_rewind_push(gb);
    sequence = &gb->rewind_sequences[gb->rewind_pos];
    sequence->instruction_count[sequence->pos] = 1;
    while (gb->backstep_instructions) {
        GB_run(gb);
    }
    GB_cpu_disassemble(gb, gb->pc, 5);
    return true;
}
#endif

static bool help(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *command);


/* Commands without implementations are aliases of the previous non-alias commands */
static const debugger_command_t commands[] = {
    {"continue", 1, cont, "Continue running until next stop"},
    {"interrupt", 1, interrupt, "Interrupt the program execution"},
    {"reset", 3, reset, "Reset the program execution. "
                        "Add 'quick' as an argument to perform a quick reset that does not reset RAM. "
                        "Add 'reload' as an argument to reload the ROM and symbols before resetting.",
                        "[quick|reload]", .argument_completer = reset_completer},
    {"next", 1, next, "Run the next instruction, skipping over function calls"},
    {"step", 1, step, "Run the next instruction, stepping into function calls"},
    {"finish", 1, finish, "Run until the current function returns"},
#ifndef DISABLE_REWIND
    {"backstep", 5, backstep, "Step one instruction backward, assuming constant inputs"},
    {"bs", 2, }, /* Alias */
#endif
    {"undo", 1, undo, "Revert the last command"},
    {"registers", 1, registers, "Print values of processor registers and other important registers"},
    {"backtrace", 2, backtrace, "Display the current call stack"},
    {"bt", 2, }, /* Alias */
    {"print", 1, print, "Evaluate and print an expression. "
                        "Use modifier to format as an address (a, default) or as a number in "
                        "decimal (d), hexadecimal (x), octal (o) or binary (b).",
                        "<expression>", "format", .argument_completer = symbol_completer, .modifiers_completer = format_completer},
    {"eval", 2, }, /* Alias */
    {"examine", 2, examine, "Examine values at address", "<expression>", "count", .argument_completer = symbol_completer},
    {"x", 1, }, /* Alias */
    {"disassemble", 1, disassemble, "Disassemble instructions at address", "<expression>", "count", .argument_completer = symbol_completer},
    {"breakpoint", 1, breakpoint, "Add a new breakpoint at the specified address/expression or range. "
                                  "Ranges are exclusive by default, unless \"inclusive\" is used. "
                                  "If the j modifier is used, the breakpoint will occur just before "
                                  "jumping to the target.",
                                  "<expression> [to <end expression> [inclusive]] [if <condition expression>]", "j",
                                  .argument_completer = symbol_completer, .modifiers_completer = j_completer},
    {"delete", 2, delete, "Delete a breakpoint by its identifier, or all breakpoints", "[<breakpoint id>]"},
    {"watch", 1, watch, "Add a new watchpoint at the specified address/expression or range. "
                        "Ranges are exclusive by default, unless \"inclusive\" is used. "
                        "The default watchpoint type is write-only.",
                        "<expression> [to <end expression> [inclusive]] [if <condition expression>]", "(r|w|rw)",
                        .argument_completer = symbol_completer, .modifiers_completer = rw_completer
    },
    {"unwatch", 3, unwatch, "Delete a watchpoint by its identifier, or all watchpoints", "[<watchpoint id>]"},
    {"softbreak", 2, softbreak, "Enable or disable software breakpoints ('ld b, b' opcodes)", "(on|off)", .argument_completer = on_off_completer},
    {"list", 1, list, "List all set breakpoints and watchpoints"},
    {"ticks", 2, ticks, "Display the number of CPU ticks since the last time 'ticks' was "
                        "used. Use 'keep' as an argument to display ticks without reseeting "
                        "the count.", "(keep)", .argument_completer = keep_completer},
    {"cartridge", 2, mbc, "Display information about the MBC and cartridge"},
    {"mbc", 3, }, /* Alias */
    {"apu", 3, apu, "Display information about the current state of the audio processing unit",
                    "[channel (1-4, 5 for NR5x)]"},
    {"wave", 3, wave, "Print a visual representation of the wave RAM. "
                      "Modifiers can be used for a (f)ull print (the default), "
                      "a more (c)ompact one, or a one-(l)iner", "", "(f|c|l)", .modifiers_completer = wave_completer},
    {"lcd", 3, lcd, "Display information about the current state of the LCD controller"},
    {"palettes", 3, palettes, "Display the current CGB palettes"},
    {"dma", 3, dma, "Display the current OAM DMA status"},

    {"help", 1, help, "List available commands or show help for the specified command", "[<command>]"},
    {NULL,}, /* Null terminator */
};

static const debugger_command_t *find_command(const char *string)
{
    size_t length = strlen(string);
    for (const debugger_command_t *command = commands; command->command; command++) {
        if (command->min_length > length) continue;
        if (strncmp(command->command, string, length) == 0) { /* Is a substring? */
            /* Aliases */
            while (!command->implementation) {
                command--;
            }
            return command;
        }
    }

    return NULL;
}

static void print_command_shortcut(GB_gameboy_t *gb, const debugger_command_t *command)
{
    GB_attributed_log(gb, GB_LOG_BOLD | GB_LOG_UNDERLINE, "%.*s", command->min_length, command->command);
    GB_attributed_log(gb, GB_LOG_BOLD, "%s", command->command + command->min_length);
}

static void print_command_description(GB_gameboy_t *gb, const debugger_command_t *command)
{
    print_command_shortcut(gb, command);
    GB_log(gb, ": ");
    GB_log(gb, "%s", (const char *)&"           " + strlen(command->command));
    
    const char *string = command->help_string;
    const unsigned width = 80 - 13;
    nounroll while (strlen(string) > width) {
        const char *space = string + width;
        nounroll while (*space != ' ') {
            space--;
            if (space == string) {
                // This help string has some extra long word? Abort line-breaking, it's going to break anyway.
                GB_log(gb, "%s\n", string);
                return;
            }
        }
        GB_log(gb, "%.*s\n             ", (unsigned)(space - string), string);
        string = space + 1;
    }
    GB_log(gb, "%s\n", string);
}

static bool help(GB_gameboy_t *gb, char *arguments, char *modifiers, const debugger_command_t *ignored)
{
    const debugger_command_t *command = find_command(arguments);
    if (command) {
        print_command_description(gb, command);
        GB_log(gb, "\n");
        print_usage(gb, command);

        command++;
        if (command->command && !command->implementation) { /* Command has aliases*/
            GB_log(gb, "\nAliases: ");
            do {
                print_command_shortcut(gb, command);
                GB_log(gb, " ");
                command++;
            } while (command->command && !command->implementation);
            GB_log(gb, "\n");
        }
        return true;
    }
    nounroll for (command = commands; command->command; command++) {
        if (command->help_string) {
            print_command_description(gb, command);
        }
    }
    return true;
}

void GB_debugger_call_hook(GB_gameboy_t *gb, uint16_t call_addr)
{
    /* Called just after the CPU calls a function/enters an interrupt/etc... */

    if (gb->backtrace_size < sizeof(gb->backtrace_sps) / sizeof(gb->backtrace_sps[0])) {
        while (gb->backtrace_size) {
            if (gb->backtrace_sps[gb->backtrace_size - 1] <= gb->sp) {
                gb->backtrace_size--;
                gb->debug_call_depth--;
            }
            else {
                break;
            }
        }

        gb->backtrace_sps[gb->backtrace_size] = gb->sp;
        gb->backtrace_returns[gb->backtrace_size].bank = bank_for_addr(gb, call_addr);
        gb->backtrace_returns[gb->backtrace_size].addr = call_addr;
        gb->backtrace_size++;
        gb->debug_call_depth++;
    }
}

void GB_debugger_ret_hook(GB_gameboy_t *gb)
{
    /* Called just before the CPU runs ret/reti */

    while (gb->backtrace_size) {
        if (gb->backtrace_sps[gb->backtrace_size - 1] <= gb->sp) {
            gb->backtrace_size--;
            gb->debug_call_depth--;
        }
        else {
            break;
        }
    }
}


// Returns the id or 0
static void test_watchpoint(GB_gameboy_t *gb, uint16_t addr, uint8_t flags, uint8_t value)
{
    if (unlikely(gb->backstep_instructions)) return;
    uint16_t bank = bank_for_addr(gb, addr);
    for (unsigned i = 0; i < gb->n_watchpoints; i++) {
        struct GB_watchpoint_s *watchpoint = &gb->watchpoints[i];
        if (watchpoint->bank != (uint16_t)-1) {
            if (watchpoint->bank != bank) continue;
        }
        if (!(watchpoint->flags & flags)) continue;
        if (addr < watchpoint->addr) continue;
        if (addr > (uint32_t)watchpoint->addr + watchpoint->length + watchpoint->inclusive) continue;
        if (!watchpoint->condition) {
        condition_ok:
            GB_debugger_break(gb);
            if (flags == WATCHPOINT_READ) {
                GB_log(gb, "Watchpoint %u: [%s]\n", watchpoint->id, value_to_string(gb, addr, true, false));
            }
            else {
                GB_log(gb, "Watchpoint %u: [%s] = $%02x\n", watchpoint->id, value_to_string(gb, addr, true, false), value);
            }
            return;
        }
        bool error;
        evaluate_conf_t conf = {
            .old_as_value = flags == WATCHPOINT_READ,
            .new_value = value,
        };
        if (flags == WATCHPOINT_READ) {
            conf.old_value = value;
        }
        else {
            conf.old_address = addr;
        }
        bool condition = debugger_evaluate(gb, watchpoint->condition,
                                           (unsigned)strlen(watchpoint->condition),
                                           &error, &conf).value;
        if (error) {
            GB_log(gb, "The condition for watchpoint %u is no longer a valid expression\n", watchpoint->id);
            GB_debugger_break(gb);
        }
        if (condition) {
            goto condition_ok;
        }
    }
}

void GB_debugger_test_write_watchpoint(GB_gameboy_t *gb, uint16_t addr, uint8_t value)
{
    if (gb->debug_stopped) return;
    test_watchpoint(gb, addr, WATCHPOINT_WRITE, value);
}


void GB_debugger_test_read_watchpoint(GB_gameboy_t *gb, uint16_t addr)
{
    if (gb->debug_stopped) return;
    test_watchpoint(gb, addr, WATCHPOINT_READ, 0);
}

/* Returns true if debugger waits for more commands */
bool GB_debugger_execute_command(GB_gameboy_t *gb, char *input)
{
    GB_ASSERT_NOT_RUNNING_OTHER_THREAD(gb)
    
    while (*input == ' ') {
        input++;
    }
    if (!input[0]) {
        return true;
    }
    
    GB_display_sync(gb);
    GB_apu_run(gb, true);

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

    char *modifiers = strchr(command_string, '/');
    if (modifiers) {
        /* Actually "split" the string. */
        modifiers[0] = 0;
        modifiers++;
    }

    gb->help_shown = true;
    const debugger_command_t *command = find_command(command_string);
    if (command) {
        uint8_t *old_state = malloc(GB_get_save_state_size_no_bess(gb));
        GB_save_state_to_buffer_no_bess(gb, old_state);
        bool ret = command->implementation(gb, arguments, modifiers, command);
        if (!ret) { // Command continues, save state in any case
            free(gb->undo_state);
            gb->undo_state = old_state;
            gb->undo_label = command->command;
        }
        else {
            uint8_t *new_state = malloc(GB_get_save_state_size_no_bess(gb));
            GB_save_state_to_buffer_no_bess(gb, new_state);
            if (memcmp(new_state, old_state, GB_get_save_state_size_no_bess(gb)) != 0) {
                // State changed, save the old state as the new undo state
                free(gb->undo_state);
                gb->undo_state = old_state;
                gb->undo_label = command->command;
            }
            else {
                // Nothing changed, just free the old state
                free(old_state);
            }
            free(new_state);
        }
        return ret;
    }
    else {
        GB_log(gb, "%s: no such command. Type 'help' to list the available debugger commands.\n", command_string);
        return true;
    }
}

char *GB_debugger_complete_substring(GB_gameboy_t *gb, char *input, uintptr_t *context)
{   
    char *command_string = input;
    char *arguments = strchr(input, ' ');
    if (arguments) {
        /* Actually "split" the string. */
        arguments[0] = 0;
        arguments++;
    }
    
    char *modifiers = strchr(command_string, '/');
    if (modifiers) {
        /* Actually "split" the string. */
        modifiers[0] = 0;
        modifiers++;
    }
    
    const debugger_command_t *command = find_command(command_string);
    if (command && command->implementation == help && arguments) {
        command_string = arguments;
        arguments = NULL;
    }
    
    /* No commands and no modifiers, complete the command */
    if (!arguments && !modifiers) {
        size_t length = strlen(command_string);
        if (*context >= sizeof(commands) / sizeof(commands[0])) {
            return NULL;
        }
        for (const debugger_command_t *command = &commands[*context]; command->command; command++) {
            (*context)++;
            if (strncmp(command->command, command_string, length) == 0) { /* Is a substring? */
                return strdup(command->command + length);
            }
        }
        return NULL;
    }
    
    if (command) {
        if (arguments) {
            if (command->argument_completer) {
                return command->argument_completer(gb, arguments, context);
            }
            return NULL;
        }
        
        if (modifiers) {
            if (command->modifiers_completer) {
                return command->modifiers_completer(gb, modifiers, context);
            }
            return NULL;
        }
    }
    return NULL;
}

typedef enum {
    JUMP_TO_NONE,
    JUMP_TO_BREAK,
    JUMP_TO_NONTRIVIAL,
} jump_to_return_t;

static jump_to_return_t test_jump_to_breakpoints(GB_gameboy_t *gb, uint16_t *address, unsigned *breakpoint_id);

static void noinline debugger_run(GB_gameboy_t *gb)
{
    if (!gb->undo_state) {
        gb->undo_state = malloc(GB_get_save_state_size_no_bess(gb));
        GB_save_state_to_buffer_no_bess(gb, gb->undo_state);
    }

    char *input = NULL;
    if (gb->debug_next_command && gb->debug_call_depth <= 0 && !gb->halted) {
        GB_debugger_break(gb);
    }
    if (gb->debug_fin_command && gb->debug_call_depth <= -1) {
        GB_debugger_break(gb);
    }
    if (gb->debug_stopped) {
        if (!gb->help_shown) {
            gb->help_shown = true;
            GB_log(gb, "Type 'help' to list the available debugger commands.\n");
        }
        GB_cpu_disassemble(gb, gb->pc, 5);
    }
next_command:
    if (input) {
        free(input);
    }
    unsigned breakpoint_id = 0;
    if (gb->breakpoints && !gb->debug_stopped && (breakpoint_id = should_break(gb, gb->pc, false))) {
        GB_debugger_break(gb);
        GB_log(gb, "Breakpoint %u: PC = %s\n", breakpoint_id, value_to_string(gb, gb->pc, true, false));
        GB_cpu_disassemble(gb, gb->pc, 5);
    }

    if (gb->breakpoints && !gb->debug_stopped) {
        uint16_t address = 0;
        jump_to_return_t jump_to_result = test_jump_to_breakpoints(gb, &address, &breakpoint_id);

        bool should_delete_state = true;
        if (jump_to_result == JUMP_TO_BREAK) {
            GB_debugger_break(gb);
            GB_log(gb, "Jumping to breakpoint %u: %s\n", breakpoint_id, value_to_string(gb, address, true, false));
            GB_cpu_disassemble(gb, gb->pc, 5);
            gb->non_trivial_jump_breakpoint_occured = false;
        }
        else if (gb->nontrivial_jump_state && (breakpoint_id = should_break(gb, gb->pc, true))) {
            if (gb->non_trivial_jump_breakpoint_occured) {
                gb->non_trivial_jump_breakpoint_occured = false;
            }
            else {
                gb->non_trivial_jump_breakpoint_occured = true;
                GB_log(gb, "Jumping to breakpoint %u: %s\n", breakpoint_id, value_to_string(gb, gb->pc, true, false));
                GB_load_state_from_buffer(gb, gb->nontrivial_jump_state, -1);
                GB_rewind_push(gb);
                GB_cpu_disassemble(gb, gb->pc, 5);
                GB_debugger_break(gb);
            }
        }
        else if (jump_to_result == JUMP_TO_NONTRIVIAL) {
            if (!gb->nontrivial_jump_state) {
                gb->nontrivial_jump_state = malloc(GB_get_save_state_size_no_bess(gb));
            }
            GB_save_state_to_buffer_no_bess(gb, gb->nontrivial_jump_state);
            gb->non_trivial_jump_breakpoint_occured = false;
            should_delete_state = false;
        }
        else {
            gb->non_trivial_jump_breakpoint_occured = false;
        }

        if (should_delete_state) {
            if (gb->nontrivial_jump_state) {
                free(gb->nontrivial_jump_state);
                gb->nontrivial_jump_state = NULL;
            }
        }
    }

    if (gb->debug_stopped && !gb->debug_disable) {
        gb->debug_next_command = false;
        gb->debug_fin_command = false;
        input = gb->input_callback(gb);

        if (input == NULL) {
            /* Debugging is no currently available, continue running */
            gb->debug_stopped = false;
            update_debug_active(gb);
            return;
        }

        if (GB_debugger_execute_command(gb, input)) {
            goto next_command;
        }

        free(input);
    }
    update_debug_active(gb);
}
void GB_debugger_run(GB_gameboy_t *gb)
{
#ifndef DISABLE_REWIND
    if (gb->rewind_sequences && gb->rewind_sequences[gb->rewind_pos].key_state) {
        typeof(gb->rewind_sequences[0]) *sequence = &gb->rewind_sequences[gb->rewind_pos];
        sequence->instruction_count[sequence->pos]++;
    }
    if (unlikely(gb->backstep_instructions)) {
        gb->backstep_instructions--;
        return;
    }
#endif
    if (likely(!gb->debug_active)) return;
    debugger_run(gb);
}

void GB_debugger_handle_async_commands(GB_gameboy_t *gb)
{
    char *input = NULL;

    while (gb->async_input_callback && (input = gb->async_input_callback(gb))) {
        GB_debugger_execute_command(gb, input);
        update_debug_active(gb);
        free(input);
    }
}

void GB_debugger_add_symbol(GB_gameboy_t *gb, uint16_t bank, uint16_t address, const char *symbol)
{
    if (bank >= gb->n_symbol_maps) {
        gb->bank_symbols = realloc(gb->bank_symbols, (bank + 1) * sizeof(*gb->bank_symbols));
        while (bank >= gb->n_symbol_maps) {
            gb->bank_symbols[gb->n_symbol_maps++] = NULL;
        }
    }

    if (!gb->bank_symbols[bank]) {
        gb->bank_symbols[bank] = GB_map_alloc();
    }
    GB_bank_symbol_t *allocated_symbol = GB_map_add_symbol(gb->bank_symbols[bank], address, symbol);
    if (allocated_symbol) {
        GB_reversed_map_add_symbol(&gb->reversed_symbol_map, bank, allocated_symbol);
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

        unsigned bank, address;
        char symbol[length];

        if (sscanf(line, "%x:%x %s", &bank, &address, symbol) == 3) {
            GB_debugger_add_symbol(gb, bank, address, symbol);
        }
    }
    free(line);
    fclose(f);
}

void GB_debugger_clear_symbols(GB_gameboy_t *gb)
{
    for (unsigned i = gb->n_symbol_maps; i--;) {
        if (gb->bank_symbols[i]) {
            GB_map_free(gb->bank_symbols[i]);
            gb->bank_symbols[i] = 0;
        }
    }
    for (unsigned i = sizeof(gb->reversed_symbol_map.buckets) / sizeof(gb->reversed_symbol_map.buckets[0]); i--;) {
        while (gb->reversed_symbol_map.buckets[i]) {
            GB_symbol_t *next = gb->reversed_symbol_map.buckets[i]->next;
            free(gb->reversed_symbol_map.buckets[i]);
            gb->reversed_symbol_map.buckets[i] = next;
        }
    }
    gb->n_symbol_maps = 0;
    if (gb->bank_symbols) {
        free(gb->bank_symbols);
        gb->bank_symbols = NULL;
    }
}

const GB_bank_symbol_t *GB_debugger_find_symbol(GB_gameboy_t *gb, uint16_t addr, bool prefer_local)
{
    uint16_t bank = bank_for_addr(gb, addr);

    const GB_bank_symbol_t *symbol = GB_map_find_symbol(get_symbol_map(gb, bank), addr, prefer_local);
    if (symbol) return symbol;
    if (bank != 0) return GB_map_find_symbol(get_symbol_map(gb, 0), addr, prefer_local); /* Maybe the symbol incorrectly uses bank 0? */

    return NULL;
}

const char *GB_debugger_name_for_address(GB_gameboy_t *gb, uint16_t addr)
{
    return GB_debugger_describe_address(gb, addr, -1, true, false);
}

const char *GB_debugger_describe_address(GB_gameboy_t *gb,
                                         uint16_t addr, uint16_t bank,
                                         bool exact_match, bool prefer_local)
{
    if (bank == (uint16_t)-1) {
        bank = bank_for_addr(gb, addr);
    }
    if (exact_match) {
        const GB_bank_symbol_t *symbol = GB_map_find_symbol(get_symbol_map(gb, bank), addr, prefer_local);
        if (symbol && symbol->addr == addr) return symbol->name;
        if (bank != 0) symbol = GB_map_find_symbol(get_symbol_map(gb, 0), addr, prefer_local); /* Maybe the symbol incorrectly uses bank 0? */
        if (symbol && symbol->addr == addr) return symbol->name;
        
        return NULL;
    }
    
    return debugger_value_to_string(gb, (value_t){
        .value = addr,
        .bank = bank,
        .has_bank = true,
    }, true, prefer_local);
}

/* The public version of debugger_evaluate */
bool GB_debugger_evaluate(GB_gameboy_t *gb, const char *string, uint16_t *result, uint16_t *result_bank)
{
    GB_ASSERT_NOT_RUNNING_OTHER_THREAD(gb)
    
    bool error = false;
    value_t value = debugger_evaluate(gb, string, strlen(string), &error, NULL);
    if (result) {
        *result = value.value;
    }
    if (result_bank) {
        *result_bank = value.has_bank? value.bank : -1;
    }
    return error;
}

#ifndef GB_DISABLE_CHEAT_SEARCH
internal bool GB_debugger_evaluate_cheat_filter(GB_gameboy_t *gb, const char *string, bool *result, uint16_t old, uint16_t new)
{
    GB_ASSERT_NOT_RUNNING_OTHER_THREAD(gb)
    
    bool error = false;
    evaluate_conf_t conf = {
        .old_as_value = true,
        .old_value = old,
        .new_value = new,
    };
    value_t value = debugger_evaluate(gb, string, strlen(string), &error, &conf);
    if (result) {
        *result = value.value;
    }

    return error;
}
#endif

void GB_debugger_break(GB_gameboy_t *gb)
{
    gb->debug_stopped = true;
    update_debug_active(gb);
}

bool GB_debugger_is_stopped(GB_gameboy_t *gb)
{
    return gb->debug_stopped;
}

void GB_debugger_set_disabled(GB_gameboy_t *gb, bool disabled)
{
    gb->debug_disable = disabled;
    update_debug_active(gb);
}

void GB_debugger_set_reload_callback(GB_gameboy_t *gb, GB_debugger_reload_callback_t callback)
{
    gb->debugger_reload_callback = callback;
}

/* Jump-to breakpoints */

static bool is_in_trivial_memory(uint16_t addr)
{
    /* ROM */
    if (addr < 0x8000) {
        return true;
    }

    /* HRAM */
    if (addr >= 0xFF80 && addr < 0xFFFF) {
        return true;
    }

    /* RAM */
    if (addr >= 0xC000 && addr < 0xE000) {
        return true;
    }

    return false;
}

typedef uint16_t opcode_address_getter_t(GB_gameboy_t *gb, uint8_t opcode);

static uint16_t trivial_1(GB_gameboy_t *gb, uint8_t opcode)
{
    return gb->pc + 1;
}

static uint16_t trivial_2(GB_gameboy_t *gb, uint8_t opcode)
{
    return gb->pc + 2;
}

static uint16_t trivial_3(GB_gameboy_t *gb, uint8_t opcode)
{
    return gb->pc + 3;
}

static uint16_t jr_r8(GB_gameboy_t *gb, uint8_t opcode)
{
    return gb->pc + 2 + (int8_t)GB_read_memory(gb, gb->pc + 1);
}

static bool condition_code(GB_gameboy_t *gb, uint8_t opcode)
{
    switch ((opcode >> 3) & 0x3) {
        case 0:
            return !(gb->af & GB_ZERO_FLAG);
        case 1:
            return (gb->af & GB_ZERO_FLAG);
        case 2:
            return !(gb->af & GB_CARRY_FLAG);
        case 3:
            return (gb->af & GB_CARRY_FLAG);
    }

    return false;
}

static uint16_t jr_cc_r8(GB_gameboy_t *gb, uint8_t opcode)
{
    if (!condition_code(gb, opcode)) {
        return gb->pc + 2;
    }

    return gb->pc + 2 + (int8_t)GB_read_memory(gb, gb->pc + 1);
}

static uint16_t ret(GB_gameboy_t *gb, uint8_t opcode)
{
    return GB_read_memory(gb, gb->sp) |
           (GB_read_memory(gb, gb->sp + 1) << 8);
}


static uint16_t ret_cc(GB_gameboy_t *gb, uint8_t opcode)
{
    if (condition_code(gb, opcode)) {
        return ret(gb, opcode);
    }
    else {
        return gb->pc + 1;
    }
}

static uint16_t jp_a16(GB_gameboy_t *gb, uint8_t opcode)
{
    return GB_read_memory(gb, gb->pc + 1) |
           (GB_read_memory(gb, gb->pc + 2) << 8);
}

static uint16_t jp_cc_a16(GB_gameboy_t *gb, uint8_t opcode)
{
    if (condition_code(gb, opcode)) {
        return jp_a16(gb, opcode);
    }
    else {
        return gb->pc + 3;
    }
}

static uint16_t rst(GB_gameboy_t *gb, uint8_t opcode)
{
    return opcode ^ 0xC7;
}

static uint16_t jp_hl(GB_gameboy_t *gb, uint8_t opcode)
{
    return gb->hl;
}

static opcode_address_getter_t *opcodes[256] = {
    /*  X0          X1          X2          X3          X4          X5          X6          X7                */
    /*  X8          X9          Xa          Xb          Xc          Xd          Xe          Xf                */
    trivial_1,  trivial_3,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_2,  trivial_1,   /* 0X */
    trivial_3,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_2,  trivial_1,
    trivial_2,  trivial_3,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_2,  trivial_1,  /* 1X */
    jr_r8,      trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_2,  trivial_1,
    jr_cc_r8,   trivial_3,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_2,  trivial_1,  /* 2X */
    jr_cc_r8,   trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_2,  trivial_1,
    jr_cc_r8,   trivial_3,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_2,  trivial_1,  /* 3X */
    jr_cc_r8,   trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_2,  trivial_1,
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  /* 4X */
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  /* 5X */
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  /* 6X */
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  NULL,       trivial_1,  /* 7X */
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  /* 8X */
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  /* 9X */
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  /* aX */
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  /* bX */
    trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,  trivial_1,
    ret_cc,     trivial_1,  jp_cc_a16,  jp_a16,     jp_cc_a16,  trivial_1,  trivial_2,  rst,        /* cX */
    ret_cc,     ret,        jp_cc_a16,  trivial_2,  jp_cc_a16,  jp_a16,     trivial_2,  rst,
    ret_cc,     trivial_1,  jp_cc_a16,  NULL,       jp_cc_a16,  trivial_1,  trivial_2,  rst,        /* dX */
    ret_cc,     ret,        jp_cc_a16,  NULL,       jp_cc_a16,  NULL,       trivial_2,  rst,
    trivial_2,  trivial_1,  trivial_1,  NULL,       NULL,       trivial_1,  trivial_2,  rst,        /* eX */
    trivial_2,  jp_hl,      trivial_3,  NULL,       NULL,       NULL,       trivial_2,  rst,
    trivial_2,  trivial_1,  trivial_1,  trivial_1,  NULL,       trivial_1,  trivial_2,  rst,        /* fX */
    trivial_2,  trivial_1,  trivial_3,  trivial_1,  NULL,       NULL,       trivial_2,  rst,
};

static jump_to_return_t test_jump_to_breakpoints(GB_gameboy_t *gb, uint16_t *address, unsigned *breakpoint_id)
{
    if (!gb->has_jump_to_breakpoints) return JUMP_TO_NONE;

    if (!is_in_trivial_memory(gb->pc) || !is_in_trivial_memory(gb->pc + 2) ||
        !is_in_trivial_memory(gb->sp) || !is_in_trivial_memory(gb->sp - 1)) {
        return JUMP_TO_NONTRIVIAL;
    }

    /* Interrupts */
    if (gb->ime) {
        for (unsigned i = 0; i < 5; i++) {
            if ((gb->interrupt_enable & (1 << i)) && (gb->io_registers[GB_IO_IF] & (1 << i))) {
                if ((*breakpoint_id = should_break(gb, 0x40 + i * 8, true))) {
                    if (address) {
                        *address = 0x40 + i * 8;
                    }
                    return JUMP_TO_BREAK;
                }
            }
        }
    }

    uint16_t n_watchpoints = gb->n_watchpoints;
    gb->n_watchpoints = 0;

    uint8_t opcode = GB_read_memory(gb, gb->pc);

    if (opcode == 0x76) {
        gb->n_watchpoints = n_watchpoints;
        if (gb->ime) { /* Already handled in above */
            return JUMP_TO_NONE;
        }

        if (gb->interrupt_enable & gb->io_registers[GB_IO_IF] & 0x1F) {
            return JUMP_TO_NONTRIVIAL; /* HALT bug could occur */
        }

        return JUMP_TO_NONE;
    }

    opcode_address_getter_t *getter = opcodes[opcode];
    if (!getter) {
        gb->n_watchpoints = n_watchpoints;
        return JUMP_TO_NONE;
    }

    uint16_t new_pc = getter(gb, opcode);

    gb->n_watchpoints = n_watchpoints;

    if (address) {
        *address = new_pc;
    }

    return (*breakpoint_id = should_break(gb, new_pc, true)) ? JUMP_TO_BREAK : JUMP_TO_NONE;
}
