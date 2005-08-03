void bCPU::op_adc_const() {
l1:
  rd.l = op_read();
  if(regs.p.m) { op_adc_b(); return; }
l2:
  rd.h = op_read();
  op_adc_w();
}

void bCPU::op_and_const() {
l1:
  rd.l = op_read();
  if(regs.p.m) { op_and_b(); return; }
l2:
  rd.h = op_read();
  op_and_w();
}

void bCPU::op_cmp_const() {
l1:
  rd.l = op_read();
  if(regs.p.m) { op_cmp_b(); return; }
l2:
  rd.h = op_read();
  op_cmp_w();
}

void bCPU::op_cpx_const() {
l1:
  rd.l = op_read();
  if(regs.p.x) { op_cpx_b(); return; }
l2:
  rd.h = op_read();
  op_cpx_w();
}

void bCPU::op_cpy_const() {
l1:
  rd.l = op_read();
  if(regs.p.x) { op_cpy_b(); return; }
l2:
  rd.h = op_read();
  op_cpy_w();
}

void bCPU::op_eor_const() {
l1:
  rd.l = op_read();
  if(regs.p.m) { op_eor_b(); return; }
l2:
  rd.h = op_read();
  op_eor_w();
}

void bCPU::op_lda_const() {
l1:
  rd.l = op_read();
  if(regs.p.m) { op_lda_b(); return; }
l2:
  rd.h = op_read();
  op_lda_w();
}

void bCPU::op_ldx_const() {
l1:
  rd.l = op_read();
  if(regs.p.x) { op_ldx_b(); return; }
l2:
  rd.h = op_read();
  op_ldx_w();
}

void bCPU::op_ldy_const() {
l1:
  rd.l = op_read();
  if(regs.p.x) { op_ldy_b(); return; }
l2:
  rd.h = op_read();
  op_ldy_w();
}

void bCPU::op_ora_const() {
l1:
  rd.l = op_read();
  if(regs.p.m) { op_ora_b(); return; }
l2:
  rd.h = op_read();
  op_ora_w();
}

void bCPU::op_sbc_const() {
l1:
  rd.l = op_read();
  if(regs.p.m) { op_sbc_b(); return; }
l2:
  rd.h = op_read();
  op_sbc_w();
}

void bCPU::op_adc_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_adc_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_adc_w();
}

void bCPU::op_and_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_and_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_and_w();
}

void bCPU::op_bit_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_bit_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_bit_w();
}

void bCPU::op_cmp_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_cmp_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_cmp_w();
}

void bCPU::op_cpx_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.x) { op_cpx_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_cpx_w();
}

void bCPU::op_cpy_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.x) { op_cpy_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_cpy_w();
}

void bCPU::op_eor_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_eor_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_eor_w();
}

void bCPU::op_lda_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_lda_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_lda_w();
}

void bCPU::op_ldx_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.x) { op_ldx_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_ldx_w();
}

void bCPU::op_ldy_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.x) { op_ldy_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_ldy_w();
}

void bCPU::op_ora_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_ora_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_addr() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_sbc_b(); return; }
l4:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_sbc_w();
}

void bCPU::op_adc_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m) { op_adc_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_adc_w();
}

void bCPU::op_and_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m) { op_and_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_and_w();
}

void bCPU::op_bit_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m) { op_bit_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_bit_w();
}

void bCPU::op_cmp_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m) { op_cmp_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m) { op_eor_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_eor_w();
}

void bCPU::op_lda_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m) { op_lda_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_lda_w();
}

void bCPU::op_ldy_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.x) { op_ldy_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_ldy_w();
}

void bCPU::op_ora_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m) { op_ora_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_addrx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.x.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m) { op_sbc_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_sbc_w();
}

void bCPU::op_adc_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_adc_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_adc_w();
}

void bCPU::op_and_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_and_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_and_w();
}

void bCPU::op_cmp_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_cmp_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_eor_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_eor_w();
}

void bCPU::op_lda_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_lda_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_lda_w();
}

void bCPU::op_ldx_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.x) { op_ldx_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_ldx_w();
}

void bCPU::op_ora_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_ora_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_addry() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_c4(aa.w, aa.w + regs.y.w);
l4:
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_sbc_b(); return; }
l5:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_sbc_w();
}

void bCPU::op_adc_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_adc_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_adc_w();
}

void bCPU::op_and_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_and_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_and_w();
}

