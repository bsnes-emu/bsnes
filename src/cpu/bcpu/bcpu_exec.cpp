void bCPU::exec_opcode() {
  (this->*optbl[op_read()])();
  snes->notify(SNES::CPU_EXEC_OPCODE);
}

void bCPU::init_op_tables() {
  optbl = optbl_e;

/* 0x */
  optbl[0x00] = op_brk;
  optbl[0x01] = op_ora_idpxb;
  optbl[0x02] = op_cop;
  optbl[0x03] = op_ora_srb;
  optbl[0x04] = op_tsb_dpb;
  optbl[0x05] = op_ora_dpb;
  optbl[0x06] = op_asl_dpb;
  optbl[0x07] = op_ora_ildpb;
  optbl[0x08] = op_php;
  optbl[0x09] = op_ora_constb;
  optbl[0x0a] = op_aslb;
  optbl[0x0b] = op_phd;
  optbl[0x0c] = op_tsb_addrb;
  optbl[0x0d] = op_ora_addrb;
  optbl[0x0e] = op_asl_addrb;
  optbl[0x0f] = op_ora_longb;
/* 1x */
  optbl[0x10] = op_bpl;
  optbl[0x11] = op_ora_idpyb;
  optbl[0x12] = op_ora_idpb;
  optbl[0x13] = op_ora_isryb;
  optbl[0x14] = op_trb_dpb;
  optbl[0x15] = op_ora_dpxb;
  optbl[0x16] = op_asl_dpxb;
  optbl[0x17] = op_ora_ildpyb;
  optbl[0x18] = op_clc;
  optbl[0x19] = op_ora_addryb;
  optbl[0x1a] = op_incb;
  optbl[0x1b] = op_tcs;
  optbl[0x1c] = op_trb_addrb;
  optbl[0x1d] = op_ora_addrxb;
  optbl[0x1e] = op_asl_addrxb;
  optbl[0x1f] = op_ora_longxb;
/* 2x */
  optbl[0x20] = op_jsr_addr;
  optbl[0x21] = op_and_idpxb;
  optbl[0x22] = op_jsr_long;
  optbl[0x23] = op_and_srb;
  optbl[0x24] = op_bit_dpb;
  optbl[0x25] = op_and_dpb;
  optbl[0x26] = op_rol_dpb;
  optbl[0x27] = op_and_ildpb;
  optbl[0x28] = op_plp;
  optbl[0x29] = op_and_constb;
  optbl[0x2a] = op_rolb;
  optbl[0x2b] = op_pld;
  optbl[0x2c] = op_bit_addrb;
  optbl[0x2d] = op_and_addrb;
  optbl[0x2e] = op_rol_addrb;
  optbl[0x2f] = op_and_longb;
/* 3x */
  optbl[0x30] = op_bmi;
  optbl[0x31] = op_and_idpyb;
  optbl[0x32] = op_and_idpb;
  optbl[0x33] = op_and_isryb;
  optbl[0x34] = op_bit_dpxb;
  optbl[0x35] = op_and_dpxb;
  optbl[0x36] = op_rol_dpxb;
  optbl[0x37] = op_and_ildpyb;
  optbl[0x38] = op_sec;
  optbl[0x39] = op_and_addryb;
  optbl[0x3a] = op_decb;
  optbl[0x3b] = op_tsc;
  optbl[0x3c] = op_bit_addrxb;
  optbl[0x3d] = op_and_addrxb;
  optbl[0x3e] = op_rol_addrxb;
  optbl[0x3f] = op_and_longxb;
/* 4x */
  optbl[0x40] = op_rtie;
  optbl[0x41] = op_eor_idpxb;
  optbl[0x42] = op_wdm;
  optbl[0x43] = op_eor_srb;
  optbl[0x44] = op_mvp;
  optbl[0x45] = op_eor_dpb;
  optbl[0x46] = op_lsr_dpb;
  optbl[0x47] = op_eor_ildpb;
  optbl[0x48] = op_phab;
  optbl[0x49] = op_eor_constb;
  optbl[0x4a] = op_lsrb;
  optbl[0x4b] = op_phk;
  optbl[0x4c] = op_jmp_addr;
  optbl[0x4d] = op_eor_addrb;
  optbl[0x4e] = op_lsr_addrb;
  optbl[0x4f] = op_eor_longb;
/* 5x */
  optbl[0x50] = op_bvc;
  optbl[0x51] = op_eor_idpyb;
  optbl[0x52] = op_eor_idpb;
  optbl[0x53] = op_eor_isryb;
  optbl[0x54] = op_mvn;
  optbl[0x55] = op_eor_dpxb;
  optbl[0x56] = op_lsr_dpxb;
  optbl[0x57] = op_eor_ildpyb;
  optbl[0x58] = op_cli;
  optbl[0x59] = op_eor_addryb;
  optbl[0x5a] = op_phyb;
  optbl[0x5b] = op_tcd;
  optbl[0x5c] = op_jmp_long;
  optbl[0x5d] = op_eor_addrxb;
  optbl[0x5e] = op_lsr_addrxb;
  optbl[0x5f] = op_eor_longxb;
/* 6x */
  optbl[0x60] = op_rts;
  optbl[0x61] = op_adc_idpxb;
  optbl[0x62] = op_per;
  optbl[0x63] = op_adc_srb;
  optbl[0x64] = op_stz_dpb;
  optbl[0x65] = op_adc_dpb;
  optbl[0x66] = op_ror_dpb;
  optbl[0x67] = op_adc_ildpb;
  optbl[0x68] = op_plab;
  optbl[0x69] = op_adc_constb;
  optbl[0x6a] = op_rorb;
  optbl[0x6b] = op_rtl;
  optbl[0x6c] = op_jmp_iaddr;
  optbl[0x6d] = op_adc_addrb;
  optbl[0x6e] = op_ror_addrb;
  optbl[0x6f] = op_adc_longb;
/* 7x */
  optbl[0x70] = op_bvs;
  optbl[0x71] = op_adc_idpyb;
  optbl[0x72] = op_adc_idpb;
  optbl[0x73] = op_adc_isryb;
  optbl[0x74] = op_stz_dpxb;
  optbl[0x75] = op_adc_dpxb;
  optbl[0x76] = op_ror_dpxb;
  optbl[0x77] = op_adc_ildpyb;
  optbl[0x78] = op_sei;
  optbl[0x79] = op_adc_addryb;
  optbl[0x7a] = op_plyb;
  optbl[0x7b] = op_tdc;
  optbl[0x7c] = op_jmp_iaddrx;
  optbl[0x7d] = op_adc_addrxb;
  optbl[0x7e] = op_ror_addrxb;
  optbl[0x7f] = op_adc_longxb;
/* 8x */
  optbl[0x80] = op_bra;
  optbl[0x81] = op_sta_idpxb;
  optbl[0x82] = op_brl;
  optbl[0x83] = op_sta_srb;
  optbl[0x84] = op_sty_dpb;
  optbl[0x85] = op_sta_dpb;
  optbl[0x86] = op_stx_dpb;
  optbl[0x87] = op_sta_ildpb;
  optbl[0x88] = op_deyb;
  optbl[0x89] = op_bit_constb;
  optbl[0x8a] = op_txab;
  optbl[0x8b] = op_phb;
  optbl[0x8c] = op_sty_addrb;
  optbl[0x8d] = op_sta_addrb;
  optbl[0x8e] = op_stx_addrb;
  optbl[0x8f] = op_sta_longb;
/* 9x */
  optbl[0x90] = op_bcc;
  optbl[0x91] = op_sta_idpyb;
  optbl[0x92] = op_sta_idpb;
  optbl[0x93] = op_sta_isryb;
  optbl[0x94] = op_sty_dpxb;
  optbl[0x95] = op_sta_dpxb;
  optbl[0x96] = op_stx_dpyb;
  optbl[0x97] = op_sta_ildpyb;
  optbl[0x98] = op_tyab;
  optbl[0x99] = op_sta_addryb;
  optbl[0x9a] = op_txsb;
  optbl[0x9b] = op_txyb;
  optbl[0x9c] = op_stz_addrb;
  optbl[0x9d] = op_sta_addrxb;
  optbl[0x9e] = op_stz_addrxb;
  optbl[0x9f] = op_sta_longxb;
/* ax */
  optbl[0xa0] = op_ldy_constb;
  optbl[0xa1] = op_lda_idpxb;
  optbl[0xa2] = op_ldx_constb;
  optbl[0xa3] = op_lda_srb;
  optbl[0xa4] = op_ldy_dpb;
  optbl[0xa5] = op_lda_dpb;
  optbl[0xa6] = op_ldx_dpb;
  optbl[0xa7] = op_lda_ildpb;
  optbl[0xa8] = op_tayb;
  optbl[0xa9] = op_lda_constb;
  optbl[0xaa] = op_taxb;
  optbl[0xab] = op_plb;
  optbl[0xac] = op_ldy_addrb;
  optbl[0xad] = op_lda_addrb;
  optbl[0xae] = op_ldx_addrb;
  optbl[0xaf] = op_lda_longb;
/* bx */
  optbl[0xb0] = op_bcs;
  optbl[0xb1] = op_lda_idpyb;
  optbl[0xb2] = op_lda_idpb;
  optbl[0xb3] = op_lda_isryb;
  optbl[0xb4] = op_ldy_dpxb;
  optbl[0xb5] = op_lda_dpxb;
  optbl[0xb6] = op_ldx_dpyb;
  optbl[0xb7] = op_lda_ildpyb;
  optbl[0xb8] = op_clv;
  optbl[0xb9] = op_lda_addryb;
  optbl[0xba] = op_tsxb;
  optbl[0xbb] = op_tyxb;
  optbl[0xbc] = op_ldy_addrxb;
  optbl[0xbd] = op_lda_addrxb;
  optbl[0xbe] = op_ldx_addryb;
  optbl[0xbf] = op_lda_longxb;
/* cx */
  optbl[0xc0] = op_cpy_constb;
  optbl[0xc1] = op_cmp_idpxb;
  optbl[0xc2] = op_rep;
  optbl[0xc3] = op_cmp_srb;
  optbl[0xc4] = op_cpy_dpb;
  optbl[0xc5] = op_cmp_dpb;
  optbl[0xc6] = op_dec_dpb;
  optbl[0xc7] = op_cmp_ildpb;
  optbl[0xc8] = op_inyb;
  optbl[0xc9] = op_cmp_constb;
  optbl[0xca] = op_dexb;
  optbl[0xcb] = op_wai;
  optbl[0xcc] = op_cpy_addrb;
  optbl[0xcd] = op_cmp_addrb;
  optbl[0xce] = op_dec_addrb;
  optbl[0xcf] = op_cmp_longb;
/* dx */
  optbl[0xd0] = op_bne;
  optbl[0xd1] = op_cmp_idpyb;
  optbl[0xd2] = op_cmp_idpb;
  optbl[0xd3] = op_cmp_isryb;
  optbl[0xd4] = op_pei;
  optbl[0xd5] = op_cmp_dpxb;
  optbl[0xd6] = op_dec_dpxb;
  optbl[0xd7] = op_cmp_ildpyb;
  optbl[0xd8] = op_cld;
  optbl[0xd9] = op_cmp_addryb;
  optbl[0xda] = op_phxb;
  optbl[0xdb] = op_stp;
  optbl[0xdc] = op_jmp_iladdr;
  optbl[0xdd] = op_cmp_addrxb;
  optbl[0xde] = op_dec_addrxb;
  optbl[0xdf] = op_cmp_longxb;
/* ex */
  optbl[0xe0] = op_cpx_constb;
  optbl[0xe1] = op_sbc_idpxb;
  optbl[0xe2] = op_sep;
  optbl[0xe3] = op_sbc_srb;
  optbl[0xe4] = op_cpx_dpb;
  optbl[0xe5] = op_sbc_dpb;
  optbl[0xe6] = op_inc_dpb;
  optbl[0xe7] = op_sbc_ildpb;
  optbl[0xe8] = op_inxb;
  optbl[0xe9] = op_sbc_constb;
  optbl[0xea] = op_nop;
  optbl[0xeb] = op_xba;
  optbl[0xec] = op_cpx_addrb;
  optbl[0xed] = op_sbc_addrb;
  optbl[0xee] = op_inc_addrb;
  optbl[0xef] = op_sbc_longb;
/* fx */
  optbl[0xf0] = op_beq;
  optbl[0xf1] = op_sbc_idpyb;
  optbl[0xf2] = op_sbc_idpb;
  optbl[0xf3] = op_sbc_isryb;
  optbl[0xf4] = op_pea;
  optbl[0xf5] = op_sbc_dpxb;
  optbl[0xf6] = op_inc_dpxb;
  optbl[0xf7] = op_sbc_ildpyb;
  optbl[0xf8] = op_sed;
  optbl[0xf9] = op_sbc_addryb;
  optbl[0xfa] = op_plxb;
  optbl[0xfb] = op_xce;
  optbl[0xfc] = op_jsr_iaddrx;
  optbl[0xfd] = op_sbc_addrxb;
  optbl[0xfe] = op_inc_addrxb;
  optbl[0xff] = op_sbc_longxb;

  memcpy(optbl_MX, optbl_e, sizeof(optbl_e));
  memcpy(optbl_Mx, optbl_e, sizeof(optbl_e));
  memcpy(optbl_mX, optbl_e, sizeof(optbl_e));
  memcpy(optbl_mx, optbl_e, sizeof(optbl_e));

/* adc */
  optbl_mX[0x69] = optbl_mx[0x69] = op_adc_constw;
  optbl_mX[0x6d] = optbl_mx[0x6d] = op_adc_addrw;
  optbl_mX[0x7d] = optbl_mx[0x7d] = op_adc_addrxw;
  optbl_mX[0x65] = optbl_mx[0x65] = op_adc_dpw;
  optbl_mX[0x72] = optbl_mx[0x72] = op_adc_idpw;
  optbl_mX[0x67] = optbl_mx[0x67] = op_adc_ildpw;
  optbl_mX[0x6f] = optbl_mx[0x6f] = op_adc_longw;
  optbl_mX[0x7f] = optbl_mx[0x7f] = op_adc_longxw;
  optbl_mX[0x79] = optbl_mx[0x79] = op_adc_addryw;
  optbl_mX[0x75] = optbl_mx[0x75] = op_adc_dpxw;
  optbl_mX[0x61] = optbl_mx[0x61] = op_adc_idpxw;
  optbl_mX[0x71] = optbl_mx[0x71] = op_adc_idpyw;
  optbl_mX[0x77] = optbl_mx[0x77] = op_adc_ildpyw;
  optbl_mX[0x63] = optbl_mx[0x63] = op_adc_srw;
  optbl_mX[0x73] = optbl_mx[0x73] = op_adc_isryw;

/* and */
  optbl_mX[0x29] = optbl_mx[0x29] = op_and_constw;
  optbl_mX[0x2d] = optbl_mx[0x2d] = op_and_addrw;
  optbl_mX[0x3d] = optbl_mx[0x3d] = op_and_addrxw;
  optbl_mX[0x25] = optbl_mx[0x25] = op_and_dpw;
  optbl_mX[0x32] = optbl_mx[0x32] = op_and_idpw;
  optbl_mX[0x27] = optbl_mx[0x27] = op_and_ildpw;
  optbl_mX[0x2f] = optbl_mx[0x2f] = op_and_longw;
  optbl_mX[0x3f] = optbl_mx[0x3f] = op_and_longxw;
  optbl_mX[0x39] = optbl_mx[0x39] = op_and_addryw;
  optbl_mX[0x35] = optbl_mx[0x35] = op_and_dpxw;
  optbl_mX[0x21] = optbl_mx[0x21] = op_and_idpxw;
  optbl_mX[0x31] = optbl_mx[0x31] = op_and_idpyw;
  optbl_mX[0x37] = optbl_mx[0x37] = op_and_ildpyw;
  optbl_mX[0x23] = optbl_mx[0x23] = op_and_srw;
  optbl_mX[0x33] = optbl_mx[0x33] = op_and_isryw;

/* cmp */
  optbl_mX[0xc9] = optbl_mx[0xc9] = op_cmp_constw;
  optbl_mX[0xcd] = optbl_mx[0xcd] = op_cmp_addrw;
  optbl_mX[0xdd] = optbl_mx[0xdd] = op_cmp_addrxw;
  optbl_mX[0xc5] = optbl_mx[0xc5] = op_cmp_dpw;
  optbl_mX[0xd2] = optbl_mx[0xd2] = op_cmp_idpw;
  optbl_mX[0xc7] = optbl_mx[0xc7] = op_cmp_ildpw;
  optbl_mX[0xcf] = optbl_mx[0xcf] = op_cmp_longw;
  optbl_mX[0xdf] = optbl_mx[0xdf] = op_cmp_longxw;
  optbl_mX[0xd9] = optbl_mx[0xd9] = op_cmp_addryw;
  optbl_mX[0xd5] = optbl_mx[0xd5] = op_cmp_dpxw;
  optbl_mX[0xc1] = optbl_mx[0xc1] = op_cmp_idpxw;
  optbl_mX[0xd1] = optbl_mx[0xd1] = op_cmp_idpyw;
  optbl_mX[0xd7] = optbl_mx[0xd7] = op_cmp_ildpyw;
  optbl_mX[0xc3] = optbl_mx[0xc3] = op_cmp_srw;
  optbl_mX[0xd3] = optbl_mx[0xd3] = op_cmp_isryw;

/* eor */
  optbl_mX[0x49] = optbl_mx[0x49] = op_eor_constw;
  optbl_mX[0x4d] = optbl_mx[0x4d] = op_eor_addrw;
  optbl_mX[0x5d] = optbl_mx[0x5d] = op_eor_addrxw;
  optbl_mX[0x45] = optbl_mx[0x45] = op_eor_dpw;
  optbl_mX[0x52] = optbl_mx[0x52] = op_eor_idpw;
  optbl_mX[0x47] = optbl_mx[0x47] = op_eor_ildpw;
  optbl_mX[0x4f] = optbl_mx[0x4f] = op_eor_longw;
  optbl_mX[0x5f] = optbl_mx[0x5f] = op_eor_longxw;
  optbl_mX[0x59] = optbl_mx[0x59] = op_eor_addryw;
  optbl_mX[0x55] = optbl_mx[0x55] = op_eor_dpxw;
  optbl_mX[0x41] = optbl_mx[0x41] = op_eor_idpxw;
  optbl_mX[0x51] = optbl_mx[0x51] = op_eor_idpyw;
  optbl_mX[0x57] = optbl_mx[0x57] = op_eor_ildpyw;
  optbl_mX[0x43] = optbl_mx[0x43] = op_eor_srw;
  optbl_mX[0x53] = optbl_mx[0x53] = op_eor_isryw;

/* lda */
  optbl_mX[0xa9] = optbl_mx[0xa9] = op_lda_constw;
  optbl_mX[0xad] = optbl_mx[0xad] = op_lda_addrw;
  optbl_mX[0xbd] = optbl_mx[0xbd] = op_lda_addrxw;
  optbl_mX[0xa5] = optbl_mx[0xa5] = op_lda_dpw;
  optbl_mX[0xb2] = optbl_mx[0xb2] = op_lda_idpw;
  optbl_mX[0xa7] = optbl_mx[0xa7] = op_lda_ildpw;
  optbl_mX[0xaf] = optbl_mx[0xaf] = op_lda_longw;
  optbl_mX[0xbf] = optbl_mx[0xbf] = op_lda_longxw;
  optbl_mX[0xb9] = optbl_mx[0xb9] = op_lda_addryw;
  optbl_mX[0xb5] = optbl_mx[0xb5] = op_lda_dpxw;
  optbl_mX[0xa1] = optbl_mx[0xa1] = op_lda_idpxw;
  optbl_mX[0xb1] = optbl_mx[0xb1] = op_lda_idpyw;
  optbl_mX[0xb7] = optbl_mx[0xb7] = op_lda_ildpyw;
  optbl_mX[0xa3] = optbl_mx[0xa3] = op_lda_srw;
  optbl_mX[0xb3] = optbl_mx[0xb3] = op_lda_isryw;

/* ora */
  optbl_mX[0x09] = optbl_mx[0x09] = op_ora_constw;
  optbl_mX[0x0d] = optbl_mx[0x0d] = op_ora_addrw;
  optbl_mX[0x1d] = optbl_mx[0x1d] = op_ora_addrxw;
  optbl_mX[0x05] = optbl_mx[0x05] = op_ora_dpw;
  optbl_mX[0x12] = optbl_mx[0x12] = op_ora_idpw;
  optbl_mX[0x07] = optbl_mx[0x07] = op_ora_ildpw;
  optbl_mX[0x0f] = optbl_mx[0x0f] = op_ora_longw;
  optbl_mX[0x1f] = optbl_mx[0x1f] = op_ora_longxw;
  optbl_mX[0x19] = optbl_mx[0x19] = op_ora_addryw;
  optbl_mX[0x15] = optbl_mx[0x15] = op_ora_dpxw;
  optbl_mX[0x01] = optbl_mx[0x01] = op_ora_idpxw;
  optbl_mX[0x11] = optbl_mx[0x11] = op_ora_idpyw;
  optbl_mX[0x17] = optbl_mx[0x17] = op_ora_ildpyw;
  optbl_mX[0x03] = optbl_mx[0x03] = op_ora_srw;
  optbl_mX[0x13] = optbl_mx[0x13] = op_ora_isryw;

/* sbc */
  optbl_mX[0xe9] = optbl_mx[0xe9] = op_sbc_constw;
  optbl_mX[0xed] = optbl_mx[0xed] = op_sbc_addrw;
  optbl_mX[0xfd] = optbl_mx[0xfd] = op_sbc_addrxw;
  optbl_mX[0xe5] = optbl_mx[0xe5] = op_sbc_dpw;
  optbl_mX[0xf2] = optbl_mx[0xf2] = op_sbc_idpw;
  optbl_mX[0xe7] = optbl_mx[0xe7] = op_sbc_ildpw;
  optbl_mX[0xef] = optbl_mx[0xef] = op_sbc_longw;
  optbl_mX[0xff] = optbl_mx[0xff] = op_sbc_longxw;
  optbl_mX[0xf9] = optbl_mx[0xf9] = op_sbc_addryw;
  optbl_mX[0xf5] = optbl_mx[0xf5] = op_sbc_dpxw;
  optbl_mX[0xe1] = optbl_mx[0xe1] = op_sbc_idpxw;
  optbl_mX[0xf1] = optbl_mx[0xf1] = op_sbc_idpyw;
  optbl_mX[0xf7] = optbl_mx[0xf7] = op_sbc_ildpyw;
  optbl_mX[0xe3] = optbl_mx[0xe3] = op_sbc_srw;
  optbl_mX[0xf3] = optbl_mx[0xf3] = op_sbc_isryw;

/* sta */
  optbl_mX[0x8d] = optbl_mx[0x8d] = op_sta_addrw;
  optbl_mX[0x9d] = optbl_mx[0x9d] = op_sta_addrxw;
  optbl_mX[0x85] = optbl_mx[0x85] = op_sta_dpw;
  optbl_mX[0x92] = optbl_mx[0x92] = op_sta_idpw;
  optbl_mX[0x87] = optbl_mx[0x87] = op_sta_ildpw;
  optbl_mX[0x8f] = optbl_mx[0x8f] = op_sta_longw;
  optbl_mX[0x9f] = optbl_mx[0x9f] = op_sta_longxw;
  optbl_mX[0x99] = optbl_mx[0x99] = op_sta_addryw;
  optbl_mX[0x95] = optbl_mx[0x95] = op_sta_dpxw;
  optbl_mX[0x81] = optbl_mx[0x81] = op_sta_idpxw;
  optbl_mX[0x91] = optbl_mx[0x91] = op_sta_idpyw;
  optbl_mX[0x97] = optbl_mx[0x97] = op_sta_ildpyw;
  optbl_mX[0x83] = optbl_mx[0x83] = op_sta_srw;
  optbl_mX[0x93] = optbl_mx[0x93] = op_sta_isryw;

/* incdec */
  optbl_mX[0x1a] = optbl_mx[0x1a] = op_incw;
  optbl_mX[0xee] = optbl_mx[0xee] = op_inc_addrw;
  optbl_mX[0xfe] = optbl_mx[0xfe] = op_inc_addrxw;
  optbl_mX[0xe6] = optbl_mx[0xe6] = op_inc_dpw;
  optbl_mX[0xf6] = optbl_mx[0xf6] = op_inc_dpxw;
  optbl_mX[0x3a] = optbl_mx[0x3a] = op_decw;
  optbl_mX[0xce] = optbl_mx[0xce] = op_dec_addrw;
  optbl_mX[0xde] = optbl_mx[0xde] = op_dec_addrxw;
  optbl_mX[0xc6] = optbl_mx[0xc6] = op_dec_dpw;
  optbl_mX[0xd6] = optbl_mx[0xd6] = op_dec_dpxw;

  optbl_Mx[0xe8] = optbl_mx[0xe8] = op_inxw;
  optbl_Mx[0xc8] = optbl_mx[0xc8] = op_inyw;
  optbl_Mx[0xca] = optbl_mx[0xca] = op_dexw;
  optbl_Mx[0x88] = optbl_mx[0x88] = op_deyw;

/* misc */
  optbl_mX[0x89] = optbl_mx[0x89] = op_bit_constw;
  optbl_mX[0x2c] = optbl_mx[0x2c] = op_bit_addrw;
  optbl_mX[0x3c] = optbl_mx[0x3c] = op_bit_addrxw;
  optbl_mX[0x24] = optbl_mx[0x24] = op_bit_dpw;
  optbl_mX[0x34] = optbl_mx[0x34] = op_bit_dpxw;
  optbl_mX[0x9c] = optbl_mx[0x9c] = op_stz_addrw;
  optbl_mX[0x9e] = optbl_mx[0x9e] = op_stz_addrxw;
  optbl_mX[0x64] = optbl_mx[0x64] = op_stz_dpw;
  optbl_mX[0x74] = optbl_mx[0x74] = op_stz_dpxw;
  optbl_mX[0x1c] = optbl_mx[0x1c] = op_trb_addrw;
  optbl_mX[0x14] = optbl_mx[0x14] = op_trb_dpw;
  optbl_mX[0x0c] = optbl_mx[0x0c] = op_tsb_addrw;
  optbl_mX[0x04] = optbl_mx[0x04] = op_tsb_dpw;
  optbl_mX[0x8a] = optbl_mx[0x8a] = op_txaw;
  optbl_mX[0x98] = optbl_mx[0x98] = op_tyaw;

  optbl_Mx[0xe0] = optbl_mx[0xe0] = op_cpx_constw;
  optbl_Mx[0xec] = optbl_mx[0xec] = op_cpx_addrw;
  optbl_Mx[0xe4] = optbl_mx[0xe4] = op_cpx_dpw;
  optbl_Mx[0xc0] = optbl_mx[0xc0] = op_cpy_constw;
  optbl_Mx[0xcc] = optbl_mx[0xcc] = op_cpy_addrw;
  optbl_Mx[0xc4] = optbl_mx[0xc4] = op_cpy_dpw;
  optbl_Mx[0xa2] = optbl_mx[0xa2] = op_ldx_constw;
  optbl_Mx[0xae] = optbl_mx[0xae] = op_ldx_addrw;
  optbl_Mx[0xbe] = optbl_mx[0xbe] = op_ldx_addryw;
  optbl_Mx[0xa6] = optbl_mx[0xa6] = op_ldx_dpw;
  optbl_Mx[0xb6] = optbl_mx[0xb6] = op_ldx_dpyw;
  optbl_Mx[0xa0] = optbl_mx[0xa0] = op_ldy_constw;
  optbl_Mx[0xac] = optbl_mx[0xac] = op_ldy_addrw;
  optbl_Mx[0xbc] = optbl_mx[0xbc] = op_ldy_addrxw;
  optbl_Mx[0xa4] = optbl_mx[0xa4] = op_ldy_dpw;
  optbl_Mx[0xb4] = optbl_mx[0xb4] = op_ldy_dpxw;
  optbl_Mx[0x8e] = optbl_mx[0x8e] = op_stx_addrw;
  optbl_Mx[0x86] = optbl_mx[0x86] = op_stx_dpw;
  optbl_Mx[0x96] = optbl_mx[0x96] = op_stx_dpyw;
  optbl_Mx[0x8c] = optbl_mx[0x8c] = op_sty_addrw;
  optbl_Mx[0x84] = optbl_mx[0x84] = op_sty_dpw;
  optbl_Mx[0x94] = optbl_mx[0x94] = op_sty_dpxw;
  optbl_Mx[0xaa] = optbl_mx[0xaa] = op_taxw;
  optbl_Mx[0xa8] = optbl_mx[0xa8] = op_tayw;
  optbl_Mx[0xba] = optbl_mx[0xba] = op_tsxw;
  optbl_Mx[0x9a] = optbl_mx[0x9a] = op_txsw;
  optbl_Mx[0x9b] = optbl_mx[0x9b] = op_txyw;
  optbl_Mx[0xbb] = optbl_mx[0xbb] = op_tyxw;

/* pc */
  optbl_MX[0x40] = optbl_Mx[0x40] = op_rtin;
  optbl_mX[0x40] = optbl_mx[0x40] = op_rtin;

/* shift */
  optbl_mX[0x0a] = optbl_mx[0x0a] = op_aslw;
  optbl_mX[0x0e] = optbl_mx[0x0e] = op_asl_addrw;
  optbl_mX[0x1e] = optbl_mx[0x1e] = op_asl_addrxw;
  optbl_mX[0x06] = optbl_mx[0x06] = op_asl_dpw;
  optbl_mX[0x16] = optbl_mx[0x16] = op_asl_dpxw;
  optbl_mX[0x4a] = optbl_mx[0x4a] = op_lsrw;
  optbl_mX[0x4e] = optbl_mx[0x4e] = op_lsr_addrw;
  optbl_mX[0x5e] = optbl_mx[0x5e] = op_lsr_addrxw;
  optbl_mX[0x46] = optbl_mx[0x46] = op_lsr_dpw;
  optbl_mX[0x56] = optbl_mx[0x56] = op_lsr_dpxw;
  optbl_mX[0x2a] = optbl_mx[0x2a] = op_rolw;
  optbl_mX[0x2e] = optbl_mx[0x2e] = op_rol_addrw;
  optbl_mX[0x3e] = optbl_mx[0x3e] = op_rol_addrxw;
  optbl_mX[0x26] = optbl_mx[0x26] = op_rol_dpw;
  optbl_mX[0x36] = optbl_mx[0x36] = op_rol_dpxw;
  optbl_mX[0x6a] = optbl_mx[0x6a] = op_rorw;
  optbl_mX[0x6e] = optbl_mx[0x6e] = op_ror_addrw;
  optbl_mX[0x7e] = optbl_mx[0x7e] = op_ror_addrxw;
  optbl_mX[0x66] = optbl_mx[0x66] = op_ror_dpw;
  optbl_mX[0x76] = optbl_mx[0x76] = op_ror_dpxw;

/* stack */
  optbl_mX[0x48] = optbl_mx[0x48] = op_phaw;
  optbl_mX[0x68] = optbl_mx[0x68] = op_plaw;

  optbl_Mx[0xda] = optbl_mx[0xda] = op_phxw;
  optbl_Mx[0x5a] = optbl_mx[0x5a] = op_phyw;
  optbl_Mx[0xfa] = optbl_mx[0xfa] = op_plxw;
  optbl_Mx[0x7a] = optbl_mx[0x7a] = op_plyw;
}
