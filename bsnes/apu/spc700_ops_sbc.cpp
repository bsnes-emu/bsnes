byte spc700_op_sbc(byte x, byte y) {
short r = (short)x - (short)y - (short)(!(spc700->regs.p & SPF_C));
  spc700_testn(r & 0x80);
  spc700_testv(((x ^ y) & 0x80) && ((x ^ (byte)r) & 0x80));
  spc700_testh(((x ^ y ^ (byte)r) & 0x10) == 0);
  spc700_testz((byte)r == 0);
  spc700_testc(r >= 0);
  return (byte)r;
}

void spc700_op_sbc_a_const(void) {
spc700_prefetchb();
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, arg);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_sbc_a_ix(void) {
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0));
  spc700_incpc(1);
  add_apu_cycles(3);
}

void spc700_op_sbc_a_dp(void) {
spc700_prefetchb();
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_sbc_a_dpx(void) {
spc700_prefetchb();
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_sbc_a_addr(void) {
spc700_prefetchw();
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_sbc_a_addrx(void) {
spc700_prefetchw();
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, spc700->mem_read(APUMODE_ADDRX, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_sbc_a_addry(void) {
spc700_prefetchw();
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, spc700->mem_read(APUMODE_ADDRY, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_sbc_a_idpx(void) {
spc700_prefetchb();
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, spc700->mem_read(APUMODE_IDPX, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_sbc_a_idpy(void) {
spc700_prefetchb();
word i;
  spc700->regs.a = spc700_op_sbc(spc700->regs.a, spc700->mem_read(APUMODE_IDPY, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_sbc_ix_iy(void) {
byte x = spc700_op_sbc(spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0), spc700->mem_read(APUMODE_IY, MEMSIZE_BYTE, 0));
  spc700->mem_write(APUMODE_IX, MEMSIZE_BYTE, 0, x);
  spc700_incpc(1);
  add_apu_cycles(5);
}

void spc700_op_sbc_dp_dp(void) {
spc700_prefetch2b();
byte x = spc700_op_sbc(spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg1), spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0));
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg1, x);
  spc700_incpc(3);
  add_apu_cycles(6);
}

void spc700_op_sbc_dp_imm(void) {
spc700_prefetch2b();
byte x = spc700_op_sbc(spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg1), arg0);
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg1, x);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_subw_ya_dp(void) {
spc700_prefetchb();
word ya = (spc700->regs.y << 8) | spc700->regs.a;
word dp = spc700->mem_read(APUMODE_DP, MEMSIZE_WORD, arg);
long r  = (long)ya - (long)dp;
  spc700_testn(r & 0x8000);
  spc700_testv(((ya ^ dp) & 0x8000) && ((ya ^ (word)r) & 0x8000));
  spc700_testh(((ya ^ dp ^ (word)r) & 0x1000) == 0);
  spc700_testz((word)r == 0);
  spc700_testc(r >= 0);
  spc700->regs.a = r;
  spc700->regs.y = r >> 8;
  spc700_incpc(2);
  add_apu_cycles(5);
}
