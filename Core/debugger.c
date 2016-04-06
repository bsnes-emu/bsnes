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
        unsigned short *register_address;
        unsigned short memory_address;
    };
} lvalue_t;

static unsigned short read_lvalue(GB_gameboy_t *gb, lvalue_t lvalue)
{
    /* Not used until we add support for operators like += */
    switch (lvalue.kind) {
        case LVALUE_MEMORY:
            return read_memory(gb, lvalue.memory_address);

        case LVALUE_REG16:
            return *lvalue.register_address;

        case LVALUE_REG_L:
            return *lvalue.register_address & 0x00FF;

        case LVALUE_REG_H:
            return *lvalue.register_address >> 8;
    }
}

static void write_lvalue(GB_gameboy_t *gb, lvalue_t lvalue, unsigned short value)
{
    switch (lvalue.kind) {
        case LVALUE_MEMORY:
            write_memory(gb, lvalue.memory_address, value);
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

static unsigned short add(unsigned short a, unsigned short b) {return  a + b;};
static unsigned short sub(unsigned short a, unsigned short b) {return  a - b;};
static unsigned short mul(unsigned short a, unsigned short b) {return  a * b;};
static unsigned short _div(unsigned short a, unsigned short b) {
    if (b == 0) {
        return 0;
    }
    return  a / b;
};
static unsigned short mod(unsigned short a, unsigned short b) {
    if (b == 0) {
        return 0;
    }
    return  a % b;
};
static unsigned short and(unsigned short a, unsigned short b) {return  a & b;};
static unsigned short or(unsigned short a, unsigned short b) {return  a | b;};
static unsigned short xor(unsigned short a, unsigned short b) {return  a ^ b;};
static unsigned short shleft(unsigned short a, unsigned short b) {return  a << b;};
static unsigned short shright(unsigned short a, unsigned short b) {return  a >> b;};
static unsigned short assign(GB_gameboy_t *gb, lvalue_t a, unsigned short b)
{
    write_lvalue(gb, a, b);
    return read_lvalue(gb, a);
}

static struct {
    const char *string;
    char priority;
    unsigned short (*operator)(unsigned short, unsigned short);
    unsigned short (*lvalue_operator)(GB_gameboy_t *, lvalue_t, unsigned short);
} operators[] =
{
    // Yes. This is not C-like. But it makes much more sense.
    // Deal with it.
    {"+", 0, add},
    {"-", 0, sub},
    {"|", 0, or},
    {"*", 1, mul},
    {"/", 1, _div},
    {"%", 1, mod},
    {"&", 1, and},
    {"^", 1, xor},
    {"<<", 2, shleft},
    {">>", 2, shright},
    {"=", 2, NULL, assign},
};

unsigned short debugger_evaluate(GB_gameboy_t *gb, const char *string, unsigned int length, bool *error);

static lvalue_t debugger_evaluate_lvalue(GB_gameboy_t *gb, const char *string, unsigned int length, bool *error)
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
        gb_log(gb, "Expected expression.\n");
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
        if (depth == 0) return debugger_evaluate_lvalue(gb, string + 1, length - 2, error);
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
            return (lvalue_t){LVALUE_MEMORY, .memory_address = debugger_evaluate(gb, string + 1, length - 2, error)};
        }
    }

    // Registers
    if (string[0] == '$') {
        if (length == 2) {
            switch (string[1]) {
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
        else if (length == 3) {
            switch (string[1]) {
                case 'a': if (string[2] == 'f') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_AF]};
                case 'b': if (string[2] == 'c') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_BC]};
                case 'd': if (string[2] == 'e') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_DE]};
                case 'h': if (string[2] == 'l') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_HL]};
                case 's': if (string[2] == 'p') return (lvalue_t){LVALUE_REG16, .register_address = &gb->registers[GB_REGISTER_SP]};
                case 'p': if (string[2] == 'c') return (lvalue_t){LVALUE_REG16, .register_address = &gb->pc};
            }
        }
        gb_log(gb, "Unknown register: %.*s\n", length, string);
        *error = true;
        return (lvalue_t){0,};
    }

    gb_log(gb, "Expression is not an lvalue: %.*s\n", length, string);
    *error = true;
    return (lvalue_t){0,};
}

