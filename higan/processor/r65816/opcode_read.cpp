auto R65816::op_read_const_b(fp op) {
L rd.l = readpc();
  call(op);
}

auto R65816::op_read_const_w(fp op) {
  rd.l = readpc();
L rd.h = readpc();
  call(op);
}

auto R65816::op_read_bit_const_b() {
L rd.l = readpc();
  regs.p.z = ((rd.l & regs.a.l) == 0);
}

auto R65816::op_read_bit_const_w() {
  rd.l = readpc();
L rd.h = readpc();
  regs.p.z = ((rd.w & regs.a.w) == 0);
}

auto R65816::op_read_addr_b(fp op) {
  aa.l = readpc();
  aa.h = readpc();
L rd.l = readdbr(aa.w);
  call(op);
}

auto R65816::op_read_addr_w(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  rd.l = readdbr(aa.w + 0);
L rd.h = readdbr(aa.w + 1);
  call(op);
}

auto R65816::op_read_addrx_b(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  io4(aa.w, aa.w + regs.x.w);
L rd.l = readdbr(aa.w + regs.x.w);
  call(op);
}

auto R65816::op_read_addrx_w(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  io4(aa.w, aa.w + regs.x.w);
  rd.l = readdbr(aa.w + regs.x.w + 0);
L rd.h = readdbr(aa.w + regs.x.w + 1);
  call(op);
}

auto R65816::op_read_addry_b(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  io4(aa.w, aa.w + regs.y.w);
L rd.l = readdbr(aa.w + regs.y.w);
  call(op);
}

auto R65816::op_read_addry_w(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  io4(aa.w, aa.w + regs.y.w);
  rd.l = readdbr(aa.w + regs.y.w + 0);
L rd.h = readdbr(aa.w + regs.y.w + 1);
  call(op);
}

auto R65816::op_read_long_b(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  aa.b = readpc();
L rd.l = readlong(aa.d);
  call(op);
}

auto R65816::op_read_long_w(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  aa.b = readpc();
  rd.l = readlong(aa.d + 0);
L rd.h = readlong(aa.d + 1);
  call(op);
}

auto R65816::op_read_longx_b(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  aa.b = readpc();
L rd.l = readlong(aa.d + regs.x.w);
  call(op);
}

auto R65816::op_read_longx_w(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  aa.b = readpc();
  rd.l = readlong(aa.d + regs.x.w + 0);
L rd.h = readlong(aa.d + regs.x.w + 1);
  call(op);
}

auto R65816::op_read_dp_b(fp op) {
  dp = readpc();
  io2();
L rd.l = readdp(dp);
  call(op);
}

auto R65816::op_read_dp_w(fp op) {
  dp = readpc();
  io2();
  rd.l = readdp(dp + 0);
L rd.h = readdp(dp + 1);
  call(op);
}

auto R65816::op_read_dpr_b(fp op, reg16_t& reg) {
  dp = readpc();
  io2();
  io();
L rd.l = readdp(dp + reg.w);
  call(op);
}

auto R65816::op_read_dpr_w(fp op, reg16_t& reg) {
  dp = readpc();
  io2();
  io();
  rd.l = readdp(dp + reg.w + 0);
L rd.h = readdp(dp + reg.w + 1);
  call(op);
}

auto R65816::op_read_idp_b(fp op) {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
L rd.l = readdbr(aa.w);
  call(op);
}

auto R65816::op_read_idp_w(fp op) {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  rd.l = readdbr(aa.w + 0);
L rd.h = readdbr(aa.w + 1);
  call(op);
}

auto R65816::op_read_idpx_b(fp op) {
  dp = readpc();
  io2();
  io();
  aa.l = readdp(dp + regs.x.w + 0);
  aa.h = readdp(dp + regs.x.w + 1);
L rd.l = readdbr(aa.w);
  call(op);
}

auto R65816::op_read_idpx_w(fp op) {
  dp = readpc();
  io2();
  io();
  aa.l = readdp(dp + regs.x.w + 0);
  aa.h = readdp(dp + regs.x.w + 1);
  rd.l = readdbr(aa.w + 0);
L rd.h = readdbr(aa.w + 1);
  call(op);
}

auto R65816::op_read_idpy_b(fp op) {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  io4(aa.w, aa.w + regs.y.w);
L rd.l = readdbr(aa.w + regs.y.w);
  call(op);
}

auto R65816::op_read_idpy_w(fp op) {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  io4(aa.w, aa.w + regs.y.w);
  rd.l = readdbr(aa.w + regs.y.w + 0);
L rd.h = readdbr(aa.w + regs.y.w + 1);
  call(op);
}

auto R65816::op_read_ildp_b(fp op) {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  aa.b = readdp(dp + 2);
L rd.l = readlong(aa.d);
  call(op);
}

auto R65816::op_read_ildp_w(fp op) {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  aa.b = readdp(dp + 2);
  rd.l = readlong(aa.d + 0);
L rd.h = readlong(aa.d + 1);
  call(op);
}

auto R65816::op_read_ildpy_b(fp op) {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  aa.b = readdp(dp + 2);
L rd.l = readlong(aa.d + regs.y.w);
  call(op);
}

auto R65816::op_read_ildpy_w(fp op) {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  aa.b = readdp(dp + 2);
  rd.l = readlong(aa.d + regs.y.w + 0);
L rd.h = readlong(aa.d + regs.y.w + 1);
  call(op);
}

auto R65816::op_read_sr_b(fp op) {
  sp = readpc();
  io();
L rd.l = readsp(sp);
  call(op);
}

auto R65816::op_read_sr_w(fp op) {
  sp = readpc();
  io();
  rd.l = readsp(sp + 0);
L rd.h = readsp(sp + 1);
  call(op);
}

auto R65816::op_read_isry_b(fp op) {
  sp = readpc();
  io();
  aa.l = readsp(sp + 0);
  aa.h = readsp(sp + 1);
  io();
L rd.l = readdbr(aa.w + regs.y.w);
  call(op);
}

auto R65816::op_read_isry_w(fp op) {
  sp = readpc();
  io();
  aa.l = readsp(sp + 0);
  aa.h = readsp(sp + 1);
  io();
  rd.l = readdbr(aa.w + regs.y.w + 0);
L rd.h = readdbr(aa.w + regs.y.w + 1);
  call(op);
}
