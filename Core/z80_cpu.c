#include <stdio.h>
#include <stdbool.h>
#include "z80_cpu.h"
#include "timing.h"
#include "memory.h"
#include "gb.h"


typedef void GB_opcode_t(GB_gameboy_t *gb, unsigned char opcode);

static void ill(GB_gameboy_t *gb, unsigned char opcode)
{
    gb_log(gb, "Illegal Opcode. Halting.");
    gb->interrupt_enable = 0;
    gb->halted = true;
}

static void nop(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    gb->pc++;
}

static void stop(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    if (gb->io_registers[GB_IO_KEY1] & 0x1) {
        /* Todo: the switch is not instant. We should emulate this. */
        gb->cgb_double_speed ^= true;
        gb->io_registers[GB_IO_KEY1] = 0;
    }
    else {
        gb->stopped = true;
    }
    gb->pc++;
}

/* Operand naming conventions for functions:
   r = 8-bit register
   lr = low 8-bit register
   hr = high 8-bit register
   rr = 16-bit register
   d8 = 8-bit imm
   d16 = 16-bit imm
   d.. = [..]
   cc = condition code (z, nz, c, nc)
   */

static void ld_rr_d16(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    unsigned short value;
    advance_cycles(gb, 12);
    register_id = (read_memory(gb, gb->pc++) >> 4) + 1;
    value = read_memory(gb, gb->pc++);
    value |= read_memory(gb, gb->pc++) << 8;
    gb->registers[register_id] = value;
}

static void ld_drr_a(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 8);
    register_id = (read_memory(gb, gb->pc++) >> 4) + 1;
    write_memory(gb, gb->registers[register_id], gb->registers[GB_REGISTER_AF] >> 8);
}

static void inc_rr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 8);
    register_id = (read_memory(gb, gb->pc++) >> 4) + 1;
    gb->registers[register_id]++;
}

static void inc_hr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 4);
    gb->pc++;
    register_id = ((opcode >> 4) + 1) & 0x03;
    gb->registers[register_id] += 0x100;
    gb->registers[GB_REGISTER_AF] &= ~(GB_SUBSTRACT_FLAG | GB_ZERO_FLAG | GB_HALF_CARRY_FLAG);

    if ((gb->registers[register_id] & 0x0F00) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ((gb->registers[register_id] & 0xFF00) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}
static void dec_hr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 4);
    gb->pc++;
    register_id = ((opcode >> 4) + 1) & 0x03;
    gb->registers[register_id] -= 0x100;
    gb->registers[GB_REGISTER_AF] &= ~(GB_ZERO_FLAG | GB_HALF_CARRY_FLAG);
    gb->registers[GB_REGISTER_AF] |= GB_SUBSTRACT_FLAG;

    if ((gb->registers[register_id] & 0x0F00) == 0xF00) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ((gb->registers[register_id] & 0xFF00) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void ld_hr_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 8);
    gb->pc++;
    register_id = ((opcode >> 4) + 1) & 0x03;
    gb->registers[register_id] &= 0xFF;
    gb->registers[register_id] |= read_memory(gb, gb->pc++) << 8;
}

static void rlca(GB_gameboy_t *gb, unsigned char opcode)
{
    bool carry = (gb->registers[GB_REGISTER_AF] & 0x8000) != 0;

    advance_cycles(gb, 4);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] = (gb->registers[GB_REGISTER_AF] & 0xFF00) << 1;
    if (carry) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG | 0x0100;
    }
}

static void rla(GB_gameboy_t *gb, unsigned char opcode)
{
    bool bit7 = (gb->registers[GB_REGISTER_AF] & 0x8000) != 0;
    bool carry = (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) != 0;

    advance_cycles(gb, 4);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] = (gb->registers[GB_REGISTER_AF] & 0xFF00) << 1;
    if (carry) {
        gb->registers[GB_REGISTER_AF] |= 0x0100;
    }
    if (bit7) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void ld_da16_sp(GB_gameboy_t *gb, unsigned char opcode){
    unsigned short addr;
    advance_cycles(gb, 20);
    gb->pc++;
    addr = read_memory(gb, gb->pc++);
    addr |= read_memory(gb, gb->pc++) << 8;
    write_memory(gb, addr, gb->registers[GB_REGISTER_SP] & 0xFF);
    write_memory(gb, addr+1, gb->registers[GB_REGISTER_SP] >> 8);
}

