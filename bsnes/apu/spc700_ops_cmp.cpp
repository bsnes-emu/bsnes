void spc700_op_cmp(byte x, byte y) {
short r = (short)x - (short)y;
  spc700_testn(r & 0x80);
  spc700_testz((byte)r == 0);
  spc700_testc(r >= 0);
}

void spc700_op_cmp_a_const(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.a, arg);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_cmp_a_ix(void) {
  spc700_op_cmp(spc700->regs.a, spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0));
  spc700_incpc(1);
  add_apu_cycles(3);
}

void spc700_op_cmp_a_dp(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.a, spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_cmp_a_dpx(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.a, spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_cmp_a_addr(void) {
spc700_prefetchw();
  spc700_op_cmp(spc700->regs.a, spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_cmp_a_addrx(void) {
spc700_prefetchw();
  spc700_op_cmp(spc700->regs.a, spc700->mem_read(APUMODE_ADDRX, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_cmp_a_addry(void) {
spc700_prefetchw();
  spc700_op_cmp(spc700->regs.a, spc700->mem_read(APUMODE_ADDRY, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_cmp_a_idpx(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.a, spc700->mem_read(APUMODE_IDPX, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_cmp_a_idpy(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.a, spc700->mem_read(APUMODE_IDPY, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_cmp_ix_iy(void) {
  spc700_op_cmp(spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0), spc700->mem_read(APUMODE_IY, MEMSIZE_BYTE, 0));
  spc700_incpc(1);
  add_apu_cycles(5);
}

void spc700_op_cmp_dp_dp(void) {
spc700_prefetch2b();
  spc700_op_cmp(spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg1), spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0));
  spc700_incpc(3);
  add_apu_cycles(6);
}

void spc700_op_cmp_dp_imm(void) {
spc700_prefetch2b();
  spc700_op_cmp(spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg1), arg0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_cmp_x_const(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.x, arg);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_cmp_x_dp(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.x, spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_cmp_x_addr(void) {
spc700_prefetchw();
  spc700_op_cmp(spc700->regs.x, spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_cmp_y_const(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.y, arg);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_cmp_y_dp(void) {
spc700_prefetchb();
  spc700_op_cmp(spc700->regs.y, spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg));
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_cmp_y_addr(void) {
spc700_prefetchw();
  spc700_op_cmp(spc700->regs.y, spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg));
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_cmpw_ya_dp(void) {
spc700_prefetchb();
word ya = ((spc700->regs.y << 8) | spc700->regs.a);
word dp = spc700->mem_read(APUMODE_DP, MEMSIZE_WORD, arg);
long r  = (long)ya - (long)dp;
  spc700_testn(r & 0x8000);
  spc700_testz((word)r == 0);
  spc700_testc(r >= 0);
  spc700_incpc(2);
  add_apu_cycles(5);
}
