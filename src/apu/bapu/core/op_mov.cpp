void bAPU::op_mov_a_x() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.a = regs.x;
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_y() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.a = regs.y;
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_x_a() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.x = regs.a;
    regs.p.n = !!(regs.x & 0x80);
    regs.p.z = (regs.x == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_y_a() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.y = regs.a;
    regs.p.n = !!(regs.y & 0x80);
    regs.p.z = (regs.y == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_x_sp() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.x = regs.sp;
    regs.p.n = !!(regs.x & 0x80);
    regs.p.z = (regs.x == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_sp_x() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.sp = regs.x;
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_const() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.a = op_read();
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_x_const() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.x = op_read();
    regs.p.n = !!(regs.x & 0x80);
    regs.p.z = (regs.x == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_y_const() {
  switch(status.cycle_pos++) {
  case 1: {
    regs.y = op_read();
    regs.p.n = !!(regs.y & 0x80);
    regs.p.z = (regs.y == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_ix() {
  switch(status.cycle_pos++) {
  case 1: {
    } break;
  case 2: {
    regs.a = op_read(OPMODE_DP, regs.x);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_ixinc() {
  switch(status.cycle_pos++) {
  case 1: {
    } break;
  case 2: {
    } break;
  case 3: {
    regs.a = op_read(OPMODE_DP, regs.x++);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    regs.a = op_read(OPMODE_DP, sp);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_x_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    regs.x = op_read(OPMODE_DP, sp);
    regs.p.n = !!(regs.x & 0x80);
    regs.p.z = (regs.x == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_y_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    regs.y = op_read(OPMODE_DP, sp);
    regs.p.n = !!(regs.y & 0x80);
    regs.p.z = (regs.y == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    regs.a = op_read(OPMODE_DP, sp + regs.x);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_x_dpy() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    regs.x = op_read(OPMODE_DP, sp + regs.y);
    regs.p.n = !!(regs.x & 0x80);
    regs.p.z = (regs.x == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_y_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    regs.y = op_read(OPMODE_DP, sp + regs.x);
    regs.p.n = !!(regs.y & 0x80);
    regs.p.z = (regs.y == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    sp  = op_read();
    } break;
  case 2: {
    sp |= op_read() << 8;
    } break;
  case 3: {
    regs.a = op_read(OPMODE_ADDR, sp);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_x_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    sp  = op_read();
    } break;
  case 2: {
    sp |= op_read() << 8;
    } break;
  case 3: {
    regs.x = op_read(OPMODE_ADDR, sp);
    regs.p.n = !!(regs.x & 0x80);
    regs.p.z = (regs.x == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_y_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    sp  = op_read();
    } break;
  case 2: {
    sp |= op_read() << 8;
    } break;
  case 3: {
    regs.y = op_read(OPMODE_ADDR, sp);
    regs.p.n = !!(regs.y & 0x80);
    regs.p.z = (regs.y == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    sp  = op_read();
    } break;
  case 2: {
    sp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    regs.a = op_read(OPMODE_ADDR, sp + regs.x);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_addry() {
  switch(status.cycle_pos++) {
  case 1: {
    sp  = op_read();
    } break;
  case 2: {
    sp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    regs.a = op_read(OPMODE_ADDR, sp + regs.y);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_idpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read() + regs.x;
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    regs.a = op_read(OPMODE_ADDR, sp);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_a_idpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    sp  = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    sp |= op_read(OPMODE_DP, dp + 1) << 8;
    } break;
  case 5: {
    regs.a = op_read(OPMODE_ADDR, sp + regs.y);
    regs.p.n = !!(regs.a & 0x80);
    regs.p.z = (regs.a == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_dp_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    rd = op_read(OPMODE_DP, sp);
    } break;
  case 4: {
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_dp_const() {
  switch(status.cycle_pos++) {
  case 1: {
    rd = op_read();
    } break;
  case 2: {
    dp = op_read();
    } break;
  case 3: {
    } break;
  case 4: {
    op_write(OPMODE_DP, dp, rd);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_ix_a() {
  switch(status.cycle_pos++) {
  case 1: {
    } break;
  case 2: {
    } break;
  case 3: {
    op_write(OPMODE_DP, regs.x, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_ixinc_a() {
  switch(status.cycle_pos++) {
  case 1: {
    } break;
  case 2: {
    } break;
  case 3: {
    op_write(OPMODE_DP, regs.x++, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_dp_a() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    op_write(OPMODE_DP, dp, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_dp_x() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    op_write(OPMODE_DP, dp, regs.x);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_dp_y() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    op_write(OPMODE_DP, dp, regs.y);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_dpx_a() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    } break;
  case 4: {
    op_write(OPMODE_DP, dp + regs.x, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_dpy_x() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    } break;
  case 4: {
    op_write(OPMODE_DP, dp + regs.y, regs.x);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_dpx_y() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    } break;
  case 4: {
    op_write(OPMODE_DP, dp + regs.x, regs.y);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_addr_a() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    op_write(OPMODE_ADDR, dp, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_addr_x() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    op_write(OPMODE_ADDR, dp, regs.x);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_addr_y() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    op_write(OPMODE_ADDR, dp, regs.y);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_addrx_a() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    op_write(OPMODE_ADDR, dp + regs.x, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_addry_a() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    } break;
  case 4: {
    } break;
  case 5: {
    op_write(OPMODE_ADDR, dp + regs.y, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_idpx_a() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read() + regs.x;
    } break;
  case 2: {
    } break;
  case 3: {
    dp  = op_read(OPMODE_DP, sp);
    } break;
  case 4: {
    dp |= op_read(OPMODE_DP, sp + 1) << 8;
    } break;
  case 5: {
    } break;
  case 6: {
    op_write(OPMODE_ADDR, dp, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov_idpy_a() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    dp  = op_read(OPMODE_DP, sp);
    } break;
  case 4: {
    dp |= op_read(OPMODE_DP, sp + 1) << 8;
    } break;
  case 5: {
    } break;
  case 6: {
    op_write(OPMODE_ADDR, dp + regs.y, regs.a);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_movw_ya_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    regs.a = op_read(OPMODE_DP, sp);
    } break;
  case 4: {
    regs.y = op_read(OPMODE_DP, sp + 1);
    regs.p.n = !!(regs.ya & 0x8000);
    regs.p.z = (regs.ya == 0);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_movw_dp_ya() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    } break;
  case 3: {
    op_write(OPMODE_DP, dp,     regs.a);
    } break;
  case 4: {
    op_write(OPMODE_DP, dp + 1, regs.y);
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov1_c_bit() {
  switch(status.cycle_pos++) {
  case 1: {
    sp  = op_read();
    } break;
  case 2: {
    sp |= op_read() << 8;
    } break;
  case 3: {
    bit = sp >> 13;
    sp &= 0x1fff;
    rd = op_read(OPMODE_ADDR, sp);
    regs.p.c = !!(rd & (1 << bit));
    status.cycle_pos = 0;
    } break;
  }
}

void bAPU::op_mov1_bit_c() {
  switch(status.cycle_pos++) {
  case 1: {
    dp  = op_read();
    } break;
  case 2: {
    dp |= op_read() << 8;
    } break;
  case 3: {
    bit = dp >> 13;
    dp &= 0x1fff;
    rd = op_read(OPMODE_ADDR, dp);
    if(regs.p.c)rd |=  (1 << bit);
    else        rd &= ~(1 << bit);
    } break;
  case 4: {
    op_write(OPMODE_ADDR, dp, rd);
    status.cycle_pos = 0;
    } break;
  }
}

