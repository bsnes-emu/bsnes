//opcode_functions.cpp was generated via bpp -> opcode_functions.bpp
















































































//===============
//opcode_read.bpp
//===============

  void CPUcore::op_adc_const_b() {
last_cycle();rd.l = op_readpc();
    op_adc_b();
  }

  void CPUcore::op_adc_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_adc_w();
  }

  void CPUcore::op_and_const_b() {
last_cycle();rd.l = op_readpc();
    op_and_b();
  }

  void CPUcore::op_and_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_and_w();
  }

  void CPUcore::op_cmp_const_b() {
last_cycle();rd.l = op_readpc();
    op_cmp_b();
  }

  void CPUcore::op_cmp_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_cmp_w();
  }

  void CPUcore::op_cpx_const_b() {
last_cycle();rd.l = op_readpc();
    op_cpx_b();
  }

  void CPUcore::op_cpx_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_cpx_w();
  }

  void CPUcore::op_cpy_const_b() {
last_cycle();rd.l = op_readpc();
    op_cpy_b();
  }

  void CPUcore::op_cpy_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_cpy_w();
  }

  void CPUcore::op_eor_const_b() {
last_cycle();rd.l = op_readpc();
    op_eor_b();
  }

  void CPUcore::op_eor_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_eor_w();
  }

  void CPUcore::op_lda_const_b() {
last_cycle();rd.l = op_readpc();
    op_lda_b();
  }

  void CPUcore::op_lda_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_lda_w();
  }

  void CPUcore::op_ldx_const_b() {
last_cycle();rd.l = op_readpc();
    op_ldx_b();
  }

  void CPUcore::op_ldx_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_ldx_w();
  }

  void CPUcore::op_ldy_const_b() {
last_cycle();rd.l = op_readpc();
    op_ldy_b();
  }

  void CPUcore::op_ldy_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_ldy_w();
  }

  void CPUcore::op_ora_const_b() {
last_cycle();rd.l = op_readpc();
    op_ora_b();
  }

  void CPUcore::op_ora_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_ora_w();
  }

  void CPUcore::op_sbc_const_b() {
last_cycle();rd.l = op_readpc();
    op_sbc_b();
  }

  void CPUcore::op_sbc_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    op_sbc_w();
  }


  void CPUcore::op_bit_const_b() {
last_cycle();rd.l = op_readpc();
    regs.p.z = ((rd.l & regs.a.l) == 0);
  }

  void CPUcore::op_bit_const_w() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    regs.p.z = ((rd.w & regs.a.w) == 0);
  }


  void CPUcore::op_adc_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_adc_b();
  }

  void CPUcore::op_adc_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_and_b();
  }

  void CPUcore::op_and_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_and_w();
  }

  void CPUcore::op_bit_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_bit_b();
  }

  void CPUcore::op_bit_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_bit_w();
  }

  void CPUcore::op_cmp_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_cpx_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_cpx_b();
  }

  void CPUcore::op_cpx_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_cpx_w();
  }

  void CPUcore::op_cpy_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_cpy_b();
  }

  void CPUcore::op_cpy_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_cpy_w();
  }

  void CPUcore::op_eor_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_eor_b();
  }

  void CPUcore::op_eor_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_lda_b();
  }

  void CPUcore::op_lda_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ldx_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_ldx_b();
  }

  void CPUcore::op_ldx_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_ldx_w();
  }

  void CPUcore::op_ldy_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_ldy_b();
  }

  void CPUcore::op_ldy_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_ldy_w();
  }

  void CPUcore::op_ora_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_ora_b();
  }

  void CPUcore::op_ora_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();rd.l = op_readdbr(aa.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_adc_b();
  }

  void CPUcore::op_adc_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_and_b();
  }

  void CPUcore::op_and_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_and_w();
  }

  void CPUcore::op_bit_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_bit_b();
  }

  void CPUcore::op_bit_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_bit_w();
  }

  void CPUcore::op_cmp_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_eor_b();
  }

  void CPUcore::op_eor_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_lda_b();
  }

  void CPUcore::op_lda_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ldy_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_ldy_b();
  }

  void CPUcore::op_ldy_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_ldy_w();
  }

  void CPUcore::op_ora_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_ora_b();
  }

  void CPUcore::op_ora_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.x.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.x.w);
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_adc_b();
  }

  void CPUcore::op_adc_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_and_b();
  }

  void CPUcore::op_and_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_eor_b();
  }

  void CPUcore::op_eor_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_lda_b();
  }

  void CPUcore::op_lda_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ldx_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_ldx_b();
  }

  void CPUcore::op_ldx_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_ldx_w();
  }

  void CPUcore::op_ora_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_ora_b();
  }

  void CPUcore::op_ora_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_long_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d);
    op_adc_b();
  }

  void CPUcore::op_adc_long_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_adc_w();
  }

  void CPUcore::op_and_long_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d);
    op_and_b();
  }

  void CPUcore::op_and_long_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_long_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d);
    op_cmp_b();
  }

  void CPUcore::op_cmp_long_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_long_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d);
    op_eor_b();
  }

  void CPUcore::op_eor_long_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_long_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d);
    op_lda_b();
  }

  void CPUcore::op_lda_long_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_long_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d);
    op_ora_b();
  }

  void CPUcore::op_ora_long_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_long_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d);
    op_sbc_b();
  }

  void CPUcore::op_sbc_long_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_longx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d + regs.x.w);
    op_adc_b();
  }

  void CPUcore::op_adc_longx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + regs.x.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.x.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_longx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d + regs.x.w);
    op_and_b();
  }

  void CPUcore::op_and_longx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + regs.x.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.x.w + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_longx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d + regs.x.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_longx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + regs.x.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.x.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_longx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d + regs.x.w);
    op_eor_b();
  }

  void CPUcore::op_eor_longx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + regs.x.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.x.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_longx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d + regs.x.w);
    op_lda_b();
  }

  void CPUcore::op_lda_longx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + regs.x.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.x.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_longx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d + regs.x.w);
    op_ora_b();
  }

  void CPUcore::op_ora_longx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + regs.x.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.x.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_longx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();rd.l = op_readlong(aa.d + regs.x.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_longx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    rd.l = op_readlong(aa.d + regs.x.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.x.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_adc_b();
  }

  void CPUcore::op_adc_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_adc_w();
  }

  void CPUcore::op_and_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_and_b();
  }

  void CPUcore::op_and_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_and_w();
  }

  void CPUcore::op_bit_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_bit_b();
  }

  void CPUcore::op_bit_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_bit_w();
  }

  void CPUcore::op_cmp_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_cmp_b();
  }

  void CPUcore::op_cmp_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_cmp_w();
  }

  void CPUcore::op_cpx_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_cpx_b();
  }

  void CPUcore::op_cpx_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_cpx_w();
  }

  void CPUcore::op_cpy_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_cpy_b();
  }

  void CPUcore::op_cpy_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_cpy_w();
  }

  void CPUcore::op_eor_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_eor_b();
  }

  void CPUcore::op_eor_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_lda_b();
  }

  void CPUcore::op_lda_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_lda_w();
  }

  void CPUcore::op_ldx_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_ldx_b();
  }

  void CPUcore::op_ldx_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_ldx_w();
  }

  void CPUcore::op_ldy_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_ldy_b();
  }

  void CPUcore::op_ldy_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_ldy_w();
  }

  void CPUcore::op_ora_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_ora_b();
  }

  void CPUcore::op_ora_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();rd.l = op_readdp(dp);
    op_sbc_b();
  }

  void CPUcore::op_sbc_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