static void add_hl_rr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned short hl = gb->registers[GB_REGISTER_HL];
    unsigned short rr;
    unsigned char register_id;
    advance_cycles(gb, 8);
    gb->pc++;
    register_id = (opcode >> 4) + 1;
    rr = gb->registers[register_id];
    gb->registers[GB_REGISTER_HL] = hl + rr;
    gb->registers[GB_REGISTER_AF] &= ~(GB_SUBSTRACT_FLAG | GB_CARRY_FLAG | GB_HALF_CARRY_FLAG);

    /* The meaning of the Half Carry flag is really hard to track -_- */
    if (((hl & 0xFFF) + (rr & 0xFFF)) & 0x1000) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ( ((unsigned long) hl) + ((unsigned long) rr) & 0x10000) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void ld_a_drr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 8);
    register_id = (read_memory(gb, gb->pc++) >> 4) + 1;
    gb->registers[GB_REGISTER_AF] &= 0xFF;
    gb->registers[GB_REGISTER_AF] |= read_memory(gb, gb->registers[register_id]) << 8;
}

static void dec_rr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 8);
    register_id = (read_memory(gb, gb->pc++) >> 4) + 1;
    gb->registers[register_id]--;
}

static void inc_lr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    unsigned char value;
    advance_cycles(gb, 4);
    register_id = (read_memory(gb, gb->pc++) >> 4) + 1;

    value = (gb->registers[register_id] & 0xFF) + 1;
    gb->registers[register_id] = (gb->registers[register_id] & 0xFF00) | value;

    gb->registers[GB_REGISTER_AF] &= ~(GB_SUBSTRACT_FLAG | GB_ZERO_FLAG | GB_HALF_CARRY_FLAG);

    if ((gb->registers[register_id] & 0x0F) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ((gb->registers[register_id] & 0xFF) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}
static void dec_lr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    unsigned char value;
    advance_cycles(gb, 4);
    register_id = (read_memory(gb, gb->pc++) >> 4) + 1;

    value = (gb->registers[register_id] & 0xFF) - 1;
    gb->registers[register_id] = (gb->registers[register_id] & 0xFF00) | value;

    gb->registers[GB_REGISTER_AF] &= ~(GB_ZERO_FLAG | GB_HALF_CARRY_FLAG);
    gb->registers[GB_REGISTER_AF] |= GB_SUBSTRACT_FLAG;

    if ((gb->registers[register_id] & 0x0F) == 0xF) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ((gb->registers[register_id] & 0xFF) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void ld_lr_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 8);
    register_id = (read_memory(gb, gb->pc++) >> 4) + 1;
    gb->registers[register_id] &= 0xFF00;
    gb->registers[register_id] |= read_memory(gb, gb->pc++);
}

static void rrca(GB_gameboy_t *gb, unsigned char opcode)
{
    bool carry = (gb->registers[GB_REGISTER_AF] & 0x100) != 0;

    advance_cycles(gb, 4);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] = (gb->registers[GB_REGISTER_AF] >> 1) & 0xFF00;
    if (carry) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG | 0x8000;
    }
}

static void rra(GB_gameboy_t *gb, unsigned char opcode)
{
    bool bit1 = (gb->registers[GB_REGISTER_AF] & 0x0100) != 0;
    bool carry = (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) != 0;

    advance_cycles(gb, 4);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] = (gb->registers[GB_REGISTER_AF] >> 1) & 0xFF00;
    if (carry) {
        gb->registers[GB_REGISTER_AF] |= 0x8000;
    }
    if (bit1) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void jr_r8(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 12);
    gb->pc++;
    gb->pc += (signed char) read_memory(gb, gb->pc++);
}

static bool condition_code(GB_gameboy_t *gb, unsigned char opcode)
{
    switch ((opcode >> 3) & 0x3) {
        case 0:
            return !(gb->registers[GB_REGISTER_AF] & GB_ZERO_FLAG);
        case 1:
            return (gb->registers[GB_REGISTER_AF] & GB_ZERO_FLAG);
        case 2:
            return !(gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG);
        case 3:
            return (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG);
    }

    return false;
}

static void jr_cc_r8(GB_gameboy_t *gb, unsigned char opcode)
{
    if (condition_code(gb, read_memory(gb, gb->pc++))) {
        advance_cycles(gb, 12);
        gb->pc += (signed char)read_memory(gb, gb->pc++);
    }
    else {
        advance_cycles(gb, 8);
        gb->pc += 1;
    }
}

