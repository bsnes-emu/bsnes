auto Z80::trap(uint8 prefix, uint8 code) -> void {
  print("[Z80] unimplemented instruction: ", prefix ? pad(hex(prefix, 2L), -3L, ' ') : "", hex(code, 2L), "\n");
  print("[Z80] instructions executed: ", --instructionsExecuted, "\n");
  while(true) wait();
}

auto Z80::instruction() -> void {
  #if 1
  if(instructionsExecuted < 20)
  print(disassemble(r.pc), "\n");
  #endif

  instructionsExecuted++;

  auto code = opcode();
  if(code == 0xdd || code == 0xfd) {
    r.prefix = code;
    return;
  }
  instruction__(code);
  r.prefix = 0x00;
}

#define op(id, name, ...) case id: return instruction##name(__VA_ARGS__);

#define A r.a
#define F r.f
#define B r.b
#define C r.c
#define D r.d
#define E r.e
#define H r.prefix == 0xdd ? r.ixh : r.prefix == 0xfd ? r.iyh : r.h
#define L r.prefix == 0xdd ? r.ixl : r.prefix == 0xfd ? r.iyl : r.l

#define AF r.af
#define BC r.bc
#define DE r.de
#define HL r.prefix == 0xdd ? r.ix : r.prefix == 0xfd ? r.iy : r.hl
#define SP r.sp

#define CF r.p.c
#define NF r.p.n
#define PF r.p.p
#define VF r.p.v
#define XF r.p.x
#define HF r.p.h
#define YF r.p.y
#define ZF r.p.z
#define SF r.p.s

auto Z80::instruction__(uint8 code) -> void {
  switch(code) {
  op(0x00, NOP)
  op(0x01, LD_rr_nn, BC)
  op(0x06, LD_r_n, B)
  op(0x0e, LD_r_n, C)
  op(0x11, LD_rr_nn, DE)
  op(0x16, LD_r_n, D)
  op(0x18, JR_c_e, 1)
  op(0x1e, LD_r_n, E)
  op(0x20, JR_c_e, ZF == 0)
  op(0x21, LD_rr_nn, HL)
  op(0x26, LD_r_n, H)
  op(0x28, JR_c_e, ZF == 1)
  op(0x2e, LD_r_n, L)
  op(0x30, JR_c_e, CF == 0)
  op(0x31, LD_rr_nn, SP)
  op(0x32, LD_inn_a)
  op(0x36, LD_irr_n, HL)
  op(0x38, JR_c_e, CF == 1)
  op(0x3e, LD_r_n, A)
  op(0x70, LD_irr_r, HL, B)
  op(0x71, LD_irr_r, HL, C)
  op(0x72, LD_irr_r, HL, D)
  op(0x73, LD_irr_r, HL, E)
  op(0x74, LD_irr_r, HL, r.h)
  op(0x75, LD_irr_r, HL, r.l)
  op(0x77, LD_irr_r, HL, A)
  op(0xb8, CP_r, B)
  op(0xb9, CP_r, C)
  op(0xba, CP_r, D)
  op(0xbb, CP_r, E)
  op(0xbc, CP_r, H)
  op(0xbd, CP_r, L)
  op(0xbe, CP_irr, HL)
  op(0xbf, CP_r, A)
  op(0xc2, JP_c_nn, ZF == 0)
  op(0xc3, JP_c_nn, 1)
  op(0xca, JP_c_nn, ZF == 1)
  op(0xcb, CB, opcode())
  op(0xd2, JP_c_nn, CF == 0)
  op(0xda, JP_c_nn, CF == 1)
  op(0xdb, IN_a_in)
  op(0xe2, JP_c_nn, PF == 0)
  op(0xea, JP_c_nn, PF == 1)
  op(0xed, ED, opcode())
  op(0xf2, JP_c_nn, SF == 0)
  op(0xf3, DI)
  op(0xfa, JP_c_nn, SF == 1)
  op(0xfb, EI)
  op(0xfe, CP_n)
  }

  trap(0x00, code);
}

auto Z80::instructionCB(uint8 code) -> void {
  switch(code) {
  }

  trap(0xcb, code);
}

auto Z80::instructionED(uint8 code) -> void {
  switch(code) {
  op(0x46, IM_o, 0)
  op(0x56, IM_o, 1)
  op(0x5e, IM_o, 2)
  }

  trap(0xed, code);
}

#undef op

#undef A
#undef F
#undef B
#undef C
#undef D
#undef E
#undef H
#undef L

#undef AF
#undef BC
#undef DE
#undef HL
#undef SP

#undef CF
#undef NF
#undef PF
#undef VF
#undef XF
#undef HF
#undef YF
#undef ZF
#undef SF