last_cycle();rd.h = op_readdp(dp + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_adc_b();
  }

  void CPUcore::op_adc_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_and_b();
  }

  void CPUcore::op_and_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_and_w();
  }

  void CPUcore::op_bit_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_bit_b();
  }

  void CPUcore::op_bit_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_bit_w();
  }

  void CPUcore::op_cmp_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_eor_b();
  }

  void CPUcore::op_eor_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_lda_b();
  }

  void CPUcore::op_lda_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ldx_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.y.w);
    op_ldx_b();
  }

  void CPUcore::op_ldx_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.y.w + 0);
    rd.h = op_readdp(dp + regs.y.w + 1);
    op_ldx_w();
  }

  void CPUcore::op_ldy_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_ldy_b();
  }

  void CPUcore::op_ldy_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_ldy_w();
  }

  void CPUcore::op_ora_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_ora_b();
  }

  void CPUcore::op_ora_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_idp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_adc_b();
  }

  void CPUcore::op_adc_idp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_idp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_and_b();
  }

  void CPUcore::op_and_idp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_idp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_idp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_idp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_eor_b();
  }

  void CPUcore::op_eor_idp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_idp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_lda_b();
  }

  void CPUcore::op_lda_idp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_idp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_ora_b();
  }

  void CPUcore::op_ora_idp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_idp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_idp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_idpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_adc_b();
  }

  void CPUcore::op_adc_idpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_idpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_and_b();
  }

  void CPUcore::op_and_idpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_idpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_idpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_idpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_eor_b();
  }

  void CPUcore::op_eor_idpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_idpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_lda_b();
  }

  void CPUcore::op_lda_idpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_idpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_ora_b();
  }

  void CPUcore::op_ora_idpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_idpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
last_cycle();rd.l = op_readdbr(aa.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_idpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
    rd.l = op_readdbr(aa.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_idpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_adc_b();
  }

  void CPUcore::op_adc_idpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_idpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_and_b();
  }

  void CPUcore::op_and_idpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_idpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_idpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_idpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_eor_b();
  }

  void CPUcore::op_eor_idpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_idpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_lda_b();
  }

  void CPUcore::op_lda_idpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_idpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_ora_b();
  }

  void CPUcore::op_ora_idpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_idpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_idpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io_cond4(aa.w, aa.w + regs.y.w);
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_ildp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d);
    op_adc_b();
  }

  void CPUcore::op_adc_ildp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_adc_w();
  }

  void CPUcore::op_and_ildp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d);
    op_and_b();
  }

  void CPUcore::op_and_ildp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_ildp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d);
    op_cmp_b();
  }

  void CPUcore::op_cmp_ildp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_ildp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d);
    op_eor_b();
  }

  void CPUcore::op_eor_ildp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_ildp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d);
    op_lda_b();
  }

  void CPUcore::op_lda_ildp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_ildp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d);
    op_ora_b();
  }

  void CPUcore::op_ora_ildp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_ildp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d);
    op_sbc_b();
  }

  void CPUcore::op_sbc_ildp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + 0);
last_cycle();rd.h = op_readlong(aa.d + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_ildpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d + regs.y.w);
    op_adc_b();
  }

  void CPUcore::op_adc_ildpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + regs.y.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.y.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_ildpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d + regs.y.w);
    op_and_b();
  }

  void CPUcore::op_and_ildpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + regs.y.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.y.w + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_ildpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d + regs.y.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_ildpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + regs.y.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.y.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_ildpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d + regs.y.w);
    op_eor_b();
  }

  void CPUcore::op_eor_ildpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + regs.y.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.y.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_ildpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d + regs.y.w);
    op_lda_b();
  }

  void CPUcore::op_lda_ildpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + regs.y.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.y.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_ildpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d + regs.y.w);
    op_ora_b();
  }

  void CPUcore::op_ora_ildpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + regs.y.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.y.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_ildpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();rd.l = op_readlong(aa.d + regs.y.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_ildpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    rd.l = op_readlong(aa.d + regs.y.w + 0);
last_cycle();rd.h = op_readlong(aa.d + regs.y.w + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_sr_b() {
    sp = op_readpc();
    op_io();
last_cycle();rd.l = op_readsp(sp);
    op_adc_b();
  }

  void CPUcore::op_adc_sr_w() {
    sp = op_readpc();
    op_io();
    rd.l = op_readsp(sp + 0);
last_cycle();rd.h = op_readsp(sp + 1);
    op_adc_w();
  }

  void CPUcore::op_and_sr_b() {
    sp = op_readpc();
    op_io();
last_cycle();rd.l = op_readsp(sp);
    op_and_b();
  }

  void CPUcore::op_and_sr_w() {
    sp = op_readpc();
    op_io();
    rd.l = op_readsp(sp + 0);
last_cycle();rd.h = op_readsp(sp + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_sr_b() {
    sp = op_readpc();
    op_io();
last_cycle();rd.l = op_readsp(sp);
    op_cmp_b();
  }

  void CPUcore::op_cmp_sr_w() {
    sp = op_readpc();
    op_io();
    rd.l = op_readsp(sp + 0);
last_cycle();rd.h = op_readsp(sp + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_sr_b() {
    sp = op_readpc();
    op_io();
last_cycle();rd.l = op_readsp(sp);
    op_eor_b();
  }

  void CPUcore::op_eor_sr_w() {
    sp = op_readpc();
    op_io();
    rd.l = op_readsp(sp + 0);
last_cycle();rd.h = op_readsp(sp + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_sr_b() {
    sp = op_readpc();
    op_io();
last_cycle();rd.l = op_readsp(sp);
    op_lda_b();
  }

  void CPUcore::op_lda_sr_w() {
    sp = op_readpc();
    op_io();
    rd.l = op_readsp(sp + 0);
last_cycle();rd.h = op_readsp(sp + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_sr_b() {
    sp = op_readpc();
    op_io();
last_cycle();rd.l = op_readsp(sp);
    op_ora_b();
  }

  void CPUcore::op_ora_sr_w() {
    sp = op_readpc();
    op_io();
    rd.l = op_readsp(sp + 0);
last_cycle();rd.h = op_readsp(sp + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_sr_b() {
    sp = op_readpc();
    op_io();
last_cycle();rd.l = op_readsp(sp);
    op_sbc_b();
  }

  void CPUcore::op_sbc_sr_w() {
    sp = op_readpc();
    op_io();
    rd.l = op_readsp(sp + 0);
last_cycle();rd.h = op_readsp(sp + 1);
    op_sbc_w();
  }


  void CPUcore::op_adc_isry_b() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_adc_b();
  }

  void CPUcore::op_adc_isry_w() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_adc_w();
  }

  void CPUcore::op_and_isry_b() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_and_b();
  }

  void CPUcore::op_and_isry_w() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_and_w();
  }

  void CPUcore::op_cmp_isry_b() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_cmp_b();
  }

  void CPUcore::op_cmp_isry_w() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_cmp_w();
  }

  void CPUcore::op_eor_isry_b() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_eor_b();
  }

  void CPUcore::op_eor_isry_w() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_eor_w();
  }

  void CPUcore::op_lda_isry_b() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_lda_b();
  }

  void CPUcore::op_lda_isry_w() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_lda_w();
  }

  void CPUcore::op_ora_isry_b() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_ora_b();
  }

  void CPUcore::op_ora_isry_w() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_ora_w();
  }

  void CPUcore::op_sbc_isry_b() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
