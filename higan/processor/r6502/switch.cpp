#define I  //prefix highlights illegal instructions
#define op(id, name, ...) case id: return op_##name(__VA_ARGS__);
#define fp(name) &R6502::fp_##name

auto R6502::instruction() -> void {
  switch(readPC()) {
  op(0x00, brk)
  op(0x01, read_indirect_zero_page_x, fp(ora))
I op(0x04, nop_zero_page)
  op(0x05, read_zero_page, fp(ora))
  op(0x06, rmw_zero_page, fp(asl))
  op(0x08, php)
  op(0x09, read_immediate, fp(ora))
  op(0x0a, shift, fp(sla))
I op(0x0c, nop_absolute)
  op(0x0d, read_absolute, fp(ora))
  op(0x0e, rmw_absolute, fp(asl))
  op(0x10, branch, regs.p.n == 0)
  op(0x11, read_indirect_zero_page_y, fp(ora))
I op(0x14, nop_zero_page_x)
  op(0x15, read_zero_page_x, fp(ora))
  op(0x16, rmw_zero_page_x, fp(asl))
  op(0x18, clear_flag, regs.p.c.bit)
  op(0x19, read_absolute_y, fp(ora))
I op(0x1a, nop_implied)
I op(0x1c, nop_absolute_x)
  op(0x1d, read_absolute_x, fp(ora))
  op(0x1e, rmw_absolute_x, fp(asl))
  op(0x20, jsr_absolute)
  op(0x21, read_indirect_zero_page_x, fp(and))
  op(0x24, read_zero_page, fp(bit))
  op(0x25, read_zero_page, fp(and))
  op(0x26, rmw_zero_page, fp(rol))
  op(0x28, plp)
  op(0x29, read_immediate, fp(and))
  op(0x2a, shift, fp(rla))
  op(0x2c, read_absolute, fp(bit))
  op(0x2d, read_absolute, fp(and))
  op(0x2e, rmw_absolute, fp(rol))
  op(0x30, branch, regs.p.n == 1)
  op(0x31, read_indirect_zero_page_y, fp(and))
I op(0x34, nop_zero_page_x)
  op(0x35, read_zero_page_x, fp(and))
  op(0x36, rmw_zero_page_x, fp(rol))
  op(0x38, set_flag, regs.p.c.bit)
  op(0x39, read_absolute_y, fp(and))
I op(0x3a, nop_implied)
I op(0x3c, nop_absolute_x)
  op(0x3d, read_absolute_x, fp(and))
  op(0x3e, rmw_absolute_x, fp(rol))
  op(0x40, rti)
  op(0x41, read_indirect_zero_page_x, fp(eor))
I op(0x44, nop_zero_page)
  op(0x45, read_zero_page, fp(eor))
  op(0x46, rmw_zero_page, fp(lsr))
  op(0x48, push, regs.a)
  op(0x49, read_immediate, fp(eor))
  op(0x4a, shift, fp(sra))
  op(0x4c, jmp_absolute)
  op(0x4d, read_absolute, fp(eor))
  op(0x4e, rmw_absolute, fp(lsr))
  op(0x50, branch, regs.p.v == 0)
  op(0x51, read_indirect_zero_page_y, fp(eor))
I op(0x54, nop_zero_page_x)
  op(0x55, read_zero_page_x, fp(eor))
  op(0x56, rmw_zero_page_x, fp(lsr))
  op(0x58, clear_flag, regs.p.i.bit)
  op(0x59, read_absolute_y, fp(eor))
I op(0x5a, nop_implied)
I op(0x5c, nop_absolute_x)
  op(0x5d, read_absolute_x, fp(eor))
  op(0x5e, rmw_absolute_x, fp(lsr))
  op(0x60, rts)
  op(0x61, read_indirect_zero_page_x, fp(adc))
I op(0x64, nop_zero_page)
  op(0x65, read_zero_page, fp(adc))
  op(0x66, rmw_zero_page, fp(ror))
  op(0x68, pull, regs.a)
  op(0x69, read_immediate, fp(adc))
  op(0x6a, shift, fp(rra))
I op(0x6b, arr_immediate)
  op(0x6c, jmp_indirect_absolute)
  op(0x6d, read_absolute, fp(adc))
  op(0x6e, rmw_absolute, fp(ror))
  op(0x70, branch, regs.p.v == 1)
  op(0x71, read_indirect_zero_page_y, fp(adc))
I op(0x74, nop_zero_page_x)
  op(0x75, read_zero_page_x, fp(adc))
  op(0x76, rmw_zero_page_x, fp(ror))
  op(0x78, set_flag, regs.p.i.bit)
  op(0x79, read_absolute_y, fp(adc))
I op(0x7a, nop_implied)
I op(0x7c, nop_absolute_x)
  op(0x7d, read_absolute_x, fp(adc))
  op(0x7e, rmw_absolute_x, fp(ror))
I case 0x80: return op_nop_absolute();
  case 0x81: return op_store_indirect_zero_page_x(regs.a);
I case 0x82: return op_nop_immediate();
  case 0x84: return op_store_zero_page(regs.y);
  case 0x85: return op_store_zero_page(regs.a);
  case 0x86: return op_store_zero_page(regs.x);
  case 0x88: return op_decrement(regs.y);
I case 0x89: return op_nop_immediate();
  case 0x8a: return op_transfer(regs.x, regs.a, 1);
  case 0x8c: return op_store_absolute(regs.y);
  case 0x8d: return op_store_absolute(regs.a);
  case 0x8e: return op_store_absolute(regs.x);
  case 0x90: return op_branch(regs.p.c == 0);
  case 0x91: return op_store_indirect_zero_page_y(regs.a);
  case 0x94: return op_store_zero_page_x(regs.y);
  case 0x95: return op_store_zero_page_x(regs.a);
  case 0x96: return op_store_zero_page_y(regs.x);
  case 0x98: return op_transfer(regs.y, regs.a, 1);
  case 0x99: return op_store_absolute_y(regs.a);
  case 0x9a: return op_transfer(regs.x, regs.s, 0);
  case 0x9d: return op_store_absolute_x(regs.a);
  case 0xa0: return op_read_immediate(fp(ldy));
  case 0xa1: return op_read_indirect_zero_page_x(fp(lda));
  case 0xa2: return op_read_immediate(fp(ldx));
  case 0xa4: return op_read_zero_page(fp(ldy));
  case 0xa5: return op_read_zero_page(fp(lda));
  case 0xa6: return op_read_zero_page(fp(ldx));
  case 0xa8: return op_transfer(regs.a, regs.y, 1);
  case 0xa9: return op_read_immediate(fp(lda));
  case 0xaa: return op_transfer(regs.a, regs.x, 1);
  case 0xac: return op_read_absolute(fp(ldy));
  case 0xad: return op_read_absolute(fp(lda));
  case 0xae: return op_read_absolute(fp(ldx));
  case 0xb0: return op_branch(regs.p.c == 1);
  case 0xb1: return op_read_indirect_zero_page_y(fp(lda));
  case 0xb4: return op_read_zero_page_x(fp(ldy));
  case 0xb5: return op_read_zero_page_x(fp(lda));
  case 0xb6: return op_read_zero_page_y(fp(ldx));
  case 0xb8: return op_clear_flag(regs.p.v.bit);
  case 0xb9: return op_read_absolute_y(fp(lda));
  case 0xba: return op_transfer(regs.s, regs.x, 1);
  case 0xbc: return op_read_absolute_x(fp(ldy));
  case 0xbd: return op_read_absolute_x(fp(lda));
  case 0xbe: return op_read_absolute_y(fp(ldx));
  case 0xc0: return op_read_immediate(fp(cpy));
  case 0xc1: return op_read_indirect_zero_page_x(fp(cmp));
I case 0xc2: return op_nop_immediate();
  case 0xc4: return op_read_zero_page(fp(cpy));
  case 0xc5: return op_read_zero_page(fp(cmp));
  case 0xc6: return op_rmw_zero_page(fp(dec));
  case 0xc8: return op_increment(regs.y);
  case 0xc9: return op_read_immediate(fp(cmp));
  case 0xca: return op_decrement(regs.x);
  case 0xcc: return op_read_absolute(fp(cpy));
  case 0xcd: return op_read_absolute(fp(cmp));
  case 0xce: return op_rmw_absolute(fp(dec));
  case 0xd0: return op_branch(regs.p.z == 0);
  case 0xd1: return op_read_indirect_zero_page_y(fp(cmp));
I case 0xd4: return op_nop_zero_page_x();
  case 0xd5: return op_read_zero_page_x(fp(cmp));
  case 0xd6: return op_rmw_zero_page_x(fp(dec));
  case 0xd8: return op_clear_flag(regs.p.d.bit);
  case 0xd9: return op_read_absolute_y(fp(cmp));
I case 0xda: return op_nop_implied();
I case 0xdc: return op_nop_absolute_x();
  case 0xdd: return op_read_absolute_x(fp(cmp));
  case 0xde: return op_rmw_absolute_x(fp(dec));
  case 0xe0: return op_read_immediate(fp(cpx));
  case 0xe1: return op_read_indirect_zero_page_x(fp(sbc));
I case 0xe2: return op_nop_immediate();
  case 0xe4: return op_read_zero_page(fp(cpx));
  case 0xe5: return op_read_zero_page(fp(sbc));
  case 0xe6: return op_rmw_zero_page(fp(inc));
  case 0xe8: return op_increment(regs.x);
  case 0xe9: return op_read_immediate(fp(sbc));
  case 0xea: return op_nop();
I case 0xeb: return op_read_immediate(fp(sbc));
  case 0xec: return op_read_absolute(fp(cpx));
  case 0xed: return op_read_absolute(fp(sbc));
  case 0xee: return op_rmw_absolute(fp(inc));
  case 0xf0: return op_branch(regs.p.z == 1);
  case 0xf1: return op_read_indirect_zero_page_y(fp(sbc));
I case 0xf4: return op_nop_zero_page_x();
  case 0xf5: return op_read_zero_page_x(fp(sbc));
  case 0xf6: return op_rmw_zero_page_x(fp(inc));
  case 0xf8: return op_set_flag(regs.p.d.bit);
  case 0xf9: return op_read_absolute_y(fp(sbc));
I case 0xfa: return op_nop_implied();
I case 0xfc: return op_nop_absolute_x();
  case 0xfd: return op_read_absolute_x(fp(sbc));
  case 0xfe: return op_rmw_absolute_x(fp(inc));
  }

  //unimplemented opcode
  return op_nop();
}

#undef I
#undef op
#undef fp
