void spc700_op_eor_a_const(void) {
spc700_prefetchb();
  spc700->regs.a ^= arg;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_eor_a_ix(void) {
  spc700->regs.a ^= spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(3);
}

void spc700_op_eor_a_dp(void) {
spc700_prefetchb();
  spc700->regs.a ^= spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_eor_a_dpx(void) {
spc700_prefetchb();
  spc700->regs.a ^= spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_eor_a_addr(void) {
spc700_prefetchw();
  spc700->regs.a ^= spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_eor_a_addrx(void) {
spc700_prefetchw();
  spc700->regs.a ^= spc700->mem_read(APUMODE_ADDRX, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_eor_a_addry(void) {
spc700_prefetchw();
  spc700->regs.a ^= spc700->mem_read(APUMODE_ADDRY, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_eor_a_idpx(void) {
spc700_prefetchb();
word i;
  spc700->regs.a ^= spc700->mem_read(APUMODE_IDPX, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_eor_a_idpy(void) {
spc700_prefetchb();
word i;
  spc700->regs.a ^= spc700->mem_read(APUMODE_IDPY, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_eor_ix_iy(void) {
word i;
byte x;
  x  = spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0);
  x ^= spc700->mem_read(APUMODE_IY, MEMSIZE_BYTE, 0);
  spc700->mem_write(APUMODE_IX, MEMSIZE_BYTE, 0, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(1);
  add_apu_cycles(5);
}

void spc700_op_eor_dp_dp(void) {
spc700_prefetch2b();
byte x;
  x  = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg1);
  x ^= spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0);
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg1, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(3);
  add_apu_cycles(6);
}

void spc700_op_eor_dp_imm(void) {
spc700_prefetch2b();
byte x;
  x  = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg1);
  x ^= arg0;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg1, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_eor1_bit(void) {
spc700_prefetchw();
word addr = arg & 0x1fff;
byte bit  = arg >> 13;
byte x;
  x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, addr);
  if(spc700->regs.p & SPF_C) {
    if(x & (1 << bit)) {
      spc700_clrc();
    }
  } else {
    if(x & (1 << bit)) {
      spc700_setc();
    }
  }
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_not1_bit(void) {
spc700_prefetchw();
word addr = arg & 0x1fff;
byte bit  = arg >> 13;
byte x;
  x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, addr);
  x ^= (1 << bit);
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, addr, x);
  spc700_incpc(3);
  add_apu_cycles(5);
}
