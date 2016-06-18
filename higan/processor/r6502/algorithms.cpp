auto R6502::fp_adc() {
  int result = regs.a + rd + regs.p.c;
  regs.p.v = ~(regs.a ^ rd) & (regs.a ^ result) & 0x80;
  regs.p.c = (result > 0xff);
  regs.p.n = (result & 0x80);
  regs.p.z = ((uint8)result == 0);
  regs.a = result;
}

auto R6502::fp_and() {
  regs.a &= rd;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto R6502::fp_asl() {
  regs.p.c = rd & 0x80;
  rd <<= 1;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

auto R6502::fp_bit() {
  regs.p.n = (rd & 0x80);
  regs.p.v = (rd & 0x40);
  regs.p.z = ((rd & regs.a) == 0);
}

auto R6502::fp_cmp() {
  int r = regs.a - rd;
  regs.p.n = (r & 0x80);
  regs.p.z = (uint8)(r == 0);
  regs.p.c = (r >= 0);
}

auto R6502::fp_cpx() {
  int r = regs.x - rd;
  regs.p.n = (r & 0x80);
  regs.p.z = (uint8)(r == 0);
  regs.p.c = (r >= 0);
}

auto R6502::fp_cpy() {
  int r = regs.y - rd;
  regs.p.n = (r & 0x80);
  regs.p.z = (uint8)(r == 0);
  regs.p.c = (r >= 0);
}

auto R6502::fp_dec() {
  rd--;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

auto R6502::fp_eor() {
  regs.a ^= rd;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto R6502::fp_inc() {
  rd++;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

auto R6502::fp_lda() {
  regs.a = rd;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto R6502::fp_ldx() {
  regs.x = rd;
  regs.p.n = (regs.x & 0x80);
  regs.p.z = (regs.x == 0);
}

auto R6502::fp_ldy() {
  regs.y = rd;
  regs.p.n = (regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

auto R6502::fp_lsr() {
  regs.p.c = rd & 0x01;
  rd >>= 1;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

auto R6502::fp_ora() {
  regs.a |= rd;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto R6502::fp_rla() {
  uint carry = (uint)regs.p.c;
  regs.p.c = regs.a & 0x80;
  regs.a = (regs.a << 1) | carry;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto R6502::fp_rol() {
  uint carry = (uint)regs.p.c;
  regs.p.c = rd & 0x80;
  rd = (rd << 1) | carry;
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

auto R6502::fp_ror() {
  uint carry = (uint)regs.p.c << 7;
  regs.p.c = rd & 0x01;
  rd = carry | (rd >> 1);
  regs.p.n = (rd & 0x80);
  regs.p.z = (rd == 0);
}

auto R6502::fp_rra() {
  uint carry = (uint)regs.p.c << 7;
  regs.p.c = regs.a & 0x01;
  regs.a = carry | (regs.a >> 1);
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto R6502::fp_sbc() {
  rd ^= 0xff;
  return fp_adc();
}

auto R6502::fp_sla() {
  regs.p.c = regs.a & 0x80;
  regs.a <<= 1;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

auto R6502::fp_sra() {
  regs.p.c = regs.a & 0x01;
  regs.a >>= 1;
  regs.p.n = (regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}