static void daa(GB_gameboy_t *gb, unsigned char opcode)
{
    /* This function is UGLY and UNREADABLE! But it passes Blargg's daa test! */
    advance_cycles(gb, 4);
    gb->registers[GB_REGISTER_AF] &= ~GB_ZERO_FLAG;
    gb->pc++;
    if (gb->registers[GB_REGISTER_AF] & GB_SUBSTRACT_FLAG) {
        if (gb->registers[GB_REGISTER_AF] & GB_HALF_CARRY_FLAG) {
            gb->registers[GB_REGISTER_AF] &= ~GB_HALF_CARRY_FLAG;
            if (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) {
                gb->registers[GB_REGISTER_AF] += 0x9A00;
            }
            else {
                gb->registers[GB_REGISTER_AF] += 0xFA00;
            }
        }
        else if(gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) {
            gb->registers[GB_REGISTER_AF] += 0xA000;
        }
    }
    else {
        if (gb->registers[GB_REGISTER_AF] & GB_HALF_CARRY_FLAG) {
            unsigned short number = gb->registers[GB_REGISTER_AF] >> 8;
            if (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) {
                number += 0x100;
            }
            gb->registers[GB_REGISTER_AF] = 0;
            number += 0x06;
            if (number >= 0xa0) {
                number -= 0xa0;
                gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
            }
            gb->registers[GB_REGISTER_AF] |= number << 8;
        }
        else {
            unsigned short number = gb->registers[GB_REGISTER_AF] >> 8;
            if (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) {
                number += 0x100;
            }
            if (number > 0x99) {
                number += 0x60;
            }
            number = (number & 0x0F) + ((number & 0x0F) > 9 ? 6 : 0) + (number & 0xFF0);
            gb->registers[GB_REGISTER_AF] = number << 8;
            if (number & 0xFF00) {
                gb->registers[GB_REGISTER_AF]  |= GB_CARRY_FLAG;
            }
        }
    }
    if ((gb->registers[GB_REGISTER_AF] & 0xFF00) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void cpl(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] ^= 0xFF00;
    gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG | GB_SUBSTRACT_FLAG;
}

static void scf(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    gb->registers[GB_REGISTER_AF] &= ~(GB_HALF_CARRY_FLAG | GB_SUBSTRACT_FLAG);
}

static void ccf(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] ^= GB_CARRY_FLAG;
    gb->registers[GB_REGISTER_AF] &= ~(GB_HALF_CARRY_FLAG | GB_SUBSTRACT_FLAG);
}

static void ld_dhli_a(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->pc++;
    write_memory(gb, gb->registers[GB_REGISTER_HL]++, gb->registers[GB_REGISTER_AF] >> 8);
}

static void ld_dhld_a(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->pc++;
    write_memory(gb, gb->registers[GB_REGISTER_HL]--, gb->registers[GB_REGISTER_AF] >> 8);
}

static void ld_a_dhli(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] &= 0xFF;
    gb->registers[GB_REGISTER_AF] |= read_memory(gb, gb->registers[GB_REGISTER_HL]++) << 8;
}

static void ld_a_dhld(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] &= 0xFF;
    gb->registers[GB_REGISTER_AF] |= read_memory(gb, gb->registers[GB_REGISTER_HL]--) << 8;
}

