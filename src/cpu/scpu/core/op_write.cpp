void sCPU::op_sta_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w,     regs.a.w);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + 1, regs.a.w >> 8);
}

void sCPU::op_stx_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.x)last_cycle();
  op_writedbr(aa.w,     regs.x.w);
  if(regs.p.x)return;
  last_cycle();
  op_writedbr(aa.w + 1, regs.x.w >> 8);
}

void sCPU::op_sty_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.x)last_cycle();
  op_writedbr(aa.w,     regs.y.w);
  if(regs.p.x)return;
  last_cycle();
  op_writedbr(aa.w + 1, regs.y.w >> 8);
}

void sCPU::op_stz_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w,     0x0000);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + 1, 0x0000 >> 8);
}

void sCPU::op_sta_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w + regs.x.w,     regs.a.w);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + regs.x.w + 1, regs.a.w >> 8);
}

void sCPU::op_stz_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w + regs.x.w,     0x0000);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + regs.x.w + 1, 0x0000 >> 8);
}

void sCPU::op_sta_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
}

void sCPU::op_sta_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  op_writelong(aa.d,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writelong(aa.d + 1, regs.a.h);
}

void sCPU::op_sta_longx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  op_writelong(aa.d + regs.x.w,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writelong(aa.d + regs.x.w + 1, regs.a.h);
}

void sCPU::op_sta_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  op_writedp(dp,     regs.a.w);
  if(regs.p.m)return;
  last_cycle();
  op_writedp(dp + 1, regs.a.w >> 8);
}

void sCPU::op_stx_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.x)last_cycle();
  op_writedp(dp,     regs.x.w);
  if(regs.p.x)return;
  last_cycle();
  op_writedp(dp + 1, regs.x.w >> 8);
}

void sCPU::op_sty_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.x)last_cycle();
  op_writedp(dp,     regs.y.w);
  if(regs.p.x)return;
  last_cycle();
  op_writedp(dp + 1, regs.y.w >> 8);
}

void sCPU::op_stz_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  op_writedp(dp,     0x0000);
  if(regs.p.m)return;
  last_cycle();
  op_writedp(dp + 1, 0x0000 >> 8);
}

void sCPU::op_sta_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  op_writedp(dp + regs.x.w,     regs.a.w);
  if(regs.p.m)return;
  last_cycle();
  op_writedp(dp + regs.x.w + 1, regs.a.w >> 8);
}

void sCPU::op_sty_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.x)last_cycle();
  op_writedp(dp + regs.x.w,     regs.y.w);
  if(regs.p.x)return;
  last_cycle();
  op_writedp(dp + regs.x.w + 1, regs.y.w >> 8);
}

void sCPU::op_stz_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  op_writedp(dp + regs.x.w,     0x0000);
  if(regs.p.m)return;
  last_cycle();
  op_writedp(dp + regs.x.w + 1, 0x0000 >> 8);
}

void sCPU::op_stx_dpy() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.x)last_cycle();
  op_writedp(dp + regs.y.w,     regs.x.l);
  if(regs.p.x)return;
  last_cycle();
  op_writedp(dp + regs.y.w + 1, regs.x.h);
}

void sCPU::op_sta_idp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + 1, regs.a.h);
}

void sCPU::op_sta_ildp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  op_writelong(aa.d,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writelong(aa.d + 1, regs.a.h);
}

void sCPU::op_sta_idpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + 1, regs.a.h);
}

void sCPU::op_sta_idpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
}

void sCPU::op_sta_ildpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  op_writelong(aa.d + regs.y.w,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writelong(aa.d + regs.y.w + 1, regs.a.h);
}

void sCPU::op_sta_sr() {
  sp = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  op_writesp(sp,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writesp(sp + 1, regs.a.h);
}

void sCPU::op_sta_isry() {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)return;
  last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
}

