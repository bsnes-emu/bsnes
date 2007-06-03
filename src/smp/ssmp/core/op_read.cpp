void sSMP::op_adc_a_const() {
  rd = op_readpc();
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_and_a_const() {
  rd = op_readpc();
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_cmp_a_const() {
  rd = op_readpc();
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_cmp_x_const() {
  rd = op_readpc();
  regs.x = op_cmp(regs.x, rd);
}

void sSMP::op_cmp_y_const() {
  rd = op_readpc();
  regs.y = op_cmp(regs.y, rd);
}

void sSMP::op_eor_a_const() {
  rd = op_readpc();
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_or_a_const() {
  rd = op_readpc();
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_sbc_a_const() {
  rd = op_readpc();
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_adc_a_ix() {
  op_io();
  rd = op_readdp(regs.x);
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_and_a_ix() {
  op_io();
  rd = op_readdp(regs.x);
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_cmp_a_ix() {
  op_io();
  rd = op_readdp(regs.x);
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_eor_a_ix() {
  op_io();
  rd = op_readdp(regs.x);
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_or_a_ix() {
  op_io();
  rd = op_readdp(regs.x);
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_sbc_a_ix() {
  op_io();
  rd = op_readdp(regs.x);
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_adc_a_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_and_a_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_cmp_a_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_cmp_x_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  regs.x = op_cmp(regs.x, rd);
}

void sSMP::op_cmp_y_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  regs.y = op_cmp(regs.y, rd);
}

void sSMP::op_eor_a_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_or_a_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_sbc_a_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_adc_a_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_and_a_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_cmp_a_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_eor_a_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_or_a_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_sbc_a_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_adc_a_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_and_a_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_cmp_a_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_cmp_x_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.x = op_cmp(regs.x, rd);
}

void sSMP::op_cmp_y_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.y = op_cmp(regs.y, rd);
}

void sSMP::op_eor_a_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_or_a_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_sbc_a_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_adc_a_addrx() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.x);
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_adc_a_addry() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.y);
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_and_a_addrx() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.x);
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_and_a_addry() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.y);
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_cmp_a_addrx() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.x);
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_cmp_a_addry() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.y);
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_eor_a_addrx() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.x);
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_eor_a_addry() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.y);
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_or_a_addrx() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.x);
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_or_a_addry() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.y);
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_sbc_a_addrx() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.x);
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_sbc_a_addry() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  rd = op_readaddr(dp + regs.y);
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_adc_a_idpx() {
  dp = op_readpc() + regs.x;
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp);
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_and_a_idpx() {
  dp = op_readpc() + regs.x;
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp);
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_cmp_a_idpx() {
  dp = op_readpc() + regs.x;
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp);
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_eor_a_idpx() {
  dp = op_readpc() + regs.x;
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp);
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_or_a_idpx() {
  dp = op_readpc() + regs.x;
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp);
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_sbc_a_idpx() {
  dp = op_readpc() + regs.x;
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp);
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_adc_a_idpy() {
  dp  = op_readpc();
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp + regs.y);
  regs.a = op_adc(regs.a, rd);
}

void sSMP::op_and_a_idpy() {
  dp  = op_readpc();
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp + regs.y);
  regs.a = op_and(regs.a, rd);
}

void sSMP::op_cmp_a_idpy() {
  dp  = op_readpc();
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp + regs.y);
  regs.a = op_cmp(regs.a, rd);
}

void sSMP::op_eor_a_idpy() {
  dp  = op_readpc();
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp + regs.y);
  regs.a = op_eor(regs.a, rd);
}

void sSMP::op_or_a_idpy() {
  dp  = op_readpc();
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp + regs.y);
  regs.a = op_or(regs.a, rd);
}

void sSMP::op_sbc_a_idpy() {
  dp  = op_readpc();
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  rd = op_readaddr(sp + regs.y);
  regs.a = op_sbc(regs.a, rd);
}

void sSMP::op_adc_ix_iy() {
  op_io();
  rd = op_readdp(regs.y);
  wr = op_readdp(regs.x);
  wr = op_adc(wr, rd);
  (1) ? op_writedp(regs.x, wr) : op_io();
}

