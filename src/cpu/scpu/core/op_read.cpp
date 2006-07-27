//adc_const
case 0x69: {
  if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_adc_w();
} break;

//and_const
case 0x29: {
  if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_and_w();
} break;

//cmp_const
case 0xc9: {
  if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_cmp_w();
} break;

//cpx_const
case 0xe0: {
  if(regs.idx_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.idx_8b) { op_cpx_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_cpx_w();
} break;

//cpy_const
case 0xc0: {
  if(regs.idx_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.idx_8b) { op_cpy_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_cpy_w();
} break;

//eor_const
case 0x49: {
  if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_eor_w();
} break;

//lda_const
case 0xa9: {
  if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_lda_w();
} break;

//ldx_const
case 0xa2: {
  if(regs.idx_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.idx_8b) { op_ldx_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_ldx_w();
} break;

//ldy_const
case 0xa0: {
  if(regs.idx_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.idx_8b) { op_ldy_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_ldy_w();
} break;

//ora_const
case 0x09: {
  if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_ora_w();
} break;

//sbc_const
case 0xe9: {
  if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readpc();
  op_sbc_w();
} break;

//adc_addr
case 0x6d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_adc_w();
} break;

//and_addr
case 0x2d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_and_w();
} break;

//bit_addr
case 0x2c: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_bit_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_bit_w();
} break;

//cmp_addr
case 0xcd: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cmp_w();
} break;

//cpx_addr
case 0xec: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.idx_8b) { op_cpx_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cpx_w();
} break;

//cpy_addr
case 0xcc: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.idx_8b) { op_cpy_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cpy_w();
} break;

//eor_addr
case 0x4d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_eor_w();
} break;

//lda_addr
case 0xad: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_lda_w();
} break;

//ldx_addr
case 0xae: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.idx_8b) { op_ldx_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ldx_w();
} break;

//ldy_addr
case 0xac: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.idx_8b) { op_ldy_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ldy_w();
} break;

//ora_addr
case 0x0d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ora_w();
} break;

//sbc_addr
case 0xed: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_sbc_w();
} break;

//adc_addrx
case 0x7d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_adc_w();
} break;

//and_addrx
case 0x3d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_and_w();
} break;

//bit_addrx
case 0x3c: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.acc_8b) { op_bit_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_bit_w();
} break;

//cmp_addrx
case 0xdd: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_cmp_w();
} break;

//eor_addrx
case 0x5d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_eor_w();
} break;

//lda_addrx
case 0xbd: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_lda_w();
} break;

//ldy_addrx
case 0xbc: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.idx_8b) { op_ldy_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_ldy_w();
} break;

//ora_addrx
case 0x1d: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_ora_w();
} break;

//sbc_addrx
case 0xfd: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.x.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_sbc_w();
} break;

//adc_addry
case 0x79: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_adc_w();
} break;

//and_addry
case 0x39: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_and_w();
} break;

//cmp_addry
case 0xd9: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_cmp_w();
} break;

//eor_addry
case 0x59: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_eor_w();
} break;

//lda_addry
case 0xb9: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_lda_w();
} break;

//ldx_addry
case 0xbe: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.idx_8b) { op_ldx_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_ldx_w();
} break;

//ora_addry
case 0x19: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_ora_w();
} break;

//sbc_addry
case 0xf9: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_sbc_w();
} break;

//adc_long
case 0x6f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_adc_w();
} break;

//and_long
case 0x2f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_and_w();
} break;

//cmp_long
case 0xcf: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_cmp_w();
} break;

//eor_long
case 0x4f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_eor_w();
} break;

//lda_long
case 0xaf: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_lda_w();
} break;

//ora_long
case 0x0f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_ora_w();
} break;

//sbc_long
case 0xef: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_sbc_w();
} break;

//adc_longx
case 0x7f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_adc_w();
} break;

//and_longx
case 0x3f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_and_w();
} break;

//cmp_longx
case 0xdf: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_cmp_w();
} break;

//eor_longx
case 0x5f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_eor_w();
} break;

//lda_longx
case 0xbf: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_lda_w();
} break;

//ora_longx
case 0x1f: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_ora_w();
} break;

//sbc_longx
case 0xff: {
  aa.l = op_readpc();
  aa.h = op_readpc();
  aa.b = op_readpc();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_sbc_w();
} break;

//adc_dp
case 0x65: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_adc_w();
} break;

//and_dp
case 0x25: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_and_w();
} break;

//bit_dp
case 0x24: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.acc_8b) { op_bit_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_bit_w();
} break;

//cmp_dp
case 0xc5: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_cmp_w();
} break;

//cpx_dp
case 0xe4: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.idx_8b) { op_cpx_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_cpx_w();
} break;

//cpy_dp
case 0xc4: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.idx_8b) { op_cpy_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_cpy_w();
} break;

//eor_dp
case 0x45: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_eor_w();
} break;

//lda_dp
case 0xa5: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_lda_w();
} break;

//ldx_dp
case 0xa6: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.idx_8b) { op_ldx_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_ldx_w();
} break;

//ldy_dp
case 0xa4: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.idx_8b) { op_ldy_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_ldy_w();
} break;

//ora_dp
case 0x05: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_ora_w();
} break;

//sbc_dp
case 0xe5: {
  dp = op_readpc();
  op_io_cond2();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + 1);
  op_sbc_w();
} break;

