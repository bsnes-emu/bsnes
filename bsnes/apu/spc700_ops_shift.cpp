void spc700_op_asl_a(void) {
  spc700_testc(spc700->regs.a & 0x80);
  spc700->regs.a <<= 1;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_asl_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x80);
  x <<= 1;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_asl_dpx(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x80);
  x <<= 1;
  spc700->mem_write(APUMODE_DPX, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_asl_addr(void) {
spc700_prefetchw();
byte x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x80);
  x <<= 1;
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_lsr_a(void) {
  spc700_testc(spc700->regs.a & 0x01);
  spc700->regs.a >>= 1;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_lsr_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x01);
  x >>= 1;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_lsr_dpx(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x01);
  x >>= 1;
  spc700->mem_write(APUMODE_DPX, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_lsr_addr(void) {
spc700_prefetchw();
byte x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x01);
  x >>= 1;
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_rol_a(void) {
byte c = (spc700->regs.p & SPF_C)?0x80:0x00;
  spc700_testc(spc700->regs.a & 0x80);
  spc700->regs.a <<= 1;
  spc700->regs.a |= c;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_rol_dp(void) {
spc700_prefetchb();
byte c = (spc700->regs.p & SPF_C)?0x80:0x00;
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x80);
  x <<= 1;
  x |= c;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_rol_dpx(void) {
spc700_prefetchb();
byte c = (spc700->regs.p & SPF_C)?0x80:0x00;
byte x = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x80);
  x <<= 1;
  x |= c;
  spc700->mem_write(APUMODE_DPX, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_rol_addr(void) {
spc700_prefetchw();
byte c = (spc700->regs.p & SPF_C)?0x80:0x00;
byte x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x80);
  x <<= 1;
  x |= c;
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_ror_a(void) {
byte c = (spc700->regs.p & SPF_C)?0x01:0x00;
  spc700_testc(spc700->regs.a & 0x01);
  spc700->regs.a >>= 1;
  spc700->regs.a |= c;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_ror_dp(void) {
spc700_prefetchb();
byte c = (spc700->regs.p & SPF_C)?0x01:0x00;
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x01);
  x >>= 1;
  x |= c;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_ror_dpx(void) {
spc700_prefetchb();
byte c = (spc700->regs.p & SPF_C)?0x01:0x00;
byte x = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x01);
  x >>= 1;
  x |= c;
  spc700->mem_write(APUMODE_DPX, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_ror_addr(void) {
spc700_prefetchw();
byte c = (spc700->regs.p & SPF_C)?0x01:0x00;
byte x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  spc700_testc(x & 0x01);
  x >>= 1;
  x |= c;
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}
