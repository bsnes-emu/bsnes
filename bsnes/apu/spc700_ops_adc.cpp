byte spc700_op_adc(byte x, byte y) {
word r = x + y + (spc700->regs.p & SPF_C);
  spc700_testn(r & 0x80);
  spc700_testv((~(x ^ y) & (y ^ (byte)r) & 0x80));
  spc700_testh((x ^ y ^ (byte)r) & 0x10);
  spc700_testz((byte)r == 0);
  spc700_testc(r >= 0x100);
  return (byte)r;
}

void spc700_op_adc_a_const(void) {
spc700_prefetchb();
  spc700->regs.a = spc700_op_adc(spc700->regs.a, arg);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_adc_a_ix(void) {
  spc700->regs.a = spc700_op_adc(spc700->regs.a, spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0));
  spc700_incpc(1);
  add_apu_cycles(3);
}

void spc700_op_adc_a_dp(void) {
spc700_prefetchb();
  spc700->regs.a = spc700_op_adc(spc700->regs.a, spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_adc_a_dpx(void) {
spc700_prefetchb();
  spc700->regs.a = spc700_op_adc(spc700->regs.a, spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_adc_a_addr(void) {
spc700_prefetchw();
  spc700->regs.a = spc700_op_adc(spc700->regs.a, spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_adc_a_addrx(void) {
spc700_prefetchw();
  spc700->regs.a = spc700_op_adc(spc700->regs.a, spc700->mem_read(APUMODE_ADDRX, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_adc_a_addry(void) {
spc700_prefetchw();
  spc700->regs.a += spc700->mem_read(APUMODE_ADDRY, MEMSIZE_BYTE, arg);
  spc700->regs.a += (spc700->regs.p & PF_C);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_adc_a_idpx(void) {
spc700_prefetchb();
word i;
  spc700->regs.a = spc700_op_adc(spc700->regs.a, spc700->mem_read(APUMODE_IDPX, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_adc_a_idpy(void) {
spc700_prefetchb();
  spc700->regs.a = spc700_op_adc(spc700->regs.a, spc700->mem_read(APUMODE_IDPY, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_adc_ix_iy(void) {
byte x = spc700_op_adc(spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0), spc700->mem_read(APUMODE_IY, MEMSIZE_BYTE, 0));
  spc700->mem_write(APUMODE_IX, MEMSIZE_BYTE, 0, x);
  spc700_incpc(1);
  add_apu_cycles(5);
}

void spc700_op_adc_dp_dp(void) {
spc700_prefetch2b();
byte x = spc700_op_adc(spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg1), spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0));
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg1, x);
  spc700_incpc(3);
  add_apu_cycles(6);
}

void spc700_op_adc_dp_imm(void) {
spc700_prefetch2b();
byte x = spc700_op_adc(spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg1), arg0);
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg1, x);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_addw_ya_dp(void) {
spc700_prefetchb();
word ya = (spc700->regs.y << 8) | spc700->regs.a;
word dp = spc700->mem_read(APUMODE_DP, MEMSIZE_WORD, arg);
ulong r = ya + dp;
  spc700_testn(r & 0x8000);
  spc700_testv(~(ya ^ dp) & (dp ^ (word)r) & 0x8000);
  spc700_testh((ya ^ dp ^ (word)r) & 0x1000);
  spc700_testz((word)r == 0);
  spc700_testc(r >= 0x10000);
  spc700->regs.a = r;
  spc700->regs.y = r >> 8;
  spc700_incpc(2);
  add_apu_cycles(5);
}