void bCPU::op_cmp_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_cmp_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_cmp_w();
}

void bCPU::op_eor_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_eor_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_eor_w();
}

void bCPU::op_lda_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_lda_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_lda_w();
}

void bCPU::op_ora_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_ora_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_ora_w();
}

void bCPU::op_sbc_long() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_sbc_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_sbc_w();
}

void bCPU::op_adc_longx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
  if(regs.p.m) { op_adc_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
  op_adc_w();
}

void bCPU::op_and_longx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
  if(regs.p.m) { op_and_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
  op_and_w();
}

void bCPU::op_cmp_longx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
  if(regs.p.m) { op_cmp_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_longx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
  if(regs.p.m) { op_eor_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
  op_eor_w();
}

void bCPU::op_lda_longx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
  if(regs.p.m) { op_lda_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
  op_lda_w();
}

void bCPU::op_ora_longx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
  if(regs.p.m) { op_ora_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_longx() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  aa.b = op_read();
l4:
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
  if(regs.p.m) { op_sbc_b(); return; }
l5:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
  op_sbc_w();
}

void bCPU::op_adc_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m) { op_adc_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_adc_w();
}

void bCPU::op_and_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m) { op_and_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_and_w();
}

void bCPU::op_bit_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m) { op_bit_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_bit_w();
}

void bCPU::op_cmp_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m) { op_cmp_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_cmp_w();
}

void bCPU::op_cpx_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.x) { op_cpx_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_cpx_w();
}

void bCPU::op_cpy_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.x) { op_cpy_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_cpy_w();
}

void bCPU::op_eor_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m) { op_eor_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_eor_w();
}

void bCPU::op_lda_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m) { op_lda_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_lda_w();
}

void bCPU::op_ldx_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.x) { op_ldx_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_ldx_w();
}

void bCPU::op_ldy_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.x) { op_ldy_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_ldy_w();
}

void bCPU::op_ora_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m) { op_ora_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_ora_w();
}

void bCPU::op_sbc_dp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m) { op_sbc_b(); return; }
l4:
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_sbc_w();
}

void bCPU::op_adc_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m) { op_adc_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_adc_w();
}

void bCPU::op_and_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m) { op_and_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_and_w();
}

void bCPU::op_bit_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m) { op_bit_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_bit_w();
}

void bCPU::op_cmp_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m) { op_cmp_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m) { op_eor_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_eor_w();
}

void bCPU::op_lda_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m) { op_lda_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_lda_w();
}

void bCPU::op_ldy_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.x) { op_ldy_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_ldy_w();
}

void bCPU::op_ora_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m) { op_ora_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_dpx() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m) { op_sbc_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_sbc_w();
}

void bCPU::op_ldx_dpy() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  cpu_io();
l4:
  rd.l = op_read(OPMODE_DP, dp + regs.y.w);
  if(regs.p.x) { op_ldx_b(); return; }
l5:
  rd.h = op_read(OPMODE_DP, dp + regs.y.w + 1);
  op_ldx_w();
}

void bCPU::op_adc_idp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_adc_b(); return; }
l6:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_adc_w();
}

void bCPU::op_and_idp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_and_b(); return; }
l6:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_and_w();
}

void bCPU::op_cmp_idp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_cmp_b(); return; }
l6:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_idp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_eor_b(); return; }
l6:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_eor_w();
}

void bCPU::op_lda_idp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_lda_b(); return; }
l6:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_lda_w();
}

void bCPU::op_ora_idp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_ora_b(); return; }
l6:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_idp() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_sbc_b(); return; }
l6:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_sbc_w();
}

void bCPU::op_adc_idpx() {
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
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_adc_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_adc_w();
}

void bCPU::op_and_idpx() {
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
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_and_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_and_w();
}

void bCPU::op_cmp_idpx() {
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
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_cmp_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_idpx() {
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
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_eor_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_eor_w();
}

void bCPU::op_lda_idpx() {
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
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_lda_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_lda_w();
}

void bCPU::op_ora_idpx() {
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
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_ora_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_idpx() {
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
  rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m) { op_sbc_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_sbc_w();
}

void bCPU::op_adc_idpy() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_adc_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_adc_w();
}

void bCPU::op_and_idpy() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_and_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_and_w();
}

void bCPU::op_cmp_idpy() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_cmp_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_idpy() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_eor_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_eor_w();
}

void bCPU::op_lda_idpy() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_lda_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_lda_w();
}