//adc_dpx
case 0x75: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_adc_w();
} break;

//and_dpx
case 0x35: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_and_w();
} break;

//bit_dpx
case 0x34: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.acc_8b) { op_bit_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_bit_w();
} break;

//cmp_dpx
case 0xd5: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_cmp_w();
} break;

//eor_dpx
case 0x55: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_eor_w();
} break;

//lda_dpx
case 0xb5: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_lda_w();
} break;

//ldy_dpx
case 0xb4: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.idx_8b) { op_ldy_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_ldy_w();
} break;

//ora_dpx
case 0x15: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_ora_w();
} break;

//sbc_dpx
case 0xf5: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_sbc_w();
} break;

//ldx_dpy
case 0xb6: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  if(regs.idx_8b)last_cycle();
  rd.l = op_readdp(dp + regs.y.w);
  if(regs.idx_8b) { op_ldx_b(); break; }
  last_cycle();
  rd.h = op_readdp(dp + regs.y.w + 1);
  op_ldx_w();
} break;

//adc_idp
case 0x72: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_adc_w();
} break;

//and_idp
case 0x32: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_and_w();
} break;

//cmp_idp
case 0xd2: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cmp_w();
} break;

//eor_idp
case 0x52: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_eor_w();
} break;

//lda_idp
case 0xb2: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_lda_w();
} break;

//ora_idp
case 0x12: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ora_w();
} break;

//sbc_idp
case 0xf2: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_sbc_w();
} break;

//adc_idpx
case 0x61: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_adc_w();
} break;

//and_idpx
case 0x21: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_and_w();
} break;

//cmp_idpx
case 0xc1: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_cmp_w();
} break;

//eor_idpx
case 0x41: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_eor_w();
} break;

//lda_idpx
case 0xa1: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_lda_w();
} break;

//ora_idpx
case 0x01: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_ora_w();
} break;

//sbc_idpx
case 0xe1: {
  dp = op_readpc();
  op_io_cond2();
  op_io();
  aa.l = op_readdp(dp + regs.x.w);
  aa.h = op_readdp(dp + regs.x.w + 1);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_sbc_w();
} break;

//adc_idpy
case 0x71: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_adc_w();
} break;

//and_idpy
case 0x31: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_and_w();
} break;

//cmp_idpy
case 0xd1: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_cmp_w();
} break;

//eor_idpy
case 0x51: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_eor_w();
} break;

//lda_idpy
case 0xb1: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_lda_w();
} break;

//ora_idpy
case 0x11: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_ora_w();
} break;

//sbc_idpy
case 0xf1: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_io_cond4(aa.w, aa.w + regs.y.w);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_sbc_w();
} break;

//adc_ildp
case 0x67: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_adc_w();
} break;

//and_ildp
case 0x27: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_and_w();
} break;

//cmp_ildp
case 0xc7: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_cmp_w();
} break;

//eor_ildp
case 0x47: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_eor_w();
} break;

//lda_ildp
case 0xa7: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_lda_w();
} break;

//ora_ildp
case 0x07: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_ora_w();
} break;

//sbc_ildp
case 0xe7: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_sbc_w();
} break;

//adc_ildpy
case 0x77: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_adc_w();
} break;

//and_ildpy
case 0x37: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_and_w();
} break;

//cmp_ildpy
case 0xd7: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_cmp_w();
} break;

//eor_ildpy
case 0x57: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_eor_w();
} break;

//lda_ildpy
case 0xb7: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_lda_w();
} break;

//ora_ildpy
case 0x17: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_ora_w();
} break;

//sbc_ildpy
case 0xf7: {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  aa.b = op_readdp(dp + 2);
  if(regs.acc_8b)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_sbc_w();
} break;

//adc_sr
case 0x63: {
  sp = op_readpc();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_adc_w();
} break;

//and_sr
case 0x23: {
  sp = op_readpc();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_and_w();
} break;

//cmp_sr
case 0xc3: {
  sp = op_readpc();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_cmp_w();
} break;

//eor_sr
case 0x43: {
  sp = op_readpc();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_eor_w();
} break;

//lda_sr
case 0xa3: {
  sp = op_readpc();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_lda_w();
} break;

//ora_sr
case 0x03: {
  sp = op_readpc();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_ora_w();
} break;

//sbc_sr
case 0xe3: {
  sp = op_readpc();
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readsp(sp);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readsp(sp + 1);
  op_sbc_w();
} break;

//adc_isry
case 0x73: {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_adc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_adc_w();
} break;

//and_isry
case 0x33: {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_and_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_and_w();
} break;

//cmp_isry
case 0xd3: {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_cmp_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_cmp_w();
} break;

//eor_isry
case 0x53: {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_eor_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_eor_w();
} break;

//lda_isry
case 0xb3: {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_lda_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_lda_w();
} break;

//ora_isry
case 0x13: {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_ora_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_ora_w();
} break;

//sbc_isry
case 0xf3: {
  sp = op_readpc();
  op_io();
  aa.l = op_readsp(sp);
  aa.h = op_readsp(sp + 1);
  op_io();
  if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_sbc_b(); break; }
  last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_sbc_w();
} break;

//bit_const
case 0x89: {
  if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) {
    regs.p.z = ((rd.l & regs.a.l) == 0);
    break;
  }
  last_cycle();
  rd.h = op_readpc();
  regs.p.z = ((rd.w & regs.a.w) == 0);
} break;

