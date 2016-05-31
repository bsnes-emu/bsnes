auto GSU::instruction(uint8 opcode) -> void {
  #define op(id, name, ...) \
    case id: return op_##name(__VA_ARGS__); \

  #define op4(id, name) \
    case id+ 0: return op_##name((uint4)opcode); \
    case id+ 1: return op_##name((uint4)opcode); \
    case id+ 2: return op_##name((uint4)opcode); \
    case id+ 3: return op_##name((uint4)opcode); \

  #define op6(id, name) \
    op4(id, name) \
    case id+ 4: return op_##name((uint4)opcode); \
    case id+ 5: return op_##name((uint4)opcode); \

  #define op12(id, name) \
    op6(id, name) \
    case id+ 6: return op_##name((uint4)opcode); \
    case id+ 7: return op_##name((uint4)opcode); \
    case id+ 8: return op_##name((uint4)opcode); \
    case id+ 9: return op_##name((uint4)opcode); \
    case id+10: return op_##name((uint4)opcode); \
    case id+11: return op_##name((uint4)opcode); \

  #define op15(id, name) \
    op12(id, name) \
    case id+12: return op_##name((uint4)opcode); \
    case id+13: return op_##name((uint4)opcode); \
    case id+14: return op_##name((uint4)opcode); \

  #define op16(id, name) \
    op15(id, name) \
    case id+15: return op_##name((uint4)opcode); \

  switch(opcode) {
  op  (0x00, stop)
  op  (0x01, nop)
  op  (0x02, cache)
  op  (0x03, lsr)
  op  (0x04, rol)
  op  (0x05, branch, 1)  //bra
  op  (0x06, branch, (regs.sfr.s ^ regs.sfr.ov) == 0)  //blt
  op  (0x07, branch, (regs.sfr.s ^ regs.sfr.ov) == 1)  //bge
  op  (0x08, branch, regs.sfr.z == 0)  //bne
  op  (0x09, branch, regs.sfr.z == 1)  //beq
  op  (0x0a, branch, regs.sfr.s == 0)  //bpl
  op  (0x0b, branch, regs.sfr.s == 1)  //bmi
  op  (0x0c, branch, regs.sfr.cy == 0)  //bcc
  op  (0x0d, branch, regs.sfr.cy == 1)  //bcs
  op  (0x0e, branch, regs.sfr.ov == 0)  //bvc
  op  (0x0f, branch, regs.sfr.ov == 1)  //bvs
  op16(0x10, to_move)
  op16(0x20, with)
  op12(0x30, store)
  op  (0x3c, loop)
  op  (0x3d, alt1)
  op  (0x3e, alt2)
  op  (0x3f, alt3)
  op12(0x40, load)
  op  (0x4c, plot_rpix)
  op  (0x4d, swap)
  op  (0x4e, color_cmode)
  op  (0x4f, not)
  op16(0x50, add_adc)
  op16(0x60, sub_sbc_cmp)
  op  (0x70, merge)
  op15(0x71, and_bic)
  op16(0x80, mult_umult)
  op  (0x90, sbk)
  op4 (0x91, link)
  op  (0x95, sex)
  op  (0x96, asr_div2)
  op  (0x97, ror)
  op6 (0x98, jmp_ljmp)
  op  (0x9e, lob)
  op  (0x9f, fmult_lmult)
  op16(0xa0, ibt_lms_sms)
  op16(0xb0, from_moves)
  op  (0xc0, hib)
  op15(0xc1, or_xor)
  op15(0xd0, inc)
  op15(0xe0, dec)
  op  (0xdf, getc_ramb_romb)
  op  (0xef, getb)
  op16(0xf0, iwt_lm_sm)
  }

  #undef op
  #undef op4
  #undef op6
  #undef op12
  #undef op15
  #undef op16
}
