auto R65816::op_read_const_b(fp op) {
L rd.l = readPC();
  call(op);
}

auto R65816::op_read_const_w(fp op) {
  rd.l = readPC();
L rd.h = readPC();
  call(op);
}

auto R65816::op_read_bit_const_b() {
L rd.l = readPC();
  r.p.z = ((rd.l & r.a.l) == 0);
}

auto R65816::op_read_bit_const_w() {
  rd.l = readPC();
L rd.h = readPC();
  r.p.z = ((rd.w & r.a.w) == 0);
}

auto R65816::op_read_addr_b(fp op) {
  aa.l = readPC();
  aa.h = readPC();
L rd.l = readDB(aa.w);
  call(op);
}

auto R65816::op_read_addr_w(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  rd.l = readDB(aa.w + 0);
L rd.h = readDB(aa.w + 1);
  call(op);
}

auto R65816::op_read_addrx_b(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  io4(aa.w, aa.w + r.x.w);
L rd.l = readDB(aa.w + r.x.w);
  call(op);
}

auto R65816::op_read_addrx_w(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  io4(aa.w, aa.w + r.x.w);
  rd.l = readDB(aa.w + r.x.w + 0);
L rd.h = readDB(aa.w + r.x.w + 1);
  call(op);
}

auto R65816::op_read_addry_b(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  io4(aa.w, aa.w + r.y.w);
L rd.l = readDB(aa.w + r.y.w);
  call(op);
}

auto R65816::op_read_addry_w(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  io4(aa.w, aa.w + r.y.w);
  rd.l = readDB(aa.w + r.y.w + 0);
L rd.h = readDB(aa.w + r.y.w + 1);
  call(op);
}

auto R65816::op_read_long_b(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  aa.b = readPC();
L rd.l = readLong(aa.d);
  call(op);
}

auto R65816::op_read_long_w(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  aa.b = readPC();
  rd.l = readLong(aa.d + 0);
L rd.h = readLong(aa.d + 1);
  call(op);
}

auto R65816::op_read_longx_b(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  aa.b = readPC();
L rd.l = readLong(aa.d + r.x.w);
  call(op);
}

auto R65816::op_read_longx_w(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  aa.b = readPC();
  rd.l = readLong(aa.d + r.x.w + 0);
L rd.h = readLong(aa.d + r.x.w + 1);
  call(op);
}

auto R65816::op_read_dp_b(fp op) {
  dp = readPC();
  io2();
L rd.l = readDP(dp);
  call(op);
}

auto R65816::op_read_dp_w(fp op) {
  dp = readPC();
  io2();
  rd.l = readDP(dp + 0);
L rd.h = readDP(dp + 1);
  call(op);
}

auto R65816::op_read_dpr_b(fp op, Reg16& reg) {
  dp = readPC();
  io2();
  io();
L rd.l = readDP(dp + reg.w);
  call(op);
}

auto R65816::op_read_dpr_w(fp op, Reg16& reg) {
  dp = readPC();
  io2();
  io();
  rd.l = readDP(dp + reg.w + 0);
L rd.h = readDP(dp + reg.w + 1);
  call(op);
}

auto R65816::op_read_idp_b(fp op) {
  dp = readPC();
  io2();
  aa.l = readDP(dp + 0);
  aa.h = readDP(dp + 1);
L rd.l = readDB(aa.w);
  call(op);
}

auto R65816::op_read_idp_w(fp op) {
  dp = readPC();
  io2();
  aa.l = readDP(dp + 0);
  aa.h = readDP(dp + 1);
  rd.l = readDB(aa.w + 0);
L rd.h = readDB(aa.w + 1);
  call(op);
}

auto R65816::op_read_idpx_b(fp op) {
  dp = readPC();
  io2();
  io();
  aa.l = readDP(dp + r.x.w + 0);
  aa.h = readDP(dp + r.x.w + 1);
L rd.l = readDB(aa.w);
  call(op);
}

auto R65816::op_read_idpx_w(fp op) {
  dp = readPC();
  io2();
  io();
  aa.l = readDP(dp + r.x.w + 0);
  aa.h = readDP(dp + r.x.w + 1);
  rd.l = readDB(aa.w + 0);
L rd.h = readDB(aa.w + 1);
  call(op);
}

auto R65816::op_read_idpy_b(fp op) {
  dp = readPC();
  io2();
  aa.l = readDP(dp + 0);
  aa.h = readDP(dp + 1);
  io4(aa.w, aa.w + r.y.w);
L rd.l = readDB(aa.w + r.y.w);
  call(op);
}

auto R65816::op_read_idpy_w(fp op) {
  dp = readPC();
  io2();
  aa.l = readDP(dp + 0);
  aa.h = readDP(dp + 1);
  io4(aa.w, aa.w + r.y.w);
  rd.l = readDB(aa.w + r.y.w + 0);
L rd.h = readDB(aa.w + r.y.w + 1);
  call(op);
}

auto R65816::op_read_ildp_b(fp op) {
  dp = readPC();
  io2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  aa.b = readDPn(dp + 2);
L rd.l = readLong(aa.d);
  call(op);
}

auto R65816::op_read_ildp_w(fp op) {
  dp = readPC();
  io2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  aa.b = readDPn(dp + 2);
  rd.l = readLong(aa.d + 0);
L rd.h = readLong(aa.d + 1);
  call(op);
}

auto R65816::op_read_ildpy_b(fp op) {
  dp = readPC();
  io2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  aa.b = readDPn(dp + 2);
L rd.l = readLong(aa.d + r.y.w);
  call(op);
}

auto R65816::op_read_ildpy_w(fp op) {
  dp = readPC();
  io2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  aa.b = readDPn(dp + 2);
  rd.l = readLong(aa.d + r.y.w + 0);
L rd.h = readLong(aa.d + r.y.w + 1);
  call(op);
}

auto R65816::op_read_sr_b(fp op) {
  sp = readPC();
  io();
L rd.l = readSP(sp);
  call(op);
}

auto R65816::op_read_sr_w(fp op) {
  sp = readPC();
  io();
  rd.l = readSP(sp + 0);
L rd.h = readSP(sp + 1);
  call(op);
}

auto R65816::op_read_isry_b(fp op) {
  sp = readPC();
  io();
  aa.l = readSP(sp + 0);
  aa.h = readSP(sp + 1);
  io();
L rd.l = readDB(aa.w + r.y.w);
  call(op);
}

auto R65816::op_read_isry_w(fp op) {
  sp = readPC();
  io();
  aa.l = readSP(sp + 0);
  aa.h = readSP(sp + 1);
  io();
  rd.l = readDB(aa.w + r.y.w + 0);
L rd.h = readDB(aa.w + r.y.w + 1);
  call(op);
}
