void bCPU::op_sta_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  op_write(OPMODE_DBR, aa.w,     regs.a.w);
  if(regs.p.m)return;
l4:
  op_write(OPMODE_DBR, aa.w + 1, regs.a.w >> 8);
}

void bCPU::op_stx_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  op_write(OPMODE_DBR, aa.w,     regs.x.w);
  if(regs.p.x)return;
l4:
  op_write(OPMODE_DBR, aa.w + 1, regs.x.w >> 8);
}

void bCPU::op_sty_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  op_write(OPMODE_DBR, aa.w,     regs.y.w);
  if(regs.p.x)return;
l4:
  op_write(OPMODE_DBR, aa.w + 1, regs.y.w >> 8);
}

void bCPU::op_stz_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  op_write(OPMODE_DBR, aa.w,     0x0000);
  if(regs.p.m)return;
l4:
  op_write(OPMODE_DBR, aa.w + 1, 0x0000 >> 8);
}

void bCPU::op_sta_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  op_write(OPMODE_DBR, aa.w + regs.x.w,     regs.a.w);
  if(regs.p.m)return;
l5:
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, regs.a.w >> 8);
}

void bCPU::op_stz_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  op_write(OPMODE_DBR, aa.w + regs.x.w,     0x0000);
  if(regs.p.m)return;
l5:
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, 0x0000 >> 8);
}

void bCPU::op_sta_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)return;
l5:
  op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
}

void bCPU::op_sta_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  op_write(OPMODE_LONG, aa.d,     regs.a.l);
  if(regs.p.m)return;
l5:
  op_write(OPMODE_LONG, aa.d + 1, regs.a.h);
}

void bCPU::op_sta_longx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  op_write(OPMODE_LONG, aa.d + regs.x.w,     regs.a.l);
  if(regs.p.m)return;
l5:
  op_write(OPMODE_LONG, aa.d + regs.x.w + 1, regs.a.h);
}

void bCPU::op_sta_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  op_write(OPMODE_DP, dp,     regs.a.w);
  if(regs.p.m)return;
l4:
  op_write(OPMODE_DP, dp + 1, regs.a.w >> 8);
}

void bCPU::op_stx_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  op_write(OPMODE_DP, dp,     regs.x.w);
  if(regs.p.x)return;
l4:
  op_write(OPMODE_DP, dp + 1, regs.x.w >> 8);
}

void bCPU::op_sty_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  op_write(OPMODE_DP, dp,     regs.y.w);
  if(regs.p.x)return;
l4:
  op_write(OPMODE_DP, dp + 1, regs.y.w >> 8);
}

void bCPU::op_stz_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  op_write(OPMODE_DP, dp,     0x0000);
  if(regs.p.m)return;
l4:
  op_write(OPMODE_DP, dp + 1, 0x0000 >> 8);
}

void bCPU::op_sta_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  op_write(OPMODE_DP, dp + regs.x.w,     regs.a.w);
  if(regs.p.m)return;
l5:
  op_write(OPMODE_DP, dp + regs.x.w + 1, regs.a.w >> 8);
}

void bCPU::op_sty_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  op_write(OPMODE_DP, dp + regs.x.w,     regs.y.w);
  if(regs.p.x)return;
l5:
  op_write(OPMODE_DP, dp + regs.x.w + 1, regs.y.w >> 8);
}

void bCPU::op_stz_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  op_write(OPMODE_DP, dp + regs.x.w,     0x0000);
  if(regs.p.m)return;
l5:
  op_write(OPMODE_DP, dp + regs.x.w + 1, 0x0000 >> 8);
}

void bCPU::op_stx_dpy() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  op_write(OPMODE_DP, dp + regs.y.w,     regs.x.l);
  if(regs.p.x)return;
l5:
  op_write(OPMODE_DP, dp + regs.y.w + 1, regs.x.h);
}

void bCPU::op_sta_idp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  op_write(OPMODE_DBR, aa.w,     regs.a.l);
  if(regs.p.m)return;
l6:
  op_write(OPMODE_DBR, aa.w + 1, regs.a.h);
}

void bCPU::op_sta_ildp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  aa.b = op_read(OPMODE_DP, dp + 2);
l6:
  op_write(OPMODE_LONG, aa.d,     regs.a.l);
  if(regs.p.m)return;
l7:
  op_write(OPMODE_LONG, aa.d + 1, regs.a.h);
}

void bCPU::op_sta_idpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);
l5:
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
l6:
  op_write(OPMODE_DBR, aa.w,     regs.a.l);
  if(regs.p.m)return;
l7:
  op_write(OPMODE_DBR, aa.w + 1, regs.a.h);
}

void bCPU::op_sta_idpy() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_c4(aa.w, aa.w + regs.y.w);
l6:
  op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)return;
l7:
  op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
}

void bCPU::op_sta_ildpy() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  aa.b = op_read(OPMODE_DP, dp + 2);
l6:
  op_write(OPMODE_LONG, aa.d + regs.y.w,     regs.a.l);
  if(regs.p.m)return;
l7:
  op_write(OPMODE_LONG, aa.d + regs.y.w + 1, regs.a.h);
}

void bCPU::op_sta_sr() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  op_write(OPMODE_SP, sp,     regs.a.l);
  if(regs.p.m)return;
l4:
  op_write(OPMODE_SP, sp + 1, regs.a.h);
}

void bCPU::op_sta_isry() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  aa.l = op_read(OPMODE_SP, sp);
l4:
  aa.h = op_read(OPMODE_SP, sp + 1);
l5:
  cpu_io();
l6:
  op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)return;
l7:
  op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
}

