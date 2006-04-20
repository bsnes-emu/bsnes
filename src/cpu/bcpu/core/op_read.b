adc_const(0x69, adc, regs.p.m),
and_const(0x29, and, regs.p.m),
cmp_const(0xc9, cmp, regs.p.m),
cpx_const(0xe0, cpx, regs.p.x),
cpy_const(0xc0, cpy, regs.p.x),
eor_const(0x49, eor, regs.p.m),
lda_const(0xa9, lda, regs.p.m),
ldx_const(0xa2, ldx, regs.p.x),
ldy_const(0xa0, ldy, regs.p.x),
ora_const(0x09, ora, regs.p.m),
sbc_const(0xe9, sbc, regs.p.m) {
1:if($2)last_cycle();
  rd.l = op_read();
  if($2) { op_$1_b(); end; }
2:last_cycle();
  rd.h = op_read();
  op_$1_w();
}

adc_addr(0x6d, adc, regs.p.m),
and_addr(0x2d, and, regs.p.m),
bit_addr(0x2c, bit, regs.p.m),
cmp_addr(0xcd, cmp, regs.p.m),
cpx_addr(0xec, cpx, regs.p.x),
cpy_addr(0xcc, cpy, regs.p.x),
eor_addr(0x4d, eor, regs.p.m),
lda_addr(0xad, lda, regs.p.m),
ldx_addr(0xae, ldx, regs.p.x),
ldy_addr(0xac, ldy, regs.p.x),
ora_addr(0x0d, ora, regs.p.m),
sbc_addr(0xed, sbc, regs.p.m) {
1:aa.l = op_read();
2:aa.h = op_read();
3:if($2)last_cycle();
  rd.l = op_read(OPMODE_DBR, aa.w);
  if($2) { op_$1_b(); end; }
4:last_cycle();
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_$1_w();
}

adc_addrx(0x7d, adc, regs.p.m),
and_addrx(0x3d, and, regs.p.m),
bit_addrx(0x3c, bit, regs.p.m),
cmp_addrx(0xdd, cmp, regs.p.m),
eor_addrx(0x5d, eor, regs.p.m),
lda_addrx(0xbd, lda, regs.p.m),
ldy_addrx(0xbc, ldy, regs.p.x),
ora_addrx(0x1d, ora, regs.p.m),
sbc_addrx(0xfd, sbc, regs.p.m) {
1:aa.l = op_read();
2:aa.h = op_read();
3:cpu_c4(aa.w, aa.w + regs.x.w);
4:if($2)last_cycle();
  rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
  op_$1_w();
}

adc_addry(0x79, adc, regs.p.m),
and_addry(0x39, and, regs.p.m),
cmp_addry(0xd9, cmp, regs.p.m),
eor_addry(0x59, eor, regs.p.m),
lda_addry(0xb9, lda, regs.p.m),
ldx_addry(0xbe, ldx, regs.p.x),
ora_addry(0x19, ora, regs.p.m),
sbc_addry(0xf9, sbc, regs.p.m) {
1:aa.l = op_read();
2:aa.h = op_read();
3:cpu_c4(aa.w, aa.w + regs.y.w);
4:if($2)last_cycle();
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_$1_w();
}

adc_long(0x6f, adc, regs.p.m),
and_long(0x2f, and, regs.p.m),
cmp_long(0xcf, cmp, regs.p.m),
eor_long(0x4f, eor, regs.p.m),
lda_long(0xaf, lda, regs.p.m),
ora_long(0x0f, ora, regs.p.m),
sbc_long(0xef, sbc, regs.p.m) {
1:aa.l = op_read();
2:aa.h = op_read();
3:aa.b = op_read();
4:if($2)last_cycle();
  rd.l = op_read(OPMODE_LONG, aa.d);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_$1_w();
}

