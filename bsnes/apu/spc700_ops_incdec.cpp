void spc700_op_inc_a(void) {
  spc700->regs.a++;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_inc_x(void) {
  spc700->regs.x++;
  spc700_testn(spc700->regs.x & 0x80);
  spc700_testz(spc700->regs.x == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_inc_y(void) {
  spc700->regs.y++;
  spc700_testn(spc700->regs.y & 0x80);
  spc700_testz(spc700->regs.y == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_inc_dp(void) {
spc700_prefetchb();
byte r;
  r = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg) + 1;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, r);
  spc700_testn(r & 0x80);
  spc700_testz(r == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_inc_dpx(void) {
spc700_prefetchb();
byte r;
  r = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg) + 1;
  spc700->mem_write(APUMODE_DPX, MEMSIZE_BYTE, arg, r);
  spc700_testn(r & 0x80);
  spc700_testz(r == 0);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_inc_addr(void) {
spc700_prefetchw();
byte r;
  r = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg) + 1;
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, r);
  spc700_testn(r & 0x80);
  spc700_testz(r == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_dec_a(void) {
  spc700->regs.a--;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_dec_x(void) {
  spc700->regs.x--;
  spc700_testn(spc700->regs.x & 0x80);
  spc700_testz(spc700->regs.x == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_dec_y(void) {
  spc700->regs.y--;
  spc700_testn(spc700->regs.y & 0x80);
  spc700_testz(spc700->regs.y == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_dec_dp(void) {
spc700_prefetchb();
byte r;
  r = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg) - 1;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, r);
  spc700_testn(r & 0x80);
  spc700_testz(r == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_dec_dpx(void) {
spc700_prefetchb();
byte r;
  r = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg) - 1;
  spc700->mem_write(APUMODE_DPX, MEMSIZE_BYTE, arg, r);
  spc700_testn(r & 0x80);
  spc700_testz(r == 0);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_dec_addr(void) {
spc700_prefetchw();
byte r;
  r = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg) - 1;
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, r);
  spc700_testn(r & 0x80);
  spc700_testz(r == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_incw_dp(void) {
spc700_prefetchb();
word r;
  r = spc700->mem_read(APUMODE_DP, MEMSIZE_WORD, arg) + 1;
  spc700->mem_write(APUMODE_DP, MEMSIZE_WORD, arg, r);
  spc700_testn(r & 0x8000);
  spc700_testz(r == 0);
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_decw_dp(void) {
spc700_prefetchb();
word r;
  r = spc700->mem_read(APUMODE_DP, MEMSIZE_WORD, arg) - 1;
  spc700->mem_write(APUMODE_DP, MEMSIZE_WORD, arg, r);
  spc700_testn(r & 0x8000);
  spc700_testz(r == 0);
  spc700_incpc(2);
  add_apu_cycles(6);
}
