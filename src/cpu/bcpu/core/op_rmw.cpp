void bCPU::op_inc() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_inx() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_iny() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_dec() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_dex() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_dey() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_asl() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_lsr() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rol() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_ror() {
  switch(status.cycle_pos++) {
  case 1: {
    last_cycle();
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
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_inc_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_inc_b(); status.cycle_pos++; }
    else op_inc_w();
    } break;
  case 6: {
    op_write(OPMODE_DBR, aa.w + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_dec_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_dec_b(); status.cycle_pos++; }
    else op_dec_w();
    } break;
  case 6: {
    op_write(OPMODE_DBR, aa.w + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_asl_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_asl_b(); status.cycle_pos++; }
    else op_asl_w();
    } break;
  case 6: {
    op_write(OPMODE_DBR, aa.w + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_lsr_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_lsr_b(); status.cycle_pos++; }
    else op_lsr_w();
    } break;
  case 6: {
    op_write(OPMODE_DBR, aa.w + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rol_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_rol_b(); status.cycle_pos++; }
    else op_rol_w();
    } break;
  case 6: {
    op_write(OPMODE_DBR, aa.w + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_ror_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_ror_b(); status.cycle_pos++; }
    else op_ror_w();
    } break;
  case 6: {
    op_write(OPMODE_DBR, aa.w + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_trb_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_trb_b(); status.cycle_pos++; }
    else op_trb_w();
    } break;
  case 6: {
    op_write(OPMODE_DBR, aa.w + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tsb_addr() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_tsb_b(); status.cycle_pos++; }
    else op_tsb_w();
    } break;
  case 6: {
    op_write(OPMODE_DBR, aa.w + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_inc_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_inc_b(); status.cycle_pos++; }
    else op_inc_w();
    } break;
  case 7: {
    op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_dec_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_dec_b(); status.cycle_pos++; }
    else op_dec_w();
    } break;
  case 7: {
    op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_asl_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_asl_b(); status.cycle_pos++; }
    else op_asl_w();
    } break;
  case 7: {
    op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_lsr_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_lsr_b(); status.cycle_pos++; }
    else op_lsr_w();
    } break;
  case 7: {
    op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rol_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_rol_b(); status.cycle_pos++; }
    else op_rol_w();
    } break;
  case 7: {
    op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_ror_addrx() {
  switch(status.cycle_pos++) {
  case 1: {
    aa.l = op_read();
    } break;
  case 2: {
    aa.h = op_read();
    } break;
  case 3: {
    cpu_io();
    } break;
  case 4: {
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_ror_b(); status.cycle_pos++; }
    else op_ror_w();
    } break;
  case 7: {
    op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_inc_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_inc_b(); status.cycle_pos++; }
    else op_inc_w();
    } break;
  case 6: {
    op_write(OPMODE_DP, dp + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DP, dp,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_dec_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_dec_b(); status.cycle_pos++; }
    else op_dec_w();
    } break;
  case 6: {
    op_write(OPMODE_DP, dp + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DP, dp,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_asl_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_asl_b(); status.cycle_pos++; }
    else op_asl_w();
    } break;
  case 6: {
    op_write(OPMODE_DP, dp + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DP, dp,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_lsr_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_lsr_b(); status.cycle_pos++; }
    else op_lsr_w();
    } break;
  case 6: {
    op_write(OPMODE_DP, dp + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DP, dp,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rol_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_rol_b(); status.cycle_pos++; }
    else op_rol_w();
    } break;
  case 6: {
    op_write(OPMODE_DP, dp + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DP, dp,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_ror_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_ror_b(); status.cycle_pos++; }
    else op_ror_w();
    } break;
  case 6: {
    op_write(OPMODE_DP, dp + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DP, dp,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_trb_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_trb_b(); status.cycle_pos++; }
    else op_trb_w();
    } break;
  case 6: {
    op_write(OPMODE_DP, dp + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DP, dp,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_tsb_dp() {
  switch(status.cycle_pos++) {
  case 1: {
    dp = op_read();
    } break;
  case 2: {
    cpu_c2();
    } break;
  case 3: {
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 4: {
    rd.h = op_read(OPMODE_DP, dp + 1);
    } break;
  case 5: {
    cpu_io();
    if(regs.p.m) { op_tsb_b(); status.cycle_pos++; }
    else op_tsb_w();
    } break;
  case 6: {
    op_write(OPMODE_DP, dp + 1, rd.h);
    } break;
  case 7: {
    last_cycle();
    op_write(OPMODE_DP, dp,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_inc_dpx() {
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
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_inc_b(); status.cycle_pos++; }
    else op_inc_w();
    } break;
  case 7: {
    op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_dec_dpx() {
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
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_dec_b(); status.cycle_pos++; }
    else op_dec_w();
    } break;
  case 7: {
    op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_asl_dpx() {
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
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_asl_b(); status.cycle_pos++; }
    else op_asl_w();
    } break;
  case 7: {
    op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_lsr_dpx() {
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
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_lsr_b(); status.cycle_pos++; }
    else op_lsr_w();
    } break;
  case 7: {
    op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_rol_dpx() {
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
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_rol_b(); status.cycle_pos++; }
    else op_rol_w();
    } break;
  case 7: {
    op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

void bCPU::op_ror_dpx() {
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
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m)status.cycle_pos++;
    } break;
  case 5: {
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    } break;
  case 6: {
    cpu_io();
    if(regs.p.m) { op_ror_b(); status.cycle_pos++; }
    else op_ror_w();
    } break;
  case 7: {
    op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
    } break;
  case 8: {
    last_cycle();
    op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
    status.cycle_pos = 0;
    } break;
  }
}