last_cycle();rd.l = op_readdbr(aa.w + regs.y.w);
    op_sbc_b();
  }

  void CPUcore::op_sbc_isry_w() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
    rd.l = op_readdbr(aa.w + regs.y.w + 0);
last_cycle();rd.h = op_readdbr(aa.w + regs.y.w + 1);
    op_sbc_w();
  }


//================
//opcode_write.bpp
//================

  void CPUcore::op_sta_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();op_writedbr(aa.w, regs.a.w);
  }

  void CPUcore::op_sta_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writedbr(aa.w + 0, regs.a.w >> 0);
last_cycle();op_writedbr(aa.w + 1, regs.a.w >> 8);
  }

  void CPUcore::op_stx_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();op_writedbr(aa.w, regs.x.w);
  }

  void CPUcore::op_stx_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writedbr(aa.w + 0, regs.x.w >> 0);
last_cycle();op_writedbr(aa.w + 1, regs.x.w >> 8);
  }

  void CPUcore::op_sty_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();op_writedbr(aa.w, regs.y.w);
  }

  void CPUcore::op_sty_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writedbr(aa.w + 0, regs.y.w >> 0);
last_cycle();op_writedbr(aa.w + 1, regs.y.w >> 8);
  }

  void CPUcore::op_stz_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
last_cycle();op_writedbr(aa.w, 0x0000);
  }

  void CPUcore::op_stz_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writedbr(aa.w + 0, 0x0000 >> 0);
last_cycle();op_writedbr(aa.w + 1, 0x0000 >> 8);
  }


  void CPUcore::op_sta_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
last_cycle();op_writedbr(aa.w + regs.x.w, regs.a.w);
  }

  void CPUcore::op_sta_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    op_writedbr(aa.w + regs.x.w + 0, regs.a.w >> 0);
last_cycle();op_writedbr(aa.w + regs.x.w + 1, regs.a.w >> 8);
  }

  void CPUcore::op_sta_addry_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
last_cycle();op_writedbr(aa.w + regs.y.w, regs.a.w);
  }

  void CPUcore::op_sta_addry_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    op_writedbr(aa.w + regs.y.w + 0, regs.a.w >> 0);
last_cycle();op_writedbr(aa.w + regs.y.w + 1, regs.a.w >> 8);
  }

  void CPUcore::op_stz_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
last_cycle();op_writedbr(aa.w + regs.x.w, 0x0000);
  }

  void CPUcore::op_stz_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    op_writedbr(aa.w + regs.x.w + 0, 0x0000 >> 0);
last_cycle();op_writedbr(aa.w + regs.x.w + 1, 0x0000 >> 8);
  }


  void CPUcore::op_sta_long_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();op_writelong(aa.d + 0x0000, regs.a.l);
  }

  void CPUcore::op_sta_long_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    op_writelong(aa.d + 0x0000 + 0, regs.a.l);
last_cycle();op_writelong(aa.d + 0x0000 + 1, regs.a.h);
  }

  void CPUcore::op_sta_longx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
last_cycle();op_writelong(aa.d + regs.x.w, regs.a.l);
  }

  void CPUcore::op_sta_longx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    aa.b = op_readpc();
    op_writelong(aa.d + regs.x.w + 0, regs.a.l);
last_cycle();op_writelong(aa.d + regs.x.w + 1, regs.a.h);
  }


  void CPUcore::op_sta_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();op_writedp(dp, regs.a.w);
  }

  void CPUcore::op_sta_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    op_writedp(dp + 0, regs.a.w >> 0);
last_cycle();op_writedp(dp + 1, regs.a.w >> 8);
  }

  void CPUcore::op_stx_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();op_writedp(dp, regs.x.w);
  }

  void CPUcore::op_stx_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    op_writedp(dp + 0, regs.x.w >> 0);
last_cycle();op_writedp(dp + 1, regs.x.w >> 8);
  }

  void CPUcore::op_sty_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();op_writedp(dp, regs.y.w);
  }

  void CPUcore::op_sty_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    op_writedp(dp + 0, regs.y.w >> 0);
last_cycle();op_writedp(dp + 1, regs.y.w >> 8);
  }

  void CPUcore::op_stz_dp_b() {
    dp = op_readpc();
    op_io_cond2();
last_cycle();op_writedp(dp, 0x0000);
  }

  void CPUcore::op_stz_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    op_writedp(dp + 0, 0x0000 >> 0);
last_cycle();op_writedp(dp + 1, 0x0000 >> 8);
  }


  void CPUcore::op_sta_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();op_writedp(dp + regs.x.w, regs.a.w);
  }

  void CPUcore::op_sta_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    op_writedp(dp + regs.x.w + 0, regs.a.w >> 0);
last_cycle();op_writedp(dp + regs.x.w + 1, regs.a.w >> 8);
  }

  void CPUcore::op_stx_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();op_writedp(dp + regs.y.w, regs.x.w);
  }

  void CPUcore::op_stx_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    op_writedp(dp + regs.y.w + 0, regs.x.w >> 0);
last_cycle();op_writedp(dp + regs.y.w + 1, regs.x.w >> 8);
  }

  void CPUcore::op_sty_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();op_writedp(dp + regs.x.w, regs.y.w);
  }

  void CPUcore::op_sty_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    op_writedp(dp + regs.x.w + 0, regs.y.w >> 0);
last_cycle();op_writedp(dp + regs.x.w + 1, regs.y.w >> 8);
  }

  void CPUcore::op_stz_dpr_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
last_cycle();op_writedp(dp + regs.x.w, 0x0000);
  }

  void CPUcore::op_stz_dpr_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    op_writedp(dp + regs.x.w + 0, 0x0000 >> 0);
last_cycle();op_writedp(dp + regs.x.w + 1, 0x0000 >> 8);
  }


  void CPUcore::op_sta_idp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
last_cycle();op_writedbr(aa.w, regs.a.l);
  }

  void CPUcore::op_sta_idp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_writedbr(aa.w + 0, regs.a.l);
last_cycle();op_writedbr(aa.w + 1, regs.a.h);
  }

  void CPUcore::op_sta_ildp_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();op_writelong(aa.d, regs.a.l);
  }

  void CPUcore::op_sta_ildp_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    op_writelong(aa.d + 0, regs.a.l);
last_cycle();op_writelong(aa.d + 1, regs.a.h);
  }

  void CPUcore::op_sta_idpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
last_cycle();op_writedbr(aa.w, regs.a.l);
  }

  void CPUcore::op_sta_idpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    aa.l = op_readdp(dp + regs.x.w + 0);
    aa.h = op_readdp(dp + regs.x.w + 1);
    op_writedbr(aa.w + 0, regs.a.l);
