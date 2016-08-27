auto Z80::CP(uint8 x) -> void {
  uint16 y = r.a - x;

  r.p.c = y > 0xff;
  r.p.n = 1;
  r.p.v = (r.a ^ x) & (r.a ^ y) & 0x80;
  r.p.h = (r.a ^ y ^ x) & 0x10;
  r.p.z = y == 0;
  r.p.s = y & 0x80;
}

auto Z80::instructionCP_ihl() -> void {
  CP(read(r.hl));
}

auto Z80::instructionCP_n() -> void {
  CP(read(r.pc++));
}

auto Z80::instructionCP_r(uint8_t& x) -> void {
  CP(x);
}

auto Z80::instructionDI() -> void {
  r.di = 1;
}

auto Z80::instructionIM(uint mode) -> void {
  r.im = mode;
}

auto Z80::instructionIN_a_in() -> void {
  r.a = in(read(r.pc++));
}

auto Z80::instructionIN_r_ic(uint8_t& x) -> void {
  x = in(read(r.c));
  r.p.n = 0;
  r.p.p = parity(x);
  r.p.h = 0;
  r.p.z = x == 0;
  r.p.s = x & 0x80;
}

auto Z80::instructionJP_c_nn(bool c) -> void {
  auto lo = read(r.pc++);
  auto hi = read(r.pc++);
  if(c) r.pc = hi << 8 | lo << 0;
}

auto Z80::instructionJP_rr(uint16_t& x) -> void {
  r.pc = x;
}

auto Z80::instructionJR_c(bool c) -> void {
  auto d = read(r.pc++);
  if(c) r.pc += (int8)d;
}

auto Z80::instructionNOP() -> void {
}
