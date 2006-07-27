adc_const(0x69, adc, regs.acc_8b),
and_const(0x29, and, regs.acc_8b),
cmp_const(0xc9, cmp, regs.acc_8b),
cpx_const(0xe0, cpx, regs.idx_8b),
cpy_const(0xc0, cpy, regs.idx_8b),
eor_const(0x49, eor, regs.acc_8b),
lda_const(0xa9, lda, regs.acc_8b),
ldx_const(0xa2, ldx, regs.idx_8b),
ldy_const(0xa0, ldy, regs.idx_8b),
ora_const(0x09, ora, regs.acc_8b),
sbc_const(0xe9, sbc, regs.acc_8b) {
1:if($2)last_cycle();
  rd.l = op_readpc();
  if($2) { op_$1_b(); end; }
2:last_cycle();
  rd.h = op_readpc();
  op_$1_w();
}

adc_addr(0x6d, adc, regs.acc_8b),
and_addr(0x2d, and, regs.acc_8b),
bit_addr(0x2c, bit, regs.acc_8b),
cmp_addr(0xcd, cmp, regs.acc_8b),
cpx_addr(0xec, cpx, regs.idx_8b),
cpy_addr(0xcc, cpy, regs.idx_8b),
eor_addr(0x4d, eor, regs.acc_8b),
lda_addr(0xad, lda, regs.acc_8b),
ldx_addr(0xae, ldx, regs.idx_8b),
ldy_addr(0xac, ldy, regs.idx_8b),
ora_addr(0x0d, ora, regs.acc_8b),
sbc_addr(0xed, sbc, regs.acc_8b) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:if($2)last_cycle();
  rd.l = op_readdbr(aa.w);
  if($2) { op_$1_b(); end; }
4:last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_$1_w();
}

adc_addrx(0x7d, adc, regs.acc_8b),
and_addrx(0x3d, and, regs.acc_8b),
bit_addrx(0x3c, bit, regs.acc_8b),
cmp_addrx(0xdd, cmp, regs.acc_8b),
eor_addrx(0x5d, eor, regs.acc_8b),
lda_addrx(0xbd, lda, regs.acc_8b),
ldy_addrx(0xbc, ldy, regs.idx_8b),
ora_addrx(0x1d, ora, regs.acc_8b),
sbc_addrx(0xfd, sbc, regs.acc_8b) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_io_cond4(aa.w, aa.w + regs.x.w);
4:if($2)last_cycle();
  rd.l = op_readdbr(aa.w + regs.x.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_readdbr(aa.w + regs.x.w + 1);
  op_$1_w();
}

adc_addry(0x79, adc, regs.acc_8b),
and_addry(0x39, and, regs.acc_8b),
cmp_addry(0xd9, cmp, regs.acc_8b),
eor_addry(0x59, eor, regs.acc_8b),
lda_addry(0xb9, lda, regs.acc_8b),
ldx_addry(0xbe, ldx, regs.idx_8b),
ora_addry(0x19, ora, regs.acc_8b),
sbc_addry(0xf9, sbc, regs.acc_8b) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:op_io_cond4(aa.w, aa.w + regs.y.w);
4:if($2)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_$1_w();
}

adc_long(0x6f, adc, regs.acc_8b),
and_long(0x2f, and, regs.acc_8b),
cmp_long(0xcf, cmp, regs.acc_8b),
eor_long(0x4f, eor, regs.acc_8b),
lda_long(0xaf, lda, regs.acc_8b),
ora_long(0x0f, ora, regs.acc_8b),
sbc_long(0xef, sbc, regs.acc_8b) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:aa.b = op_readpc();
4:if($2)last_cycle();
  rd.l = op_readlong(aa.d);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_$1_w();
}