last_cycle();op_writedbr(aa.w + 1, regs.a.h);
  }

  void CPUcore::op_sta_idpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io();
last_cycle();op_writedbr(aa.w + regs.y.w, regs.a.l);
  }

  void CPUcore::op_sta_idpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_io();
    op_writedbr(aa.w + regs.y.w + 0, regs.a.l);
last_cycle();op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
  }

  void CPUcore::op_sta_ildpy_b() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
last_cycle();op_writelong(aa.d + regs.y.w, regs.a.l);
  }

  void CPUcore::op_sta_ildpy_w() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    aa.b = op_readdp(dp + 2);
    op_writelong(aa.d + regs.y.w + 0, regs.a.l);
last_cycle();op_writelong(aa.d + regs.y.w + 1, regs.a.h);
  }

  void CPUcore::op_sta_sr_b() {
    sp = op_readpc();
    op_io();
last_cycle();op_writesp(sp, regs.a.l);
  }

  void CPUcore::op_sta_sr_w() {
    sp = op_readpc();
    op_io();
    op_writesp(sp + 0, regs.a.l);
last_cycle();op_writesp(sp + 1, regs.a.h);
  }

  void CPUcore::op_sta_isry_b() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
last_cycle();op_writedbr(aa.w + regs.y.w, regs.a.l);
  }

  void CPUcore::op_sta_isry_w() {
    sp = op_readpc();
    op_io();
    aa.l = op_readsp(sp + 0);
    aa.h = op_readsp(sp + 1);
    op_io();
    op_writedbr(aa.w + regs.y.w + 0, regs.a.l);
last_cycle();op_writedbr(aa.w + regs.y.w + 1, regs.a.h);
  }


//==============
//opcode_rmw.bpp
//==============

  void CPUcore::op_inc_imm_b() {
last_cycle();op_io_irq();
    regs.a.l ++;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_inc_imm_w() {
last_cycle();op_io_irq();
    regs.a.w ++;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_inx_imm_b() {
last_cycle();op_io_irq();
    regs.x.l ++;
    regs.p.n = (regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  }

  void CPUcore::op_inx_imm_w() {
last_cycle();op_io_irq();
    regs.x.w ++;
    regs.p.n = (regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }

  void CPUcore::op_iny_imm_b() {
last_cycle();op_io_irq();
    regs.y.l ++;
    regs.p.n = (regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  }

  void CPUcore::op_iny_imm_w() {
last_cycle();op_io_irq();
    regs.y.w ++;
    regs.p.n = (regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }

  void CPUcore::op_dec_imm_b() {
last_cycle();op_io_irq();
    regs.a.l --;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_dec_imm_w() {
last_cycle();op_io_irq();
    regs.a.w --;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_dex_imm_b() {
last_cycle();op_io_irq();
    regs.x.l --;
    regs.p.n = (regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  }

  void CPUcore::op_dex_imm_w() {
last_cycle();op_io_irq();
    regs.x.w --;
    regs.p.n = (regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }

  void CPUcore::op_dey_imm_b() {
last_cycle();op_io_irq();
    regs.y.l --;
    regs.p.n = (regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  }

  void CPUcore::op_dey_imm_w() {
last_cycle();op_io_irq();
    regs.y.w --;
    regs.p.n = (regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }


  void CPUcore::op_asl_imm_b() {
last_cycle();op_io_irq();
    regs.p.c = (regs.a.l & 0x80);
    regs.a.l <<= 1;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_asl_imm_w() {
last_cycle();op_io_irq();
    regs.p.c = (regs.a.w & 0x8000);
    regs.a.w <<= 1;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_lsr_imm_b() {
last_cycle();op_io_irq();
    regs.p.c = (regs.a.l & 0x01);
    regs.a.l >>= 1;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_lsr_imm_w() {
last_cycle();op_io_irq();
    regs.p.c = (regs.a.w & 0x0001);
    regs.a.w >>= 1;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_rol_imm_b() {
last_cycle();op_io_irq();
    bool carry = regs.p.c;
    regs.p.c = (regs.a.l & 0x80);
    regs.a.l = (regs.a.l << 1) | carry;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_rol_imm_w() {
last_cycle();op_io_irq();
    bool carry = regs.p.c;
    regs.p.c = (regs.a.w & 0x8000);
    regs.a.w = (regs.a.w << 1) | carry;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_ror_imm_b() {
last_cycle();op_io_irq();
    bool carry = regs.p.c;
    regs.p.c = (regs.a.l & 0x01);
    regs.a.l = (carry << 7) | (regs.a.l >> 1);
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_ror_imm_w() {
last_cycle();op_io_irq();
    bool carry = regs.p.c;
    regs.p.c = (regs.a.w & 0x0001);
    regs.a.w = (carry << 15) | (regs.a.w >> 1);
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }


  void CPUcore::op_inc_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w);
    op_io();
    op_inc_b();
last_cycle();op_writedbr(aa.w, rd.l);
  }

  void CPUcore::op_inc_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
    rd.h = op_readdbr(aa.w + 1);
    op_io();
    op_inc_w();
    op_writedbr(aa.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + 0, rd.l);
  }

  void CPUcore::op_dec_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w);
    op_io();
    op_dec_b();
last_cycle();op_writedbr(aa.w, rd.l);
  }

  void CPUcore::op_dec_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
    rd.h = op_readdbr(aa.w + 1);
    op_io();
    op_dec_w();
    op_writedbr(aa.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + 0, rd.l);
  }

  void CPUcore::op_asl_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w);
    op_io();
    op_asl_b();
last_cycle();op_writedbr(aa.w, rd.l);
  }

  void CPUcore::op_asl_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
    rd.h = op_readdbr(aa.w + 1);
    op_io();
    op_asl_w();
    op_writedbr(aa.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + 0, rd.l);
  }

  void CPUcore::op_lsr_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w);
    op_io();
    op_lsr_b();
last_cycle();op_writedbr(aa.w, rd.l);
  }

  void CPUcore::op_lsr_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
    rd.h = op_readdbr(aa.w + 1);
    op_io();
    op_lsr_w();
    op_writedbr(aa.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + 0, rd.l);
  }

  void CPUcore::op_rol_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w);
    op_io();
    op_rol_b();
last_cycle();op_writedbr(aa.w, rd.l);
  }

  void CPUcore::op_rol_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
    rd.h = op_readdbr(aa.w + 1);
    op_io();
    op_rol_w();
    op_writedbr(aa.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + 0, rd.l);
  }

  void CPUcore::op_ror_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w);
    op_io();
    op_ror_b();
last_cycle();op_writedbr(aa.w, rd.l);
  }

  void CPUcore::op_ror_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
    rd.h = op_readdbr(aa.w + 1);
    op_io();
    op_ror_w();
    op_writedbr(aa.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + 0, rd.l);
  }

  void CPUcore::op_trb_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w);
    op_io();
    op_trb_b();
last_cycle();op_writedbr(aa.w, rd.l);
  }

  void CPUcore::op_trb_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
    rd.h = op_readdbr(aa.w + 1);
    op_io();
    op_trb_w();
    op_writedbr(aa.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + 0, rd.l);
  }

  void CPUcore::op_tsb_addr_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w);
    op_io();
    op_tsb_b();
