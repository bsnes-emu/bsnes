uint8 bAPU::op_adc(uint8 x, uint8 y) {
int16 r = x + y + regs.p.c;
  regs.p.n = !!(r & 0x80);
  regs.p.v = !!(~(x ^ y) & (y ^ (uint8)r) & 0x80);
  regs.p.h = !!((x ^ y ^ (uint8)r) & 0x10);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r > 0xff);
  return r;
}

uint16 bAPU::op_addw(uint16 x, uint16 y) {
int32 r = x + y;
  regs.p.n = !!(r & 0x8000);
  regs.p.v = !!(~(x ^ y) & (y ^ (uint16)r) & 0x8000);
  regs.p.h = !!((x ^ y ^ (uint16)r) & 0x10);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r > 0xffff);
  return r;
}

uint8 bAPU::op_and(uint8 x, uint8 y) {
  x &= y;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 bAPU::op_cmp(uint8 x, uint8 y) {
int16 r = x - y;
  regs.p.n = !!(r & 0x80);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
  return x;
}

uint16 bAPU::op_cmpw(uint16 x, uint16 y) {
int32 r = x - y;
  regs.p.n = !!(r & 0x8000);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
  return x;
}

uint8 bAPU::op_eor(uint8 x, uint8 y) {
  x ^= y;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 bAPU::op_or(uint8 x, uint8 y) {
  x |= y;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 bAPU::op_sbc(uint8 x, uint8 y) {
int16 r = x - y - !regs.p.c;
  regs.p.n = !!(r & 0x80);
  regs.p.v = !!((x ^ y) & (x ^ (uint8)r) & 0x80);
  regs.p.h = !((x ^ y ^ (uint8)r) & 0x10);
  regs.p.z = ((uint8)r == 0);
  regs.p.c = (r >= 0);
  return r;
}

uint16 bAPU::op_subw(uint16 x, uint16 y) {
int32 r = x - y;
  regs.p.n = !!(r & 0x8000);
  regs.p.v = !!((x ^ y) & (x ^ (uint16)r) & 0x8000);
  regs.p.h = !((x ^ y ^ (uint16)r) & 0x10);
  regs.p.z = ((uint16)r == 0);
  regs.p.c = (r >= 0);
  return r;
}

uint8 bAPU::op_inc(uint8 x) {
  x++;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint16 bAPU::op_incw(uint16 x) {
  x++;
  regs.p.n = !!(x & 0x8000);
  regs.p.z = (x == 0);
  return x;
}

uint8 bAPU::op_dec(uint8 x) {
  x--;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint16 bAPU::op_decw(uint16 x) {
  x--;
  regs.p.n = !!(x & 0x8000);
  regs.p.z = (x == 0);
  return x;
}

uint8 bAPU::op_asl(uint8 x) {
  regs.p.c = !!(x & 0x80);
  x <<= 1;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 bAPU::op_lsr(uint8 x) {
  regs.p.c = !!(x & 0x01);
  x >>= 1;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 bAPU::op_rol(uint8 x) {
uint8 c = regs.p.c;
  regs.p.c = !!(x & 0x80);
  x <<= 1;
  x |= c;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}

uint8 bAPU::op_ror(uint8 x) {
uint8 c = (regs.p.c)?0x80:0x00;
  regs.p.c = !!(x & 0x01);
  x >>= 1;
  x |= c;
  regs.p.n = !!(x & 0x80);
  regs.p.z = (x == 0);
  return x;
}
