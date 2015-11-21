auto SPC700::op_adc(uint8 x, uint8 y) -> uint8 {
  int r = x + y + regs.p.c;
  regs.p.n = r & 0x80;
  regs.p.v = ~(x ^ y) & (x ^ r) & 0x80;
  regs.p.h = (x ^ y ^ r) & 0x10;
  regs.p.z = (uint8)r == 0;
  regs.p.c = r > 0xff;
  return r;
}

auto SPC700::op_and(uint8 x, uint8 y) -> uint8 {
  x &= y;
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_asl(uint8 x) -> uint8 {
  regs.p.c = x & 0x80;
  x <<= 1;
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_cmp(uint8 x, uint8 y) -> uint8 {
  int r = x - y;
  regs.p.n = r & 0x80;
  regs.p.z = (uint8)r == 0;
  regs.p.c = r >= 0;
  return x;
}

auto SPC700::op_dec(uint8 x) -> uint8 {
  x--;
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_eor(uint8 x, uint8 y) -> uint8 {
  x ^= y;
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_inc(uint8 x) -> uint8 {
  x++;
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_ld(uint8 x, uint8 y) -> uint8 {
  regs.p.n = y & 0x80;
  regs.p.z = y == 0;
  return y;
}

auto SPC700::op_lsr(uint8 x) -> uint8 {
  regs.p.c = x & 0x01;
  x >>= 1;
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_or(uint8 x, uint8 y) -> uint8 {
  x |= y;
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_rol(uint8 x) -> uint8 {
  uint carry = regs.p.c << 0;
  regs.p.c = x & 0x80;
  x = (x << 1) | carry;
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_ror(uint8 x) -> uint8 {
  uint carry = regs.p.c << 7;
  regs.p.c = x & 0x01;
  x = carry | (x >> 1);
  regs.p.n = x & 0x80;
  regs.p.z = x == 0;
  return x;
}

auto SPC700::op_sbc(uint8 x, uint8 y) -> uint8 {
  return op_adc(x, ~y);
}

auto SPC700::op_st(uint8 x, uint8 y) -> uint8 {
  return y;
}

//

auto SPC700::op_adw(uint16 x, uint16 y) -> uint16 {
  uint16 r;
  regs.p.c = 0;
  r  = op_adc(x, y);
  r |= op_adc(x >> 8, y >> 8) << 8;
  regs.p.z = r == 0;
  return r;
}

auto SPC700::op_cpw(uint16 x, uint16 y) -> uint16 {
  int r = x - y;
  regs.p.n = r & 0x8000;
  regs.p.z = (uint16)r == 0;
  regs.p.c = r >= 0;
  return x;
}

auto SPC700::op_ldw(uint16 x, uint16 y) -> uint16 {
  regs.p.n = y & 0x8000;
  regs.p.z = y == 0;
  return y;
}

auto SPC700::op_sbw(uint16 x, uint16 y) -> uint16 {
  uint16 r;
  regs.p.c = 1;
  r  = op_sbc(x, y);
  r |= op_sbc(x >> 8, y >> 8) << 8;
  regs.p.z = r == 0;
  return r;
}
