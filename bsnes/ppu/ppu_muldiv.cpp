byte mmio_r2134(void) {
ulong r = ((signed short)ppu.m7a * (signed char)(ppu.m7b >> 8));
  return (r);
}

byte mmio_r2135(void) {
ulong r = ((signed short)ppu.m7a * (signed char)(ppu.m7b >> 8));
  return (r >> 8);
}

byte mmio_r2136(void) {
ulong r = ((signed short)ppu.m7a * (signed char)(ppu.m7b >> 8));
  return (r >> 16);
}

void mmio_w4202(byte value) {
  ppu.mul_a = value;
}

void mmio_w4203(byte value) {
  ppu.mul_b = value;
  ppu.r_4216 = ppu.mul_a * ppu.mul_b;
}

void mmio_w4204(byte value) {
  ppu.div_a = (ppu.div_a & 0xff00) | value;
}

void mmio_w4205(byte value) {
  ppu.div_a = (ppu.div_a & 0x00ff) | (value << 8);
}

void mmio_w4206(byte value) {
  ppu.div_b = value;

  ppu.r_4214 = (ppu.div_b)?ppu.div_a / ppu.div_b : 0;
  ppu.r_4216 = (ppu.div_b)?ppu.div_a % ppu.div_b : 0;
}

byte mmio_r4214(void) {
  return ppu.r_4214;
}

byte mmio_r4215(void) {
  return ppu.r_4214 >> 8;
}

byte mmio_r4216(void) {
  return ppu.r_4216;
}

byte mmio_r4217(void) {
  return ppu.r_4216 >> 8;
}
