//===============
//opcode_read.bpp
//===============

















//================
//opcode_write.bpp
//================













//==============
//opcode_rmw.bpp
//==============










//=============
//opcode_pc.bpp
//=============















//===============
//opcode_misc.bpp
//===============



























//===============
//opcode_read.bpp
//===============

void op_adc_const_b();
void op_adc_const_w();

void op_and_const_b();
void op_and_const_w();

void op_cmp_const_b();
void op_cmp_const_w();

void op_cpx_const_b();
void op_cpx_const_w();

void op_cpy_const_b();
void op_cpy_const_w();

void op_eor_const_b();
void op_eor_const_w();

void op_lda_const_b();
void op_lda_const_w();

void op_ldx_const_b();
void op_ldx_const_w();

void op_ldy_const_b();
void op_ldy_const_w();

void op_ora_const_b();
void op_ora_const_w();

void op_sbc_const_b();
void op_sbc_const_w();


void op_bit_const_b();
void op_bit_const_w();


void op_adc_addr_b();
void op_adc_addr_w();

void op_and_addr_b();
void op_and_addr_w();

void op_bit_addr_b();
void op_bit_addr_w();

void op_cmp_addr_b();
void op_cmp_addr_w();

void op_cpx_addr_b();
void op_cpx_addr_w();

void op_cpy_addr_b();
void op_cpy_addr_w();

void op_eor_addr_b();
void op_eor_addr_w();

void op_lda_addr_b();
void op_lda_addr_w();

void op_ldx_addr_b();
void op_ldx_addr_w();

void op_ldy_addr_b();
void op_ldy_addr_w();

void op_ora_addr_b();
void op_ora_addr_w();

void op_sbc_addr_b();
void op_sbc_addr_w();


void op_adc_addrx_b();
void op_adc_addrx_w();

void op_and_addrx_b();
void op_and_addrx_w();

void op_bit_addrx_b();
void op_bit_addrx_w();

void op_cmp_addrx_b();
void op_cmp_addrx_w();

void op_eor_addrx_b();
void op_eor_addrx_w();

void op_lda_addrx_b();
void op_lda_addrx_w();

void op_ldy_addrx_b();
void op_ldy_addrx_w();

void op_ora_addrx_b();
void op_ora_addrx_w();

void op_sbc_addrx_b();
void op_sbc_addrx_w();


void op_adc_addry_b();
void op_adc_addry_w();

void op_and_addry_b();
void op_and_addry_w();

void op_cmp_addry_b();
void op_cmp_addry_w();

void op_eor_addry_b();
void op_eor_addry_w();

void op_lda_addry_b();
void op_lda_addry_w();

void op_ldx_addry_b();
void op_ldx_addry_w();

void op_ora_addry_b();
void op_ora_addry_w();

void op_sbc_addry_b();
void op_sbc_addry_w();


void op_adc_long_b();
void op_adc_long_w();

void op_and_long_b();
void op_and_long_w();

void op_cmp_long_b();
void op_cmp_long_w();

void op_eor_long_b();
void op_eor_long_w();

void op_lda_long_b();
void op_lda_long_w();

void op_ora_long_b();
void op_ora_long_w();

void op_sbc_long_b();
void op_sbc_long_w();


void op_adc_longx_b();
void op_adc_longx_w();

void op_and_longx_b();
void op_and_longx_w();

void op_cmp_longx_b();
void op_cmp_longx_w();

void op_eor_longx_b();
void op_eor_longx_w();

void op_lda_longx_b();
void op_lda_longx_w();

void op_ora_longx_b();
void op_ora_longx_w();

void op_sbc_longx_b();
void op_sbc_longx_w();


void op_adc_dp_b();
void op_adc_dp_w();

void op_and_dp_b();
void op_and_dp_w();

void op_bit_dp_b();
void op_bit_dp_w();

void op_cmp_dp_b();
void op_cmp_dp_w();

void op_cpx_dp_b();
void op_cpx_dp_w();

void op_cpy_dp_b();
void op_cpy_dp_w();

void op_eor_dp_b();
void op_eor_dp_w();

void op_lda_dp_b();
void op_lda_dp_w();

void op_ldx_dp_b();
void op_ldx_dp_w();

void op_ldy_dp_b();
void op_ldy_dp_w();

void op_ora_dp_b();
void op_ora_dp_w();

void op_sbc_dp_b();
void op_sbc_dp_w();


void op_adc_dpr_b();
void op_adc_dpr_w();

void op_and_dpr_b();
void op_and_dpr_w();

void op_bit_dpr_b();
void op_bit_dpr_w();

void op_cmp_dpr_b();
void op_cmp_dpr_w();

void op_eor_dpr_b();
void op_eor_dpr_w();

void op_lda_dpr_b();
void op_lda_dpr_w();

void op_ldx_dpr_b();
void op_ldx_dpr_w();