adc_longx(0x7f, adc, regs.p.m),
and_longx(0x3f, and, regs.p.m),
cmp_longx(0xdf, cmp, regs.p.m),
eor_longx(0x5f, eor, regs.p.m),
lda_longx(0xbf, lda, regs.p.m),
ora_longx(0x1f, ora, regs.p.m),
sbc_longx(0xff, sbc, regs.p.m) {
1:aa.l = op_read();
2:aa.h = op_read();
3:aa.b = op_read();
4:if($2)last_cycle();
  rd.l = op_read(OPMODE_LONG, aa.d + regs.x.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1);
  op_$1_w();
}

adc_dp(0x65, adc, regs.p.m),
and_dp(0x25, and, regs.p.m),
bit_dp(0x24, bit, regs.p.m),
cmp_dp(0xc5, cmp, regs.p.m),
cpx_dp(0xe4, cpx, regs.p.x),
cpy_dp(0xc4, cpy, regs.p.x),
eor_dp(0x45, eor, regs.p.m),
lda_dp(0xa5, lda, regs.p.m),
ldx_dp(0xa6, ldx, regs.p.x),
ldy_dp(0xa4, ldy, regs.p.x),
ora_dp(0x05, ora, regs.p.m),
sbc_dp(0xe5, sbc, regs.p.m) {
1:dp = op_read();
2:cpu_c2();
3:if($2)last_cycle();
  rd.l = op_read(OPMODE_DP, dp);
  if($2) { op_$1_b(); end; }
4:last_cycle();
  rd.h = op_read(OPMODE_DP, dp + 1);
  op_$1_w();
}

adc_dpx(0x75, adc, regs.p.m),
and_dpx(0x35, and, regs.p.m),
bit_dpx(0x34, bit, regs.p.m),
cmp_dpx(0xd5, cmp, regs.p.m),
eor_dpx(0x55, eor, regs.p.m),
lda_dpx(0xb5, lda, regs.p.m),
ldy_dpx(0xb4, ldy, regs.p.x),
ora_dpx(0x15, ora, regs.p.m),
sbc_dpx(0xf5, sbc, regs.p.m) {
1:dp = op_read();
2:cpu_c2();
3:cpu_io();
4:if($2)last_cycle();
  rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
  op_$1_w();
}

ldx_dpy(0xb6, ldx, regs.p.x) {
1:dp = op_read();
2:cpu_c2();
3:cpu_io();
4:if($2)last_cycle();
  rd.l = op_read(OPMODE_DP, dp + regs.y.w);
  if($2) { op_$1_b(); end; }
5:last_cycle();
  rd.h = op_read(OPMODE_DP, dp + regs.y.w + 1);
  op_$1_w();
}

adc_idp(0x72, adc, regs.p.m),
and_idp(0x32, and, regs.p.m),
cmp_idp(0xd2, cmp, regs.p.m),
eor_idp(0x52, eor, regs.p.m),
lda_idp(0xb2, lda, regs.p.m),
ora_idp(0x12, ora, regs.p.m),
sbc_idp(0xf2, sbc, regs.p.m) {
1:dp = op_read();
2:cpu_c2();
3:aa.l = op_read(OPMODE_DP, dp);
4:aa.h = op_read(OPMODE_DP, dp + 1);
5:if($2)last_cycle();
  rd.l = op_read(OPMODE_DBR, aa.w);
  if($2) { op_$1_b(); end; }
6:last_cycle();
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_$1_w();
}

adc_idpx(0x61, adc, regs.p.m),
and_idpx(0x21, and, regs.p.m),
cmp_idpx(0xc1, cmp, regs.p.m),
eor_idpx(0x41, eor, regs.p.m),
lda_idpx(0xa1, lda, regs.p.m),
ora_idpx(0x01, ora, regs.p.m),
sbc_idpx(0xe1, sbc, regs.p.m) {
1:dp = op_read();
2:cpu_c2();
3:cpu_io();
4:aa.l = op_read(OPMODE_DP, dp + regs.x.w);
5:aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
6:if($2)last_cycle();
  rd.l = op_read(OPMODE_DBR, aa.w);
  if($2) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_read(OPMODE_DBR, aa.w + 1);
  op_$1_w();
}

