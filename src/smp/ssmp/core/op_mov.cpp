void sSMP::op_mov_a_x() {
  op_io();
  regs.a = regs.x;
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_a_y() {
  op_io();
  regs.a = regs.y;
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_x_a() {
  op_io();
  regs.x = regs.a;
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
}

void sSMP::op_mov_y_a() {
  op_io();
  regs.y = regs.a;
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

void sSMP::op_mov_x_sp() {
  op_io();
  regs.x = regs.sp;
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
}

void sSMP::op_mov_sp_x() {
  op_io();
  regs.sp = regs.x;
}

void sSMP::op_mov_a_const() {
  regs.a = op_readpc();
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_x_const() {
  regs.x = op_readpc();
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
}

void sSMP::op_mov_y_const() {
  regs.y = op_readpc();
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

void sSMP::op_mov_a_ix() {
  op_io();
  regs.a = op_readdp(regs.x);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_a_ixinc() {
  op_io();
  regs.a = op_readdp(regs.x++);
  op_io();
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_a_dp() {
  sp = op_readpc();
  regs.a = op_readdp(sp);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_x_dp() {
  sp = op_readpc();
  regs.x = op_readdp(sp);
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
}

void sSMP::op_mov_y_dp() {
  sp = op_readpc();
  regs.y = op_readdp(sp);
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

void sSMP::op_mov_a_dpx() {
  sp = op_readpc();
  op_io();
  regs.a = op_readdp(sp + regs.x);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_x_dpy() {
  sp = op_readpc();
  op_io();
  regs.x = op_readdp(sp + regs.y);
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
}

void sSMP::op_mov_y_dpx() {
  sp = op_readpc();
  op_io();
  regs.y = op_readdp(sp + regs.x);
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

void sSMP::op_mov_a_addr() {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  regs.a = op_readaddr(sp);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_x_addr() {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  regs.x = op_readaddr(sp);
  regs.p.n = !!(regs.x & 0x80);
  regs.p.z = (regs.x == 0);
}

void sSMP::op_mov_y_addr() {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  regs.y = op_readaddr(sp);
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

void sSMP::op_mov_a_addrx() {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  op_io();
  regs.a = op_readaddr(sp + regs.x);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_a_addry() {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  op_io();
  regs.a = op_readaddr(sp + regs.y);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_a_idpx() {
  dp = op_readpc() + regs.x;
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  regs.a = op_readaddr(sp);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_a_idpy() {
  dp = op_readpc();
  op_io();
  sp  = op_readdp(dp);
  sp |= op_readdp(dp + 1) << 8;
  regs.a = op_readaddr(sp + regs.y);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

void sSMP::op_mov_dp_dp() {
  sp = op_readpc();
  rd = op_readdp(sp);
  dp = op_readpc();
  op_writedp(dp, rd);
}

void sSMP::op_mov_dp_const() {
  rd = op_readpc();
  dp = op_readpc();
  op_readdp(dp);
  op_writedp(dp, rd);
}

void sSMP::op_mov_ix_a() {
  op_io();
  op_readdp(regs.x);
  op_writedp(regs.x, regs.a);
}

void sSMP::op_mov_ixinc_a() {
  op_io();
  op_io();
  op_writedp(regs.x++, regs.a);
}

void sSMP::op_mov_dp_a() {
  dp = op_readpc();
  op_readdp(dp);
  op_writedp(dp, regs.a);
}

void sSMP::op_mov_dp_x() {
  dp = op_readpc();
  op_readdp(dp);
  op_writedp(dp, regs.x);
}

void sSMP::op_mov_dp_y() {
  dp = op_readpc();
  op_readdp(dp);
  op_writedp(dp, regs.y);
}

void sSMP::op_mov_dpx_a() {
  dp  = op_readpc();
  op_io();
  dp += regs.x;
  op_readdp(dp);
  op_writedp(dp, regs.a);
}

void sSMP::op_mov_dpy_x() {
  dp  = op_readpc();
  op_io();
  dp += regs.y;
  op_readdp(dp);
  op_writedp(dp, regs.x);
}

void sSMP::op_mov_dpx_y() {
  dp  = op_readpc();
  op_io();
  dp += regs.x;
  op_readdp(dp);
  op_writedp(dp, regs.y);
}

void sSMP::op_mov_addr_a() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_readaddr(dp);
  op_writeaddr(dp, regs.a);
}

void sSMP::op_mov_addr_x() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_readaddr(dp);
  op_writeaddr(dp, regs.x);
}

void sSMP::op_mov_addr_y() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_readaddr(dp);
  op_writeaddr(dp, regs.y);
}

void sSMP::op_mov_addrx_a() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  dp += regs.x;
  op_readaddr(dp);
  op_writeaddr(dp, regs.a);
}

void sSMP::op_mov_addry_a() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  op_io();
  dp += regs.y;
  op_readaddr(dp);
  op_writeaddr(dp, regs.a);
}

void sSMP::op_mov_idpx_a() {
  sp  = op_readpc();
  op_io();
  sp += regs.x;
  dp  = op_readdp(sp);
  dp |= op_readdp(sp + 1) << 8;
  op_readaddr(dp);
  op_writeaddr(dp, regs.a);
}

void sSMP::op_mov_idpy_a() {
  sp  = op_readpc();
  dp  = op_readdp(sp);
  dp |= op_readdp(sp + 1) << 8;
  op_io();
  dp += regs.y;
  op_readaddr(dp);
  op_writeaddr(dp, regs.a);
}

void sSMP::op_movw_ya_dp() {
  sp = op_readpc();
  regs.a = op_readdp(sp);
  op_io();
  regs.y = op_readdp(sp + 1);
  regs.p.n = !!(regs.ya & 0x8000);
  regs.p.z = (regs.ya == 0);
}

void sSMP::op_movw_dp_ya() {
  dp = op_readpc();
  op_readdp(dp);
  op_writedp(dp,     regs.a);
  op_writedp(dp + 1, regs.y);
}

void sSMP::op_mov1_c_bit() {
  sp  = op_readpc();
  sp |= op_readpc() << 8;
  bit = sp >> 13;
  sp &= 0x1fff;
  rd = op_readaddr(sp);
  regs.p.c = !!(rd & (1 << bit));
}

void sSMP::op_mov1_bit_c() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = op_readaddr(dp);
  if(regs.p.c)rd |=  (1 << bit);
  else        rd &= ~(1 << bit);
  op_io();
  op_writeaddr(dp, rd);
}

