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

#define A  (uint8&)(r.a)
#define F  (uint8&)(r.f)
#define B  (uint8&)(r.b)
#define C  (uint8&)(r.c)
#define D  (uint8&)(r.d)
#define E  (uint8&)(r.e)
#define H  (uint8&)(r.prefix == 0xdd ? r.ixh : r.prefix == 0xfd ? r.iyh : r.h)
#define L  (uint8&)(r.prefix == 0xdd ? r.ixl : r.prefix == 0xfd ? r.iyl : r.l)
#define HD (uint8&)(r.h)
#define LD (uint8&)(r.l)

#define AF (uint16&)(r.af)
#define BC (uint16&)(r.bc)
#define DE (uint16&)(r.de)
#define HL (uint16&)(r.prefix == 0xdd ? r.ix : r.prefix == 0xfd ? r.iy : r.hl)
#define SP (uint16&)(r.sp)

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
  op(0x40, LD_r_r, B, B)
  op(0x41, LD_r_r, B, C)
  op(0x42, LD_r_r, B, D)
  op(0x43, LD_r_r, B, E)
  op(0x44, LD_r_r, B, H)
  op(0x45, LD_r_r, B, L)
  op(0x46, LD_r_irr, B, HL)
  op(0x47, LD_r_r, B, A)
  op(0x48, LD_r_r, C, B)
  op(0x49, LD_r_r, C, C)
  op(0x4a, LD_r_r, C, D)
  op(0x4b, LD_r_r, C, E)
  op(0x4c, LD_r_r, C, H)
  op(0x4d, LD_r_r, C, L)
  op(0x4e, LD_r_irr, C, HL)
  op(0x4f, LD_r_r, C, A)
  op(0x50, LD_r_r, D, B)
  op(0x51, LD_r_r, D, C)
  op(0x52, LD_r_r, D, D)
  op(0x53, LD_r_r, D, E)
  op(0x54, LD_r_r, D, H)
  op(0x55, LD_r_r, D, L)
  op(0x56, LD_r_irr, D, HL)
  op(0x57, LD_r_r, D, A)
  op(0x58, LD_r_r, E, B)
  op(0x59, LD_r_r, E, C)
  op(0x5a, LD_r_r, E, D)
  op(0x5b, LD_r_r, E, E)
  op(0x5c, LD_r_r, E, H)
  op(0x5d, LD_r_r, E, L)
  op(0x5e, LD_r_irr, E, HL)
  op(0x5f, LD_r_r, E, A)
  op(0x60, LD_r_r, H, B)
  op(0x61, LD_r_r, H, C)
  op(0x62, LD_r_r, H, D)
  op(0x63, LD_r_r, H, E)
  op(0x64, LD_r_r, H, H)
  op(0x65, LD_r_r, H, L)
  op(0x66, LD_r_irr, HD, HL)
  op(0x67, LD_r_r, H, A)
  op(0x68, LD_r_r, L, B)
  op(0x69, LD_r_r, L, C)
  op(0x6a, LD_r_r, L, D)
  op(0x6b, LD_r_r, L, E)
  op(0x6c, LD_r_r, L, H)
  op(0x6d, LD_r_r, L, L)
  op(0x6e, LD_r_irr, LD, HL)
  op(0x6f, LD_r_r, L, A)
  op(0x70, LD_irr_r, HL, B)
  op(0x71, LD_irr_r, HL, C)
  op(0x72, LD_irr_r, HL, D)
  op(0x73, LD_irr_r, HL, E)
  op(0x74, LD_irr_r, HL, HD)
  op(0x75, LD_irr_r, HL, LD)
  op(0x76, HALT)
  op(0x77, LD_irr_r, HL, A)
  op(0x78, LD_r_r, A, B)
  op(0x79, LD_r_r, A, C)
  op(0x7a, LD_r_r, A, D)
  op(0x7b, LD_r_r, A, E)
  op(0x7c, LD_r_r, A, H)
  op(0x7d, LD_r_r, A, L)
  op(0x7e, LD_r_irr, A, HL)
  op(0x7f, LD_r_r, A, A)
  op(0x80, ADD_a_r, B)
  op(0x81, ADD_a_r, C)
  op(0x82, ADD_a_r, D)
  op(0x83, ADD_a_r, E)
  op(0x84, ADD_a_r, H)
  op(0x85, ADD_a_r, L)
  op(0x86, ADD_a_irr, HL)
  op(0x87, ADD_a_r, A)
  op(0x88, ADC_a_r, B)
  op(0x89, ADC_a_r, C)
  op(0x8a, ADC_a_r, D)
  op(0x8b, ADC_a_r, E)
  op(0x8c, ADC_a_r, H)
  op(0x8d, ADC_a_r, L)
  op(0x8e, ADC_a_irr, HL)
  op(0x8f, ADC_a_r, A)
  op(0x90, SUB_a_r, B)
  op(0x91, SUB_a_r, C)
  op(0x92, SUB_a_r, D)
  op(0x93, SUB_a_r, E)
  op(0x94, SUB_a_r, H)
  op(0x95, SUB_a_r, L)
  op(0x96, SUB_a_irr, HL)
  op(0x97, SUB_a_r, A)
  op(0x98, SBC_a_r, B)
  op(0x99, SBC_a_r, C)
  op(0x9a, SBC_a_r, D)
  op(0x9b, SBC_a_r, E)
  op(0x9c, SBC_a_r, H)
  op(0x9d, SBC_a_r, L)
  op(0x9e, SBC_a_irr, HL)
  op(0x9f, SBC_a_r, A)
  op(0xa0, AND_a_r, B)
  op(0xa1, AND_a_r, C)
  op(0xa2, AND_a_r, D)
  op(0xa3, AND_a_r, E)
  op(0xa4, AND_a_r, H)
  op(0xa5, AND_a_r, L)
  op(0xa6, AND_a_irr, HL)
  op(0xa7, AND_a_r, A)
  op(0xa8, XOR_a_r, B)
  op(0xa9, XOR_a_r, C)
  op(0xaa, XOR_a_r, D)
  op(0xab, XOR_a_r, E)
  op(0xac, XOR_a_r, H)
  op(0xad, XOR_a_r, L)
  op(0xae, XOR_a_irr, HL)
  op(0xaf, XOR_a_r, A)
  op(0xb0, OR_a_r, B)
  op(0xb1, OR_a_r, C)
  op(0xb2, OR_a_r, D)
  op(0xb3, OR_a_r, E)
  op(0xb4, OR_a_r, H)
  op(0xb5, OR_a_r, L)
  op(0xb6, OR_a_irr, HL)
  op(0xb7, OR_a_r, A)
  op(0xb8, CP_a_r, B)
  op(0xb9, CP_a_r, C)
  op(0xba, CP_a_r, D)
  op(0xbb, CP_a_r, E)
  op(0xbc, CP_a_r, H)
  op(0xbd, CP_a_r, L)
  op(0xbe, CP_a_irr, HL)
  op(0xbf, CP_a_r, A)
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
  op(0xfe, CP_a_n)
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
#undef HD
#undef LD

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