static void inc_dhl(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->registers[GB_REGISTER_HL]) + 1;
    advance_cycles(gb, 4);
    write_memory(gb, gb->registers[GB_REGISTER_HL], value);

    gb->registers[GB_REGISTER_AF] &= ~(GB_SUBSTRACT_FLAG | GB_ZERO_FLAG | GB_HALF_CARRY_FLAG);
    if ((value & 0x0F) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ((value & 0xFF) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void dec_dhl(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->registers[GB_REGISTER_HL]) - 1;
    advance_cycles(gb, 4);
    write_memory(gb, gb->registers[GB_REGISTER_HL], value);

    gb->registers[GB_REGISTER_AF] &= ~( GB_ZERO_FLAG | GB_HALF_CARRY_FLAG);
    gb->registers[GB_REGISTER_AF] |= GB_SUBSTRACT_FLAG;
    if ((value & 0x0F) == 0x0F) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ((value & 0xFF) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void ld_dhl_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 12);
    gb->pc++;
    write_memory(gb, gb->registers[GB_REGISTER_HL], read_memory(gb, gb->pc++));
}

unsigned char get_src_value(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char src_register_id;
    unsigned char src_low;
    src_register_id = ((opcode >> 1) + 1) & 3;
    src_low = opcode & 1;
    if (src_register_id == GB_REGISTER_AF) {
        if (src_low) {
            return gb->registers[GB_REGISTER_AF] >> 8;
        }
        advance_cycles(gb, 4);
        return read_memory(gb, gb->registers[GB_REGISTER_HL]);
    }
    if (src_low) {
        return gb->registers[src_register_id] & 0xFF;
    }
    return gb->registers[src_register_id] >> 8;
}

static void set_src_value(GB_gameboy_t *gb, unsigned char opcode, unsigned char value)
{
    unsigned char src_register_id;
    unsigned char src_low;
    src_register_id = ((opcode >> 1) + 1) & 3;
    src_low = opcode & 1;

    if (src_register_id == GB_REGISTER_AF) {
        if (src_low) {
            gb->registers[GB_REGISTER_AF] &= 0xFF;
            gb->registers[GB_REGISTER_AF] |= value << 8;
        }
        else {
            advance_cycles(gb, 4);
            write_memory(gb, gb->registers[GB_REGISTER_HL], value);
        }
    }
    else {
        if (src_low) {
            gb->registers[src_register_id] &= 0xFF00;
            gb->registers[src_register_id] |= value;
        }
        else {
            gb->registers[src_register_id] &= 0xFF;
            gb->registers[src_register_id] |= value << 8;
        }
    }
}

static void ld_r_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char dst_register_id;
    unsigned char dst_low;
    unsigned char value;
    advance_cycles(gb, 4);
    gb->pc++;

    dst_register_id = ((opcode >> 4) + 1) & 3;
    dst_low = opcode & 8;
    value = get_src_value(gb, opcode);



    if (dst_register_id == GB_REGISTER_AF) {
        if (dst_low) {
            gb->registers[GB_REGISTER_AF] &= 0xFF;
            gb->registers[GB_REGISTER_AF] |= value << 8;
        }
        else {
            advance_cycles(gb, 4);
            write_memory(gb, gb->registers[GB_REGISTER_HL], value);
        }
    }
    else {
        if (dst_low) {
            gb->registers[dst_register_id] &= 0xFF00;
            gb->registers[dst_register_id] |= value;
        }
        else {
            gb->registers[dst_register_id] &= 0xFF;
            gb->registers[dst_register_id] |= value << 8;
        }
    }

}

static void add_a_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 4);
    gb->pc++;
    value = get_src_value(gb, opcode);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = (a + value) << 8;
    if ((unsigned char)(a + value) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) + (value & 0xF) > 0x0F) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (((unsigned long) a) + ((unsigned long) value) > 0xFF) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void adc_a_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a, carry;
    advance_cycles(gb, 4);
    gb->pc++;
    value = get_src_value(gb, opcode);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    carry = (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) != 0;
    gb->registers[GB_REGISTER_AF] = (a + value + carry) << 8;

    if ((unsigned char)(a + value + carry) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) + (value & 0xF) + carry > 0x0F) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (((unsigned long) a) + ((unsigned long) value) + carry > 0xFF) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void sub_a_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 4);
    gb->pc++;
    value = get_src_value(gb, opcode);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = ((a - value) << 8) | GB_SUBSTRACT_FLAG;
    if (a == value) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) < (value & 0xF)) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (a < value) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void sbc_a_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a, carry;
    advance_cycles(gb, 4);
    gb->pc++;
    value = get_src_value(gb, opcode);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    carry = (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) != 0;
    gb->registers[GB_REGISTER_AF] = ((a - value - carry) << 8) | GB_SUBSTRACT_FLAG;

    if ((unsigned char) (a - value - carry) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) < (value & 0xF) + carry) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (((unsigned long) a) - ((unsigned long) value) - carry > 0xFF) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void and_a_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 4);
    gb->pc++;
    value = get_src_value(gb, opcode);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = ((a & value) << 8) | GB_HALF_CARRY_FLAG;
    if ((a & value) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void xor_a_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 4);
    gb->pc++;
    value = get_src_value(gb, opcode);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = (a ^ value) << 8;
    if ((a ^ value) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void or_a_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 4);
    gb->pc++;
    value = get_src_value(gb, opcode);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = (a | value) << 8;
    if ((a | value) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void cp_a_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 4);
    gb->pc++;
    value = get_src_value(gb, opcode);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    gb->registers[GB_REGISTER_AF] |= GB_SUBSTRACT_FLAG;
    if (a == value) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) < (value & 0xF)) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (a < value) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void halt(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    gb->halted = true;
    gb->pc++;
}

static void ret_cc(GB_gameboy_t *gb, unsigned char opcode)
{
    if (condition_code(gb, read_memory(gb, gb->pc++))) {
        advance_cycles(gb, 20);
        gb->pc = read_memory(gb, gb->registers[GB_REGISTER_SP]) |
        (read_memory(gb, gb->registers[GB_REGISTER_SP] + 1) << 8);
        gb->registers[GB_REGISTER_SP] += 2;
        gb->debug_call_depth--;
    }
    else {
        advance_cycles(gb, 8);
    }
}

