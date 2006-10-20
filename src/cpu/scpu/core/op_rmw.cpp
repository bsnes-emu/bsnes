//inc
case 0x1a: {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.a.l++;
    regs.p.n = bool(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.a.w++;
    regs.p.n = bool(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//inx
case 0xe8: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l++;
    regs.p.n = bool(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w++;
    regs.p.n = bool(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
} break;

//iny
case 0xc8: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.y.l++;
    regs.p.n = bool(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  } else {
    regs.y.w++;
    regs.p.n = bool(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }
} break;

//dec
case 0x3a: {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.a.l--;
    regs.p.n = bool(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.a.w--;
    regs.p.n = bool(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//dex
case 0xca: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l--;
    regs.p.n = bool(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w--;
    regs.p.n = bool(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
} break;

//dey
case 0x88: {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.y.l--;
    regs.p.n = bool(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  } else {
    regs.y.w--;
    regs.p.n = bool(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }
} break;

//asl
case 0x0a: {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.p.c = bool(regs.a.l & 0x80);
    regs.a.l <<= 1;
    regs.p.n = bool(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = bool(regs.a.w & 0x8000);
    regs.a.w <<= 1;
    regs.p.n = bool(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//lsr
case 0x4a: {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.p.c = regs.a.l & 1;
    regs.a.l >>= 1;
    regs.p.n = bool(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = regs.a.w & 1;
    regs.a.w >>= 1;
    regs.p.n = bool(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//rol
case 0x2a: {
  last_cycle();
  op_io();
  uint16 c = regs.p.c;
  if(regs.p.m) {
    regs.p.c = bool(regs.a.l & 0x80);
    regs.a.l <<= 1;
    regs.a.l |= c;
    regs.p.n = bool(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = bool(regs.a.w & 0x8000);
    regs.a.w <<= 1;
    regs.a.w |= c;
    regs.p.n = bool(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//ror
case 0x6a: {
  last_cycle();
  op_io();
  uint16 c;
  if(regs.p.m) {
    c = (regs.p.c)?0x80:0;
    regs.p.c = regs.a.l & 1;
    regs.a.l >>= 1;
    regs.a.l |= c;
    regs.p.n = bool(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    c = (regs.p.c)?0x8000:0;
    regs.p.c = regs.a.w & 1;
    regs.a.w >>= 1;
    regs.a.w |= c;
    regs.p.n = bool(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
} break;

//inc_addr
case 0xee: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readdbr(aa.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + 1);
  op_io();
  if(regs.p.m) { op_inc_b(); }
  else { op_inc_w();
  op_writedbr(aa.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w,     rd.l);
} break;

//dec_addr
case 0xce: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readdbr(aa.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + 1);
  op_io();
  if(regs.p.m) { op_dec_b(); }
  else { op_dec_w();
  op_writedbr(aa.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w,     rd.l);
} break;

//asl_addr
case 0x0e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readdbr(aa.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + 1);
  op_io();
  if(regs.p.m) { op_asl_b(); }
  else { op_asl_w();
  op_writedbr(aa.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w,     rd.l);
} break;

//lsr_addr
case 0x4e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readdbr(aa.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + 1);
  op_io();
  if(regs.p.m) { op_lsr_b(); }
  else { op_lsr_w();
  op_writedbr(aa.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w,     rd.l);
} break;

//rol_addr
case 0x2e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readdbr(aa.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + 1);
  op_io();
  if(regs.p.m) { op_rol_b(); }
  else { op_rol_w();
  op_writedbr(aa.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w,     rd.l);
} break;

//ror_addr
case 0x6e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readdbr(aa.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + 1);
  op_io();
  if(regs.p.m) { op_ror_b(); }
  else { op_ror_w();
  op_writedbr(aa.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w,     rd.l);
} break;

//trb_addr
case 0x1c: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readdbr(aa.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + 1);
  op_io();
  if(regs.p.m) { op_trb_b(); }
  else { op_trb_w();
  op_writedbr(aa.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w,     rd.l);
} break;

//tsb_addr
case 0x0c: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  rd.l = op_readdbr(aa.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + 1);
  op_io();
  if(regs.p.m) { op_tsb_b(); }
  else { op_tsb_w();
  op_writedbr(aa.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w,     rd.l);
} break;

//inc_addrx
case 0xfe: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_inc_b(); }
  else { op_inc_w();
  op_writedbr(aa.w + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w + regs.x.w,     rd.l);
} break;

//dec_addrx
case 0xde: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_dec_b(); }
  else { op_dec_w();
  op_writedbr(aa.w + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w + regs.x.w,     rd.l);
} break;

//asl_addrx
case 0x1e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_asl_b(); }
  else { op_asl_w();
  op_writedbr(aa.w + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w + regs.x.w,     rd.l);
} break;

//lsr_addrx
case 0x5e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_lsr_b(); }
  else { op_lsr_w();
  op_writedbr(aa.w + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w + regs.x.w,     rd.l);
} break;

//rol_addrx
case 0x3e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_rol_b(); }
  else { op_rol_w();
  op_writedbr(aa.w + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w + regs.x.w,     rd.l);
} break;

//ror_addrx
case 0x7e: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(!regs.p.m)rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_ror_b(); }
  else { op_ror_w();
  op_writedbr(aa.w + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedbr(aa.w + regs.x.w,     rd.l);
} break;

//inc_dp
case 0xe6: {
  dp = op_readpc();
  op_io_cond2();
  rd.l = op_readdp(dp);
  if(!regs.p.m)rd.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) { op_inc_b(); }
  else { op_inc_w();
  op_writedp(dp + 1, rd.h); }
  last_cycle();
  op_writedp(dp,     rd.l);
} break;

//dec_dp
case 0xc6: {
  dp = op_readpc();
  op_io_cond2();
  rd.l = op_readdp(dp);
  if(!regs.p.m)rd.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) { op_dec_b(); }
  else { op_dec_w();
  op_writedp(dp + 1, rd.h); }
  last_cycle();
  op_writedp(dp,     rd.l);
} break;

//asl_dp
case 0x06: {
  dp = op_readpc();
  op_io_cond2();
  rd.l = op_readdp(dp);
  if(!regs.p.m)rd.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) { op_asl_b(); }
  else { op_asl_w();
  op_writedp(dp + 1, rd.h); }
  last_cycle();
  op_writedp(dp,     rd.l);
} break;

//lsr_dp
case 0x46: {
  dp = op_readpc();
  op_io_cond2();
  rd.l = op_readdp(dp);
  if(!regs.p.m)rd.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) { op_lsr_b(); }
  else { op_lsr_w();
  op_writedp(dp + 1, rd.h); }
  last_cycle();
  op_writedp(dp,     rd.l);
} break;

//rol_dp
case 0x26: {
  dp = op_readpc();
  op_io_cond2();
  rd.l = op_readdp(dp);
  if(!regs.p.m)rd.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) { op_rol_b(); }
  else { op_rol_w();
  op_writedp(dp + 1, rd.h); }
  last_cycle();
  op_writedp(dp,     rd.l);
} break;

//ror_dp
case 0x66: {
  dp = op_readpc();
  op_io_cond2();
  rd.l = op_readdp(dp);
  if(!regs.p.m)rd.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) { op_ror_b(); }
  else { op_ror_w();
  op_writedp(dp + 1, rd.h); }
  last_cycle();
  op_writedp(dp,     rd.l);
} break;

//trb_dp
case 0x14: {
  dp = op_readpc();
  op_io_cond2();
  rd.l = op_readdp(dp);
  if(!regs.p.m)rd.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) { op_trb_b(); }
  else { op_trb_w();
  op_writedp(dp + 1, rd.h); }
  last_cycle();
  op_writedp(dp,     rd.l);
} break;

//tsb_dp
case 0x04: {
  dp = op_readpc();
  op_io_cond2();
  rd.l = op_readdp(dp);
  if(!regs.p.m)rd.h = op_readdp(dp + 1);
  op_io();
  if(regs.p.m) { op_tsb_b(); }
  else { op_tsb_w();
  op_writedp(dp + 1, rd.h); }
  last_cycle();
  op_writedp(dp,     rd.l);
} break;

//inc_dpx
case 0xf6: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  rd.l = op_readdp(dp + regs.x.w);
  if(!regs.p.m)rd.h = op_readdp(dp + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_inc_b(); }
  else { op_inc_w();
  op_writedp(dp + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedp(dp + regs.x.w,     rd.l);
} break;

//dec_dpx
case 0xd6: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  rd.l = op_readdp(dp + regs.x.w);
  if(!regs.p.m)rd.h = op_readdp(dp + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_dec_b(); }
  else { op_dec_w();
  op_writedp(dp + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedp(dp + regs.x.w,     rd.l);
} break;

//asl_dpx
case 0x16: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  rd.l = op_readdp(dp + regs.x.w);
  if(!regs.p.m)rd.h = op_readdp(dp + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_asl_b(); }
  else { op_asl_w();
  op_writedp(dp + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedp(dp + regs.x.w,     rd.l);
} break;

//lsr_dpx
case 0x56: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  rd.l = op_readdp(dp + regs.x.w);
  if(!regs.p.m)rd.h = op_readdp(dp + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_lsr_b(); }
  else { op_lsr_w();
  op_writedp(dp + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedp(dp + regs.x.w,     rd.l);
} break;

//rol_dpx
case 0x36: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  rd.l = op_readdp(dp + regs.x.w);
  if(!regs.p.m)rd.h = op_readdp(dp + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_rol_b(); }
  else { op_rol_w();
  op_writedp(dp + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedp(dp + regs.x.w,     rd.l);
} break;

//ror_dpx
case 0x76: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  rd.l = op_readdp(dp + regs.x.w);
  if(!regs.p.m)rd.h = op_readdp(dp + regs.x.w + 1);
  op_io();
  if(regs.p.m) { op_ror_b(); }
  else { op_ror_w();
  op_writedp(dp + regs.x.w + 1, rd.h); }
  last_cycle();
  op_writedp(dp + regs.x.w,     rd.l);
} break;

