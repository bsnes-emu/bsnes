sta_addr(0x8d, regs.p.m, regs.a.w),
stx_addr(0x8e, regs.p.x, regs.x.w),
sty_addr(0x8c, regs.p.x, regs.y.w),
stz_addr(0x9c, regs.p.m, 0x0000) {
1:aa.l = op_read();
2:aa.h = op_read();
3:op_write(OPMODE_DBR, aa.w,     $2);
  if($1)end;
4:op_write(OPMODE_DBR, aa.w + 1, $2 >> 8);
}

sta_addrx(0x9d, regs.p.m, regs.a.w),
stz_addrx(0x9e, regs.p.m, 0x0000) {
1:aa.l = op_read();
2:aa.h = op_read();
3:cpu_c4(aa.w, aa.w + regs.x.w);
4:op_write(OPMODE_DBR, aa.w + regs.x.w,     $2);
  if($1)end;
5:op_write(OPMODE_DBR, aa.w + regs.x.w + 1, $2 >> 8);
}

sta_addry(0x99) {
1:aa.l = op_read();
2:aa.h = op_read();
3:cpu_c4(aa.w, aa.w + regs.y.w);
4:op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)end;
5:op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
}

sta_long(0x8f) {
1:aa.l = op_read();
2:aa.h = op_read();
3:aa.b = op_read();
4:op_write(OPMODE_LONG, aa.d,     regs.a.l);
  if(regs.p.m)end;
5:op_write(OPMODE_LONG, aa.d + 1, regs.a.h);
}

sta_longx(0x9f) {
1:aa.l = op_read();
2:aa.h = op_read();
3:aa.b = op_read();
4:op_write(OPMODE_LONG, aa.d + regs.x.w,     regs.a.l);
  if(regs.p.m)end;
5:op_write(OPMODE_LONG, aa.d + regs.x.w + 1, regs.a.h);
}

sta_dp(0x85, regs.p.m, regs.a.w),
stx_dp(0x86, regs.p.x, regs.x.w),
sty_dp(0x84, regs.p.x, regs.y.w),
stz_dp(0x64, regs.p.m, 0x0000) {
1:dp = op_read();
2:cpu_c2();
3:op_write(OPMODE_DP, dp,     $2);
  if($1)end;
4:op_write(OPMODE_DP, dp + 1, $2 >> 8);
}

sta_dpx(0x95, regs.p.m, regs.a.w),
sty_dpx(0x94, regs.p.x, regs.y.w),
stz_dpx(0x74, regs.p.m, 0x0000) {
1:dp = op_read();
2:cpu_c2();
3:cpu_io();
4:op_write(OPMODE_DP, dp + regs.x.w,     $2);
  if($1)end;
5:op_write(OPMODE_DP, dp + regs.x.w + 1, $2 >> 8);
}

stx_dpy(0x96) {
1:dp = op_read();
2:cpu_c2();
3:cpu_io();
4:op_write(OPMODE_DP, dp + regs.y.w,     regs.x.l);
  if(regs.p.x)end;
5:op_write(OPMODE_DP, dp + regs.y.w + 1, regs.x.h);
}

sta_idp(0x92) {
1:dp = op_read();
2:cpu_c2();
3:aa.l = op_read(OPMODE_DP, dp);
4:aa.h = op_read(OPMODE_DP, dp + 1);
5:op_write(OPMODE_DBR, aa.w,     regs.a.l);
  if(regs.p.m)end;
6:op_write(OPMODE_DBR, aa.w + 1, regs.a.h);
}

sta_ildp(0x87) {
1:dp = op_read();
2:cpu_c2();
3:aa.l = op_read(OPMODE_DP, dp);
4:aa.h = op_read(OPMODE_DP, dp + 1);
5:aa.b = op_read(OPMODE_DP, dp + 2);
6:op_write(OPMODE_LONG, aa.d,     regs.a.l);
  if(regs.p.m)end;
7:op_write(OPMODE_LONG, aa.d + 1, regs.a.h);
}

sta_idpx(0x81) {
1:dp = op_read();
2:cpu_c2();
3:cpu_io();
4:aa.l = op_read(OPMODE_DP, dp + regs.x.w);
5:aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
6:op_write(OPMODE_DBR, aa.w,     regs.a.l);
  if(regs.p.m)end;
7:op_write(OPMODE_DBR, aa.w + 1, regs.a.h);
}

sta_idpy(0x91) {
1:dp = op_read();
2:cpu_c2();
3:aa.l = op_read(OPMODE_DP, dp);
4:aa.h = op_read(OPMODE_DP, dp + 1);
5:cpu_c4(aa.w, aa.w + regs.y.w);
6:op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)end;
7:op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
}

sta_ildpy(0x97) {
1:dp = op_read();
2:cpu_c2();
3:aa.l = op_read(OPMODE_DP, dp);
4:aa.h = op_read(OPMODE_DP, dp + 1);
5:aa.b = op_read(OPMODE_DP, dp + 2);
6:op_write(OPMODE_LONG, aa.d + regs.y.w,     regs.a.l);
  if(regs.p.m)end;
7:op_write(OPMODE_LONG, aa.d + regs.y.w + 1, regs.a.h);
}

sta_sr(0x83) {
1:sp = op_read();
2:cpu_io();
3:op_write(OPMODE_SP, sp,     regs.a.l);
  if(regs.p.m)end;
4:op_write(OPMODE_SP, sp + 1, regs.a.h);
}

sta_isry(0x93) {
1:sp = op_read();
2:cpu_io();
3:aa.l = op_read(OPMODE_SP, sp);
4:aa.h = op_read(OPMODE_SP, sp + 1);
5:cpu_io();
6:op_write(OPMODE_DBR, aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m)end;
7:op_write(OPMODE_DBR, aa.w + regs.y.w + 1, regs.a.h);
}
