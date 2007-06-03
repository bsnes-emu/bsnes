void sCPU::op_adc_const() {
  if(regs.p.m)last_cycle();
  rd.l = op_readpc();
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_adc_w();
}

void sCPU::op_and_const() {
  if(regs.p.m)last_cycle();
  rd.l = op_readpc();
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_and_w();
}

void sCPU::op_cmp_const() {
  if(regs.p.m)last_cycle();
  rd.l = op_readpc();
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_cmp_w();
}

void sCPU::op_cpx_const() {
  if(regs.p.x)last_cycle();
  rd.l = op_readpc();
  if(regs.p.x) { op_cpx_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_cpx_w();
}

void sCPU::op_cpy_const() {
  if(regs.p.x)last_cycle();
  rd.l = op_readpc();
  if(regs.p.x) { op_cpy_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_cpy_w();
}

void sCPU::op_eor_const() {
  if(regs.p.m)last_cycle();
  rd.l = op_readpc();
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_eor_w();
}

void sCPU::op_lda_const() {
  if(regs.p.m)last_cycle();
  rd.l = op_readpc();
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_lda_w();
}

void sCPU::op_ldx_const() {
  if(regs.p.x)last_cycle();
  rd.l = op_readpc();
  if(regs.p.x) { op_ldx_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_ldx_w();
}

void sCPU::op_ldy_const() {
  if(regs.p.x)last_cycle();
  rd.l = op_readpc();
  if(regs.p.x) { op_ldy_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_ldy_w();
}

void sCPU::op_ora_const() {
  if(regs.p.m)last_cycle();
  rd.l = op_readpc();
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_ora_w();
}

void sCPU::op_sbc_const() {
  if(regs.p.m)last_cycle();
  rd.l = op_readpc();
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readpc();
  op_sbc_w();
}

void sCPU::op_adc_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_adc_w();
}

void sCPU::op_and_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_and_w();
}

void sCPU::op_bit_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_bit_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_bit_w();
}

void sCPU::op_cmp_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cmp_w();
}

void sCPU::op_cpx_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.x)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.x) { op_cpx_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cpx_w();
}

void sCPU::op_cpy_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.x)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.x) { op_cpy_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cpy_w();
}

void sCPU::op_eor_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_eor_w();
}

void sCPU::op_lda_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_lda_w();
}

void sCPU::op_ldx_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.x)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.x) { op_ldx_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ldx_w();
}

void sCPU::op_ldy_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.x)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.x) { op_ldy_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ldy_w();
}

void sCPU::op_ora_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_addr() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_sbc_w();
}

void sCPU::op_adc_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_adc_w();
}

void sCPU::op_and_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_and_w();
}

void sCPU::op_bit_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.m) { op_bit_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_bit_w();
}

void sCPU::op_cmp_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_eor_w();
}

void sCPU::op_lda_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_lda_w();
}

void sCPU::op_ldy_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.x)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.x) { op_ldy_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_ldy_w();
}

void sCPU::op_ora_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_addrx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_sbc_w();
}

void sCPU::op_adc_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_adc_w();
}

void sCPU::op_and_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_and_w();
}

void sCPU::op_cmp_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_eor_w();
}

void sCPU::op_lda_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_lda_w();
}

void sCPU::op_ldx_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.x)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.x) { op_ldx_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_ldx_w();
}

void sCPU::op_ora_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_addry() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_sbc_w();
}

void sCPU::op_adc_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_adc_w();
}

void sCPU::op_and_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_and_w();
}

void sCPU::op_cmp_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_cmp_w();
}

void sCPU::op_eor_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_eor_w();
}

void sCPU::op_lda_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_lda_w();
}

void sCPU::op_ora_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_ora_w();
}

void sCPU::op_sbc_long() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_sbc_w();
}

void sCPU::op_adc_longx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_adc_w();
}

void sCPU::op_and_longx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_and_w();
}

void sCPU::op_cmp_longx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_longx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_eor_w();
}

void sCPU::op_lda_longx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_lda_w();
}

void sCPU::op_ora_longx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_longx() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_sbc_w();
}

void sCPU::op_adc_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_adc_w();
}

void sCPU::op_and_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_and_w();
}

void sCPU::op_bit_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.m) { op_bit_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_bit_w();
}

void sCPU::op_cmp_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_cmp_w();
}

void sCPU::op_cpx_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.x)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.x) { op_cpx_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_cpx_w();
}

void sCPU::op_cpy_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.x)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.x) { op_cpy_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_cpy_w();
}

