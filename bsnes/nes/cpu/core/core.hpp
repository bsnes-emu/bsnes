struct Flags {
  bool n, v, d, i, z, c;

  inline operator unsigned() {
    return (n << 7) | (v << 6) | (d << 3) | (i << 2) | (z << 1) | (c << 0);
  }

  inline Flags& operator=(uint8 data) {
    n = data & 0x80; v = data & 0x40;
    d = data & 0x08; i = data & 0x04; z = data & 0x02; c = data & 0x01;
    return *this;
  }
};

struct Registers {
  uint16 pc;
  uint8  a, x, y, s;
  Flags  p;
} regs;

struct Register16 {
  union {
    uint16 w;
    struct { uint8 order_lsb2(l, h); };
  };
} abs, iabs;

uint8 rd;
uint8 zp;
uint16 aa;

//opcodes.cpp
void opf_asl();
void opf_adc();
void opf_and();
void opf_bit();
void opf_cmp();
void opf_cpx();
void opf_cpy();
void opf_dec();
void opf_eor();
void opf_inc();
void opf_lda();
void opf_ldx();
void opf_ldy();
void opf_lsr();
void opf_ora();
void opf_rla();
void opf_rol();
void opf_ror();
void opf_rra();
void opf_sbc();
void opf_sla();
void opf_sra();

void opi_branch(bool condition);
void opi_clear_flag(bool &flag);
void opi_decrement(uint8 &r);
void opi_increment(uint8 &r);
void opi_pull(uint8 &r);
void opi_push(uint8 &r);
template<void (CPU::*op)()> void opi_read_absolute();
template<void (CPU::*op)()> void opi_read_absolute_x();
template<void (CPU::*op)()> void opi_read_absolute_y();
template<void (CPU::*op)()> void opi_read_immediate();
template<void (CPU::*op)()> void opi_read_indirect_zero_page_x();
template<void (CPU::*op)()> void opi_read_indirect_zero_page_y();
template<void (CPU::*op)()> void opi_read_zero_page();
template<void (CPU::*op)()> void opi_read_zero_page_x();
template<void (CPU::*op)()> void opi_read_zero_page_y();
template<void (CPU::*op)()> void opi_rmw_absolute();
template<void (CPU::*op)()> void opi_rmw_absolute_x();
template<void (CPU::*op)()> void opi_rmw_zero_page();
template<void (CPU::*op)()> void opi_rmw_zero_page_x();
void opi_set_flag(bool &flag);
template<void (CPU::*op)()> void opi_shift();
void opi_store_absolute(uint8 &r);
void opi_store_absolute_x(uint8 &r);
void opi_store_absolute_y(uint8 &r);
void opi_store_indirect_zero_page_x(uint8 &r);
void opi_store_indirect_zero_page_y(uint8 &r);
void opi_store_zero_page(uint8 &r);
void opi_store_zero_page_x(uint8 &r);
void opi_store_zero_page_y(uint8 &r);
void opi_transfer(uint8 &s, uint8 &d, bool flag);

void op_brk();
void op_jmp_absolute();
void op_jmp_indirect_absolute();
void op_jsr_absolute();
void op_nop();
void op_php();
void op_plp();
void op_rti();
void op_rts();

void opill_nop_absolute();
void opill_nop_absolute_x();
void opill_nop_immediate();
void opill_nop_implied();
void opill_nop_zero_page();
void opill_nop_zero_page_x();

//exec.cpp
void op_exec();

//disassembler.cpp
string disassemble();
