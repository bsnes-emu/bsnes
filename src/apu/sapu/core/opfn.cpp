uint8 sAPU::op_adc(uint8 x, uint8 y) {
int16 r = x + y + regs.p.c;
  regs.p.n = bool(r & 0x80);
  regs.p.v = bool(~(x ^ y) & (y ^ (uint8)r) & 0x80);
  regs.p.h = bool((x ^ y ^ (uint8)r) & 0x10);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r > 0xff);
  return r;
}

uint16 sAPU::op_addw(uint16 x, uint16 y) {
int16 r;
  regs.p.c = 0;
  r  = op_adc(x, y);
  r |= op_adc(x >> 8, y >> 8) << 8;
  regs.p.z = ((uint16)r == 0);
  return r;
}

uint8 sAPU::op_and(uint8 x, uint8 y) {
  x &= y;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 sAPU::op_cmp(uint8 x, uint8 y) {
int16 r = x - y;
  regs.p.n = bool(r & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
  return x;
}

uint16 sAPU::op_cmpw(uint16 x, uint16 y) {
int32 r = x - y;
  regs.p.n = bool(r & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
  return x;
}

uint8 sAPU::op_eor(uint8 x, uint8 y) {
  x ^= y;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 sAPU::op_or(uint8 x, uint8 y) {
  x |= y;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 sAPU::op_sbc(uint8 x, uint8 y) {
int16 r = x - y - !regs.p.c;
  regs.p.n = bool(r & 0x80);
  regs.p.v = bool((x ^ y) & (x ^ (uint8)r) & 0x80);
  regs.p.h = !((x ^ y ^ (uint8)r) & 0x10);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
  return r;
}

uint16 sAPU::op_subw(uint16 x, uint16 y) {
int16 r;
  regs.p.c = 1;
  r  = op_sbc(x, y);
  r |= op_sbc(x >> 8, y >> 8) << 8;
  regs.p.z = ((uint16)r == 0);
  return r;
}

uint8 sAPU::op_inc(uint8 x) {
  x++;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint16 sAPU::op_incw(uint16 x) {
  x++;
  regs.p.n = bool(x & 0x8000);
  regs.p.z = (x == 0);
  return x;
}

uint8 sAPU::op_dec(uint8 x) {
  x--;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint16 sAPU::op_decw(uint16 x) {
  x--;
  regs.p.n = bool(x & 0x8000);
  regs.p.z = (x == 0);
  return x;
}

uint8 sAPU::op_asl(uint8 x) {
  regs.p.c = bool(x & 0x80);
  x <<= 1;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 sAPU::op_lsr(uint8 x) {
  regs.p.c = bool(x & 0x01);
  x >>= 1;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 sAPU::op_rol(uint8 x) {
uint8 c = regs.p.c;
  regs.p.c = bool(x & 0x80);
  x <<= 1;
  x |= c;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 sAPU::op_ror(uint8 x) {
uint8 c = (regs.p.c)?0x80:0x00;
  regs.p.c = bool(x & 0x01);
  x >>= 1;
  x |= c;
  regs.p.n = bool(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}
