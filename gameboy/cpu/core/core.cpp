#ifdef CPU_CPP

#include "disassembler.cpp"

unsigned opcode_counter = 0;

void CPU::op_unknown() {
  uint8 opcode = bus.read(--r[PC]);
  print(
    "CPU: unknown opcode [", hex<2>(opcode), "]\n",
    "af:", hex<4>(r[AF]), " bc:", hex<4>(r[BC]), " de:", hex<4>(r[DE]), " hl:", hex<4>(r[HL]), " ",
    "sp:", hex<4>(r[SP]), " pc:", hex<4>(r[PC]), "\n",
    "ly:", decimal<3, ' '>(status.lycounter), " exec:", opcode_counter, "\n"
  );
  while(true) scheduler.exit();
}

//8-bit load commands

template<unsigned x> void CPU::op_ld_r_n() {
  r[x] = op_read(r[PC]++);
}

void CPU::op_ldd_hl_a() {
  op_write(r[HL], r[A]);
  r[HL]--;
}

//16-bit load commands

template<unsigned x, unsigned y> void CPU::op_ld_rr_nn() {
  r[y] = op_read(r[PC]++);
  r[x] = op_read(r[PC]++);
}

//8-bit arithmetic commands

template<unsigned x> void CPU::op_xor_r() {
  r[A] ^= r[x];
  r.f.z = r[A] == 0;
  r.f.n = 0;
  r.f.h = 0;
  r.f.c = 0;
}

template<unsigned x> void CPU::op_dec_r() {
  r[x]--;
  r.f.z = r[x] == 0;
  r.f.n = 0;  //???
  r.f.h = 0;  //???
}

//control commands

void CPU::op_nop() {
}

//jump commands

void CPU::op_jp_nn() {
  uint8 lo = op_read(r[PC]++);
  uint8 hi = op_read(r[PC]++);
  r[PC] = (hi << 8) | (lo << 0);
  op_io();
}

template<unsigned x, bool y> void CPU::op_jr_f_n() {
  int8 n = op_read(r[PC]++);
  if(r.f[x] == y) {
    r[PC] += n;
    op_io();
  }
}

void CPU::initialize_opcode_table() {
  for(unsigned n = 0; n < 256; n++) opcode_table[n] = &CPU::op_unknown;

  opcode_table[0x00] = &CPU::op_nop;
  opcode_table[0x05] = &CPU::op_dec_r<B>;
  opcode_table[0x06] = &CPU::op_ld_r_n<B>;
  opcode_table[0x0d] = &CPU::op_dec_r<C>;
  opcode_table[0x0e] = &CPU::op_ld_r_n<C>;
  opcode_table[0x20] = &CPU::op_jr_f_n<ZF, 0>;
  opcode_table[0x21] = &CPU::op_ld_rr_nn<H, L>;
  opcode_table[0x32] = &CPU::op_ldd_hl_a;
  opcode_table[0x3e] = &CPU::op_ld_r_n<A>;
  opcode_table[0xaf] = &CPU::op_xor_r<A>;
  opcode_table[0xc3] = &CPU::op_jp_nn;
}

#endif