adc_longx(0x7f, adc, regs.acc_8b),
and_longx(0x3f, and, regs.acc_8b),
cmp_longx(0xdf, cmp, regs.acc_8b),
eor_longx(0x5f, eor, regs.acc_8b),
lda_longx(0xbf, lda, regs.acc_8b),
ora_longx(0x1f, ora, regs.acc_8b),
sbc_longx(0xff, sbc, regs.acc_8b) {
1:aa.l = op_readpc();
2:aa.h = op_readpc();
3:aa.b = op_readpc();
4:if($2)last_cycle();
  rd.l = op_readlong(aa.d + regs.x.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_readlong(aa.d + regs.x.w + 1);
  op_$1_w();
}

adc_dp(0x65, adc, regs.acc_8b),
and_dp(0x25, and, regs.acc_8b),
bit_dp(0x24, bit, regs.acc_8b),
cmp_dp(0xc5, cmp, regs.acc_8b),
cpx_dp(0xe4, cpx, regs.idx_8b),
cpy_dp(0xc4, cpy, regs.idx_8b),
eor_dp(0x45, eor, regs.acc_8b),
lda_dp(0xa5, lda, regs.acc_8b),
ldx_dp(0xa6, ldx, regs.idx_8b),
ldy_dp(0xa4, ldy, regs.idx_8b),
ora_dp(0x05, ora, regs.acc_8b),
sbc_dp(0xe5, sbc, regs.acc_8b) {
1:dp = op_readpc();
2:op_io_cond2();
3:if($2)last_cycle();
  rd.l = op_readdp(dp);
  if($2) { op_$1_b(); end; }
4:last_cycle();
  rd.h = op_readdp(dp + 1);
  op_$1_w();
}

adc_dpx(0x75, adc, regs.acc_8b),
and_dpx(0x35, and, regs.acc_8b),
bit_dpx(0x34, bit, regs.acc_8b),
cmp_dpx(0xd5, cmp, regs.acc_8b),
eor_dpx(0x55, eor, regs.acc_8b),
lda_dpx(0xb5, lda, regs.acc_8b),
ldy_dpx(0xb4, ldy, regs.idx_8b),
ora_dpx(0x15, ora, regs.acc_8b),
sbc_dpx(0xf5, sbc, regs.acc_8b) {
1:dp = op_readpc();
2:op_io_cond2();
3:op_io();
4:if($2)last_cycle();
  rd.l = op_readdp(dp + regs.x.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_readdp(dp + regs.x.w + 1);
  op_$1_w();
}

ldx_dpy(0xb6, ldx, regs.idx_8b) {
1:dp = op_readpc();
2:op_io_cond2();
3:op_io();
4:if($2)last_cycle();
  rd.l = op_readdp(dp + regs.y.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_readdp(dp + regs.y.w + 1);
  op_$1_w();
}

adc_idp(0x72, adc, regs.acc_8b),
and_idp(0x32, and, regs.acc_8b),
cmp_idp(0xd2, cmp, regs.acc_8b),
eor_idp(0x52, eor, regs.acc_8b),
lda_idp(0xb2, lda, regs.acc_8b),
ora_idp(0x12, ora, regs.acc_8b),
sbc_idp(0xf2, sbc, regs.acc_8b) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:if($2)last_cycle();
  rd.l = op_readdbr(aa.w);
  if($2) { op_$1_b(); end; }
6:last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_$1_w();
}

adc_idpx(0x61, adc, regs.acc_8b),
and_idpx(0x21, and, regs.acc_8b),
cmp_idpx(0xc1, cmp, regs.acc_8b),
eor_idpx(0x41, eor, regs.acc_8b),
lda_idpx(0xa1, lda, regs.acc_8b),
ora_idpx(0x01, ora, regs.acc_8b),
sbc_idpx(0xe1, sbc, regs.acc_8b) {
1:dp = op_readpc();
2:op_io_cond2();
3:op_io();
4:aa.l = op_readdp(dp + regs.x.w);
5:aa.h = op_readdp(dp + regs.x.w + 1);
6:if($2)last_cycle();
  rd.l = op_readdbr(aa.w);
  if($2) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_readdbr(aa.w + 1);
  op_$1_w();
}

adc_idpy(0x71, adc, regs.acc_8b),
and_idpy(0x31, and, regs.acc_8b),
cmp_idpy(0xd1, cmp, regs.acc_8b),
eor_idpy(0x51, eor, regs.acc_8b),
lda_idpy(0xb1, lda, regs.acc_8b),
ora_idpy(0x11, ora, regs.acc_8b),
sbc_idpy(0xf1, sbc, regs.acc_8b) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:op_io_cond4(aa.w, aa.w + regs.y.w);
6:if($2)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if($2) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_$1_w();
}

adc_ildp(0x67, adc, regs.acc_8b),
and_ildp(0x27, and, regs.acc_8b),
cmp_ildp(0xc7, cmp, regs.acc_8b),
eor_ildp(0x47, eor, regs.acc_8b),
lda_ildp(0xa7, lda, regs.acc_8b),
ora_ildp(0x07, ora, regs.acc_8b),
sbc_ildp(0xe7, sbc, regs.acc_8b) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:aa.b = op_readdp(dp + 2);
6:if($2)last_cycle();
  rd.l = op_readlong(aa.d);
  if($2) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_readlong(aa.d + 1);
  op_$1_w();
}