unsigned short debugger_evaluate(GB_gameboy_t *gb, const char *string, unsigned int length, bool *error)
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
        gb_log(gb, "Expected expression.\n");
        *error = true;
        return -1;
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
        if (depth == 0) return debugger_evaluate(gb, string + 1, length - 2, error);
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
        if (depth == 0) return read_memory(gb, debugger_evaluate(gb, string + 1, length - 2, error));
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
                if (operator_index != -1 && operators[operator_index].priority > operators[j].priority) continue;
                if (memcmp(string + i, operators[j].string, strlen(operators[j].string)) == 0) {
                    // Found an operator!
                    operator_pos = i;
                    operator_index = j;
                }
            }
        }
    }
    if (operator_index != -1) {
        unsigned int right_start = (unsigned int)(operator_pos + strlen(operators[operator_index].string));
        unsigned short right = debugger_evaluate(gb, string + right_start, length - right_start, error);
        if (*error) return  -1;
        if (operators[operator_index].lvalue_operator) {
            lvalue_t left = debugger_evaluate_lvalue(gb, string, operator_pos, error);
            if (*error) return  -1;
            return operators[operator_index].lvalue_operator(gb, left, right);
        }
        unsigned short left = debugger_evaluate(gb, string, operator_pos, error);
        if (*error) return  -1;
        return operators[operator_index].operator(left, right);
    }

    // Not an expression - must be a register or a literal

    // Registers
    if (string[0] == '$') {
        if (length == 2) {
            switch (string[1]) {
                case 'a': return gb->registers[GB_REGISTER_AF] >> 8;
                case 'f': return gb->registers[GB_REGISTER_AF] & 0xFF;
                case 'b': return gb->registers[GB_REGISTER_BC] >> 8;
                case 'c': return gb->registers[GB_REGISTER_BC] & 0xFF;
                case 'd': return gb->registers[GB_REGISTER_DE] >> 8;
                case 'e': return gb->registers[GB_REGISTER_DE] & 0xFF;
                case 'h': return gb->registers[GB_REGISTER_HL] >> 8;
                case 'l': return gb->registers[GB_REGISTER_HL] & 0xFF;
            }
        }
        else if (length == 3) {
            switch (string[1]) {
                case 'a': if (string[2] == 'f') return gb->registers[GB_REGISTER_AF];
                case 'b': if (string[2] == 'c') return gb->registers[GB_REGISTER_BC];
                case 'd': if (string[2] == 'e') return gb->registers[GB_REGISTER_DE];
                case 'h': if (string[2] == 'l') return gb->registers[GB_REGISTER_HL];
                case 's': if (string[2] == 'p') return gb->registers[GB_REGISTER_SP];
                case 'p': if (string[2] == 'c') return gb->pc;
            }
        }
        gb_log(gb, "Unknown register: %.*s\n", length, string);
        *error = true;
        return -1;
    }

    char *end;
    unsigned short literal = (unsigned short) (strtol(string, &end, 16));
    if (end != string + length) {
        gb_log(gb, "Failed to parse: %.*s\n", length, string);
        *error = true;
        return -1;
    }
    return literal;
}

typedef bool debugger_command_imp_t(GB_gameboy_t *gb, char *arguments);

typedef struct {
    const char *command;
    unsigned char min_length;
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
        gb_log(gb, "Usage: continue\n");
        return true;
    }
    gb->debug_stopped = false;
    return false;
}

static bool next(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        gb_log(gb, "Usage: next\n");
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
        gb_log(gb, "Usage: step\n");
        return true;
    }

    return false;
}

static bool finish(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        gb_log(gb, "Usage: finish\n");
        return true;
    }

    gb->debug_stopped = false;
    gb->debug_fin_command = true;
    gb->debug_call_depth = 0;
    return false;
}

static bool registers(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        gb_log(gb, "Usage: registers\n");
        return true;
    }

    gb_log(gb, "AF = %04x\n", gb->registers[GB_REGISTER_AF]);
    gb_log(gb, "BC = %04x\n", gb->registers[GB_REGISTER_BC]);
    gb_log(gb, "DE = %04x\n", gb->registers[GB_REGISTER_DE]);
    gb_log(gb, "HL = %04x\n", gb->registers[GB_REGISTER_HL]);
    gb_log(gb, "SP = %04x\n", gb->registers[GB_REGISTER_SP]);
    gb_log(gb, "PC = %04x\n", gb->pc);
    gb_log(gb, "TIMA = %d/%lu\n", gb->io_registers[GB_IO_TIMA], gb->tima_cycles);
    gb_log(gb, "Display Controller: LY = %d/%lu\n", gb->io_registers[GB_IO_LY], gb->display_cycles % 456);
    return true;
}

/* Find the index of the closest breakpoint equal or greater to addr */
static unsigned short find_breakpoint(GB_gameboy_t *gb, unsigned short addr)
{
    if (!gb->breakpoints) {
        return 0;
    }
    int min = 0;
    int max = gb->n_breakpoints;
    while (min < max) {
        unsigned short pivot = (min + max) / 2;
        if (gb->breakpoints[pivot] == addr) return pivot;
        if (gb->breakpoints[pivot] > addr) {
            max = pivot - 1;
        }
        else {
            min = pivot + 1;
        }
    }
    return (unsigned short) min;
}

static bool breakpoint(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        gb_log(gb, "Usage: breakpoint <expression>\n");
        return true;
    }

    bool error;
    unsigned short result = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error);

    if (error) return true;

    unsigned short index = find_breakpoint(gb, result);
    if (index < gb->n_breakpoints && gb->breakpoints[index] == result) {
        gb_log(gb, "Breakpoint already set at %04x\n", result);
        return true;
    }

    gb->breakpoints = realloc(gb->breakpoints, gb->n_breakpoints * sizeof(gb->breakpoints[0]));
    memmove(&gb->breakpoints[index + 1], &gb->breakpoints[index], (gb->n_breakpoints - index) * sizeof(gb->breakpoints[0]));
    gb->breakpoints[index] = result;
    gb->n_breakpoints++;

    gb_log(gb, "Breakpoint set at %04x\n", result);
    return true;
}