adc_idpy(0x71, adc, regs.p.m),
and_idpy(0x31, and, regs.p.m),
cmp_idpy(0xd1, cmp, regs.p.m),
eor_idpy(0x51, eor, regs.p.m),
lda_idpy(0xb1, lda, regs.p.m),
ora_idpy(0x11, ora, regs.p.m),
sbc_idpy(0xf1, sbc, regs.p.m) {
1:dp = op_read();
2:cpu_c2();
3:aa.l = op_read(OPMODE_DP, dp);
4:aa.h = op_read(OPMODE_DP, dp + 1);
5:cpu_c4(aa.w, aa.w + regs.y.w);
6:if($2)last_cycle();
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if($2) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_$1_w();
}

adc_ildp(0x67, adc, regs.p.m),
and_ildp(0x27, and, regs.p.m),
cmp_ildp(0xc7, cmp, regs.p.m),
eor_ildp(0x47, eor, regs.p.m),
lda_ildp(0xa7, lda, regs.p.m),
ora_ildp(0x07, ora, regs.p.m),
sbc_ildp(0xe7, sbc, regs.p.m) {
1:dp = op_read();
2:cpu_c2();
3:aa.l = op_read(OPMODE_DP, dp);
4:aa.h = op_read(OPMODE_DP, dp + 1);
5:aa.b = op_read(OPMODE_DP, dp + 2);
6:if($2)last_cycle();
  rd.l = op_read(OPMODE_LONG, aa.d);
  if($2) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_read(OPMODE_LONG, aa.d + 1);
  op_$1_w();
}

adc_ildpy(0x77, adc, regs.p.m),
and_ildpy(0x37, and, regs.p.m),
cmp_ildpy(0xd7, cmp, regs.p.m),
eor_ildpy(0x57, eor, regs.p.m),
lda_ildpy(0xb7, lda, regs.p.m),
ora_ildpy(0x17, ora, regs.p.m),
sbc_ildpy(0xf7, sbc, regs.p.m) {
1:dp = op_read();
2:cpu_c2();
3:aa.l = op_read(OPMODE_DP, dp);
4:aa.h = op_read(OPMODE_DP, dp + 1);
5:aa.b = op_read(OPMODE_DP, dp + 2);
6:if($2)last_cycle();
  rd.l = op_read(OPMODE_LONG, aa.d + regs.y.w);
  if($2) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1);
  op_$1_w();
}

adc_sr(0x63, adc, regs.p.m),
and_sr(0x23, and, regs.p.m),
cmp_sr(0xc3, cmp, regs.p.m),
eor_sr(0x43, eor, regs.p.m),
lda_sr(0xa3, lda, regs.p.m),
ora_sr(0x03, ora, regs.p.m),
sbc_sr(0xe3, sbc, regs.p.m) {
1:sp = op_read();
2:cpu_io();
3:if($2)last_cycle();
  rd.l = op_read(OPMODE_SP, sp);
  if($2) { op_$1_b(); end; }
4:last_cycle();
  rd.h = op_read(OPMODE_SP, sp + 1);
  op_$1_w();
}

adc_isry(0x73, adc),
and_isry(0x33, and),
cmp_isry(0xd3, cmp),
eor_isry(0x53, eor),
lda_isry(0xb3, lda),
ora_isry(0x13, ora),
sbc_isry(0xf3, sbc) {
1:sp = op_read();
2:cpu_io();
3:aa.l = op_read(OPMODE_SP, sp);
4:aa.h = op_read(OPMODE_SP, sp + 1);
5:cpu_io();
6:if(regs.p.m)last_cycle();
  rd.l = op_read(OPMODE_DBR, aa.w + regs.y.w);
  if(regs.p.m) { op_$1_b(); end; }
7:last_cycle();
  rd.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1);
  op_$1_w();
}

bit_const(0x89) {
1:if(regs.p.m)last_cycle();
  rd.l = op_read();
  if(regs.p.m) {
    regs.p.z = ((rd.l & regs.a.l) == 0);
    end;
  }
2:last_cycle();
  rd.h = op_read();
  regs.p.z = ((rd.w & regs.a.w) == 0);
}
