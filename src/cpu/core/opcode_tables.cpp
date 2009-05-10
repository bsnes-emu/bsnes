#ifdef CPUCORE_CPP

void CPUcore::initialize_opcode_table() {
  //same implementation for all processor states
  #define opA(id, name) \
    op_table[table_EM + id] = &CPUcore::op_ ## name; \
    op_table[table_MX + id] = &CPUcore::op_ ## name; \
    op_table[table_Mx + id] = &CPUcore::op_ ## name; \
    op_table[table_mX + id] = &CPUcore::op_ ## name; \
    op_table[table_mx + id] = &CPUcore::op_ ## name;

  //implementation changes based on E processor state
  #define opE(id, name) \
    op_table[table_EM + id] = &CPUcore::op_ ## name ## _e; \
    op_table[table_MX + id] = &CPUcore::op_ ## name ## _n; \
    op_table[table_Mx + id] = &CPUcore::op_ ## name ## _n; \
    op_table[table_mX + id] = &CPUcore::op_ ## name ## _n; \
    op_table[table_mx + id] = &CPUcore::op_ ## name ## _n; \

  //implementation changes based on M processor state
  #define opM(id, name) \
    op_table[table_EM + id] = &CPUcore::op_ ## name ## _b; \
    op_table[table_MX + id] = &CPUcore::op_ ## name ## _b; \
    op_table[table_Mx + id] = &CPUcore::op_ ## name ## _b; \
    op_table[table_mX + id] = &CPUcore::op_ ## name ## _w; \
    op_table[table_mx + id] = &CPUcore::op_ ## name ## _w;

  //implementation changes based on X processor state
  #define opX(id, name) \
    op_table[table_EM + id] = &CPUcore::op_ ## name ## _b; \
    op_table[table_MX + id] = &CPUcore::op_ ## name ## _b; \
    op_table[table_Mx + id] = &CPUcore::op_ ## name ## _w; \
    op_table[table_mX + id] = &CPUcore::op_ ## name ## _b; \
    op_table[table_mx + id] = &CPUcore::op_ ## name ## _w;

  opE(0x00, brk)         opM(0x01, ora_idpx)   opE(0x02, cop)        opM(0x03, ora_sr)
  opM(0x04, tsb_dp)      opM(0x05, ora_dp)     opM(0x06, asl_dp)     opM(0x07, ora_ildp)
  opA(0x08, php)         opM(0x09, ora_const)  opM(0x0a, asl_imm)    opE(0x0b, phd)
  opM(0x0c, tsb_addr)    opM(0x0d, ora_addr)   opM(0x0e, asl_addr)   opM(0x0f, ora_long)

  opA(0x10, bpl)         opM(0x11, ora_idpy)   opM(0x12, ora_idp)    opM(0x13, ora_isry)
  opM(0x14, trb_dp)      opM(0x15, ora_dpr)    opM(0x16, asl_dpx)    opM(0x17, ora_ildpy)
  opA(0x18, clc)         opM(0x19, ora_addry)  opM(0x1a, inc_imm)    opE(0x1b, tcs)
  opM(0x1c, trb_addr)    opM(0x1d, ora_addrx)  opM(0x1e, asl_addrx)  opM(0x1f, ora_longx)

  opA(0x20, jsr_addr)    opM(0x21, and_idpx)   opE(0x22, jsr_long)   opM(0x23, and_sr)
  opM(0x24, bit_dp)      opM(0x25, and_dp)     opM(0x26, rol_dp)     opM(0x27, and_ildp)
  opE(0x28, plp)         opM(0x29, and_const)  opM(0x2a, rol_imm)    opE(0x2b, pld)
  opM(0x2c, bit_addr)    opM(0x2d, and_addr)   opM(0x2e, rol_addr)   opM(0x2f, and_long)

  opA(0x30, bmi)         opM(0x31, and_idpy)   opM(0x32, and_idp)    opM(0x33, and_isry)
  opM(0x34, bit_dpr)     opM(0x35, and_dpr)    opM(0x36, rol_dpx)    opM(0x37, and_ildpy)
  opA(0x38, sec)         opM(0x39, and_addry)  opM(0x3a, dec_imm)    opE(0x3b, tsc)
  opM(0x3c, bit_addrx)   opM(0x3d, and_addrx)  opM(0x3e, rol_addrx)  opM(0x3f, and_longx)

  opE(0x40, rti)         opM(0x41, eor_idpx)   opA(0x42, wdm)        opM(0x43, eor_sr)
  opX(0x44, mvp)         opM(0x45, eor_dp)     opM(0x46, lsr_dp)     opM(0x47, eor_ildp)
  opM(0x48, pha)         opM(0x49, eor_const)  opM(0x4a, lsr_imm)    opA(0x4b, phk)
  opA(0x4c, jmp_addr)    opM(0x4d, eor_addr)   opM(0x4e, lsr_addr)   opM(0x4f, eor_long)

  opA(0x50, bvc)         opM(0x51, eor_idpy)   opM(0x52, eor_idp)    opM(0x53, eor_isry)
  opX(0x54, mvn)         opM(0x55, eor_dpr)    opM(0x56, lsr_dpx)    opM(0x57, eor_ildpy)
  opA(0x58, cli)         opM(0x59, eor_addry)  opX(0x5a, phy)        opA(0x5b, tcd)
  opA(0x5c, jmp_long)    opM(0x5d, eor_addrx)  opM(0x5e, lsr_addrx)  opM(0x5f, eor_longx)

  opA(0x60, rts)         opM(0x61, adc_idpx)   opE(0x62, per)        opM(0x63, adc_sr)
  opM(0x64, stz_dp)      opM(0x65, adc_dp)     opM(0x66, ror_dp)     opM(0x67, adc_ildp)
  opM(0x68, pla)         opM(0x69, adc_const)  opM(0x6a, ror_imm)    opE(0x6b, rtl)
  opA(0x6c, jmp_iaddr)   opM(0x6d, adc_addr)   opM(0x6e, ror_addr)   opM(0x6f, adc_long)

  opA(0x70, bvs)         opM(0x71, adc_idpy)   opM(0x72, adc_idp)    opM(0x73, adc_isry)
  opM(0x74, stz_dpr)     opM(0x75, adc_dpr)    opM(0x76, ror_dpx)    opM(0x77, adc_ildpy)
  opA(0x78, sei)         opM(0x79, adc_addry)  opX(0x7a, ply)        opA(0x7b, tdc)
  opA(0x7c, jmp_iaddrx)  opM(0x7d, adc_addrx)  opM(0x7e, ror_addrx)  opM(0x7f, adc_longx)

  opA(0x80, bra)         opM(0x81, sta_idpx)   opA(0x82, brl)        opM(0x83, sta_sr)
  opX(0x84, sty_dp)      opM(0x85, sta_dp)     opX(0x86, stx_dp)     opM(0x87, sta_ildp)
  opX(0x88, dey_imm)     opM(0x89, bit_const)  opM(0x8a, txa)        opA(0x8b, phb)
  opX(0x8c, sty_addr)    opM(0x8d, sta_addr)   opX(0x8e, stx_addr)   opM(0x8f, sta_long)

  opA(0x90, bcc)         opM(0x91, sta_idpy)   opM(0x92, sta_idp)    opM(0x93, sta_isry)
  opX(0x94, sty_dpr)     opM(0x95, sta_dpr)    opX(0x96, stx_dpr)    opM(0x97, sta_ildpy)
  opM(0x98, tya)         opM(0x99, sta_addry)  opE(0x9a, txs)        opX(0x9b, txy)
  opM(0x9c, stz_addr)    opM(0x9d, sta_addrx)  opM(0x9e, stz_addrx)  opM(0x9f, sta_longx)

  opX(0xa0, ldy_const)   opM(0xa1, lda_idpx)   opX(0xa2, ldx_const)  opM(0xa3, lda_sr)
  opX(0xa4, ldy_dp)      opM(0xa5, lda_dp)     opX(0xa6, ldx_dp)     opM(0xa7, lda_ildp)
  opX(0xa8, tay)         opM(0xa9, lda_const)  opX(0xaa, tax)        opA(0xab, plb)
  opX(0xac, ldy_addr)    opM(0xad, lda_addr)   opX(0xae, ldx_addr)   opM(0xaf, lda_long)

  opA(0xb0, bcs)         opM(0xb1, lda_idpy)   opM(0xb2, lda_idp)    opM(0xb3, lda_isry)
  opX(0xb4, ldy_dpr)     opM(0xb5, lda_dpr)    opX(0xb6, ldx_dpr)    opM(0xb7, lda_ildpy)
  opA(0xb8, clv)         opM(0xb9, lda_addry)  opX(0xba, tsx)        opX(0xbb, tyx)
  opX(0xbc, ldy_addrx)   opM(0xbd, lda_addrx)  opX(0xbe, ldx_addry)  opM(0xbf, lda_longx)

  opX(0xc0, cpy_const)   opM(0xc1, cmp_idpx)   opE(0xc2, rep)        opM(0xc3, cmp_sr)
  opX(0xc4, cpy_dp)      opM(0xc5, cmp_dp)     opM(0xc6, dec_dp)     opM(0xc7, cmp_ildp)
  opX(0xc8, iny_imm)     opM(0xc9, cmp_const)  opX(0xca, dex_imm)    opA(0xcb, wai)
  opX(0xcc, cpy_addr)    opM(0xcd, cmp_addr)   opM(0xce, dec_addr)   opM(0xcf, cmp_long)

  opA(0xd0, bne)         opM(0xd1, cmp_idpy)   opM(0xd2, cmp_idp)    opM(0xd3, cmp_isry)
  opE(0xd4, pei)         opM(0xd5, cmp_dpr)    opM(0xd6, dec_dpx)    opM(0xd7, cmp_ildpy)
  opA(0xd8, cld)         opM(0xd9, cmp_addry)  opX(0xda, phx)        opA(0xdb, stp)
  opA(0xdc, jmp_iladdr)  opM(0xdd, cmp_addrx)  opM(0xde, dec_addrx)  opM(0xdf, cmp_longx)

  opX(0xe0, cpx_const)   opM(0xe1, sbc_idpx)   opE(0xe2, sep)        opM(0xe3, sbc_sr)
  opX(0xe4, cpx_dp)      opM(0xe5, sbc_dp)     opM(0xe6, inc_dp)     opM(0xe7, sbc_ildp)
  opX(0xe8, inx_imm)     opM(0xe9, sbc_const)  opA(0xea, nop)        opA(0xeb, xba)
  opX(0xec, cpx_addr)    opM(0xed, sbc_addr)   opM(0xee, inc_addr)   opM(0xef, sbc_long)

  opA(0xf0, beq)         opM(0xf1, sbc_idpy)   opM(0xf2, sbc_idp)    opM(0xf3, sbc_isry)
  opE(0xf4, pea)         opM(0xf5, sbc_dpr)    opM(0xf6, inc_dpx)    opM(0xf7, sbc_ildpy)
  opA(0xf8, sed)         opM(0xf9, sbc_addry)  opX(0xfa, plx)        opA(0xfb, xce)
  opE(0xfc, jsr_iaddrx)  opM(0xfd, sbc_addrx)  opM(0xfe, inc_addrx)  opM(0xff, sbc_longx)

  #undef opA
  #undef opE
  #undef opM
  #undef opX
}

void CPUcore::update_table() {
  if(regs.e) {
    opcode_table = &op_table[table_EM];
  } else if(regs.p.m) {
    if(regs.p.x) {
      opcode_table = &op_table[table_MX];
    } else {
      opcode_table = &op_table[table_Mx];
    }
  } else {
    if(regs.p.x) {
      opcode_table = &op_table[table_mX];
    } else {
      opcode_table = &op_table[table_mx];
    }
  }
}

#endif

