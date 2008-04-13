  CPUReg24 aa, rd;
  uint8_t dp, sp;

  void op_irq();

  inline bool in_opcode() { return status.in_opcode; }

  //op_read
  void op_adc_b();
  void op_adc_w();
  void op_and_b();
  void op_and_w();
  void op_bit_b();
  void op_bit_w();
  void op_cmp_b();
  void op_cmp_w();
  void op_cpx_b();
  void op_cpx_w();
  void op_cpy_b();
  void op_cpy_w();
  void op_eor_b();
  void op_eor_w();
  void op_lda_b();
  void op_lda_w();
  void op_ldx_b();
  void op_ldx_w();
  void op_ldy_b();
  void op_ldy_w();
  void op_ora_b();
  void op_ora_w();
  void op_sbc_b();
  void op_sbc_w();
  //op_rmw
  void op_inc_b();
  void op_inc_w();
  void op_dec_b();
  void op_dec_w();
  void op_asl_b();
  void op_asl_w();
  void op_lsr_b();
  void op_lsr_w();
  void op_rol_b();
  void op_rol_w();
  void op_ror_b();
  void op_ror_w();
  void op_trb_b();
  void op_trb_w();
  void op_tsb_b();
  void op_tsb_w();

  void op_io_irq();
  void op_io_cond2();
  void op_io_cond4(uint16 x, uint16 y);
  void op_io_cond6(uint16 addr);
