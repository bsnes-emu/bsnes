sta_addr(0x8d, regs.p.m, regs.a.w),
stx_addr(0x8e, regs.p.x, regs.x.w),
sty_addr(0x8c, regs.p.x, regs.y.w),
stz_addr(0x9c, regs.p.m, 0x0000) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:if($1)last_cycle();
  op_writedbr(aa.w,     $2);
  if($1)end;
4:last_cycle();
  op_writedbr(aa.w + 1, $2 >> 8);
}

sta_addrx(0x9d, regs.p.m, regs.a.w),
stz_addrx(0x9e, regs.p.m, 0x0000) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_io();
4:if($1)last_cycle();
  op_writedbr(aa.w + regs.x.w,     $2);
  if($1)end;
5:last_cycle();
  op_writedbr(aa.w + regs.x.w + 1, $2 >> 8);
}

sta_addry(0x99) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_io();
4:if(regs.p.m)last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)end;
5:last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
}

sta_long(0x8f) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:aa.b = op_readpc();
4:if(regs.p.m)last_cycle();
  op_writelong(aa.d,     regs.a.l);
  if(regs.p.m)end;
5:last_cycle();
  op_writelong(aa.d + 1, regs.a.h);
}

sta_longx(0x9f) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:aa.b = op_readpc();
4:if(regs.p.m)last_cycle();
  op_writelong(aa.d + regs.x.w,     regs.a.l);
  if(regs.p.m)end;
5:last_cycle();
  op_writelong(aa.d + regs.x.w + 1, regs.a.h);
}

sta_dp(0x85, regs.p.m, regs.a.w),
stx_dp(0x86, regs.p.x, regs.x.w),
sty_dp(0x84, regs.p.x, regs.y.w),
stz_dp(0x64, regs.p.m, 0x0000) {
1:dp = op_readpc();
2:op_io_cond2();
3:if($1)last_cycle();
  op_writedp(dp,     $2);
  if($1)end;
4:last_cycle();
  op_writedp(dp + 1, $2 >> 8);
}

sta_dpx(0x95, regs.p.m, regs.a.w),
sty_dpx(0x94, regs.p.x, regs.y.w),
stz_dpx(0x74, regs.p.m, 0x0000) {
1:dp = op_readpc();
2:op_io_cond2();
3:op_io();
4:if($1)last_cycle();
  op_writedp(dp + regs.x.w,     $2);
  if($1)end;
5:last_cycle();
  op_writedp(dp + regs.x.w + 1, $2 >> 8);
}

stx_dpy(0x96) {
1:dp = op_readpc();
2:op_io_cond2();
3:op_io();
4:if(regs.p.x)last_cycle();
  op_writedp(dp + regs.y.w,     regs.x.l);
  if(regs.p.x)end;
5:last_cycle();
  op_writedp(dp + regs.y.w + 1, regs.x.h);
}

sta_idp(0x92) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:if(regs.p.m)last_cycle();
  op_writedbr(aa.w,     regs.a.l);
  if(regs.p.m)end;
6:last_cycle();
  op_writedbr(aa.w + 1, regs.a.h);
}

sta_ildp(0x87) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:aa.b = op_readdp(dp + 2);
6:if(regs.p.m)last_cycle();
  op_writelong(aa.d,     regs.a.l);
  if(regs.p.m)end;
7:last_cycle();
  op_writelong(aa.d + 1, regs.a.h);
}

sta_idpx(0x81) {
1:dp = op_readpc();
2:op_io_cond2();
3:op_io();
4:aa.l = op_readdp(dp + regs.x.w);
5:aa.h = op_readdp(dp + regs.x.w + 1);
6:if(regs.p.m)last_cycle();
  op_writedbr(aa.w,     regs.a.l);
  if(regs.p.m)end;
7:last_cycle();
  op_writedbr(aa.w + 1, regs.a.h);
}

sta_idpy(0x91) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:op_io();
6:if(regs.p.m)last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)end;
7:last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
}

sta_ildpy(0x97) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:aa.b = op_readdp(dp + 2);
6:if(regs.p.m)last_cycle();
  op_writelong(aa.d + regs.y.w,     regs.a.l);
  if(regs.p.m)end;
7:last_cycle();
  op_writelong(aa.d + regs.y.w + 1, regs.a.h);
}

sta_sr(0x83) {
1:sp = op_readpc();
2:op_io();
3:if(regs.p.m)last_cycle();
  op_writesp(sp,     regs.a.l);
  if(regs.p.m)end;
4:last_cycle();
  op_writesp(sp + 1, regs.a.h);
}

sta_isry(0x93) {
1:sp = op_readpc();
2:op_io();
3:aa.l = op_readsp(sp);
4:aa.h = op_readsp(sp + 1);
5:op_io();
6:if(regs.p.m)last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)end;
7:last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
}