static void pop_rr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 12);
    register_id = ((read_memory(gb, gb->pc++) >> 4) + 1) & 3;
    gb->registers[register_id] = read_memory(gb, gb->registers[GB_REGISTER_SP]) |
    (read_memory(gb, gb->registers[GB_REGISTER_SP] + 1) << 8);
    gb->registers[GB_REGISTER_AF] &= 0xFFF0; // Make sure we don't set impossible flags on F! See Blargg's PUSH AF test.
    gb->registers[GB_REGISTER_SP] += 2;
}

static void jp_cc_a16(GB_gameboy_t *gb, unsigned char opcode)
{
    gb->pc++;
    if (condition_code(gb, opcode)) {
        advance_cycles(gb, 16);
        gb->pc = read_memory(gb, gb->pc) | (read_memory(gb, gb->pc + 1) << 8);
    }
    else {
        advance_cycles(gb, 12);
        gb->pc += 2;
    }
}

static void jp_a16(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 16);
    gb->pc++;
    gb->pc = read_memory(gb, gb->pc) | (read_memory(gb, gb->pc + 1) << 8);
}

static void call_cc_a16(GB_gameboy_t *gb, unsigned char opcode)
{
    gb->pc++;
    if (condition_code(gb, opcode)) {
        advance_cycles(gb, 24);
        gb->registers[GB_REGISTER_SP] -= 2;
        write_memory(gb, gb->registers[GB_REGISTER_SP], (gb->pc + 2) & 0xFF);
        write_memory(gb, gb->registers[GB_REGISTER_SP] + 1, (gb->pc + 2) >> 8);
        gb->pc = read_memory(gb, gb->pc) | (read_memory(gb, gb->pc + 1) << 8);
        gb->debug_call_depth++;
    }
    else {
        advance_cycles(gb, 12);
        gb->pc += 2;
    }
}

static void push_rr(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char register_id;
    advance_cycles(gb, 16);
    gb->pc++;
    register_id = ((opcode >> 4) + 1) & 3;
    gb->registers[GB_REGISTER_SP] -= 2;
    write_memory(gb, gb->registers[GB_REGISTER_SP], (gb->registers[register_id]) & 0xFF);
    write_memory(gb, gb->registers[GB_REGISTER_SP] + 1, (gb->registers[register_id]) >> 8);
}

static void add_a_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->pc++);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = (a + value) << 8;
    if ((unsigned char) (a + value) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) + (value & 0xF) > 0x0F) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (((unsigned long) a) + ((unsigned long) value) > 0xFF) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void adc_a_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a, carry;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->pc++);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    carry = (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) != 0;
    gb->registers[GB_REGISTER_AF] = (a + value + carry) << 8;

    if (gb->registers[GB_REGISTER_AF] == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) + (value & 0xF) + carry > 0x0F) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (((unsigned long) a) + ((unsigned long) value) + carry > 0xFF) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void sub_a_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->pc++);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = ((a - value) << 8) | GB_SUBSTRACT_FLAG;
    if (a == value) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) < (value & 0xF)) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (a < value) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void sbc_a_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a, carry;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->pc++);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    carry = (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) != 0;
    gb->registers[GB_REGISTER_AF] = ((a - value - carry) << 8) | GB_SUBSTRACT_FLAG;

    if ((unsigned char) (a - value - carry) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) < (value & 0xF) + carry) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (((unsigned long) a) - ((unsigned long) value) - carry > 0xFF) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void and_a_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->pc++);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = ((a & value) << 8) | GB_HALF_CARRY_FLAG;
    if ((a & value) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void xor_a_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->pc++);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = (a ^ value) << 8;
    if ((a ^ value) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void or_a_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->pc++);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] = (a | value) << 8;
    if ((a | value) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void cp_a_d8(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value, a;
    advance_cycles(gb, 8);
    gb->pc++;
    value = read_memory(gb, gb->pc++);
    a = gb->registers[GB_REGISTER_AF] >> 8;
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    gb->registers[GB_REGISTER_AF] |= GB_SUBSTRACT_FLAG;
    if (a == value) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
    if ((a & 0xF) < (value & 0xF)) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }
    if (a < value) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void rst(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 16);
    gb->registers[GB_REGISTER_SP] -= 2;
    write_memory(gb, gb->registers[GB_REGISTER_SP], (gb->pc + 1) & 0xFF);
    write_memory(gb, gb->registers[GB_REGISTER_SP] + 1, (gb->pc + 1) >> 8);
    gb->pc = opcode ^ 0xC7;
}