adc_ildpy(0x77, adc, regs.acc_8b),
and_ildpy(0x37, and, regs.acc_8b),
cmp_ildpy(0xd7, cmp, regs.acc_8b),
eor_ildpy(0x57, eor, regs.acc_8b),
lda_ildpy(0xb7, lda, regs.acc_8b),
ora_ildpy(0x17, ora, regs.acc_8b),
sbc_ildpy(0xf7, sbc, regs.acc_8b) {
1:dp = op_readpc();
2:op_io_cond2();
3:aa.l = op_readdp(dp);
4:aa.h = op_readdp(dp + 1);
5:aa.b = op_readdp(dp + 2);
6:if($2)last_cycle();
  rd.l = op_readlong(aa.d + regs.y.w);
  if($2) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_readlong(aa.d + regs.y.w + 1);
  op_$1_w();
}

adc_sr(0x63, adc, regs.acc_8b),
and_sr(0x23, and, regs.acc_8b),
cmp_sr(0xc3, cmp, regs.acc_8b),
eor_sr(0x43, eor, regs.acc_8b),
lda_sr(0xa3, lda, regs.acc_8b),
ora_sr(0x03, ora, regs.acc_8b),
sbc_sr(0xe3, sbc, regs.acc_8b) {
1:sp = op_readpc();
2:op_io();
3:if($2)last_cycle();
  rd.l = op_readsp(sp);
  if($2) { op_$1_b(); end; }
4:last_cycle();
  rd.h = op_readsp(sp + 1);
  op_$1_w();
}

adc_isry(0x73, adc),
and_isry(0x33, and),
cmp_isry(0xd3, cmp),
eor_isry(0x53, eor),
lda_isry(0xb3, lda),
ora_isry(0x13, ora),
sbc_isry(0xf3, sbc) {
1:sp = op_readpc();
2:op_io();
3:aa.l = op_readsp(sp);
4:aa.h = op_readsp(sp + 1);
5:op_io();
6:if(regs.acc_8b)last_cycle();
  rd.l = op_readdbr(aa.w + regs.y.w);
  if(regs.acc_8b) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_readdbr(aa.w + regs.y.w + 1);
  op_$1_w();
}

bit_const(0x89) {
1:if(regs.acc_8b)last_cycle();
  rd.l = op_readpc();
  if(regs.acc_8b) {
    regs.p.z = ((rd.l & regs.a.l) == 0);
    end;
  }
2:last_cycle();
  rd.h = op_readpc();
  regs.p.z = ((rd.w & regs.a.w) == 0);
}
