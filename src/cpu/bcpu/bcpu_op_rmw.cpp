void bCPU::op_inc() {
l1:
  cpu_io();
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

void bCPU::op_inx() {
l1:
  cpu_io();
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

void bCPU::op_iny() {
l1:
  cpu_io();
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

void bCPU::op_dec() {
l1:
  cpu_io();
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

void bCPU::op_dex() {
l1:
  cpu_io();
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

void bCPU::op_dey() {
l1:
  cpu_io();
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

void bCPU::op_asl() {
l1:
  cpu_io();
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

void bCPU::op_lsr() {
l1:
  cpu_io();
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

void bCPU::op_rol() {
l1:
  cpu_io();
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

void bCPU::op_ror() {
l1:
  cpu_io();
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

void bCPU::op_inc_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_inc_b(); goto l7; }
  else op_inc_w();
l6:
  op_write(OPMODE_DBR, aa.w + 1, rd.h);
l7:
  op_write(OPMODE_DBR, aa.w,     rd.l);
}

void bCPU::op_dec_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_dec_b(); goto l7; }
  else op_dec_w();
l6:
  op_write(OPMODE_DBR, aa.w + 1, rd.h);
l7:
  op_write(OPMODE_DBR, aa.w,     rd.l);
}

void bCPU::op_asl_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_asl_b(); goto l7; }
  else op_asl_w();
l6:
  op_write(OPMODE_DBR, aa.w + 1, rd.h);
l7:
  op_write(OPMODE_DBR, aa.w,     rd.l);
}

void bCPU::op_lsr_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_lsr_b(); goto l7; }
  else op_lsr_w();
l6:
  op_write(OPMODE_DBR, aa.w + 1, rd.h);
l7:
  op_write(OPMODE_DBR, aa.w,     rd.l);
}

void bCPU::op_rol_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_rol_b(); goto l7; }
  else op_rol_w();
l6:
  op_write(OPMODE_DBR, aa.w + 1, rd.h);
l7:
  op_write(OPMODE_DBR, aa.w,     rd.l);
}

void bCPU::op_ror_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_ror_b(); goto l7; }
  else op_ror_w();
l6:
  op_write(OPMODE_DBR, aa.w + 1, rd.h);
l7:
  op_write(OPMODE_DBR, aa.w,     rd.l);
}

void bCPU::op_trb_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_trb_b(); goto l7; }
  else op_trb_w();
l6:
  op_write(OPMODE_DBR, aa.w + 1, rd.h);
l7:
  op_write(OPMODE_DBR, aa.w,     rd.l);
}

void bCPU::op_tsb_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_tsb_b(); goto l7; }
  else op_tsb_w();
l6:
  op_write(OPMODE_DBR, aa.w + 1, rd.h);
l7:
  op_write(OPMODE_DBR, aa.w,     rd.l);
}

void bCPU::op_inc_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_inc_b(); goto l8; }
  else op_inc_w();
l7:
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
}

void bCPU::op_dec_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_dec_b(); goto l8; }
  else op_dec_w();
l7:
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
}

void bCPU::op_asl_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_asl_b(); goto l8; }
  else op_asl_w();
l7:
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
}

void bCPU::op_lsr_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_lsr_b(); goto l8; }
  else op_lsr_w();
l7:
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
}

void bCPU::op_rol_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_rol_b(); goto l8; }
  else op_rol_w();
l7:
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
}

void bCPU::op_ror_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_ror_b(); goto l8; }
  else op_ror_w();
l7:
  op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
}

void bCPU::op_inc_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_inc_b(); goto l7; }
  else op_inc_w();
l6:
  op_write(OPMODE_DP, dp + 1, rd.h);
l7:
  op_write(OPMODE_DP, dp,     rd.l);
}

void bCPU::op_dec_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_dec_b(); goto l7; }
  else op_dec_w();
l6:
  op_write(OPMODE_DP, dp + 1, rd.h);
l7:
  op_write(OPMODE_DP, dp,     rd.l);
}

void bCPU::op_asl_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_asl_b(); goto l7; }
  else op_asl_w();
l6:
  op_write(OPMODE_DP, dp + 1, rd.h);
l7:
  op_write(OPMODE_DP, dp,     rd.l);
}

void bCPU::op_lsr_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_lsr_b(); goto l7; }
  else op_lsr_w();
l6:
  op_write(OPMODE_DP, dp + 1, rd.h);
l7:
  op_write(OPMODE_DP, dp,     rd.l);
}

void bCPU::op_rol_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_rol_b(); goto l7; }
  else op_rol_w();
l6:
  op_write(OPMODE_DP, dp + 1, rd.h);
l7:
  op_write(OPMODE_DP, dp,     rd.l);
}

void bCPU::op_ror_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_ror_b(); goto l7; }
  else op_ror_w();
l6:
  op_write(OPMODE_DP, dp + 1, rd.h);
l7:
  op_write(OPMODE_DP, dp,     rd.l);
}

void bCPU::op_trb_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_trb_b(); goto l7; }
  else op_trb_w();
l6:
  op_write(OPMODE_DP, dp + 1, rd.h);
l7:
  op_write(OPMODE_DP, dp,     rd.l);
}

void bCPU::op_tsb_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)goto l5;
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
l5:
  cpu_io();
  if(regs.p.m) { op_tsb_b(); goto l7; }
  else op_tsb_w();
l6:
  op_write(OPMODE_DP, dp + 1, rd.h);
l7:
  op_write(OPMODE_DP, dp,     rd.l);
}

void bCPU::op_inc_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_inc_b(); goto l8; }
  else op_inc_w();
l7:
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
}

void bCPU::op_dec_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_dec_b(); goto l8; }
  else op_dec_w();
l7:
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
}

void bCPU::op_asl_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_asl_b(); goto l8; }
  else op_asl_w();
l7:
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
}

void bCPU::op_lsr_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_lsr_b(); goto l8; }
  else op_lsr_w();
l7:
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
}

void bCPU::op_rol_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_rol_b(); goto l8; }
  else op_rol_w();
l7:
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
}

void bCPU::op_ror_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m)goto l6;
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
l6:
  cpu_io();
  if(regs.p.m) { op_ror_b(); goto l8; }
  else op_ror_w();
l7:
  op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
l8:
  op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
}