static void ret(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 16);
    gb->pc = read_memory(gb, gb->registers[GB_REGISTER_SP]) |
    (read_memory(gb, gb->registers[GB_REGISTER_SP] + 1) << 8);
    gb->registers[GB_REGISTER_SP] += 2;
    gb->debug_call_depth--;
}

static void reti(GB_gameboy_t *gb, unsigned char opcode)
{
    ret(gb, opcode);
    gb->ime = true;
}

static void call_a16(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 24);
    gb->pc++;
    gb->registers[GB_REGISTER_SP] -= 2;
    write_memory(gb, gb->registers[GB_REGISTER_SP], (gb->pc + 2) & 0xFF);
    write_memory(gb, gb->registers[GB_REGISTER_SP] + 1, (gb->pc + 2) >> 8);
    gb->pc = read_memory(gb, gb->pc) | (read_memory(gb, gb->pc + 1) << 8);
    gb->debug_call_depth++;
}

static void ld_da8_a(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->pc++;
    unsigned char temp = read_memory(gb, gb->pc++);
    advance_cycles(gb, 4);
    write_memory(gb, 0xFF00 + temp, gb->registers[GB_REGISTER_AF] >> 8);
}

static void ld_a_da8(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->registers[GB_REGISTER_AF] &= 0xFF;
    gb->pc++;
    unsigned char temp = read_memory(gb, gb->pc++);
    advance_cycles(gb, 4);
    gb->registers[GB_REGISTER_AF] |= read_memory(gb, 0xFF00 + temp) << 8;
}

static void ld_dc_a(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->pc++;
    write_memory(gb, 0xFF00 + (gb->registers[GB_REGISTER_BC] & 0xFF), gb->registers[GB_REGISTER_AF] >> 8);
}

static void ld_a_dc(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->registers[GB_REGISTER_AF] &= 0xFF;
    gb->pc++;
    gb->registers[GB_REGISTER_AF] |= read_memory(gb, 0xFF00 + (gb->registers[GB_REGISTER_BC] & 0xFF)) << 8;
}