void bCPU::op_ora_idpy() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_ora_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_idpy() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_sbc_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_sbc_w();
}

void bCPU::op_adc_ildp() {
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
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_adc_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_adc_w();
}

void bCPU::op_and_ildp() {
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
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_and_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_and_w();
}

void bCPU::op_cmp_ildp() {
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
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_cmp_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_cmp_w();
}

void bCPU::op_eor_ildp() {
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
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_eor_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_eor_w();
}

void bCPU::op_lda_ildp() {
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
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_lda_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_lda_w();
}

void bCPU::op_ora_ildp() {
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
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_ora_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_ora_w();
}

void bCPU::op_sbc_ildp() {
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
  rd.l = op_read(OPMODE_LONG, aa.d);
  if(regs.p.m) { op_sbc_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_sbc_w();
}

void bCPU::op_adc_ildpy() {
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
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
  if(regs.p.m) { op_adc_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
  op_adc_w();
}

void bCPU::op_and_ildpy() {
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
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
  if(regs.p.m) { op_and_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
  op_and_w();
}

void bCPU::op_cmp_ildpy() {
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
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
  if(regs.p.m) { op_cmp_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_ildpy() {
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
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
  if(regs.p.m) { op_eor_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
  op_eor_w();
}

void bCPU::op_lda_ildpy() {
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
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
  if(regs.p.m) { op_lda_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
  op_lda_w();
}

void bCPU::op_ora_ildpy() {
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
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
  if(regs.p.m) { op_ora_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_ildpy() {
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
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
  if(regs.p.m) { op_sbc_b(); return; }
l7:
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
  op_sbc_w();
}

void bCPU::op_adc_sr() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  rd.l = op_read(OPMODE_SP, sp);
  if(regs.p.m) { op_adc_b(); return; }
l4:
  rd.h = op_read(OPMODE_SP, sp + 1);
  op_adc_w();
}

void bCPU::op_and_sr() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  rd.l = op_read(OPMODE_SP, sp);
  if(regs.p.m) { op_and_b(); return; }
l4:
  rd.h = op_read(OPMODE_SP, sp + 1);
  op_and_w();
}

void bCPU::op_cmp_sr() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  rd.l = op_read(OPMODE_SP, sp);
  if(regs.p.m) { op_cmp_b(); return; }
l4:
  rd.h = op_read(OPMODE_SP, sp + 1);
  op_cmp_w();
}

void bCPU::op_eor_sr() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  rd.l = op_read(OPMODE_SP, sp);
  if(regs.p.m) { op_eor_b(); return; }
l4:
  rd.h = op_read(OPMODE_SP, sp + 1);
  op_eor_w();
}

void bCPU::op_lda_sr() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  rd.l = op_read(OPMODE_SP, sp);
  if(regs.p.m) { op_lda_b(); return; }
l4:
  rd.h = op_read(OPMODE_SP, sp + 1);
  op_lda_w();
}

void bCPU::op_ora_sr() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  rd.l = op_read(OPMODE_SP, sp);
  if(regs.p.m) { op_ora_b(); return; }
l4:
  rd.h = op_read(OPMODE_SP, sp + 1);
  op_ora_w();
}

void bCPU::op_sbc_sr() {
l1:
  sp = op_read();
l2:
  cpu_io();
l3:
  rd.l = op_read(OPMODE_SP, sp);
  if(regs.p.m) { op_sbc_b(); return; }
l4:
  rd.h = op_read(OPMODE_SP, sp + 1);
  op_sbc_w();
}

void bCPU::op_adc_isry() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_adc_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_adc_w();
}

void bCPU::op_and_isry() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_and_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_and_w();
}

void bCPU::op_cmp_isry() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_cmp_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_cmp_w();
}

void bCPU::op_eor_isry() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_eor_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_eor_w();
}

void bCPU::op_lda_isry() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_lda_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_lda_w();
}

void bCPU::op_ora_isry() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_ora_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_ora_w();
}

void bCPU::op_sbc_isry() {
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
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_sbc_b(); return; }
l7:
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_sbc_w();
}

void bCPU::op_bit_const() {
l1:
  rd.l = op_read();
  if(regs.p.m) {
    regs.p.z = ((rd.l & regs.a.l) == 0);
    return;
  }
l2:
  rd.h = op_read();
  regs.p.z = ((rd.w & regs.a.w) == 0);
}