void sCPU::op_eor_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_eor_w();
}

void sCPU::op_lda_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_lda_w();
}

void sCPU::op_ldx_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.x)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.x) { op_ldx_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_ldx_w();
}

void sCPU::op_ldy_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.x)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.x) { op_ldy_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_ldy_w();
}

void sCPU::op_ora_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_ora_w();
}

void sCPU::op_sbc_dp() {
  dp = op_readpc();
  op_io_cond2();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_sbc_w();
}

void sCPU::op_adc_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_adc_w();
}

void sCPU::op_and_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_and_w();
}

void sCPU::op_bit_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.m) { op_bit_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_bit_w();
}

void sCPU::op_cmp_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_eor_w();
}

void sCPU::op_lda_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_lda_w();
}

void sCPU::op_ldy_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.x)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.x) { op_ldy_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_ldy_w();
}

void sCPU::op_ora_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_dpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_sbc_w();
}

void sCPU::op_ldx_dpy() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.p.x)last_cycle();
  rd.l = op_readdp(dp + regs.y.w);
  if(regs.p.x) { op_ldx_b(); return; }
  last_cycle();
  rd.h = op_readdp(dp + regs.y.w + 1);
  op_ldx_w();
}

void sCPU::op_adc_idp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_adc_w();
}

void sCPU::op_and_idp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_and_w();
}

void sCPU::op_cmp_idp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_idp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_eor_w();
}

void sCPU::op_lda_idp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_lda_w();
}

void sCPU::op_ora_idp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_idp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_sbc_w();
}

void sCPU::op_adc_idpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_adc_w();
}

void sCPU::op_and_idpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_and_w();
}

void sCPU::op_cmp_idpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_idpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_eor_w();
}

void sCPU::op_lda_idpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_lda_w();
}

void sCPU::op_ora_idpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_idpx() {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_sbc_w();
}

void sCPU::op_adc_idpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_adc_w();
}

void sCPU::op_and_idpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_and_w();
}

void sCPU::op_cmp_idpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_idpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_eor_w();
}

void sCPU::op_lda_idpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_lda_w();
}

void sCPU::op_ora_idpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_idpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_sbc_w();
}

void sCPU::op_adc_ildp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_adc_w();
}

void sCPU::op_and_ildp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_and_w();
}

void sCPU::op_cmp_ildp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_cmp_w();
}

void sCPU::op_eor_ildp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_eor_w();
}

void sCPU::op_lda_ildp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_lda_w();
}

void sCPU::op_ora_ildp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_ora_w();
}

void sCPU::op_sbc_ildp() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_sbc_w();
}

void sCPU::op_adc_ildpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_adc_w();
}

void sCPU::op_and_ildpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_and_w();
}

void sCPU::op_cmp_ildpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_ildpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_eor_w();
}

void sCPU::op_lda_ildpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_lda_w();
}

void sCPU::op_ora_ildpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_ildpy() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.p.m)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_sbc_w();
}

void sCPU::op_adc_sr() {
  sp = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_adc_w();
}

void sCPU::op_and_sr() {
  sp = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_and_w();
}

void sCPU::op_cmp_sr() {
  sp = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_cmp_w();
}

void sCPU::op_eor_sr() {
  sp = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_eor_w();
}

void sCPU::op_lda_sr() {
  sp = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_lda_w();
}

void sCPU::op_ora_sr() {
  sp = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_ora_w();
}

void sCPU::op_sbc_sr() {
  sp = op_readpc();
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_sbc_w();
}

void sCPU::op_adc_isry() {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_adc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_adc_w();
}

void sCPU::op_and_isry() {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_and_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_and_w();
}

void sCPU::op_cmp_isry() {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_cmp_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_cmp_w();
}

void sCPU::op_eor_isry() {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_eor_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_eor_w();
}

void sCPU::op_lda_isry() {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_lda_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_lda_w();
}

void sCPU::op_ora_isry() {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_ora_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_ora_w();
}

void sCPU::op_sbc_isry() {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.p.m)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.p.m) { op_sbc_b(); return; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_sbc_w();
}

void sCPU::op_bit_const() {
  if(regs.p.m)last_cycle();
  rd.l = op_readpc();
  if(regs.p.m) {
    regs.p.z = ((rd.l & regs.a.l) == 0);
    return;
  }
  last_cycle();
  rd.h = op_readpc();
  regs.p.z = ((rd.w & regs.a.w) == 0);
}

