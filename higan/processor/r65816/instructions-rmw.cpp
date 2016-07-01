auto R65816::op_adjust_imm_b(Reg16& reg, int adjust) {
L idleIRQ();
  reg.l += adjust;
  r.p.n = (reg.l & 0x80);
  r.p.z = (reg.l == 0);
}

auto R65816::op_adjust_imm_w(Reg16& reg, int adjust) {
L idleIRQ();
  reg.w += adjust;
  r.p.n = (reg.w & 0x8000);
  r.p.z = (reg.w == 0);
}

auto R65816::op_asl_imm_b() {
L idleIRQ();
  r.p.c = (r.a.l & 0x80);
  r.a.l <<= 1;
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto R65816::op_asl_imm_w() {
L idleIRQ();
  r.p.c = (r.a.w & 0x8000);
  r.a.w <<= 1;
  r.p.n = (r.a.w & 0x8000);
  r.p.z = (r.a.w == 0);
}

auto R65816::op_lsr_imm_b() {
L idleIRQ();
  r.p.c = (r.a.l & 0x01);
  r.a.l >>= 1;
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto R65816::op_lsr_imm_w() {
L idleIRQ();
  r.p.c = (r.a.w & 0x0001);
  r.a.w >>= 1;
  r.p.n = (r.a.w & 0x8000);
  r.p.z = (r.a.w == 0);
}

auto R65816::op_rol_imm_b() {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = (r.a.l & 0x80);
  r.a.l = (r.a.l << 1) | carry;
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto R65816::op_rol_imm_w() {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = (r.a.w & 0x8000);
  r.a.w = (r.a.w << 1) | carry;
  r.p.n = (r.a.w & 0x8000);
  r.p.z = (r.a.w == 0);
}

auto R65816::op_ror_imm_b() {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = (r.a.l & 0x01);
  r.a.l = (carry << 7) | (r.a.l >> 1);
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto R65816::op_ror_imm_w() {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = (r.a.w & 0x0001);
  r.a.w = (carry << 15) | (r.a.w >> 1);
  r.p.n = (r.a.w & 0x8000);
  r.p.z = (r.a.w == 0);
}

auto R65816::op_adjust_addr_b(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  rd.l = readDB(aa.w);
  idle();
  call(op);
L writeDB(aa.w, rd.l);
}

auto R65816::op_adjust_addr_w(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  rd.l = readDB(aa.w + 0);
  rd.h = readDB(aa.w + 1);
  idle();
  call(op);
  writeDB(aa.w + 1, rd.h);
L writeDB(aa.w + 0, rd.l);
}

auto R65816::op_adjust_addrx_b(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  idle();
  rd.l = readDB(aa.w + r.x.w);
  idle();
  call(op);
L writeDB(aa.w + r.x.w, rd.l);
}

auto R65816::op_adjust_addrx_w(fp op) {
  aa.l = readPC();
  aa.h = readPC();
  idle();
  rd.l = readDB(aa.w + r.x.w + 0);
  rd.h = readDB(aa.w + r.x.w + 1);
  idle();
  call(op);
  writeDB(aa.w + r.x.w + 1, rd.h);
L writeDB(aa.w + r.x.w + 0, rd.l);
}

auto R65816::op_adjust_dp_b(fp op) {
  dp = readPC();
  idle2();
  rd.l = readDP(dp);
  idle();
  call(op);
L writeDP(dp, rd.l);
}

auto R65816::op_adjust_dp_w(fp op) {
  dp = readPC();
  idle2();
  rd.l = readDP(dp + 0);
  rd.h = readDP(dp + 1);
  idle();
  call(op);
  writeDP(dp + 1, rd.h);
L writeDP(dp + 0, rd.l);
}

auto R65816::op_adjust_dpx_b(fp op) {
  dp = readPC();
  idle2();
  idle();
  rd.l = readDP(dp + r.x.w);
  idle();
  call(op);
L writeDP(dp + r.x.w, rd.l);
}

auto R65816::op_adjust_dpx_w(fp op) {
  dp = readPC();
  idle2();
  idle();
  rd.l = readDP(dp + r.x.w + 0);
  rd.h = readDP(dp + r.x.w + 1);
  idle();
  call(op);
  writeDP(dp + r.x.w + 1, rd.h);
L writeDP(dp + r.x.w + 0, rd.l);
}
