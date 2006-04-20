void bCPU::op_sta_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w,     regs.a.w);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + 1, regs.a.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_stx_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    if(regs.p.x)last_cycle();
    op_write(OPMODE_DBR, aa.w,     regs.x.w);
    if(regs.p.x)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + 1, regs.x.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sty_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    if(regs.p.x)last_cycle();
    op_write(OPMODE_DBR, aa.w,     regs.y.w);
    if(regs.p.x)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + 1, regs.y.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_stz_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w,     0x0000);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + 1, 0x0000 >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_c4(aa.w, aa.w + regs.x.w);
    } break;
  case 4: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w,     regs.a.w);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w + 1, regs.a.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_stz_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_c4(aa.w, aa.w + regs.x.w);
    } break;
  case 4: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w,     0x0000);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w + 1, 0x0000 >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_addry() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_c4(aa.w, aa.w + regs.y.w);
    } break;
  case 4: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_long() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    aa.b = op_read();
    } break;
  case 4: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_LONG, aa.d,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_LONG, aa.d + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_longx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    aa.b = op_read();
    } break;
  case 4: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_LONG, aa.d + regs.x.w,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_LONG, aa.d + regs.x.w + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DP, dp,     regs.a.w);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_DP, dp + 1, regs.a.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_stx_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    if(regs.p.x)last_cycle();
    op_write(OPMODE_DP, dp,     regs.x.w);
    if(regs.p.x)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_DP, dp + 1, regs.x.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sty_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    if(regs.p.x)last_cycle();
    op_write(OPMODE_DP, dp,     regs.y.w);
    if(regs.p.x)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_DP, dp + 1, regs.y.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_stz_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DP, dp,     0x0000);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_DP, dp + 1, 0x0000 >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     regs.a.w);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w + 1, regs.a.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sty_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    if(regs.p.x)last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     regs.y.w);
    if(regs.p.x)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w + 1, regs.y.w >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_stz_dpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     0x0000);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w + 1, 0x0000 >> 8);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_stx_dpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    if(regs.p.x)last_cycle();
    op_write(OPMODE_DP, dp + regs.y.w,     regs.x.l);
    if(regs.p.x)status.cycle_pos = 0;
    } break;
  case 5: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.y.w + 1, regs.x.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_idp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    aa.l = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    aa.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 6: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_ildp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    aa.l = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    aa.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    aa.b = op_read(OPMODE_DP, dp + 2);
    } break;
  case 6: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_LONG, aa.d,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_LONG, aa.d + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_idpx() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    aa.l = op_read(OPMODE_DP, dp + regs.x.w);
    } break;
  case 5: {
    aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    } break;
  case 6: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_idpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    aa.l = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    aa.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_c4(aa.w, aa.w + regs.y.w);
    } break;
  case 6: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_ildpy() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    aa.l = op_read(OPMODE_DP, dp);
    } break;
  case 4: {
    aa.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    aa.b = op_read(OPMODE_DP, dp + 2);
    } break;
  case 6: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_LONG, aa.d + regs.y.w,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_LONG, aa.d + regs.y.w + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_sr() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_SP, sp,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 4: {
    last_cycle();
    op_write(OPMODE_SP, sp + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_sta_isry() {
  switch(status.cycle_pos++) {
  case 1: {
    sp = op_read();
    } break;
  case 2: {
    cpu_io();
    } break;
  case 3: {
    aa.l = op_read(OPMODE_SP, sp);
    } break;
  case 4: {
    aa.h = op_read(OPMODE_SP, sp + 1);
    } break;
  case 5: {
    cpu_io();
    } break;
  case 6: {
    if(regs.p.m)last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
    if(regs.p.m)status.cycle_pos = 0;
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
    status.cycle_pos = 0;
    } break;
  }
}

