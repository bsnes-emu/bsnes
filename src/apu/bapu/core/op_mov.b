mov_a_x(0x7d, a, x),
mov_a_y(0xdd, a, y),
mov_x_a(0x5d, x, a),
mov_y_a(0xfd, y, a),
mov_x_sp(0x9d, x, sp) {
1:regs.$1 = regs.$2;
  regs.p.n = !!(regs.$1 & 0x80);
  regs.p.z = (regs.$1 == 0);
}

mov_sp_x(0xbd, sp, x) {
1:regs.$1 = regs.$2;
}

mov_a_const(0xe8, a),
mov_x_const(0xcd, x),
mov_y_const(0x8d, y) {
1:regs.$1 = op_read();
  regs.p.n = !!(regs.$1 & 0x80);
  regs.p.z = (regs.$1 == 0);
}

mov_a_ix(0xe6) {
1:
2:regs.a = op_read(OPMODE_DP, regs.x);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

mov_a_ixinc(0xbf) {
1:
2:
3:regs.a = op_read(OPMODE_DP, regs.x++);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

mov_a_dp(0xe4, a),
mov_x_dp(0xf8, x),
mov_y_dp(0xeb, y) {
1:sp = op_read();
2:regs.$1 = op_read(OPMODE_DP, sp);
  regs.p.n = !!(regs.$1 & 0x80);
  regs.p.z = (regs.$1 == 0);
}

mov_a_dpx(0xf4, a, x),
mov_x_dpy(0xf9, x, y),
mov_y_dpx(0xfb, y, x) {
1:sp = op_read();
2:
3:regs.$1 = op_read(OPMODE_DP, sp + regs.$2);
  regs.p.n = !!(regs.$1 & 0x80);
  regs.p.z = (regs.$1 == 0);
}

mov_a_addr(0xe5, a),
mov_x_addr(0xe9, x),
mov_y_addr(0xec, y) {
1:sp  = op_read();
2:sp |= op_read() << 8;
3:regs.$1 = op_read(OPMODE_ADDR, sp);
  regs.p.n = !!(regs.$1 & 0x80);
  regs.p.z = (regs.$1 == 0);
}

mov_a_addrx(0xf5, x),
mov_a_addry(0xf6, y) {
1:sp  = op_read();
2:sp |= op_read() << 8;
3:
4:regs.a = op_read(OPMODE_ADDR, sp + regs.$1);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

mov_a_idpx(0xe7) {
1:dp = op_read() + regs.x;
2:
3:sp  = op_read(OPMODE_DP, dp);
4:sp |= op_read(OPMODE_DP, dp + 1) << 8;
5:regs.a = op_read(OPMODE_ADDR, sp);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

mov_a_idpy(0xf7) {
1:dp = op_read();
2:
3:sp  = op_read(OPMODE_DP, dp);
4:sp |= op_read(OPMODE_DP, dp + 1) << 8;
5:regs.a = op_read(OPMODE_ADDR, sp + regs.y);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

mov_dp_dp(0xfa) {
1:sp = op_read();
2:dp = op_read();
3:rd = op_read(OPMODE_DP, sp);
4:op_write(OPMODE_DP, dp, rd);
}

mov_dp_const(0x8f) {
1:rd = op_read();
2:dp = op_read();
3:
4:op_write(OPMODE_DP, dp, rd);
}

mov_ix_a(0xc6) {
1:
2:
3:op_write(OPMODE_DP, regs.x, regs.a);
}

mov_ixinc_a(0xaf) {
1:
2:
3:op_write(OPMODE_DP, regs.x++, regs.a);
}

mov_dp_a(0xc4, a),
mov_dp_x(0xd8, x),
mov_dp_y(0xcb, y) {
1:dp = op_read();
2:
3:op_write(OPMODE_DP, dp, regs.$1);
}

mov_dpx_a(0xd4, x, a),
mov_dpy_x(0xd9, y, x),
mov_dpx_y(0xdb, x, y) {
1:dp = op_read();
2:
3:
4:op_write(OPMODE_DP, dp + regs.$1, regs.$2);
}

mov_addr_a(0xc5, a),
mov_addr_x(0xc9, x),
mov_addr_y(0xcc, y) {
1:dp  = op_read();
2:dp |= op_read() << 8;
3:
4:op_write(OPMODE_ADDR, dp, regs.$1);
}

mov_addrx_a(0xd5, x),
mov_addry_a(0xd6, y) {
1:dp  = op_read();
2:dp |= op_read() << 8;
3:
4:
5:op_write(OPMODE_ADDR, dp + regs.$1, regs.a);
}

mov_idpx_a(0xc7) {
1:sp = op_read() + regs.x;
2:
3:dp  = op_read(OPMODE_DP, sp);
4:dp |= op_read(OPMODE_DP, sp + 1) << 8;
5:
6:op_write(OPMODE_ADDR, dp, regs.a);
}

mov_idpy_a(0xd7) {
1:sp = op_read();
2:
3:dp  = op_read(OPMODE_DP, sp);
4:dp |= op_read(OPMODE_DP, sp + 1) << 8;
5:
6:op_write(OPMODE_ADDR, dp + regs.y, regs.a);
}

movw_ya_dp(0xba) {
1:sp = op_read();
2:
3:regs.a = op_read(OPMODE_DP, sp);
4:regs.y = op_read(OPMODE_DP, sp + 1);
  regs.p.n = !!(regs.ya & 0x8000);
  regs.p.z = (regs.ya == 0);
}

movw_dp_ya(0xda) {
1:dp = op_read();
2:
3:op_write(OPMODE_DP, dp,     regs.a);
4:op_write(OPMODE_DP, dp + 1, regs.y);
}

mov1_c_bit(0xaa) {
1:sp  = op_read();
2:sp |= op_read() << 8;
3:bit = sp >> 13;
  sp &= 0x1fff;
  rd = op_read(OPMODE_ADDR, sp);
  regs.p.c = !!(rd & (1 << bit));
}

mov1_bit_c(0xca) {
1:dp  = op_read();
2:dp |= op_read() << 8;
3:bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_read(OPMODE_ADDR, dp);
  if(regs.p.c)rd |=  (1 << bit);
  else        rd &= ~(1 << bit);
4:op_write(OPMODE_ADDR, dp, rd);
}