last_cycle();op_writedbr(aa.w, rd.l);
  }

  void CPUcore::op_tsb_addr_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readdbr(aa.w + 0);
    rd.h = op_readdbr(aa.w + 1);
    op_io();
    op_tsb_w();
    op_writedbr(aa.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + 0, rd.l);
  }


  void CPUcore::op_inc_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w);
    op_io();
    op_inc_b();
last_cycle();op_writedbr(aa.w + regs.x.w, rd.l);
  }

  void CPUcore::op_inc_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
    rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_io();
    op_inc_w();
    op_writedbr(aa.w + regs.x.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_dec_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w);
    op_io();
    op_dec_b();
last_cycle();op_writedbr(aa.w + regs.x.w, rd.l);
  }

  void CPUcore::op_dec_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
    rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_io();
    op_dec_w();
    op_writedbr(aa.w + regs.x.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_asl_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w);
    op_io();
    op_asl_b();
last_cycle();op_writedbr(aa.w + regs.x.w, rd.l);
  }

  void CPUcore::op_asl_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
    rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_io();
    op_asl_w();
    op_writedbr(aa.w + regs.x.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_lsr_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w);
    op_io();
    op_lsr_b();
last_cycle();op_writedbr(aa.w + regs.x.w, rd.l);
  }

  void CPUcore::op_lsr_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
    rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_io();
    op_lsr_w();
    op_writedbr(aa.w + regs.x.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_rol_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w);
    op_io();
    op_rol_b();
last_cycle();op_writedbr(aa.w + regs.x.w, rd.l);
  }

  void CPUcore::op_rol_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
    rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_io();
    op_rol_w();
    op_writedbr(aa.w + regs.x.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_ror_addrx_b() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w);
    op_io();
    op_ror_b();
last_cycle();op_writedbr(aa.w + regs.x.w, rd.l);
  }

  void CPUcore::op_ror_addrx_w() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readdbr(aa.w + regs.x.w + 0);
    rd.h = op_readdbr(aa.w + regs.x.w + 1);
    op_io();
    op_ror_w();
    op_writedbr(aa.w + regs.x.w + 1, rd.h);
last_cycle();op_writedbr(aa.w + regs.x.w + 0, rd.l);
  }


  void CPUcore::op_inc_dp_b() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp);
    op_io();
    op_inc_b();
last_cycle();op_writedp(dp, rd.l);
  }

  void CPUcore::op_inc_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
    rd.h = op_readdp(dp + 1);
    op_io();
    op_inc_w();
    op_writedp(dp + 1, rd.h);
last_cycle();op_writedp(dp + 0, rd.l);
  }

  void CPUcore::op_dec_dp_b() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp);
    op_io();
    op_dec_b();
last_cycle();op_writedp(dp, rd.l);
  }

  void CPUcore::op_dec_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
    rd.h = op_readdp(dp + 1);
    op_io();
    op_dec_w();
    op_writedp(dp + 1, rd.h);
last_cycle();op_writedp(dp + 0, rd.l);
  }

  void CPUcore::op_asl_dp_b() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp);
    op_io();
    op_asl_b();
last_cycle();op_writedp(dp, rd.l);
  }

  void CPUcore::op_asl_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
    rd.h = op_readdp(dp + 1);
    op_io();
    op_asl_w();
    op_writedp(dp + 1, rd.h);
last_cycle();op_writedp(dp + 0, rd.l);
  }

  void CPUcore::op_lsr_dp_b() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp);
    op_io();
    op_lsr_b();
last_cycle();op_writedp(dp, rd.l);
  }

  void CPUcore::op_lsr_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
    rd.h = op_readdp(dp + 1);
    op_io();
    op_lsr_w();
    op_writedp(dp + 1, rd.h);
last_cycle();op_writedp(dp + 0, rd.l);
  }

  void CPUcore::op_rol_dp_b() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp);
    op_io();
    op_rol_b();
last_cycle();op_writedp(dp, rd.l);
  }

  void CPUcore::op_rol_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
    rd.h = op_readdp(dp + 1);
    op_io();
    op_rol_w();
    op_writedp(dp + 1, rd.h);
last_cycle();op_writedp(dp + 0, rd.l);
  }

  void CPUcore::op_ror_dp_b() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp);
    op_io();
    op_ror_b();
last_cycle();op_writedp(dp, rd.l);
  }

  void CPUcore::op_ror_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
    rd.h = op_readdp(dp + 1);
    op_io();
    op_ror_w();
    op_writedp(dp + 1, rd.h);
last_cycle();op_writedp(dp + 0, rd.l);
  }

  void CPUcore::op_trb_dp_b() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp);
    op_io();
    op_trb_b();
last_cycle();op_writedp(dp, rd.l);
  }

  void CPUcore::op_trb_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
    rd.h = op_readdp(dp + 1);
    op_io();
    op_trb_w();
    op_writedp(dp + 1, rd.h);
last_cycle();op_writedp(dp + 0, rd.l);
  }

  void CPUcore::op_tsb_dp_b() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp);
    op_io();
    op_tsb_b();
last_cycle();op_writedp(dp, rd.l);
  }

  void CPUcore::op_tsb_dp_w() {
    dp = op_readpc();
    op_io_cond2();
    rd.l = op_readdp(dp + 0);
    rd.h = op_readdp(dp + 1);
    op_io();
    op_tsb_w();
    op_writedp(dp + 1, rd.h);
last_cycle();op_writedp(dp + 0, rd.l);
  }


  void CPUcore::op_inc_dpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w);
    op_io();
    op_inc_b();
last_cycle();op_writedp(dp + regs.x.w, rd.l);
  }

  void CPUcore::op_inc_dpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_io();
    op_inc_w();
    op_writedp(dp + regs.x.w + 1, rd.h);
last_cycle();op_writedp(dp + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_dec_dpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w);
    op_io();
    op_dec_b();
last_cycle();op_writedp(dp + regs.x.w, rd.l);
  }

  void CPUcore::op_dec_dpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_io();
    op_dec_w();
    op_writedp(dp + regs.x.w + 1, rd.h);
last_cycle();op_writedp(dp + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_asl_dpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w);
    op_io();
    op_asl_b();
last_cycle();op_writedp(dp + regs.x.w, rd.l);
  }

  void CPUcore::op_asl_dpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_io();
    op_asl_w();
    op_writedp(dp + regs.x.w + 1, rd.h);
last_cycle();op_writedp(dp + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_lsr_dpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w);
    op_io();
    op_lsr_b();
last_cycle();op_writedp(dp + regs.x.w, rd.l);
  }

  void CPUcore::op_lsr_dpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_io();
    op_lsr_w();
    op_writedp(dp + regs.x.w + 1, rd.h);
last_cycle();op_writedp(dp + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_rol_dpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w);
    op_io();
    op_rol_b();
last_cycle();op_writedp(dp + regs.x.w, rd.l);
  }

  void CPUcore::op_rol_dpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_io();
    op_rol_w();
    op_writedp(dp + regs.x.w + 1, rd.h);
last_cycle();op_writedp(dp + regs.x.w + 0, rd.l);
  }

  void CPUcore::op_ror_dpx_b() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w);
    op_io();
    op_ror_b();
