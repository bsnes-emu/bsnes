void sCPU::op_inc() {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.a.l++;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.a.w++;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_inx() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l++;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w++;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
}

void sCPU::op_iny() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.y.l++;
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  } else {
    regs.y.w++;
    regs.p.n = !!(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }
}

void sCPU::op_dec() {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.a.l--;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.a.w--;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_dex() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l--;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w--;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
}

void sCPU::op_dey() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.y.l--;
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  } else {
    regs.y.w--;
    regs.p.n = !!(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }
}

void sCPU::op_asl() {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.p.c = !!(regs.a.l & 0x80);
    regs.a.l <<= 1;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = !!(regs.a.w & 0x8000);
    regs.a.w <<= 1;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_lsr() {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.p.c = regs.a.l & 1;
    regs.a.l >>= 1;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = regs.a.w & 1;
    regs.a.w >>= 1;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_rol() {
  last_cycle();
  op_io();
  uint16 c = regs.p.c;
  if(regs.p.m) {
    regs.p.c = !!(regs.a.l & 0x80);
    regs.a.l <<= 1;
    regs.a.l |= c;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = !!(regs.a.w & 0x8000);
    regs.a.w <<= 1;
    regs.a.w |= c;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_ror() {
  last_cycle();
  op_io();
  uint16 c;
  if(regs.p.m) {
    c = (regs.p.c)?0x80:0;
    regs.p.c = regs.a.l & 1;
    regs.a.l >>= 1;
    regs.a.l |= c;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    c = (regs.p.c)?0x8000:0;
    regs.p.c = regs.a.w & 1;
    regs.a.w >>= 1;
    regs.a.w |= c;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_inc_addr() {
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
}

void sCPU::op_dec_addr() {
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
}

void sCPU::op_asl_addr() {
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
}

void sCPU::op_lsr_addr() {
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
}

void sCPU::op_rol_addr() {
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
}

void sCPU::op_ror_addr() {
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
}

void sCPU::op_trb_addr() {
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
}

void sCPU::op_tsb_addr() {
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
}

void sCPU::op_inc_addrx() {
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
}

void sCPU::op_dec_addrx() {
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
}

void sCPU::op_asl_addrx() {
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
}

void sCPU::op_lsr_addrx() {
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
}

void sCPU::op_rol_addrx() {
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
}

void sCPU::op_ror_addrx() {
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
}

void sCPU::op_inc_dp() {
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
}

void sCPU::op_dec_dp() {
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
}

void sCPU::op_asl_dp() {
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
}

void sCPU::op_lsr_dp() {
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
}

void sCPU::op_rol_dp() {
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
}

void sCPU::op_ror_dp() {
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
}

void sCPU::op_trb_dp() {
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
}

void sCPU::op_tsb_dp() {
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
}

void sCPU::op_inc_dpx() {
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
}

void sCPU::op_dec_dpx() {
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
}

void sCPU::op_asl_dpx() {
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
}

void sCPU::op_lsr_dpx() {
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
}

void sCPU::op_rol_dpx() {
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
}

void sCPU::op_ror_dpx() {
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
}

