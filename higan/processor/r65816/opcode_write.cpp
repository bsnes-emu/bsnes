auto R65816::op_write_addr_b(reg16_t& reg) {
  aa.l = readpc();
  aa.h = readpc();
L writedbr(aa.w, reg);
}

auto R65816::op_write_addr_w(reg16_t& reg) {
  aa.l = readpc();
  aa.h = readpc();
  writedbr(aa.w + 0, reg >> 0);
L writedbr(aa.w + 1, reg >> 8);
}

auto R65816::op_write_addrr_b(reg16_t& reg, reg16_t& idx) {
  aa.l = readpc();
  aa.h = readpc();
  io();
L writedbr(aa.w + idx, reg);
}

auto R65816::op_write_addrr_w(reg16_t& reg, reg16_t& idx) {
  aa.l = readpc();
  aa.h = readpc();
  io();
  writedbr(aa.w + idx + 0, reg >> 0);
L writedbr(aa.w + idx + 1, reg >> 8);
}

auto R65816::op_write_longr_b(reg16_t& idx) {
  aa.l = readpc();
  aa.h = readpc();
  aa.b = readpc();
L writelong(aa.d + idx, regs.a.l);
}

auto R65816::op_write_longr_w(reg16_t& idx) {
  aa.l = readpc();
  aa.h = readpc();
  aa.b = readpc();
  writelong(aa.d + idx + 0, regs.a.l);
L writelong(aa.d + idx + 1, regs.a.h);
}

auto R65816::op_write_dp_b(reg16_t& reg) {
  dp = readpc();
  io2();
L writedp(dp, reg);
}

auto R65816::op_write_dp_w(reg16_t& reg) {
  dp = readpc();
  io2();
  writedp(dp + 0, reg >> 0);
L writedp(dp + 1, reg >> 8);
}

auto R65816::op_write_dpr_b(reg16_t& reg, reg16_t& idx) {
  dp = readpc();
  io2();
  io();
L writedp(dp + idx, reg);
}

auto R65816::op_write_dpr_w(reg16_t& reg, reg16_t& idx) {
  dp = readpc();
  io2();
  io();
  writedp(dp + idx + 0, reg >> 0);
L writedp(dp + idx + 1, reg >> 8);
}

auto R65816::op_sta_idp_b() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
L writedbr(aa.w, regs.a.l);
}

auto R65816::op_sta_idp_w() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  writedbr(aa.w + 0, regs.a.l);
L writedbr(aa.w + 1, regs.a.h);
}

auto R65816::op_sta_ildp_b() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  aa.b = readdp(dp + 2);
L writelong(aa.d, regs.a.l);
}

auto R65816::op_sta_ildp_w() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  aa.b = readdp(dp + 2);
  writelong(aa.d + 0, regs.a.l);
L writelong(aa.d + 1, regs.a.h);
}

auto R65816::op_sta_idpx_b() {
  dp = readpc();
  io2();
  io();
  aa.l = readdp(dp + regs.x.w + 0);
  aa.h = readdp(dp + regs.x.w + 1);
L writedbr(aa.w, regs.a.l);
}

auto R65816::op_sta_idpx_w() {
  dp = readpc();
  io2();
  io();
  aa.l = readdp(dp + regs.x.w + 0);
  aa.h = readdp(dp + regs.x.w + 1);
  writedbr(aa.w + 0, regs.a.l);
L writedbr(aa.w + 1, regs.a.h);
}

auto R65816::op_sta_idpy_b() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  io();
L writedbr(aa.w + regs.y.w, regs.a.l);
}

auto R65816::op_sta_idpy_w() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  io();
  writedbr(aa.w + regs.y.w + 0, regs.a.l);
L writedbr(aa.w + regs.y.w + 1, regs.a.h);
}

auto R65816::op_sta_ildpy_b() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  aa.b = readdp(dp + 2);
L writelong(aa.d + regs.y.w, regs.a.l);
}

auto R65816::op_sta_ildpy_w() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  aa.b = readdp(dp + 2);
  writelong(aa.d + regs.y.w + 0, regs.a.l);
L writelong(aa.d + regs.y.w + 1, regs.a.h);
}

auto R65816::op_sta_sr_b() {
  sp = readpc();
  io();
L writesp(sp, regs.a.l);
}

auto R65816::op_sta_sr_w() {
  sp = readpc();
  io();
  writesp(sp + 0, regs.a.l);
L writesp(sp + 1, regs.a.h);
}

auto R65816::op_sta_isry_b() {
  sp = readpc();
  io();
  aa.l = readsp(sp + 0);
  aa.h = readsp(sp + 1);
  io();
L writedbr(aa.w + regs.y.w, regs.a.l);
}

auto R65816::op_sta_isry_w() {
  sp = readpc();
  io();
  aa.l = readsp(sp + 0);
  aa.h = readsp(sp + 1);
  io();
  writedbr(aa.w + regs.y.w + 0, regs.a.l);
L writedbr(aa.w + regs.y.w + 1, regs.a.h);
}