void sSMP::op_and_ix_iy() {
  op_io();
  rd = op_readdp(regs.y);
  wr = op_readdp(regs.x);
  wr = op_and(wr, rd);
  (1) ? op_writedp(regs.x, wr) : op_io();
}

void sSMP::op_cmp_ix_iy() {
  op_io();
  rd = op_readdp(regs.y);
  wr = op_readdp(regs.x);
  wr = op_cmp(wr, rd);
  (0) ? op_writedp(regs.x, wr) : op_io();
}

void sSMP::op_eor_ix_iy() {
  op_io();
  rd = op_readdp(regs.y);
  wr = op_readdp(regs.x);
  wr = op_eor(wr, rd);
  (1) ? op_writedp(regs.x, wr) : op_io();
}

void sSMP::op_or_ix_iy() {
  op_io();
  rd = op_readdp(regs.y);
  wr = op_readdp(regs.x);
  wr = op_or(wr, rd);
  (1) ? op_writedp(regs.x, wr) : op_io();
}

void sSMP::op_sbc_ix_iy() {
  op_io();
  rd = op_readdp(regs.y);
  wr = op_readdp(regs.x);
  wr = op_sbc(wr, rd);
  (1) ? op_writedp(regs.x, wr) : op_io();
}

void sSMP::op_adc_dp_dp() {
  sp = op_readpc();
  rd = op_readdp(sp);
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_adc(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_and_dp_dp() {
  sp = op_readpc();
  rd = op_readdp(sp);
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_and(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_cmp_dp_dp() {
  sp = op_readpc();
  rd = op_readdp(sp);
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_cmp(wr, rd);
  (0) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_eor_dp_dp() {
  sp = op_readpc();
  rd = op_readdp(sp);
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_eor(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_or_dp_dp() {
  sp = op_readpc();
  rd = op_readdp(sp);
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_or(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_sbc_dp_dp() {
  sp = op_readpc();
  rd = op_readdp(sp);
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_sbc(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_adc_dp_const() {
  rd = op_readpc();
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_adc(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_and_dp_const() {
  rd = op_readpc();
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_and(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_cmp_dp_const() {
  rd = op_readpc();
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_cmp(wr, rd);
  (0) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_eor_dp_const() {
  rd = op_readpc();
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_eor(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_or_dp_const() {
  rd = op_readpc();
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_or(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_sbc_dp_const() {
  rd = op_readpc();
  dp = op_readpc();
  wr = op_readdp(dp);
  wr = op_sbc(wr, rd);
  (1) ? op_writedp(dp, wr) : op_io();
}

void sSMP::op_addw_ya_dp() {
  dp  = op_readpc();
  rd  = op_readdp(dp);
  op_io();
  rd |= op_readdp(dp + 1) << 8;
  regs.ya = op_addw(regs.ya, rd);
}

void sSMP::op_subw_ya_dp() {
  dp  = op_readpc();
  rd  = op_readdp(dp);
  op_io();
  rd |= op_readdp(dp + 1) << 8;
  regs.ya = op_subw(regs.ya, rd);
}

void sSMP::op_cmpw_ya_dp() {
  dp  = op_readpc();
  rd  = op_readdp(dp);
  rd |= op_readdp(dp + 1) << 8;
  op_cmpw(regs.ya, rd);
}

void sSMP::op_and1_bit() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_readaddr(dp);
  regs.p.c = regs.p.c & !!(rd & (1 << bit));
}

void sSMP::op_and1_notbit() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_readaddr(dp);
  regs.p.c = regs.p.c & !(rd & (1 << bit));
}

void sSMP::op_eor1_bit() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_readaddr(dp);
  op_io();
  regs.p.c = regs.p.c ^ !!(rd & (1 << bit));
}

void sSMP::op_not1_bit() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_readaddr(dp);
  rd ^= (1 << bit);
  op_writeaddr(dp, rd);
}

void sSMP::op_or1_bit() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_readaddr(dp);
  op_io();
  regs.p.c = regs.p.c | !!(rd & (1 << bit));
}

void sSMP::op_or1_notbit() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_readaddr(dp);
  op_io();
  regs.p.c = regs.p.c | !(rd & (1 << bit));
}

