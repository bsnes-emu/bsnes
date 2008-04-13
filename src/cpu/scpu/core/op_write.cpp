//sta_addr
case 0x8d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w,     regs.a.w);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + 1, regs.a.w >> 8);
} break;

//stx_addr
case 0x8e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.x) last_cycle();
  op_writedbr(aa.w,     regs.x.w);
  if(regs.p.x) break;
  last_cycle();
  op_writedbr(aa.w + 1, regs.x.w >> 8);
} break;

//sty_addr
case 0x8c: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.x) last_cycle();
  op_writedbr(aa.w,     regs.y.w);
  if(regs.p.x) break;
  last_cycle();
  op_writedbr(aa.w + 1, regs.y.w >> 8);
} break;

//stz_addr
case 0x9c: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w,     0x0000);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + 1, 0x0000 >> 8);
} break;

//sta_addrx
case 0x9d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w + regs.x.w,     regs.a.w);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + regs.x.w + 1, regs.a.w >> 8);
} break;

//stz_addrx
case 0x9e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w + regs.x.w,     0x0000);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + regs.x.w + 1, 0x0000 >> 8);
} break;

//sta_addry
case 0x99: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
} break;

//sta_long
case 0x8f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m) last_cycle();
  op_writelong(aa.d,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writelong(aa.d + 1, regs.a.h);
} break;

//sta_longx
case 0x9f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m) last_cycle();
  op_writelong(aa.d + regs.x.w,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writelong(aa.d + regs.x.w + 1, regs.a.h);
} break;

//sta_dp
case 0x85: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m) last_cycle();
  op_writedp(dp,     regs.a.w);
  if(regs.p.m) break;
  last_cycle();
  op_writedp(dp + 1, regs.a.w >> 8);
} break;

//stx_dp
case 0x86: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.x) last_cycle();
  op_writedp(dp,     regs.x.w);
  if(regs.p.x) break;
  last_cycle();
  op_writedp(dp + 1, regs.x.w >> 8);
} break;

//sty_dp
case 0x84: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.x) last_cycle();
  op_writedp(dp,     regs.y.w);
  if(regs.p.x) break;
  last_cycle();
  op_writedp(dp + 1, regs.y.w >> 8);
} break;

//stz_dp
case 0x64: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m) last_cycle();
  op_writedp(dp,     0x0000);
  if(regs.p.m) break;
  last_cycle();
  op_writedp(dp + 1, 0x0000 >> 8);
} break;

//sta_dpx
case 0x95: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m) last_cycle();
  op_writedp(dp + regs.x.w,     regs.a.w);
  if(regs.p.m) break;
  last_cycle();
  op_writedp(dp + regs.x.w + 1, regs.a.w >> 8);
} break;

//sty_dpx
case 0x94: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.x) last_cycle();
  op_writedp(dp + regs.x.w,     regs.y.w);
  if(regs.p.x) break;
  last_cycle();
  op_writedp(dp + regs.x.w + 1, regs.y.w >> 8);
} break;

//stz_dpx
case 0x74: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m) last_cycle();
  op_writedp(dp + regs.x.w,     0x0000);
  if(regs.p.m) break;
  last_cycle();
  op_writedp(dp + regs.x.w + 1, 0x0000 >> 8);
} break;

//stx_dpy
case 0x96: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.x) last_cycle();
  op_writedp(dp + regs.y.w,     regs.x.l);
  if(regs.p.x) break;
  last_cycle();
  op_writedp(dp + regs.y.w + 1, regs.x.h);
} break;

//sta_idp
case 0x92: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + 1, regs.a.h);
} break;

//sta_ildp
case 0x87: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m) last_cycle();
  op_writelong(aa.d,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writelong(aa.d + 1, regs.a.h);
} break;

//sta_idpx
case 0x81: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + 1, regs.a.h);
} break;

//sta_idpy
case 0x91: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
} break;

//sta_ildpy
case 0x97: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m) last_cycle();
  op_writelong(aa.d + regs.y.w,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writelong(aa.d + regs.y.w + 1, regs.a.h);
} break;

//sta_sr
case 0x83: {
  sp = op_readpc();
  op_io();
  if(regs.p.m) last_cycle();
  op_writesp(sp,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writesp(sp + 1, regs.a.h);
} break;

//sta_isry
case 0x93: {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m) last_cycle();
  op_writedbr(aa.w + regs.y.w,     regs.a.l);
  if(regs.p.m) break;
  last_cycle();
  op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
} break;