last_cycle();op_writedp(dp + regs.x.w, rd.l);
  }

  void CPUcore::op_ror_dpx_w() {
    dp = op_readpc();
    op_io_cond2();
    op_io();
    rd.l = op_readdp(dp + regs.x.w + 0);
    rd.h = op_readdp(dp + regs.x.w + 1);
    op_io();
    op_ror_w();
    op_writedp(dp + regs.x.w + 1, rd.h);
last_cycle();op_writedp(dp + regs.x.w + 0, rd.l);
  }


//=============
//opcode_pc.bpp
//=============

  void CPUcore::op_bcc() {
    if(!regs.p.c == false) {
last_cycle();  rd.l = op_readpc();
    } else {
      rd.l = op_readpc();
      aa.w = regs.pc.d + (int8_t)rd.l;
      op_io_cond6(aa.w);
last_cycle();  op_io();
      regs.pc.w = aa.w;
    }
  }

  void CPUcore::op_bcs() {
    if(regs.p.c == false) {
last_cycle();  rd.l = op_readpc();
    } else {
      rd.l = op_readpc();
      aa.w = regs.pc.d + (int8_t)rd.l;
      op_io_cond6(aa.w);
last_cycle();  op_io();
      regs.pc.w = aa.w;
    }
  }

  void CPUcore::op_bne() {
    if(!regs.p.z == false) {
last_cycle();  rd.l = op_readpc();
    } else {
      rd.l = op_readpc();
      aa.w = regs.pc.d + (int8_t)rd.l;
      op_io_cond6(aa.w);
last_cycle();  op_io();
      regs.pc.w = aa.w;
    }
  }

  void CPUcore::op_beq() {
    if(regs.p.z == false) {
last_cycle();  rd.l = op_readpc();
    } else {
      rd.l = op_readpc();
      aa.w = regs.pc.d + (int8_t)rd.l;
      op_io_cond6(aa.w);
last_cycle();  op_io();
      regs.pc.w = aa.w;
    }
  }

  void CPUcore::op_bpl() {
    if(!regs.p.n == false) {
last_cycle();  rd.l = op_readpc();
    } else {
      rd.l = op_readpc();
      aa.w = regs.pc.d + (int8_t)rd.l;
      op_io_cond6(aa.w);
last_cycle();  op_io();
      regs.pc.w = aa.w;
    }
  }

  void CPUcore::op_bmi() {
    if(regs.p.n == false) {
last_cycle();  rd.l = op_readpc();
    } else {
      rd.l = op_readpc();
      aa.w = regs.pc.d + (int8_t)rd.l;
      op_io_cond6(aa.w);
last_cycle();  op_io();
      regs.pc.w = aa.w;
    }
  }

  void CPUcore::op_bvc() {
    if(!regs.p.v == false) {
last_cycle();  rd.l = op_readpc();
    } else {
      rd.l = op_readpc();
      aa.w = regs.pc.d + (int8_t)rd.l;
      op_io_cond6(aa.w);
last_cycle();  op_io();
      regs.pc.w = aa.w;
    }
  }

  void CPUcore::op_bvs() {
    if(regs.p.v == false) {
last_cycle();  rd.l = op_readpc();
    } else {
      rd.l = op_readpc();
      aa.w = regs.pc.d + (int8_t)rd.l;
      op_io_cond6(aa.w);
last_cycle();  op_io();
      regs.pc.w = aa.w;
    }
  }


  void CPUcore::op_bra() {
    rd.l = op_readpc();
    aa.w = regs.pc.d + (int8_t)rd.l;
    op_io_cond6(aa.w);
last_cycle();op_io();
    regs.pc.w = aa.w;
  }

  void CPUcore::op_brl() {
    rd.l = op_readpc();
    rd.h = op_readpc();
last_cycle();op_io();
    regs.pc.w = regs.pc.d + (int16_t)rd.w;
  }

  void CPUcore::op_jmp_addr() {
    rd.l = op_readpc();
last_cycle();rd.h = op_readpc();
    regs.pc.w = rd.w;
  }

  void CPUcore::op_jmp_long() {
    rd.l = op_readpc();
    rd.h = op_readpc();
last_cycle();rd.b = op_readpc();
    regs.pc.d = rd.d & 0xffffff;
  }

  void CPUcore::op_jmp_iaddr() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readaddr(aa.w + 0);
last_cycle();rd.h = op_readaddr(aa.w + 1);
    regs.pc.w = rd.w;
  }

  void CPUcore::op_jmp_iaddrx() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.l = op_readpbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readpbr(aa.w + regs.x.w + 1);
    regs.pc.w = rd.w;
  }

  void CPUcore::op_jmp_iladdr() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    rd.l = op_readaddr(aa.w + 0);
    rd.h = op_readaddr(aa.w + 1);
last_cycle();rd.b = op_readaddr(aa.w + 2);
    regs.pc.d = rd.d & 0xffffff;
  }

  void CPUcore::op_jsr_addr() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    regs.pc.w--;
    op_writestack(regs.pc.h);
last_cycle();op_writestack(regs.pc.l);
    regs.pc.w = aa.w;
  }

  void CPUcore::op_jsr_long_e() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writestackn(regs.pc.b);
    op_io();
    aa.b = op_readpc();
    regs.pc.w--;
    op_writestackn(regs.pc.h);
last_cycle();op_writestackn(regs.pc.l);
    regs.pc.d = aa.d & 0xffffff;
    regs.s.h = 0x01;
  }

  void CPUcore::op_jsr_long_n() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writestackn(regs.pc.b);
    op_io();
    aa.b = op_readpc();
    regs.pc.w--;
    op_writestackn(regs.pc.h);
last_cycle();op_writestackn(regs.pc.l);
    regs.pc.d = aa.d & 0xffffff;
  }

  void CPUcore::op_jsr_iaddrx_e() {
    aa.l = op_readpc();
    op_writestackn(regs.pc.h);
    op_writestackn(regs.pc.l);
    aa.h = op_readpc();
    op_io();
    rd.l = op_readpbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readpbr(aa.w + regs.x.w + 1);
    regs.pc.w = rd.w;
    regs.s.h = 0x01;
  }

  void CPUcore::op_jsr_iaddrx_n() {
    aa.l = op_readpc();
    op_writestackn(regs.pc.h);
    op_writestackn(regs.pc.l);
    aa.h = op_readpc();
    op_io();
    rd.l = op_readpbr(aa.w + regs.x.w + 0);
last_cycle();rd.h = op_readpbr(aa.w + regs.x.w + 1);
    regs.pc.w = rd.w;
  }

  void CPUcore::op_rti_e() {
    op_io();
    op_io();
    regs.p = op_readstack() | 0x30;
    rd.l = op_readstack();
last_cycle();rd.h = op_readstack();
    regs.pc.w = rd.w;
  }

  void CPUcore::op_rti_n() {
    op_io();
    op_io();
    regs.p = op_readstack();
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    rd.l = op_readstack();
    rd.h = op_readstack();
last_cycle();rd.b = op_readstack();
    regs.pc.d = rd.d & 0xffffff;
    update_table();
  }

  void CPUcore::op_rts() {
    op_io();
    op_io();
    rd.l = op_readstack();
    rd.h = op_readstack();
last_cycle();op_io();
    regs.pc.w = ++rd.w;
  }

  void CPUcore::op_rtl_e() {
    op_io();
    op_io();
    rd.l = op_readstackn();
    rd.h = op_readstackn();
last_cycle();rd.b = op_readstackn();
    regs.pc.b = rd.b;
    regs.pc.w = ++rd.w;
    regs.s.h = 0x01;
  }

  void CPUcore::op_rtl_n() {
    op_io();
    op_io();
    rd.l = op_readstackn();
    rd.h = op_readstackn();
last_cycle();rd.b = op_readstackn();
    regs.pc.b = rd.b;
    regs.pc.w = ++rd.w;
  }


