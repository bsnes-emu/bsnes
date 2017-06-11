auto R65816::op_write_addr_b(Reg16& reg) {
  aa.l = readPC();
  aa.h = readPC();
L writeDB(aa.w, reg);
}

auto R65816::op_write_addr_w(Reg16& reg) {
  aa.l = readPC();
  aa.h = readPC();
  writeDB(aa.w + 0, reg >> 0);
L writeDB(aa.w + 1, reg >> 8);
}

auto R65816::op_write_addrr_b(Reg16& reg, Reg16& idx) {
  aa.l = readPC();
  aa.h = readPC();
  idle();
L writeDB(aa.w + idx, reg);
}

auto R65816::op_write_addrr_w(Reg16& reg, Reg16& idx) {
  aa.l = readPC();
  aa.h = readPC();
  idle();
  writeDB(aa.w + idx + 0, reg >> 0);
L writeDB(aa.w + idx + 1, reg >> 8);
}

auto R65816::op_write_longr_b(Reg16& idx) {
  aa.l = readPC();
  aa.h = readPC();
  aa.b = readPC();
L writeLong(aa.d + idx, r.a.l);
}

auto R65816::op_write_longr_w(Reg16& idx) {
  aa.l = readPC();
  aa.h = readPC();
  aa.b = readPC();
  writeLong(aa.d + idx + 0, r.a.l);
L writeLong(aa.d + idx + 1, r.a.h);
}

auto R65816::op_write_dp_b(Reg16& reg) {
  dp = readPC();
  idle2();
L writeDP(dp, reg);
}

auto R65816::op_write_dp_w(Reg16& reg) {
  dp = readPC();
  idle2();
  writeDP(dp + 0, reg >> 0);
L writeDP(dp + 1, reg >> 8);
}

auto R65816::op_write_dpr_b(Reg16& reg, Reg16& idx) {
  dp = readPC();
  idle2();
  idle();
L writeDP(dp + idx, reg);
}

auto R65816::op_write_dpr_w(Reg16& reg, Reg16& idx) {
  dp = readPC();
  idle2();
  idle();
  writeDP(dp + idx + 0, reg >> 0);
L writeDP(dp + idx + 1, reg >> 8);
}

auto R65816::op_sta_idp_b() {
  dp = readPC();
  idle2();
  aa.l = readDP(dp + 0);
  aa.h = readDP(dp + 1);
L writeDB(aa.w, r.a.l);
}

auto R65816::op_sta_idp_w() {
  dp = readPC();
  idle2();
  aa.l = readDP(dp + 0);
  aa.h = readDP(dp + 1);
  writeDB(aa.w + 0, r.a.l);
L writeDB(aa.w + 1, r.a.h);
}

auto R65816::op_sta_ildp_b() {
  dp = readPC();
  idle2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  aa.b = readDPn(dp + 2);
L writeLong(aa.d, r.a.l);
}

auto R65816::op_sta_ildp_w() {
  dp = readPC();
  idle2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  aa.b = readDPn(dp + 2);
  writeLong(aa.d + 0, r.a.l);
L writeLong(aa.d + 1, r.a.h);
}

auto R65816::op_sta_idpx_b() {
  dp = readPC();
  idle2();
  idle();
  aa.l = readDP(dp + r.x.w + 0);
  aa.h = readDP(dp + r.x.w + 1);
L writeDB(aa.w, r.a.l);
}

auto R65816::op_sta_idpx_w() {
  dp = readPC();
  idle2();
  idle();
  aa.l = readDP(dp + r.x.w + 0);
  aa.h = readDP(dp + r.x.w + 1);
  writeDB(aa.w + 0, r.a.l);
L writeDB(aa.w + 1, r.a.h);
}

auto R65816::op_sta_idpy_b() {
  dp = readPC();
  idle2();
  aa.l = readDP(dp + 0);
  aa.h = readDP(dp + 1);
  idle();
L writeDB(aa.w + r.y.w, r.a.l);
}

auto R65816::op_sta_idpy_w() {
  dp = readPC();
  idle2();
  aa.l = readDP(dp + 0);
  aa.h = readDP(dp + 1);
  idle();
  writeDB(aa.w + r.y.w + 0, r.a.l);
L writeDB(aa.w + r.y.w + 1, r.a.h);
}

auto R65816::op_sta_ildpy_b() {
  dp = readPC();
  idle2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  aa.b = readDPn(dp + 2);
L writeLong(aa.d + r.y.w, r.a.l);
}

auto R65816::op_sta_ildpy_w() {
  dp = readPC();
  idle2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  aa.b = readDPn(dp + 2);
  writeLong(aa.d + r.y.w + 0, r.a.l);
L writeLong(aa.d + r.y.w + 1, r.a.h);
}

auto R65816::op_sta_sr_b() {
  sp = readPC();
  idle();
L writeSP(sp, r.a.l);
}

auto R65816::op_sta_sr_w() {
  sp = readPC();
  idle();
  writeSP(sp + 0, r.a.l);
L writeSP(sp + 1, r.a.h);
}

auto R65816::op_sta_isry_b() {
  sp = readPC();
  idle();
  aa.l = readSP(sp + 0);
  aa.h = readSP(sp + 1);
  idle();
L writeDB(aa.w + r.y.w, r.a.l);
}

auto R65816::op_sta_isry_w() {
  sp = readPC();
  idle();
  aa.l = readSP(sp + 0);
  aa.h = readSP(sp + 1);
  idle();
  writeDB(aa.w + r.y.w + 0, r.a.l);
L writeDB(aa.w + r.y.w + 1, r.a.h);
}