void op_ldy_dpr_b();
void op_ldy_dpr_w();

void op_ora_dpr_b();
void op_ora_dpr_w();

void op_sbc_dpr_b();
void op_sbc_dpr_w();


void op_adc_idp_b();
void op_adc_idp_w();

void op_and_idp_b();
void op_and_idp_w();

void op_cmp_idp_b();
void op_cmp_idp_w();

void op_eor_idp_b();
void op_eor_idp_w();

void op_lda_idp_b();
void op_lda_idp_w();

void op_ora_idp_b();
void op_ora_idp_w();

void op_sbc_idp_b();
void op_sbc_idp_w();


void op_adc_idpx_b();
void op_adc_idpx_w();

void op_and_idpx_b();
void op_and_idpx_w();

void op_cmp_idpx_b();
void op_cmp_idpx_w();

void op_eor_idpx_b();
void op_eor_idpx_w();

void op_lda_idpx_b();
void op_lda_idpx_w();

void op_ora_idpx_b();
void op_ora_idpx_w();

void op_sbc_idpx_b();
void op_sbc_idpx_w();


void op_adc_idpy_b();
void op_adc_idpy_w();

void op_and_idpy_b();
void op_and_idpy_w();

void op_cmp_idpy_b();
void op_cmp_idpy_w();

void op_eor_idpy_b();
void op_eor_idpy_w();

void op_lda_idpy_b();
void op_lda_idpy_w();

void op_ora_idpy_b();
void op_ora_idpy_w();

void op_sbc_idpy_b();
void op_sbc_idpy_w();


void op_adc_ildp_b();
void op_adc_ildp_w();

void op_and_ildp_b();
void op_and_ildp_w();

void op_cmp_ildp_b();
void op_cmp_ildp_w();

void op_eor_ildp_b();
void op_eor_ildp_w();

void op_lda_ildp_b();
void op_lda_ildp_w();

void op_ora_ildp_b();
void op_ora_ildp_w();

void op_sbc_ildp_b();
void op_sbc_ildp_w();


void op_adc_ildpy_b();
void op_adc_ildpy_w();

void op_and_ildpy_b();
void op_and_ildpy_w();

void op_cmp_ildpy_b();
void op_cmp_ildpy_w();

void op_eor_ildpy_b();
void op_eor_ildpy_w();

void op_lda_ildpy_b();
void op_lda_ildpy_w();

void op_ora_ildpy_b();
void op_ora_ildpy_w();

void op_sbc_ildpy_b();
void op_sbc_ildpy_w();


void op_adc_sr_b();
void op_adc_sr_w();

void op_and_sr_b();
void op_and_sr_w();

void op_cmp_sr_b();
void op_cmp_sr_w();

void op_eor_sr_b();
void op_eor_sr_w();

void op_lda_sr_b();
void op_lda_sr_w();

void op_ora_sr_b();
void op_ora_sr_w();

void op_sbc_sr_b();
void op_sbc_sr_w();


void op_adc_isry_b();
void op_adc_isry_w();

void op_and_isry_b();
void op_and_isry_w();

void op_cmp_isry_b();
void op_cmp_isry_w();

void op_eor_isry_b();
void op_eor_isry_w();

void op_lda_isry_b();
void op_lda_isry_w();

void op_ora_isry_b();
void op_ora_isry_w();

void op_sbc_isry_b();
void op_sbc_isry_w();


//================
//opcode_write.bpp
//================

void op_sta_addr_b();
void op_sta_addr_w();

void op_stx_addr_b();
void op_stx_addr_w();

void op_sty_addr_b();
void op_sty_addr_w();

void op_stz_addr_b();
void op_stz_addr_w();


void op_sta_addrx_b();
void op_sta_addrx_w();

void op_sta_addry_b();
void op_sta_addry_w();

void op_stz_addrx_b();
void op_stz_addrx_w();


void op_sta_long_b();
void op_sta_long_w();

void op_sta_longx_b();
void op_sta_longx_w();


void op_sta_dp_b();
void op_sta_dp_w();

void op_stx_dp_b();
void op_stx_dp_w();

void op_sty_dp_b();
void op_sty_dp_w();

void op_stz_dp_b();
void op_stz_dp_w();


void op_sta_dpr_b();
void op_sta_dpr_w();

void op_stx_dpr_b();
void op_stx_dpr_w();

void op_sty_dpr_b();
void op_sty_dpr_w();

void op_stz_dpr_b();
void op_stz_dpr_w();


void op_sta_idp_b();
void op_sta_idp_w();

void op_sta_ildp_b();
void op_sta_ildp_w();

void op_sta_idpx_b();
void op_sta_idpx_w();

void op_sta_idpy_b();
void op_sta_idpy_w();

void op_sta_ildpy_b();
void op_sta_ildpy_w();

void op_sta_sr_b();
void op_sta_sr_w();

