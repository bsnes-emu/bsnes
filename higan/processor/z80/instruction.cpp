auto Z80::trap(uint8_t prefix, uint8_t opcode) -> void {
  print("[Z80] unimplemented instruction: ", prefix ? pad(hex(prefix, 2L), ' ', -3) : "", hex(opcode, 2L), "\n");
  print("[Z80] instructions executed: ", --instructionsExecuted, "\n");
  while(true) wait();
}

#define op(id, name, ...) case id: return instruction##name(__VA_ARGS__);

auto Z80::instruction() -> void {
  #if 1
  if(instructionsExecuted < 20)
  print(disassemble(r.pc), "\n");
  #endif

  instructionsExecuted++;

  r.r = (r.r & 0x80) | (r.r + 1 & 0x7f);

  auto opcode = read(r.pc++);
  switch(opcode) {
  op(0x00, NOP)
  op(0x18, JR_c, true)
  op(0x20, JR_c, r.p.z == 0)
  op(0x28, JR_c, r.p.z == 1)
  op(0x30, JR_c, r.p.c == 0)
  op(0x38, JR_c, r.p.c == 1)
  op(0xb8, CP_r, r.b)
  op(0xb9, CP_r, r.c)
  op(0xba, CP_r, r.d)
  op(0xbb, CP_r, r.e)
  op(0xbc, CP_r, r.h)
  op(0xbd, CP_r, r.l)
  op(0xbe, CP_ihl)
  op(0xbf, CP_r, r.a)
  op(0xc2, JP_c_nn, r.p.z == 0)
  op(0xc3, JP_c_nn, true)
  op(0xca, JP_c_nn, r.p.z == 1)
  op(0xd2, JP_c_nn, r.p.c == 0)
  op(0xda, JP_c_nn, r.p.c == 1)
  op(0xdb, IN_a_in)
  op(0xdd, DD)
  op(0xe2, JP_c_nn, r.p.p == 0)
  op(0xe9, JP_rr, r.hl)
  op(0xea, JP_c_nn, r.p.p == 1)
  op(0xed, ED)
  op(0xf2, JP_c_nn, r.p.s == 0)
  op(0xf3, DI)
  op(0xfa, JP_c_nn, r.p.s == 1)
  op(0xfd, FD)
  op(0xfe, CP_n)
  }
  trap(0x00, opcode);
}

auto Z80::instructionDD() -> void {
  auto opcode = read(r.pc++);
  switch(opcode) {
  op(0xe9, JP_rr, r.ix)
  }
  trap(0xdd, opcode);
}

auto Z80::instructionED() -> void {
  auto opcode = read(r.pc++);
  switch(opcode) {
  op(0x40, IN_r_ic, r.b)
  op(0x46, IM, 0)
  op(0x48, IN_r_ic, r.c)
  op(0x50, IN_r_ic, r.d)
  op(0x56, IM, 1)
  op(0x58, IN_r_ic, r.e)
  op(0x5e, IM, 2)
  op(0x60, IN_r_ic, r.h)
  op(0x68, IN_r_ic, r.l)
  op(0x70, IN_r_ic, r.flag)
  op(0x78, IN_r_ic, r.a)
  }
  trap(0xed, opcode);
}

auto Z80::instructionFD() -> void {
  auto opcode = read(r.pc++);
  switch(opcode) {
  op(0xe9, JP_rr, r.iy)
  }
  trap(0xfd, opcode);
}

#undef op