static void add_sp_r8(GB_gameboy_t *gb, unsigned char opcode)
{
    signed short offset;
    unsigned short sp = gb->registers[GB_REGISTER_SP];
    advance_cycles(gb, 16);
    gb->pc++;
    offset = (signed char) read_memory(gb, gb->pc++);
    gb->registers[GB_REGISTER_SP] += offset;

    gb->registers[GB_REGISTER_AF] &= 0xFF00;

    /* A new instruction, a new meaning for Half Carry! */
    if ((sp & 0xF) + (offset & 0xF) > 0xF) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ((sp & 0xFF) + (offset & 0xFF) > 0xFF)  {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void jp_hl(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    gb->pc = gb->registers[GB_REGISTER_HL];
}

static void ld_da16_a(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned short addr;
    advance_cycles(gb, 16);
    gb->pc++;
    addr = read_memory(gb, gb->pc++);
    addr |= read_memory(gb, gb->pc++) << 8;
    write_memory(gb, addr, gb->registers[GB_REGISTER_AF] >> 8);
}

static void ld_a_da16(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned short addr;
    advance_cycles(gb, 16);
    gb->registers[GB_REGISTER_AF] &= 0xFF;
    gb->pc++;
    addr = read_memory(gb, gb->pc++);
    addr |= read_memory(gb, gb->pc++) << 8 ;
    gb->registers[GB_REGISTER_AF] |= read_memory(gb, addr) << 8;
}

static void di(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    gb->pc++;
    gb->ime = false;
}

static void ei(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 4);
    gb->pc++;
    gb->ime = true;
}

static void ld_hl_sp_r8(GB_gameboy_t *gb, unsigned char opcode)
{
    signed short offset;
    advance_cycles(gb, 12);
    gb->pc++;
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    offset = (signed char) read_memory(gb, gb->pc++);
    gb->registers[GB_REGISTER_HL] = gb->registers[GB_REGISTER_SP] + offset;

    if ((gb->registers[GB_REGISTER_SP] & 0xF) + (offset & 0xF) > 0xF) {
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
    }

    if ((gb->registers[GB_REGISTER_SP] & 0xFF)  + (offset & 0xFF) > 0xFF) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
}

static void ld_sp_hl(GB_gameboy_t *gb, unsigned char opcode)
{
    advance_cycles(gb, 8);
    gb->pc++;
    gb->registers[GB_REGISTER_SP] = gb->registers[GB_REGISTER_HL];
}

static void rlc_r(GB_gameboy_t *gb, unsigned char opcode)
{
    bool carry;
    unsigned char value;
    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    carry = (value & 0x80) != 0;
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    set_src_value(gb, opcode, (value << 1) | carry);
    if (carry) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
    if (!(value << 1)) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void rrc_r(GB_gameboy_t *gb, unsigned char opcode)
{
    bool carry;
    unsigned char value;
    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    carry = (value & 0x01) != 0;
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    value = (value >> 1) | (carry << 7);
    set_src_value(gb, opcode, value);
    if (carry) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
    if (value == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void rl_r(GB_gameboy_t *gb, unsigned char opcode)
{
    bool carry;
    unsigned char value;
    bool bit7;
    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    carry = (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) != 0;
    bit7 = (value & 0x80) != 0;

    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    value = (value << 1) | carry;
    set_src_value(gb, opcode, value);
    if (bit7) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
    if (value == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void rr_r(GB_gameboy_t *gb, unsigned char opcode)
{
    bool carry;
    unsigned char value;
    bool bit1;

    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    carry = (gb->registers[GB_REGISTER_AF] & GB_CARRY_FLAG) != 0;
    bit1 = (value & 0x1) != 0;

    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    value = (value >> 1) | (carry << 7);
    set_src_value(gb, opcode, value);
    if (bit1) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
    if (value == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void sla_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value;
    bool carry;
    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    carry = (value & 0x80) != 0;
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    set_src_value(gb, opcode, (value << 1));
    if (carry) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
    if ((value & 0x7F) == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void sra_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char bit7;
    unsigned char value;
    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    bit7 = value & 0x80;
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    if (value & 1) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
    value = (value >> 1) | bit7;
    set_src_value(gb, opcode, value);
    if (value == 0) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void srl_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value;
    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    set_src_value(gb, opcode, (value >> 1));
    if (value & 1) {
        gb->registers[GB_REGISTER_AF] |= GB_CARRY_FLAG;
    }
    if (!(value >> 1)) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void swap_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value;
    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    gb->registers[GB_REGISTER_AF] &= 0xFF00;
    set_src_value(gb, opcode, (value >> 4) | (value << 4));
    if (!value) {
        gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
    }
}

static void bit_r(GB_gameboy_t *gb, unsigned char opcode)
{
    unsigned char value;
    unsigned char bit;
    advance_cycles(gb, 8);
    gb->pc++;
    value = get_src_value(gb, opcode);
    bit = 1 << ((opcode >> 3) & 7);
    if ((opcode & 0xC0) == 0x40) { /* Bit */
        gb->registers[GB_REGISTER_AF] &= 0xFF00 | GB_CARRY_FLAG;
        gb->registers[GB_REGISTER_AF] |= GB_HALF_CARRY_FLAG;
        if (!(bit & value)) {
            gb->registers[GB_REGISTER_AF] |= GB_ZERO_FLAG;
        }
    }
    else if ((opcode & 0xC0) == 0x80) { /* res */
        set_src_value(gb, opcode, value & ~bit) ;
    }
    else if ((opcode & 0xC0) == 0xC0) { /* set */
        set_src_value(gb, opcode, value | bit) ;
    }
}

static void cb_prefix(GB_gameboy_t *gb, unsigned char opcode)
{
    opcode = read_memory(gb, ++gb->pc);
    switch (opcode >> 3) {
        case 0:
            rlc_r(gb, opcode);
            break;
        case 1:
            rrc_r(gb, opcode);
            break;
        case 2:
            rl_r(gb, opcode);
            break;
        case 3:
            rr_r(gb, opcode);
            break;
        case 4:
            sla_r(gb, opcode);
            break;
        case 5:
            sra_r(gb, opcode);
            break;
        case 6:
            swap_r(gb, opcode);
            break;
        case 7:
            srl_r(gb, opcode);
            break;
        default:
            bit_r(gb, opcode);
            break;
    }
}

static GB_opcode_t *opcodes[256] = {
    /*  X0          X1          X2          X3          X4          X5          X6          X7                */
    /*  X8          X9          Xa          Xb          Xc          Xd          Xe          Xf                */
    nop,        ld_rr_d16,  ld_drr_a,   inc_rr,     inc_hr,     dec_hr,     ld_hr_d8,   rlca,       /* 0X */
    ld_da16_sp, add_hl_rr,  ld_a_drr,   dec_rr,     inc_lr,     dec_lr,     ld_lr_d8,   rrca,
    stop,       ld_rr_d16,  ld_drr_a,   inc_rr,     inc_hr,     dec_hr,     ld_hr_d8,   rla,        /* 1X */
    jr_r8,      add_hl_rr,  ld_a_drr,   dec_rr,     inc_lr,     dec_lr,     ld_lr_d8,   rra,
    jr_cc_r8,   ld_rr_d16,  ld_dhli_a,  inc_rr,     inc_hr,     dec_hr,     ld_hr_d8,   daa,        /* 2X */
    jr_cc_r8,   add_hl_rr,  ld_a_dhli,  dec_rr,     inc_lr,     dec_lr,     ld_lr_d8,   cpl,
    jr_cc_r8,   ld_rr_d16,  ld_dhld_a,  inc_rr,     inc_dhl,    dec_dhl,    ld_dhl_d8,  scf,        /* 3X */
    jr_cc_r8,   add_hl_rr,  ld_a_dhld,  dec_rr,     inc_hr,     dec_hr,     ld_hr_d8,   ccf,
    ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     /* 4X */
    ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,
    ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     /* 5X */
    ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,
    ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     /* 6X */
    ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,
    ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     halt,       ld_r_r,     /* 7X */
    ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,     ld_r_r,
    add_a_r,    add_a_r,    add_a_r,    add_a_r,    add_a_r,    add_a_r,    add_a_r,    add_a_r,    /* 8X */
    adc_a_r,    adc_a_r,    adc_a_r,    adc_a_r,    adc_a_r,    adc_a_r,    adc_a_r,    adc_a_r,
    sub_a_r,    sub_a_r,    sub_a_r,    sub_a_r,    sub_a_r,    sub_a_r,    sub_a_r,    sub_a_r,    /* 9X */
    sbc_a_r,    sbc_a_r,    sbc_a_r,    sbc_a_r,    sbc_a_r,    sbc_a_r,    sbc_a_r,    sbc_a_r,
    and_a_r,    and_a_r,    and_a_r,    and_a_r,    and_a_r,    and_a_r,    and_a_r,    and_a_r,    /* aX */
    xor_a_r,    xor_a_r,    xor_a_r,    xor_a_r,    xor_a_r,    xor_a_r,    xor_a_r,    xor_a_r,
    or_a_r,     or_a_r,     or_a_r,     or_a_r,     or_a_r,     or_a_r,     or_a_r,     or_a_r,     /* bX */
    cp_a_r,     cp_a_r,     cp_a_r,     cp_a_r,     cp_a_r,     cp_a_r,     cp_a_r,     cp_a_r,
    ret_cc,     pop_rr,     jp_cc_a16,  jp_a16,     call_cc_a16,push_rr,    add_a_d8,   rst,        /* cX */
    ret_cc,     ret,        jp_cc_a16,  cb_prefix,  call_cc_a16,call_a16,   adc_a_d8,   rst,
    ret_cc,     pop_rr,     jp_cc_a16,  ill,        call_cc_a16,push_rr,    sub_a_d8,   rst,        /* dX */
    ret_cc,     reti,       jp_cc_a16,  ill,        call_cc_a16,ill,        sbc_a_d8,   rst,
    ld_da8_a,   pop_rr,     ld_dc_a,    ill,        ill,        push_rr,    and_a_d8,   rst,        /* eX */
    add_sp_r8,  jp_hl,      ld_da16_a,  ill,        ill,        ill,        xor_a_d8,   rst,
    ld_a_da8,   pop_rr,     ld_a_dc,    di,         ill,        push_rr,    or_a_d8,    rst,        /* fX */
    ld_hl_sp_r8,ld_sp_hl,   ld_a_da16,  ei,         ill,        ill,        cp_a_d8,    rst,
};

void cpu_run(GB_gameboy_t *gb)
{
    bool interrupt = gb->interrupt_enable & gb->io_registers[GB_IO_IF];
    if (interrupt) {
        gb->halted = false;
    }

    if (gb->hdma_on) {
        advance_cycles(gb, 4);
        return;
    }

    if (gb->ime && interrupt) {
        unsigned char interrupt_bit = 0;
        unsigned char interrupt_queue = gb->interrupt_enable & gb->io_registers[GB_IO_IF];
        while (!(interrupt_queue & 1)) {
            interrupt_queue >>= 1;
            interrupt_bit++;
        }
        gb->io_registers[GB_IO_IF] &= ~(1 << interrupt_bit);
        gb->ime = false;
        nop(gb, 0);
        gb->pc -= 2;
        /* Run pseudo instructions rst 40-60*/
        rst(gb, 0x87 + interrupt_bit * 8);
    }
    else if(!gb->halted) {
        unsigned char opcode = read_memory(gb, gb->pc);
        opcodes[opcode](gb, opcode);
    }
    else {
        advance_cycles(gb, 4);
    }
}
