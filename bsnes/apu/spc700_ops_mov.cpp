void spc700_op_mov_a_x(void) {
  spc700->regs.a = spc700->regs.x;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_mov_a_y(void) {
  spc700->regs.a = spc700->regs.y;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_mov_x_a(void) {
  spc700->regs.x = spc700->regs.a;
  spc700_testn(spc700->regs.x & 0x80);
  spc700_testz(spc700->regs.x == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_mov_y_a(void) {
  spc700->regs.y = spc700->regs.a;
  spc700_testn(spc700->regs.y & 0x80);
  spc700_testz(spc700->regs.y == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_mov_x_sp(void) {
  spc700->regs.x = spc700->regs.sp;
  spc700_testn(spc700->regs.x & 0x80);
  spc700_testz(spc700->regs.x == 0);
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_mov_sp_x(void) {
  spc700->regs.sp = spc700->regs.x;
  spc700_incpc(1);
  add_apu_cycles(2);
}

void spc700_op_mov_dp_dp(void) {
spc700_prefetch2b();
byte x;
  x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg0);
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg1, x);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_mov_dp_imm(void) {
spc700_prefetch2b();
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg1, arg0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_mov_a_const(void) {
spc700_prefetchb();
  spc700->regs.a = arg;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_mov_x_const(void) {
spc700_prefetchb();
  spc700->regs.x = arg;
  spc700_testn(spc700->regs.x & 0x80);
  spc700_testz(spc700->regs.x == 0);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_mov_y_const(void) {
spc700_prefetchb();
  spc700->regs.y = arg;
  spc700_testn(spc700->regs.y & 0x80);
  spc700_testz(spc700->regs.y == 0);
  spc700_incpc(2);
  add_apu_cycles(2);
}

void spc700_op_mov_ix_a(void) {
  spc700->mem_write(APUMODE_IX, MEMSIZE_BYTE, 0, spc700->regs.a);
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_mov_ixinc_a(void) {
  spc700->mem_write(APUMODE_IX, MEMSIZE_BYTE, 0, spc700->regs.a);
  spc700->regs.x++;
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_mov_dp_a(void) {
spc700_prefetchb();
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, spc700->regs.a);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_mov_dpx_a(void) {
spc700_prefetchb();
  spc700->mem_write(APUMODE_DPX, MEMSIZE_BYTE, arg, spc700->regs.a);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_mov_addr_a(void) {
spc700_prefetchw();
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, spc700->regs.a);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_mov_addrx_a(void) {
spc700_prefetchw();
  spc700->mem_write(APUMODE_ADDRX, MEMSIZE_BYTE, arg, spc700->regs.a);
  spc700_incpc(3);
  add_apu_cycles(6);
}

void spc700_op_mov_addry_a(void) {
spc700_prefetchw();
  spc700->mem_write(APUMODE_ADDRY, MEMSIZE_BYTE, arg, spc700->regs.a);
  spc700_incpc(3);
  add_apu_cycles(6);
}

void spc700_op_mov_idpx_a(void) {
spc700_prefetchb();
word i;
  spc700->mem_write(APUMODE_IDPX, MEMSIZE_BYTE, arg, spc700->regs.a);
  spc700_incpc(2);
  add_apu_cycles(7);
}

void spc700_op_mov_idpy_a(void) {
spc700_prefetchb();
word i;
  spc700->mem_write(APUMODE_IDPY, MEMSIZE_BYTE, arg, spc700->regs.a);
  spc700_incpc(2);
  add_apu_cycles(7);
}

void spc700_op_mov_dp_x(void) {
spc700_prefetchb();
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, spc700->regs.x);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_mov_dpy_x(void) {
spc700_prefetchb();
  spc700->mem_write(APUMODE_DPY, MEMSIZE_BYTE, arg, spc700->regs.x);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_mov_addr_x(void) {
spc700_prefetchw();
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, spc700->regs.x);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_mov_dp_y(void) {
spc700_prefetchb();
  spc700->mem_write(APUMODE_DP, MEMSIZE_BYTE, arg, spc700->regs.y);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_mov_dpx_y(void) {
spc700_prefetchb();
  spc700->mem_write(APUMODE_DPX, MEMSIZE_BYTE, arg, spc700->regs.y);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_mov_addr_y(void) {
spc700_prefetchw();
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, arg, spc700->regs.y);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_mov_a_ix(void) {
  spc700->regs.a = spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(3);
}

void spc700_op_mov_a_ixinc(void) {
  spc700->regs.a = spc700->mem_read(APUMODE_IX, MEMSIZE_BYTE, 0);
  spc700->regs.x++;
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(1);
  add_apu_cycles(4);
}

void spc700_op_mov_a_dp(void) {
spc700_prefetchb();
  spc700->regs.a = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_mov_a_dpx(void) {
spc700_prefetchb();
  spc700->regs.a = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_mov_a_addr(void) {
spc700_prefetchw();
  spc700->regs.a = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_mov_a_addrx(void) {
spc700_prefetchw();
  spc700->regs.a = spc700->mem_read(APUMODE_ADDRX, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_mov_a_addry(void) {
spc700_prefetchw();
  spc700->regs.a = spc700->mem_read(APUMODE_ADDRY, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(3);
  add_apu_cycles(5);
}

void spc700_op_mov_a_idpx(void) {
spc700_prefetchb();
word i;
  spc700->regs.a = spc700->mem_read(APUMODE_IDPX, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_mov_a_idpy(void) {
spc700_prefetchb();
word i;
  spc700->regs.a = spc700->mem_read(APUMODE_IDPY, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.a & 0x80);
  spc700_testz(spc700->regs.a == 0);
  spc700_incpc(2);
  add_apu_cycles(6);
}

void spc700_op_mov_x_dp(void) {
spc700_prefetchb();
  spc700->regs.x = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.x & 0x80);
  spc700_testz(spc700->regs.x == 0);
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_mov_x_dpy(void) {
spc700_prefetchb();
  spc700->regs.x = spc700->mem_read(APUMODE_DPY, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.x & 0x80);
  spc700_testz(spc700->regs.x == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_mov_x_addr(void) {
spc700_prefetchw();
  spc700->regs.x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.x & 0x80);
  spc700_testz(spc700->regs.x == 0);
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_mov_y_dp(void) {
spc700_prefetchb();
  spc700->regs.y = spc700->mem_read(APUMODE_DP, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.y & 0x80);
  spc700_testz(spc700->regs.y == 0);
  spc700_incpc(2);
  add_apu_cycles(3);
}

void spc700_op_mov_y_dpx(void) {
spc700_prefetchb();
  spc700->regs.y = spc700->mem_read(APUMODE_DPX, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.y & 0x80);
  spc700_testz(spc700->regs.y == 0);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_mov_y_addr(void) {
spc700_prefetchw();
  spc700->regs.y = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, arg);
  spc700_testn(spc700->regs.y & 0x80);
  spc700_testz(spc700->regs.y == 0);
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_mov_ya_dp(void) {
spc700_prefetchb();
word ya;
  ya = spc700->mem_read(APUMODE_DP, MEMSIZE_WORD, arg);
  spc700->regs.a = ya;
  spc700->regs.y = ya >> 8;
  spc700_testn(ya & 0x8000);
  spc700_testz(ya == 0);
  spc700_incpc(2);
  add_apu_cycles(5);
}

void spc700_op_mov_dp_ya(void) {
spc700_prefetchb();
word ya = (spc700->regs.y << 8) | spc700->regs.a;
  spc700->mem_write(APUMODE_DP, MEMSIZE_WORD, arg, ya);
  spc700_incpc(2);
  add_apu_cycles(4);
}

void spc700_op_mov1_c_bit(void) {
spc700_prefetchw();
word addr = arg & 0x1fff;
byte bit  = arg >> 13;
byte x;
  x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, addr);
  spc700_testc(x & (1 << bit));
  spc700_incpc(3);
  add_apu_cycles(4);
}

void spc700_op_mov1_bit_c(void) {
spc700_prefetchw();
word addr = arg & 0x1fff;
byte bit  = arg >> 13;
byte x;
  x = spc700->mem_read(APUMODE_ADDR, MEMSIZE_BYTE, addr);
  if(spc700->regs.p & SPF_C) {
    x |= (1 << bit);
  } else {
    x &= ~(1 << bit);
  }
  spc700->mem_write(APUMODE_ADDR, MEMSIZE_BYTE, addr, x);
  spc700_incpc(3);
  add_apu_cycles(5);
}