void op_sta_isry_b();
void op_sta_isry_w();


//==============
//opcode_rmw.bpp
//==============

void op_inc_imm_b();
void op_inc_imm_w();

void op_inx_imm_b();
void op_inx_imm_w();

void op_iny_imm_b();
void op_iny_imm_w();

void op_dec_imm_b();
void op_dec_imm_w();

void op_dex_imm_b();
void op_dex_imm_w();

void op_dey_imm_b();
void op_dey_imm_w();


void op_asl_imm_b();
void op_asl_imm_w();

void op_lsr_imm_b();
void op_lsr_imm_w();

void op_rol_imm_b();
void op_rol_imm_w();

void op_ror_imm_b();
void op_ror_imm_w();


void op_inc_addr_b();
void op_inc_addr_w();

void op_dec_addr_b();
void op_dec_addr_w();

void op_asl_addr_b();
void op_asl_addr_w();

void op_lsr_addr_b();
void op_lsr_addr_w();

void op_rol_addr_b();
void op_rol_addr_w();

void op_ror_addr_b();
void op_ror_addr_w();

void op_trb_addr_b();
void op_trb_addr_w();

void op_tsb_addr_b();
void op_tsb_addr_w();


void op_inc_addrx_b();
void op_inc_addrx_w();

void op_dec_addrx_b();
void op_dec_addrx_w();

void op_asl_addrx_b();
void op_asl_addrx_w();

void op_lsr_addrx_b();
void op_lsr_addrx_w();

void op_rol_addrx_b();
void op_rol_addrx_w();

void op_ror_addrx_b();
void op_ror_addrx_w();


void op_inc_dp_b();
void op_inc_dp_w();

void op_dec_dp_b();
void op_dec_dp_w();

void op_asl_dp_b();
void op_asl_dp_w();

void op_lsr_dp_b();
void op_lsr_dp_w();

void op_rol_dp_b();
void op_rol_dp_w();

void op_ror_dp_b();
void op_ror_dp_w();

void op_trb_dp_b();
void op_trb_dp_w();

void op_tsb_dp_b();
void op_tsb_dp_w();


void op_inc_dpx_b();
void op_inc_dpx_w();

void op_dec_dpx_b();
void op_dec_dpx_w();

void op_asl_dpx_b();
void op_asl_dpx_w();

void op_lsr_dpx_b();
void op_lsr_dpx_w();

void op_rol_dpx_b();
void op_rol_dpx_w();

void op_ror_dpx_b();
void op_ror_dpx_w();


//=============
//opcode_pc.bpp
//=============

void op_bcc();

void op_bcs();

void op_bne();

void op_beq();

void op_bpl();

void op_bmi();

void op_bvc();

void op_bvs();


void op_bra();

void op_brl();

void op_jmp_addr();

void op_jmp_long();

void op_jmp_iaddr();

void op_jmp_iaddrx();

void op_jmp_iladdr();

void op_jsr_addr();

void op_jsr_long_e();
void op_jsr_long_n();

void op_jsr_iaddrx_e();
void op_jsr_iaddrx_n();

void op_rti_e();
void op_rti_n();

void op_rts();

void op_rtl_e();
void op_rtl_n();


//===============
//opcode_misc.bpp
//===============

void op_nop();

void op_wdm();

void op_xba();


void op_mvn_b();
void op_mvn_w();

void op_mvp_b();
void op_mvp_w();


void op_brk_e();
void op_brk_n();

void op_cop_e();
void op_cop_n();


void op_stp();

void op_wai();

void op_xce();


void op_clc();

void op_cld();

void op_cli();

void op_clv();

void op_sec();

void op_sed();

void op_sei();


void op_rep_e();
void op_rep_n();

void op_sep_e();
void op_sep_n();


void op_tax_b();
void op_tax_w();

void op_tay_b();
void op_tay_w();

void op_txa_b();
void op_txa_w();

void op_txy_b();
void op_txy_w();

void op_tya_b();
void op_tya_w();

void op_tyx_b();
void op_tyx_w();


void op_tcd();

void op_tdc();


void op_tcs_e();
void op_tcs_n();

void op_tsc_e();
void op_tsc_n();

void op_tsx_b();
void op_tsx_w();

void op_txs_e();
void op_txs_n();


void op_pha_b();
void op_pha_w();

void op_phx_b();
void op_phx_w();

void op_phy_b();
void op_phy_w();

void op_phd_e();
void op_phd_n();

void op_phb();

void op_phk();

void op_php();


void op_pla_b();
void op_pla_w();

void op_plx_b();
void op_plx_w();

void op_ply_b();
void op_ply_w();

void op_pld_e();
void op_pld_n();

void op_plb();

void op_plp_e();
void op_plp_n();


void op_pea_e();
void op_pea_n();

void op_pei_e();
void op_pei_n();

void op_per_e();
void op_per_n();



