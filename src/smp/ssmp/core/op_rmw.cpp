void sSMP::op_inc_a() {
  op_io();
  regs.a = op_inc(regs.a);
}

void sSMP::op_inc_x() {
  op_io();
  regs.x = op_inc(regs.x);
}

void sSMP::op_inc_y() {
  op_io();
  regs.y = op_inc(regs.y);
}

void sSMP::op_dec_a() {
  op_io();
  regs.a = op_dec(regs.a);
}

void sSMP::op_dec_x() {
  op_io();
  regs.x = op_dec(regs.x);
}

void sSMP::op_dec_y() {
  op_io();
  regs.y = op_dec(regs.y);
}

void sSMP::op_asl_a() {
  op_io();
  regs.a = op_asl(regs.a);
}

void sSMP::op_lsr_a() {
  op_io();
  regs.a = op_lsr(regs.a);
}

void sSMP::op_rol_a() {
  op_io();
  regs.a = op_rol(regs.a);
}

void sSMP::op_ror_a() {
  op_io();
  regs.a = op_ror(regs.a);
}

void sSMP::op_inc_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_inc(rd);
  op_writedp(dp, rd);
}

void sSMP::op_dec_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_dec(rd);
  op_writedp(dp, rd);
}

void sSMP::op_asl_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_asl(rd);
  op_writedp(dp, rd);
}

void sSMP::op_lsr_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_lsr(rd);
  op_writedp(dp, rd);
}

void sSMP::op_rol_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_rol(rd);
  op_writedp(dp, rd);
}

void sSMP::op_ror_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd = op_ror(rd);
  op_writedp(dp, rd);
}

void sSMP::op_inc_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_inc(rd);
  op_writedp(dp + regs.x, rd);
}

void sSMP::op_dec_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_dec(rd);
  op_writedp(dp + regs.x, rd);
}

void sSMP::op_asl_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_asl(rd);
  op_writedp(dp + regs.x, rd);
}

void sSMP::op_lsr_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_lsr(rd);
  op_writedp(dp + regs.x, rd);
}

void sSMP::op_rol_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_rol(rd);
  op_writedp(dp + regs.x, rd);
}

void sSMP::op_ror_dpx() {
  dp = op_readpc();
  op_io();
  rd = op_readdp(dp + regs.x);
  rd = op_ror(rd);
  op_writedp(dp + regs.x, rd);
}

void sSMP::op_inc_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_inc(rd);
  op_writeaddr(dp, rd);
}

void sSMP::op_dec_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_dec(rd);
  op_writeaddr(dp, rd);
}

void sSMP::op_asl_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_asl(rd);
  op_writeaddr(dp, rd);
}

void sSMP::op_lsr_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_lsr(rd);
  op_writeaddr(dp, rd);
}

void sSMP::op_rol_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_rol(rd);
  op_writeaddr(dp, rd);
}

void sSMP::op_ror_addr() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  rd = op_ror(rd);
  op_writeaddr(dp, rd);
}

void sSMP::op_tset_addr_a() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.p.n = !!((regs.a - rd) & 0x80);
  regs.p.z = ((regs.a - rd) == 0);
  op_readaddr(dp);
  op_writeaddr(dp, rd | regs.a);
}

void sSMP::op_tclr_addr_a() {
  dp  = op_readpc();
  dp |= op_readpc() << 8;
  rd = op_readaddr(dp);
  regs.p.n = !!((regs.a - rd) & 0x80);
  regs.p.z = ((regs.a - rd) == 0);
  op_readaddr(dp);
  op_writeaddr(dp, rd &~ regs.a);
}

void sSMP::op_incw_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd++;
  op_writedp(dp++, rd);
  rd += op_readdp(dp) << 8;
  op_write(dp, rd >> 8);
  regs.p.n = !!(rd & 0x8000);
  regs.p.z = (rd == 0);
}

void sSMP::op_decw_dp() {
  dp = op_readpc();
  rd = op_readdp(dp);
  rd--;
  op_writedp(dp++, rd);
  rd += op_readdp(dp) << 8;
  op_write(dp, rd >> 8);
  regs.p.n = !!(rd & 0x8000);
  regs.p.z = (rd == 0);
}

