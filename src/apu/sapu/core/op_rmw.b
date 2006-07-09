inc_a(0xbc, inc, a),
inc_x(0x3d, inc, x),
inc_y(0xfc, inc, y),
dec_a(0x9c, dec, a),
dec_x(0x1d, dec, x),
dec_y(0xdc, dec, y),
asl_a(0x1c, asl, a),
lsr_a(0x5c, lsr, a),
rol_a(0x3c, rol, a),
ror_a(0x7c, ror, a) {
1:op_io();
  regs.$2 = op_$1(regs.$2);
}

inc_dp(0xab, inc),
dec_dp(0x8b, dec),
asl_dp(0x0b, asl),
lsr_dp(0x4b, lsr),
rol_dp(0x2b, rol),
ror_dp(0x6b, ror) {
1:dp = op_readpc();
2:rd = op_readdp(dp);
3:rd = op_$1(rd);
  op_writedp(dp, rd);
}

inc_dpx(0xbb, inc),
dec_dpx(0x9b, dec),
asl_dpx(0x1b, asl),
lsr_dpx(0x5b, lsr),
rol_dpx(0x3b, rol),
ror_dpx(0x7b, ror) {
1:dp = op_readpc();
2:op_io();
3:rd = op_readdp(dp + regs.x);
4:rd = op_$1(rd);
  op_writedp(dp + regs.x, rd);
}

inc_addr(0xac, inc),
dec_addr(0x8c, dec),
asl_addr(0x0c, asl),
lsr_addr(0x4c, lsr),
rol_addr(0x2c, rol),
ror_addr(0x6c, ror) {
1:dp  = op_readpc();
2:dp |= op_readpc() << 8;
3:rd = op_readaddr(dp);
4:rd = op_$1(rd);
  op_writeaddr(dp, rd);
}

incw_dp(0x3a, incw),
decw_dp(0x1a, decw) {
1:dp = op_readpc();
2:rd  = op_readdp(dp);
3:rd |= op_readdp(dp + 1) << 8;
4:rd = op_$1(rd);
  op_writedp(dp + 1, rd >> 8);
5:op_writedp(dp,     rd);
}
