auto R65816::op_adjust_imm_b(reg16_t& reg, signed adjust) {
L ioIRQ();
  reg.l += adjust;
  regs.p.n = (reg.l & 0x80);
  regs.p.z = (reg.l == 0);
}

auto R65816::op_adjust_imm_w(reg16_t& reg, signed adjust) {
L ioIRQ();
  reg.w += adjust;
  regs.p.n = (reg.w & 0x8000);
  regs.p.z = (reg.w == 0);
}

auto R65816::op_asl_imm_b() {
L ioIRQ();
  regs.p.c = (regs.a.l & 0x80);
  regs.a.l <<= 1;
  regs.p.n = (regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

auto R65816::op_asl_imm_w() {
L ioIRQ();
  regs.p.c = (regs.a.w & 0x8000);
  regs.a.w <<= 1;
  regs.p.n = (regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

auto R65816::op_lsr_imm_b() {
L ioIRQ();
  regs.p.c = (regs.a.l & 0x01);
  regs.a.l >>= 1;
  regs.p.n = (regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

auto R65816::op_lsr_imm_w() {
L ioIRQ();
  regs.p.c = (regs.a.w & 0x0001);
  regs.a.w >>= 1;
  regs.p.n = (regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

auto R65816::op_rol_imm_b() {
L ioIRQ();
  bool carry = regs.p.c;
  regs.p.c = (regs.a.l & 0x80);
  regs.a.l = (regs.a.l << 1) | carry;
  regs.p.n = (regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

auto R65816::op_rol_imm_w() {
L ioIRQ();
  bool carry = regs.p.c;
  regs.p.c = (regs.a.w & 0x8000);
  regs.a.w = (regs.a.w << 1) | carry;
  regs.p.n = (regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

auto R65816::op_ror_imm_b() {
L ioIRQ();
  bool carry = regs.p.c;
  regs.p.c = (regs.a.l & 0x01);
  regs.a.l = (carry << 7) | (regs.a.l >> 1);
  regs.p.n = (regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

auto R65816::op_ror_imm_w() {
L ioIRQ();
  bool carry = regs.p.c;
  regs.p.c = (regs.a.w & 0x0001);
  regs.a.w = (carry << 15) | (regs.a.w >> 1);
  regs.p.n = (regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

auto R65816::op_adjust_addr_b(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  rd.l = readdbr(aa.w);
  io();
  call(op);
L writedbr(aa.w, rd.l);
}

auto R65816::op_adjust_addr_w(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  rd.l = readdbr(aa.w + 0);
  rd.h = readdbr(aa.w + 1);
  io();
  call(op);
  writedbr(aa.w + 1, rd.h);
L writedbr(aa.w + 0, rd.l);
}

auto R65816::op_adjust_addrx_b(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  io();
  rd.l = readdbr(aa.w + regs.x.w);
  io();
  call(op);
L writedbr(aa.w + regs.x.w, rd.l);
}

auto R65816::op_adjust_addrx_w(fp op) {
  aa.l = readpc();
  aa.h = readpc();
  io();
  rd.l = readdbr(aa.w + regs.x.w + 0);
  rd.h = readdbr(aa.w + regs.x.w + 1);
  io();
  call(op);
  writedbr(aa.w + regs.x.w + 1, rd.h);
L writedbr(aa.w + regs.x.w + 0, rd.l);
}

auto R65816::op_adjust_dp_b(fp op) {
  dp = readpc();
  io2();
  rd.l = readdp(dp);
  io();
  call(op);
L writedp(dp, rd.l);
}

auto R65816::op_adjust_dp_w(fp op) {
  dp = readpc();
  io2();
  rd.l = readdp(dp + 0);
  rd.h = readdp(dp + 1);
  io();
  call(op);
  writedp(dp + 1, rd.h);
L writedp(dp + 0, rd.l);
}

auto R65816::op_adjust_dpx_b(fp op) {
  dp = readpc();
  io2();
  io();
  rd.l = readdp(dp + regs.x.w);
  io();
  call(op);
L writedp(dp + regs.x.w, rd.l);
}

auto R65816::op_adjust_dpx_w(fp op) {
  dp = readpc();
  io2();
  io();
  rd.l = readdp(dp + regs.x.w + 0);
  rd.h = readdp(dp + regs.x.w + 1);
  io();
  call(op);
  writedp(dp + regs.x.w + 1, rd.h);
L writedp(dp + regs.x.w + 0, rd.l);
}
