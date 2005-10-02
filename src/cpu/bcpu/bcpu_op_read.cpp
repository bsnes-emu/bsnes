void bCPU::op_adc_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.m)last_cycle();
    rd.l = op_read();
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.m)last_cycle();
    rd.l = op_read();
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.m)last_cycle();
    rd.l = op_read();
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cpx_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.x)last_cycle();
    rd.l = op_read();
    if(regs.p.x) { op_cpx_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_cpx_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cpy_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.x)last_cycle();
    rd.l = op_read();
    if(regs.p.x) { op_cpy_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_cpy_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.m)last_cycle();
    rd.l = op_read();
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.m)last_cycle();
    rd.l = op_read();
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldx_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.x)last_cycle();
    rd.l = op_read();
    if(regs.p.x) { op_ldx_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_ldx_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldy_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.x)last_cycle();
    rd.l = op_read();
    if(regs.p.x) { op_ldy_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_ldy_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.m)last_cycle();
    rd.l = op_read();
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.m)last_cycle();
    rd.l = op_read();
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_bit_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_bit_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_bit_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cpx_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.x) { op_cpx_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_cpx_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cpy_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.x) { op_cpy_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_cpy_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldx_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.x) { op_ldx_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_ldx_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldy_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.x) { op_ldy_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_ldy_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_addr() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_bit_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m) { op_bit_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_bit_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldy_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.x) { op_ldy_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_ldy_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_addrx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.x.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_addry() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_addry() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_addry() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_addry() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_addry() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldx_addry() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 4:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.x) { op_ldx_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_ldx_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_addry() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_addry() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_long() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_long() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_long() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_long() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_long() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_long() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_long() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_longx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_longx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_longx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_longx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_longx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_longx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_longx() {
  switch(status.cycle_pos++) {
  case 1:
    aa.l = op_read();
    break;
  case 2:
    aa.h = op_read();
    break;
  case 3:
    aa.b = op_read();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_bit_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m) { op_bit_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_bit_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cpx_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.x) { op_cpx_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_cpx_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cpy_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.x) { op_cpy_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_cpy_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldx_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.x) { op_ldx_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_ldx_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldy_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.x) { op_ldy_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_ldy_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_dp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_bit_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m) { op_bit_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_bit_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldy_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.x) { op_ldy_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_ldy_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_dpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.x.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ldx_dpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    if(regs.p.x)last_cycle();
    rd.l = op_read(OPMODE_DP, dp + regs.y.w);
    if(regs.p.x) { op_ldx_b(); status.cycle_pos = 0; }
    break;
  case 5:
    last_cycle();
    rd.h = op_read(OPMODE_DP, dp + regs.y.w + 1);
    op_ldx_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_idp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 6:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_idp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 6:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_idp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 6:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_idp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 6:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_idp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 6:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_idp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 6:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_idp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 6:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_idpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    aa.l = op_read(OPMODE_DP, dp + regs.x.w);
    break;
  case 5:
    aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_idpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    aa.l = op_read(OPMODE_DP, dp + regs.x.w);
    break;
  case 5:
    aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_idpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    aa.l = op_read(OPMODE_DP, dp + regs.x.w);
    break;
  case 5:
    aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_idpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    aa.l = op_read(OPMODE_DP, dp + regs.x.w);
    break;
  case 5:
    aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_idpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    aa.l = op_read(OPMODE_DP, dp + regs.x.w);
    break;
  case 5:
    aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_idpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    aa.l = op_read(OPMODE_DP, dp + regs.x.w);
    break;
  case 5:
    aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_idpx() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    cpu_io();
    break;
  case 4:
    aa.l = op_read(OPMODE_DP, dp + regs.x.w);
    break;
  case 5:
    aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_idpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_idpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_idpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_idpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_idpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_idpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_idpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    cpu_c4(aa.w, aa.w + regs.y.w);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_ildp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_ildp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_ildp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_ildp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_ildp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_ildp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_ildp() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_ildpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_ildpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_ildpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_ildpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_ildpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_ildpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_ildpy() {
  switch(status.cycle_pos++) {
  case 1:
    dp = op_read();
    break;
  case 2:
    cpu_c2();
    break;
  case 3:
    aa.l = op_read(OPMODE_DP, dp);
    break;
  case 4:
    aa.h = op_read(OPMODE_DP, dp + 1);
    break;
  case 5:
    aa.b = op_read(OPMODE_DP, dp + 2);
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_sr() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_SP, sp);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_SP, sp + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_sr() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_SP, sp);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_SP, sp + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_sr() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_SP, sp);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_SP, sp + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_sr() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_SP, sp);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_SP, sp + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_sr() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_SP, sp);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_SP, sp + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_sr() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_SP, sp);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_SP, sp + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_sr() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_SP, sp);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 4:
    last_cycle();
    rd.h = op_read(OPMODE_SP, sp + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_adc_isry() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    aa.l = op_read(OPMODE_SP, sp);
    break;
  case 4:
    aa.h = op_read(OPMODE_SP, sp + 1);
    break;
  case 5:
    cpu_io();
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_adc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_adc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_and_isry() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    aa.l = op_read(OPMODE_SP, sp);
    break;
  case 4:
    aa.h = op_read(OPMODE_SP, sp + 1);
    break;
  case 5:
    cpu_io();
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_and_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_and_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_cmp_isry() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    aa.l = op_read(OPMODE_SP, sp);
    break;
  case 4:
    aa.h = op_read(OPMODE_SP, sp + 1);
    break;
  case 5:
    cpu_io();
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_cmp_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_cmp_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_eor_isry() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    aa.l = op_read(OPMODE_SP, sp);
    break;
  case 4:
    aa.h = op_read(OPMODE_SP, sp + 1);
    break;
  case 5:
    cpu_io();
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_eor_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_eor_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_lda_isry() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    aa.l = op_read(OPMODE_SP, sp);
    break;
  case 4:
    aa.h = op_read(OPMODE_SP, sp + 1);
    break;
  case 5:
    cpu_io();
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_lda_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_lda_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_ora_isry() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    aa.l = op_read(OPMODE_SP, sp);
    break;
  case 4:
    aa.h = op_read(OPMODE_SP, sp + 1);
    break;
  case 5:
    cpu_io();
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_ora_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_ora_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_sbc_isry() {
  switch(status.cycle_pos++) {
  case 1:
    sp = op_read();
    break;
  case 2:
    cpu_io();
    break;
  case 3:
    aa.l = op_read(OPMODE_SP, sp);
    break;
  case 4:
    aa.h = op_read(OPMODE_SP, sp + 1);
    break;
  case 5:
    cpu_io();
    break;
  case 6:
    if(regs.p.m)last_cycle();
    rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
    if(regs.p.m) { op_sbc_b(); status.cycle_pos = 0; }
    break;
  case 7:
    last_cycle();
    rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
    op_sbc_w();
    status.cycle_pos = 0;
    break;
  }
}

void bCPU::op_bit_const() {
  switch(status.cycle_pos++) {
  case 1:
    if(regs.p.m)last_cycle();
    rd.l = op_read();
    if(regs.p.m) {
      regs.p.z = ((rd.l & regs.a.l) == 0);
      status.cycle_pos = 0;
    }
    break;
  case 2:
    last_cycle();
    rd.h = op_read();
    regs.p.z = ((rd.w & regs.a.w) == 0);
    status.cycle_pos = 0;
    break;
  }
}