//===============
//opcode_misc.bpp
//===============

  void CPUcore::op_nop() {
last_cycle();op_io_irq();
  }

  void CPUcore::op_wdm() {
last_cycle();op_readpc();
  }

  void CPUcore::op_xba() {
    op_io();
last_cycle();op_io();
    regs.a.l ^= regs.a.h;
    regs.a.h ^= regs.a.l;
    regs.a.l ^= regs.a.h;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }


  void CPUcore::op_mvn_b() {
    dp = op_readpc();
    sp = op_readpc();
    regs.db = dp;
    rd.l = op_readlong((sp << 16) | regs.x.w);
    op_writelong((dp << 16) | regs.y.w, rd.l);
    op_io();
    regs.x.l ++;
    regs.y.l ++;
last_cycle();op_io();
    if(regs.a.w--) regs.pc.w -= 3;
  }

  void CPUcore::op_mvn_w() {
    dp = op_readpc();
    sp = op_readpc();
    regs.db = dp;
    rd.l = op_readlong((sp << 16) | regs.x.w);
    op_writelong((dp << 16) | regs.y.w, rd.l);
    op_io();
    regs.x.w ++;
    regs.y.w ++;
last_cycle();op_io();
    if(regs.a.w--) regs.pc.w -= 3;
  }

  void CPUcore::op_mvp_b() {
    dp = op_readpc();
    sp = op_readpc();
    regs.db = dp;
    rd.l = op_readlong((sp << 16) | regs.x.w);
    op_writelong((dp << 16) | regs.y.w, rd.l);
    op_io();
    regs.x.l --;
    regs.y.l --;
last_cycle();op_io();
    if(regs.a.w--) regs.pc.w -= 3;
  }

  void CPUcore::op_mvp_w() {
    dp = op_readpc();
    sp = op_readpc();
    regs.db = dp;
    rd.l = op_readlong((sp << 16) | regs.x.w);
    op_writelong((dp << 16) | regs.y.w, rd.l);
    op_io();
    regs.x.w --;
    regs.y.w --;
last_cycle();op_io();
    if(regs.a.w--) regs.pc.w -= 3;
  }


  void CPUcore::op_brk_e() {
    op_readpc();
    op_writestack(regs.pc.h);
    op_writestack(regs.pc.l);
    op_writestack(regs.p);
    rd.l = op_readlong(0xfffe + 0);
    regs.pc.b = 0;
    regs.p.i = 1;
    regs.p.d = 0;
last_cycle();rd.h = op_readlong(0xfffe + 1);
    regs.pc.w = rd.w;
  }

  void CPUcore::op_brk_n() {
    op_readpc();
    op_writestack(regs.pc.b);
    op_writestack(regs.pc.h);
    op_writestack(regs.pc.l);
    op_writestack(regs.p);
    rd.l = op_readlong(0xffe6 + 0);
    regs.pc.b = 0x00;
    regs.p.i = 1;
    regs.p.d = 0;
last_cycle();rd.h = op_readlong(0xffe6 + 1);
    regs.pc.w = rd.w;
  }

  void CPUcore::op_cop_e() {
    op_readpc();
    op_writestack(regs.pc.h);
    op_writestack(regs.pc.l);
    op_writestack(regs.p);
    rd.l = op_readlong(0xfff4 + 0);
    regs.pc.b = 0;
    regs.p.i = 1;
    regs.p.d = 0;
last_cycle();rd.h = op_readlong(0xfff4 + 1);
    regs.pc.w = rd.w;
  }

  void CPUcore::op_cop_n() {
    op_readpc();
    op_writestack(regs.pc.b);
    op_writestack(regs.pc.h);
    op_writestack(regs.pc.l);
    op_writestack(regs.p);
    rd.l = op_readlong(0xffe4 + 0);
    regs.pc.b = 0x00;
    regs.p.i = 1;
    regs.p.d = 0;
last_cycle();rd.h = op_readlong(0xffe4 + 1);
    regs.pc.w = rd.w;
  }


  void CPUcore::op_stp() {
    while(regs.wai = true) {
last_cycle();  op_io();
    }
  }

  void CPUcore::op_wai() {
    regs.wai = true;
    while(regs.wai) {
last_cycle();  op_io();
    }
    op_io();
  }

  void CPUcore::op_xce() {
last_cycle();op_io_irq();
    bool carry = regs.p.c;
    regs.p.c = regs.e;
    regs.e = carry;
    if(regs.e) {
      regs.p |= 0x30;
      regs.s.h = 0x01;
    }
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    update_table();
  }


  void CPUcore::op_clc() {
last_cycle();op_io_irq();
    regs.p.c=0;
  }

  void CPUcore::op_cld() {
last_cycle();op_io_irq();
    regs.p.d=0;
  }

  void CPUcore::op_cli() {
last_cycle();op_io_irq();
    regs.p.i=0;
  }

  void CPUcore::op_clv() {
last_cycle();op_io_irq();
    regs.p.v=0;
  }

  void CPUcore::op_sec() {
last_cycle();op_io_irq();
    regs.p.c=1;
  }

  void CPUcore::op_sed() {
last_cycle();op_io_irq();
    regs.p.d=1;
  }

  void CPUcore::op_sei() {
last_cycle();op_io_irq();
    regs.p.i=1;
  }


  void CPUcore::op_rep_e() {
    rd.l = op_readpc();
last_cycle();op_io();
    regs.p &=~ rd.l;
    regs.p |= 0x30;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    update_table();
  }

  void CPUcore::op_rep_n() {
    rd.l = op_readpc();
last_cycle();op_io();
    regs.p &=~ rd.l;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    update_table();
  }

  void CPUcore::op_sep_e() {
    rd.l = op_readpc();
last_cycle();op_io();
    regs.p |= rd.l;
    regs.p |= 0x30;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    update_table();
  }

  void CPUcore::op_sep_n() {
    rd.l = op_readpc();
last_cycle();op_io();
    regs.p |= rd.l;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    update_table();
  }


  void CPUcore::op_tax_b() {
last_cycle();op_io_irq();
    regs.x.l = regs.a.l;
    regs.p.n = (regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  }

  void CPUcore::op_tax_w() {
last_cycle();op_io_irq();
    regs.x.w = regs.a.w;
    regs.p.n = (regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }

  void CPUcore::op_tay_b() {
last_cycle();op_io_irq();
    regs.y.l = regs.a.l;
    regs.p.n = (regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  }

  void CPUcore::op_tay_w() {
last_cycle();op_io_irq();
    regs.y.w = regs.a.w;
    regs.p.n = (regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }

  void CPUcore::op_txa_b() {
last_cycle();op_io_irq();
    regs.a.l = regs.x.l;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_txa_w() {
last_cycle();op_io_irq();
    regs.a.w = regs.x.w;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_txy_b() {
last_cycle();op_io_irq();
    regs.y.l = regs.x.l;
    regs.p.n = (regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  }

  void CPUcore::op_txy_w() {
last_cycle();op_io_irq();
    regs.y.w = regs.x.w;
    regs.p.n = (regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }

  void CPUcore::op_tya_b() {
last_cycle();op_io_irq();
    regs.a.l = regs.y.l;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_tya_w() {
last_cycle();op_io_irq();
    regs.a.w = regs.y.w;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_tyx_b() {
last_cycle();op_io_irq();
    regs.x.l = regs.y.l;
    regs.p.n = (regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  }

  void CPUcore::op_tyx_w() {
last_cycle();op_io_irq();
    regs.x.w = regs.y.w;
    regs.p.n = (regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }


  void CPUcore::op_tcd() {
last_cycle();op_io_irq();
    regs.d.w = regs.a.w;
    regs.p.n = (regs.d.w & 0x8000);
    regs.p.z = (regs.d.w == 0);
  }

  void CPUcore::op_tdc() {
last_cycle();op_io_irq();
    regs.a.w = regs.d.w;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }


  void CPUcore::op_tcs_e() {
last_cycle();op_io_irq();
    regs.s.l = regs.a.l;
  }

  void CPUcore::op_tcs_n() {
last_cycle();op_io_irq();
    regs.s.w = regs.a.w;
  }

  void CPUcore::op_tsc_e() {
last_cycle();op_io_irq();
    regs.a.w = regs.s.w;
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_tsc_n() {
last_cycle();op_io_irq();
    regs.a.w = regs.s.w;
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_tsx_b() {
last_cycle();op_io_irq();
    regs.x.l = regs.s.l;
    regs.p.n = (regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  }

  void CPUcore::op_tsx_w() {
last_cycle();op_io_irq();
    regs.x.w = regs.s.w;
    regs.p.n = (regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }

  void CPUcore::op_txs_e() {
last_cycle();op_io_irq();
    regs.s.l = regs.x.l;
  }

  void CPUcore::op_txs_n() {
last_cycle();op_io_irq();
    regs.s.w = regs.x.w;
  }


  void CPUcore::op_pha_b() {
    op_io();
last_cycle();op_writestack(regs.a.l);
  }

  void CPUcore::op_pha_w() {
    op_io();
    op_writestack(regs.a.h);
last_cycle();op_writestack(regs.a.l);
  }

  void CPUcore::op_phx_b() {
    op_io();
last_cycle();op_writestack(regs.x.l);
  }

  void CPUcore::op_phx_w() {
    op_io();
    op_writestack(regs.x.h);
last_cycle();op_writestack(regs.x.l);
  }

  void CPUcore::op_phy_b() {
    op_io();
last_cycle();op_writestack(regs.y.l);
  }

  void CPUcore::op_phy_w() {
    op_io();
    op_writestack(regs.y.h);
last_cycle();op_writestack(regs.y.l);
  }

  void CPUcore::op_phd_e() {
    op_io();
    op_writestackn(regs.d.h);
last_cycle();op_writestackn(regs.d.l);
    regs.s.h = 0x01;
  }

  void CPUcore::op_phd_n() {
    op_io();
    op_writestackn(regs.d.h);
last_cycle();op_writestackn(regs.d.l);
  }

  void CPUcore::op_phb() {
    op_io();
last_cycle();op_writestack(regs.db);
  }

  void CPUcore::op_phk() {
    op_io();
last_cycle();op_writestack(regs.pc.b);
  }

  void CPUcore::op_php() {
    op_io();
last_cycle();op_writestack(regs.p);
  }


  void CPUcore::op_pla_b() {
    op_io();
    op_io();
last_cycle();regs.a.l = op_readstack();
    regs.p.n = (regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  }

  void CPUcore::op_pla_w() {
    op_io();
    op_io();
    regs.a.l = op_readstack();
last_cycle();regs.a.h = op_readstack();
    regs.p.n = (regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }

  void CPUcore::op_plx_b() {
    op_io();
    op_io();
last_cycle();regs.x.l = op_readstack();
    regs.p.n = (regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  }

  void CPUcore::op_plx_w() {
    op_io();
    op_io();
    regs.x.l = op_readstack();
last_cycle();regs.x.h = op_readstack();
    regs.p.n = (regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }

  void CPUcore::op_ply_b() {
    op_io();
    op_io();
last_cycle();regs.y.l = op_readstack();
    regs.p.n = (regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  }

  void CPUcore::op_ply_w() {
    op_io();
    op_io();
    regs.y.l = op_readstack();
last_cycle();regs.y.h = op_readstack();
    regs.p.n = (regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }

  void CPUcore::op_pld_e() {
    op_io();
    op_io();
    regs.d.l = op_readstackn();
last_cycle();regs.d.h = op_readstackn();
    regs.p.n = (regs.d.w & 0x8000);
    regs.p.z = (regs.d.w == 0);
    regs.s.h = 0x01;
  }

  void CPUcore::op_pld_n() {
    op_io();
    op_io();
    regs.d.l = op_readstackn();
last_cycle();regs.d.h = op_readstackn();
    regs.p.n = (regs.d.w & 0x8000);
    regs.p.z = (regs.d.w == 0);
  }

  void CPUcore::op_plb() {
    op_io();
    op_io();
last_cycle();regs.db = op_readstack();
    regs.p.n = (regs.db & 0x80);
    regs.p.z = (regs.db == 0);
  }

  void CPUcore::op_plp_e() {
    op_io();
    op_io();
last_cycle();regs.p = op_readstack() | 0x30;
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    update_table();
  }

  void CPUcore::op_plp_n() {
    op_io();
    op_io();
last_cycle();regs.p = op_readstack();
    if(regs.p.x) {
      regs.x.h = 0x00;
      regs.y.h = 0x00;
    }
    update_table();
  }


  void CPUcore::op_pea_e() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writestackn(aa.h);
last_cycle();op_writestackn(aa.l);
    regs.s.h = 0x01;
  }

  void CPUcore::op_pea_n() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_writestackn(aa.h);
last_cycle();op_writestackn(aa.l);
  }

  void CPUcore::op_pei_e() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_writestackn(aa.h);
last_cycle();op_writestackn(aa.l);
    regs.s.h = 0x01;
  }

  void CPUcore::op_pei_n() {
    dp = op_readpc();
    op_io_cond2();
    aa.l = op_readdp(dp + 0);
    aa.h = op_readdp(dp + 1);
    op_writestackn(aa.h);
last_cycle();op_writestackn(aa.l);
  }

  void CPUcore::op_per_e() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.w = regs.pc.d + (int16_t)aa.w;
    op_writestackn(rd.h);
last_cycle();op_writestackn(rd.l);
    regs.s.h = 0x01;
  }

  void CPUcore::op_per_n() {
    aa.l = op_readpc();
    aa.h = op_readpc();
    op_io();
    rd.w = regs.pc.d + (int16_t)aa.w;
    op_writestackn(rd.h);
last_cycle();op_writestackn(rd.l);
  }



