#define call(op) (this->*op)()
#define L

//opcode functions
//================

void CPU::opf_adc() {
  signed result = regs.a + rd + regs.p.c;
  regs.p.v = ~(regs.a ^ rd) & (regs.a ^ result) & 0x80;
  regs.p.c = (result > 0xff);
  regs.p.n = (result & 0x80);
  regs.p.z = ((uint8)result == 0);
  regs.a = result;
}

void CPU::opf_and() {
  regs.a &= rd;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void CPU::opf_asl() {
  regs.p.c = rd & 0x80;
  rd <<= 1;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

void CPU::opf_bit() {
  regs.p.n = (rd & 0x80);
  regs.p.v = (rd & 0x40);
  regs.p.z = ((rd & regs.a) == 0);
}

void CPU::opf_cmp() {
  signed r = regs.a - rd;
  regs.p.n = (r & 0x80);
  regs.p.z = (uint8)(r == 0);
  regs.p.c = (r >= 0);
}

void CPU::opf_cpx() {
  signed r = regs.x - rd;
  regs.p.n = (r & 0x80);
  regs.p.z = (uint8)(r == 0);
  regs.p.c = (r >= 0);
}

void CPU::opf_cpy() {
  signed r = regs.y - rd;
  regs.p.n = (r & 0x80);
  regs.p.z = (uint8)(r == 0);
  regs.p.c = (r >= 0);
}

void CPU::opf_dec() {
  rd--;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

void CPU::opf_eor() {
  regs.a ^= rd;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void CPU::opf_inc() {
  rd++;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

void CPU::opf_lda() {
  regs.a = rd;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void CPU::opf_ldx() {
  regs.x = rd;
  regs.p.n = (regs.x & 0x80);
  regs.p.z = (regs.x == 0);
}

void CPU::opf_ldy() {
  regs.y = rd;
  regs.p.n = (regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

void CPU::opf_lsr() {
  regs.p.c = rd & 0x01;
  rd >>= 1;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

void CPU::opf_ora() {
  regs.a |= rd;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void CPU::opf_rla() {
  unsigned carry = (unsigned)regs.p.c;
  regs.p.c = regs.a & 0x80;
  regs.a = (regs.a << 1) | carry;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void CPU::opf_rol() {
  unsigned carry = (unsigned)regs.p.c;
  regs.p.c = rd & 0x80;
  rd = (rd << 1) | carry;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

void CPU::opf_ror() {
  unsigned carry = (unsigned)regs.p.c << 7;
  regs.p.c = rd & 0x01;
  rd = carry | (rd >> 1);
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

void CPU::opf_rra() {
  unsigned carry = (unsigned)regs.p.c << 7;
  regs.p.c = regs.a & 0x01;
  regs.a = carry | (regs.a >> 1);
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void CPU::opf_sbc() {
  rd ^= 0xff;
  return opf_adc();
}

void CPU::opf_sla() {
  regs.p.c = regs.a & 0x80;
  regs.a <<= 1;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void CPU::opf_sra() {
  regs.p.c = regs.a & 0x01;
  regs.a >>= 1;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

//opcode implementations
//======================

void CPU::opi_branch(bool condition) {
  if(condition == false) {
L   rd = op_readpci();
  } else {
    rd = op_readpci();
    aa = regs.pc + (int8)rd;
    op_page(regs.pc, aa);
L   op_readpc();
    regs.pc = aa;
  }
}

void CPU::opi_clear_flag(bool &flag) {
L op_readpc();
  flag = 0;
}

void CPU::opi_decrement(uint8 &r) {
L op_readpc();
  r--;
  regs.p.n = (r & 0x80);
  regs.p.z = (r == 0);
}

void CPU::opi_increment(uint8 &r) {
L op_readpc();
  r++;
  regs.p.n = (r & 0x80);
  regs.p.z = (r == 0);
}

void CPU::opi_pull(uint8 &r) {
  op_readpc();
  op_readpc();
L r = op_readsp();
  regs.p.n = (r & 0x80);
  regs.p.z = (r == 0);
}

void CPU::opi_push(uint8 &r) {
  op_readpc();
L op_writesp(r);
}

template<void (CPU::*op)()>
void CPU::opi_read_absolute() {
  abs.l = op_readpci();
  abs.h = op_readpci();
L rd = op_read(abs.w);
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_read_absolute_x() {
  abs.l = op_readpci();
  abs.h = op_readpci();
  op_page(abs.w, abs.w + regs.x);
L rd = op_read(abs.w + regs.x);
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_read_absolute_y() {
  abs.l = op_readpci();
  abs.h = op_readpci();
  op_page(abs.w, abs.w + regs.y);
L rd = op_read(abs.w + regs.y);
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_read_immediate() {
L rd = op_readpci();
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_read_indirect_zero_page_x() {
  zp = op_readpci();
  op_readpc();
  abs.l = op_readdp(zp++ + regs.x);
  abs.h = op_readdp(zp++ + regs.x);
L rd = op_read(abs.w);
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_read_indirect_zero_page_y() {
  rd = op_readpci();
  abs.l = op_read(rd++);
  abs.h = op_read(rd++);
  op_page(abs.w, abs.w + regs.y);
L rd = op_read(abs.w + regs.y);
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_read_zero_page() {
  zp = op_readpci();
L rd = op_read(zp);
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_read_zero_page_x() {
  zp = op_readpci();
  op_readpc();
L rd = op_readdp(zp + regs.x);
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_read_zero_page_y() {
  zp = op_readpci();
  op_readpc();
L rd = op_readdp(zp + regs.y);
  call(op);
}

template<void (CPU::*op)()>
void CPU::opi_rmw_absolute() {
  abs.l = op_readpci();
  abs.h = op_readpci();
  rd = op_read(abs.w);
  op_readpc();
  call(op);
L op_write(abs.w, rd);
}

template<void (CPU::*op)()>
void CPU::opi_rmw_absolute_x() {
  abs.l = op_readpci();
  abs.h = op_readpci();
  op_readpc();
  rd = op_read(abs.w + regs.x);
  op_readpc();
  call(op);
L op_write(abs.w + regs.x, rd);
}

template<void (CPU::*op)()>
void CPU::opi_rmw_zero_page() {
  zp = op_readpci();
  rd = op_read(zp);
  call(op);
  op_readpc();
L op_write(zp, rd);
}

template<void (CPU::*op)()>
void CPU::opi_rmw_zero_page_x() {
  zp = op_readpci();
  op_readpc();
  rd = op_readdp(zp + regs.x);
  call(op);
  op_readpc();
L op_writedp(zp + regs.x, rd);
}

void CPU::opi_set_flag(bool &flag) {
L op_readpc();
  flag = 1;
}

template<void (CPU::*op)()>
void CPU::opi_shift() {
L op_readpc();
  call(op);
}

void CPU::opi_store_absolute(uint8 &r) {
  abs.l = op_readpci();
  abs.h = op_readpci();
L op_write(abs.w, r);
}

void CPU::opi_store_absolute_x(uint8 &r) {
  abs.l = op_readpci();
  abs.h = op_readpci();
  op_page(abs.w, abs.w + regs.x);
L op_write(abs.w + regs.x, r);
}

void CPU::opi_store_absolute_y(uint8 &r) {
  abs.l = op_readpci();
  abs.h = op_readpci();
  op_page(abs.w, abs.w + regs.y);
L op_write(abs.w + regs.y, r);
}

void CPU::opi_store_indirect_zero_page_x(uint8 &r) {
  zp = op_readpci();
  op_readpc();
  abs.l = op_readdp(zp++ + regs.x);
  abs.h = op_readdp(zp++ + regs.x);
L op_write(abs.w, r);
}

void CPU::opi_store_indirect_zero_page_y(uint8 &r) {
  rd = op_readpci();
  abs.l = op_read(rd++);
  abs.h = op_read(rd++);
  op_page(abs.w, abs.w + regs.y);
L op_write(abs.w + regs.y, r);
}

void CPU::opi_store_zero_page(uint8 &r) {
  rd = op_readpci();
L op_write(rd, r);
}

void CPU::opi_store_zero_page_x(uint8 &r) {
  zp = op_readpci();
  op_readpc();
L op_writedp(zp + regs.x, r);
}

void CPU::opi_store_zero_page_y(uint8 &r) {
  zp = op_readpci();
  op_readpc();
L op_writedp(zp + regs.y, r);
}

void CPU::opi_transfer(uint8 &s, uint8 &d, bool flag) {
L op_readpc();
  d = s;
  if(flag == false) return;
  regs.p.n = (d & 0x80);
  regs.p.z = (d == 0);
}

//opcodes
//=======

void CPU::op_brk() {
  op_readpci();
  op_writesp(regs.pc >> 8);
  op_writesp(regs.pc >> 0);
  op_writesp(regs.p | 0x30);
  abs.l = op_read(0xfffe);
  regs.p.i = 1;
  regs.p.d = 0;
  abs.h = op_read(0xffff);
  regs.pc = abs.w;
}

void CPU::op_jmp_absolute() {
  abs.l = op_readpci();
L abs.h = op_readpci();
  regs.pc = abs.w;
}

void CPU::op_jmp_indirect_absolute() {
  abs.l = op_readpci();
  abs.h = op_readpci();
  iabs.l = op_read(abs.w); abs.l++;
L iabs.h = op_read(abs.w); abs.l++;
  regs.pc = iabs.w;
}

void CPU::op_jsr_absolute() {
  abs.l = op_readpci();
  abs.h = op_readpci();
  op_readpc();
  regs.pc--;
  op_writesp(regs.pc >> 8);
L op_writesp(regs.pc >> 0);
  regs.pc = abs.w;
}

void CPU::op_nop() {
L op_readpc();
}

void CPU::op_php() {
  op_readpc();
L op_writesp(regs.p | 0x30);
}

void CPU::op_plp() {
  op_readpc();
  op_readpc();
L regs.p = op_readsp();
}

void CPU::op_rti() {
  op_readpc();
  op_readpc();
  regs.p = op_readsp();
  abs.l = op_readsp();
L abs.h = op_readsp();
  regs.pc = abs.w;
}

void CPU::op_rts() {
  op_readpc();
  op_readpc();
  abs.l = op_readsp();
  abs.h = op_readsp();
L op_readpc();
  regs.pc = ++abs.w;
}

//illegal opcodes
//===============

void CPU::opill_nop_absolute() {
  abs.l = op_readpci();
  abs.h = op_readpci();
L op_readpc();
}

void CPU::opill_nop_absolute_x() {
  abs.l = op_readpci();
  abs.h = op_readpci();
  op_page(abs.w, abs.w + regs.x);
L op_readpc();
}

void CPU::opill_nop_immediate() {
L rd = op_readpc();
}

void CPU::opill_nop_implied() {
L op_readpc();
}

void CPU::opill_nop_zero_page() {
  zp = op_readpci();
L op_readpc();
}

void CPU::opill_nop_zero_page_x() {
  zp = op_readpci();
  op_readpc();
L op_readpc();
}

#undef call
#undef L