static bool delete(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        gb_log(gb, "Delete all breakpoints? ");
        char *answer = gb->input_callback(gb);
        if (answer[0] == 'Y' || answer[0] == 'y') {
            free(gb->breakpoints);
            gb->breakpoints = NULL;
            gb->n_breakpoints = 0;
        }
        return true;
    }

    bool error;
    unsigned short result = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error);

    if (error) return true;

    unsigned short index = find_breakpoint(gb, result);
    if (index >= gb->n_breakpoints || gb->breakpoints[index] != result) {
        gb_log(gb, "No breakpoint set at %04x\n", result);
        return true;
    }

    gb->breakpoints = realloc(gb->breakpoints, gb->n_breakpoints * sizeof(gb->breakpoints[0]));
    memmove(&gb->breakpoints[index], &gb->breakpoints[index + 1], (gb->n_breakpoints - index) * sizeof(gb->breakpoints[0]));
    gb->n_breakpoints--;

    gb_log(gb, "Breakpoint removed from %04x\n", result);
    return true;
}

static bool list(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments))) {
        gb_log(gb, "Usage: list\n");
        return true;
    }

    if (gb->n_breakpoints == 0) {
        gb_log(gb, "No breakpoints set.\n");
        return true;
    }

    gb_log(gb, "%d breakpoint(s) set:\n", gb->n_breakpoints);
    for (unsigned short i = 0; i < gb->n_breakpoints; i++) {
        gb_log(gb, " %d. %04x\n", i + 1, gb->breakpoints[i]);
    }

    return true;
}

static bool should_break(GB_gameboy_t *gb, unsigned short addr)
{
    unsigned short index = find_breakpoint(gb, addr);
    if (index < gb->n_breakpoints && gb->breakpoints[index] == addr) {
        return true;
    }
    return false;
}

static bool print(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        gb_log(gb, "Usage: print <expression>\n");
        return true;
    }

    bool error;
    unsigned short result = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error);
    if (!error) {
        gb_log(gb, "=%04x\n", result);
    }
    return true;
}

static bool examine(GB_gameboy_t *gb, char *arguments)
{
    if (strlen(lstrip(arguments)) == 0) {
        gb_log(gb, "Usage: examine <expression>\n");
        return true;
    }

    bool error;
    unsigned short addr = debugger_evaluate(gb, arguments, (unsigned int)strlen(arguments), &error);
    if (!error) {
        gb_log(gb, "%4x: ", addr);
        for (int i = 0; i < 16; i++) {
            gb_log(gb, "%02x ", read_memory(gb, addr + i));
        }
        gb_log(gb, "\n");
    }
    return true;
}

static bool help(GB_gameboy_t *gb, char *arguments);
static const debugger_command_t commands[] = {
    {"continue", 1, cont, "Continue running until next stop"},
    {"next", 1, next, "Run the next instruction, skipping over function calls"},
    {"step", 1, step, "Run the next instruction, stepping into function calls"},
    {"finish", 1, finish, "Run until the current function returns"},
    {"registers", 1, registers, "Print values of processor registers and other important registers"},
    {"breakpoint", 1, breakpoint, "Move the breakpoint to a new position"},
    {"list", 1, list, "List all set breakpoints"},
    {"delete", 2, delete, "Delete a breakpoint by its address, or all breakpoints"},
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
            gb_attributed_log(gb, GB_LOG_BOLD, "%s", command->command);
            gb_log(gb, ": %s\n", command->help_string);
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

/* The debugger interface is quite primitive. One letter commands with a single parameter maximum.
   Only one breakpoint is allowed at a time. More features will be added later. */
void debugger_run(GB_gameboy_t *gb)
{
    char *input = NULL;
    if (gb->debug_next_command && gb->debug_call_depth <= 0) {
        gb->debug_stopped = true;
    }
    if (gb->debug_fin_command && gb->debug_call_depth == -1) {
        gb->debug_stopped = true;
    }
    if (gb->debug_stopped) {
        cpu_disassemble(gb, gb->pc, 5);
    }
next_command:
    if (input) {
        free(input);
    }
    if (!gb->debug_stopped && should_break(gb, gb->pc)) {
        gb->debug_stopped = true;
        gb_log(gb, "Breakpoint: PC = %04x\n", gb->pc);
        cpu_disassemble(gb, gb->pc, 5);
    }
    if (gb->debug_stopped) {
        gb->debug_next_command = false;
        gb->debug_fin_command = false;
        input = gb->input_callback(gb);

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
            gb_log(gb, "%s: no such command.\n", command_string);
            goto next_command;
        }

        /* Split to arguments and command */

        free(input);
    }
}