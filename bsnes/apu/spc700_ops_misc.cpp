void spc700_op_nop(void) {
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_sleep(void) {
  add_apu_cycles(3);
}

void spc700_op_stop(void) {
  add_apu_cycles(3);
}

void spc700_op_xcn_a(void) {
  spc700->regs.a = (spc700->regs.a >> 4) | (spc700->regs.a << 4);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(5);
}

void spc700_op_daa_a(void) {
int a = spc700->regs.a;
  if(((a     ) & 15) > 9)a += 6;
  if(((a >> 4) & 15) > 9)a += 6 << 4;
  spc700->regs.a = a;
  spc700_testn(a & 0x80);
  spc700_testz(a == 0);
  spc700_testc(a >= 0x0100);
  spc700_incpc(1);
  add_apu_cycles(3);
}

void spc700_op_das_a(void) {
int a = spc700->regs.a;
  if(((a     ) & 15) > 9)a -= 6;
  if(((a >> 4) & 15) > 9)a -= 6 << 4;
  spc700->regs.a = a;
  spc700_testn(a & 0x80);
  spc700_testz(a == 0);
  spc700_testc(a < 0);
  spc700_incpc(1);
  add_apu_cycles(3);
}

void spc700_op_set0_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x |= 0x01;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_clr0_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x &= ~0x01;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_set1_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x |= 0x02;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_clr1_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x &= ~0x02;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_set2_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x |= 0x04;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_clr2_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x &= ~0x04;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_set3_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x |= 0x08;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_clr3_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x &= ~0x08;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_set4_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x |= 0x10;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_clr4_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x &= ~0x10;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_set5_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x |= 0x20;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_clr5_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x &= ~0x20;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_set6_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x |= 0x40;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_clr6_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x &= ~0x40;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_set7_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x |= 0x80;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_clr7_dp(void) {
spc700_prefetchb();
byte x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  x &= ~0x80;
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_tset1_addr(void) {
spc700_prefetchw();
byte x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  x |= spc700->regs.a;
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(3);
  add_apu_cycles(6);
}

void spc700_op_tclr1_addr(void) {
spc700_prefetchw();
byte x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  x &= ~spc700->regs.a;
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, x);
  spc700_testn(x & 0x80);
  spc700_testz(x == 0);
  spc700_incpc(3);
  add_apu_cycles(6);
}

void spc700_op_mul_ya(void) {
word ya;
  ya = spc700->regs.y * spc700->regs.a;
  spc700->regs.a = ya;
  spc700->regs.y = ya >> 8;
  spc700_testn(ya & 0x8000);
  spc700_testz(ya == 0);
  spc700_incpc(1);
  add_apu_cycles(9);
}

/*
  v/h flags not set, may have result backwards
  (a and y may need to be swapped)
*/
void spc700_op_div_ya_x(void) {
word ya = (spc700->regs.y << 8) | spc700->regs.a;
  if(spc700->regs.x == 0) {
    spc700->regs.a = 0;
    spc700->regs.y = 0;
  } else {
    spc700->regs.a = ya / spc700->regs.x;
    spc700->regs.y = ya % spc700->regs.x;
  }
  ya = (spc700->regs.y << 8) | spc700->regs.a;
  spc700_testn(ya & 0x8000);
  spc700_testz(ya == 0);
  spc700_incpc(1);
  add_apu_cycles(12);
}
