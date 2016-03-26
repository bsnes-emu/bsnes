auto R65816::op_nop() {
L ioIRQ();
}

auto R65816::op_wdm() {
L readpc();
}

auto R65816::op_xba() {
  io();
L io();
  regs.a.l ^= regs.a.h;
  regs.a.h ^= regs.a.l;
  regs.a.l ^= regs.a.h;
  regs.p.n = (regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

auto R65816::op_move_b(signed adjust) {
  dp = readpc();
  sp = readpc();
  regs.db = dp;
  rd.l = readlong((sp << 16) | regs.x.w);
  writelong((dp << 16) | regs.y.w, rd.l);
  io();
  regs.x.l += adjust;
  regs.y.l += adjust;
L io();
  if(regs.a.w--) regs.pc.w -= 3;
}

auto R65816::op_move_w(signed adjust) {
  dp = readpc();
  sp = readpc();
  regs.db = dp;
  rd.l = readlong((sp << 16) | regs.x.w);
  writelong((dp << 16) | regs.y.w, rd.l);
  io();
  regs.x.w += adjust;
  regs.y.w += adjust;
L io();
  if(regs.a.w--) regs.pc.w -= 3;
}

auto R65816::op_interrupt_e(uint16 vector) {
  readpc();
  writestack(regs.pc.h);
  writestack(regs.pc.l);
  writestack(regs.p);
  rd.l = readlong(vector + 0);
  regs.pc.b = 0;
  regs.p.i = 1;
  regs.p.d = 0;
L rd.h = readlong(vector + 1);
  regs.pc.w = rd.w;
}

auto R65816::op_interrupt_n(uint16 vector) {
  readpc();
  writestack(regs.pc.b);
  writestack(regs.pc.h);
  writestack(regs.pc.l);
  writestack(regs.p);
  rd.l = readlong(vector + 0);
  regs.pc.b = 0x00;
  regs.p.i = 1;
  regs.p.d = 0;
L rd.h = readlong(vector + 1);
  regs.pc.w = rd.w;
}

auto R65816::op_stp() {
  while(regs.wai = true) {
L   io();
  }
}

auto R65816::op_wai() {
  regs.wai = true;
  while(regs.wai) {
L   io();
  }
  io();
}

auto R65816::op_xce() {
L ioIRQ();
  bool carry = regs.p.c;
  regs.p.c = regs.e;
  regs.e = carry;
  if(regs.e) {
    regs.p.m = 1;
    regs.p.x = 1;
    regs.x.h = 0x00;
    regs.y.h = 0x00;
    regs.s.h = 0x01;
  }
}

auto R65816::op_flag(bool& flag, bool value) {
L ioIRQ();
  flag = value;
}

auto R65816::op_pflag_e(bool mode) {
  rd.l = readpc();
L io();
  regs.p = (mode ? regs.p | rd.l : regs.p & ~rd.l);
  regs.p.m = 1;
  regs.p.x = 1;
  regs.x.h = 0x00;
  regs.y.h = 0x00;
}

auto R65816::op_pflag_n(bool mode) {
  rd.l = readpc();
L io();
  regs.p = (mode ? regs.p | rd.l : regs.p & ~rd.l);
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

auto R65816::op_transfer_b(reg16_t& from, reg16_t& to) {
L ioIRQ();
  to.l = from.l;
  regs.p.n = (to.l & 0x80);
  regs.p.z = (to.l == 0);
}

auto R65816::op_transfer_w(reg16_t& from, reg16_t& to) {
L ioIRQ();
  to.w = from.w;
  regs.p.n = (to.w & 0x8000);
  regs.p.z = (to.w == 0);
}

auto R65816::op_tcs_e() {
L ioIRQ();
  regs.s.l = regs.a.l;
}

auto R65816::op_tcs_n() {
L ioIRQ();
  regs.s.w = regs.a.w;
}

auto R65816::op_tsx_b() {
L ioIRQ();
  regs.x.l = regs.s.l;
  regs.p.n = (regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

auto R65816::op_tsx_w() {
L ioIRQ();
  regs.x.w = regs.s.w;
  regs.p.n = (regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

auto R65816::op_txs_e() {
L ioIRQ();
  regs.s.l = regs.x.l;
}

auto R65816::op_txs_n() {
L ioIRQ();
  regs.s.w = regs.x.w;
}

auto R65816::op_push_b(reg16_t& reg) {
  io();
L writestack(reg.l);
}

auto R65816::op_push_w(reg16_t& reg) {
  io();
  writestack(reg.h);
L writestack(reg.l);
}

auto R65816::op_phd_e() {
  io();
  writestackn(regs.d.h);
L writestackn(regs.d.l);
  regs.s.h = 0x01;
}

auto R65816::op_phd_n() {
  io();
  writestackn(regs.d.h);
L writestackn(regs.d.l);
}

auto R65816::op_phb() {
  io();
L writestack(regs.db);
}

auto R65816::op_phk() {
  io();
L writestack(regs.pc.b);
}

auto R65816::op_php() {
  io();
L writestack(regs.p);
}

auto R65816::op_pull_b(reg16_t& reg) {
  io();
  io();
L reg.l = readstack();
  regs.p.n = (reg.l & 0x80);
  regs.p.z = (reg.l == 0);
}

auto R65816::op_pull_w(reg16_t& reg) {
  io();
  io();
  reg.l = readstack();
L reg.h = readstack();
  regs.p.n = (reg.w & 0x8000);
  regs.p.z = (reg.w == 0);
}

auto R65816::op_pld_e() {
  io();
  io();
  regs.d.l = readstackn();
L regs.d.h = readstackn();
  regs.p.n = (regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
  regs.s.h = 0x01;
}

auto R65816::op_pld_n() {
  io();
  io();
  regs.d.l = readstackn();
L regs.d.h = readstackn();
  regs.p.n = (regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
}

auto R65816::op_plb() {
  io();
  io();
L regs.db = readstack();
  regs.p.n = (regs.db & 0x80);
  regs.p.z = (regs.db == 0);
}

auto R65816::op_plp_e() {
  io();
  io();
L regs.p = readstack() | 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

auto R65816::op_plp_n() {
  io();
  io();
L regs.p = readstack();
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

auto R65816::op_pea_e() {
  aa.l = readpc();
  aa.h = readpc();
  writestackn(aa.h);
L writestackn(aa.l);
  regs.s.h = 0x01;
}

auto R65816::op_pea_n() {
  aa.l = readpc();
  aa.h = readpc();
  writestackn(aa.h);
L writestackn(aa.l);
}

auto R65816::op_pei_e() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  writestackn(aa.h);
L writestackn(aa.l);
  regs.s.h = 0x01;
}

auto R65816::op_pei_n() {
  dp = readpc();
  io2();
  aa.l = readdp(dp + 0);
  aa.h = readdp(dp + 1);
  writestackn(aa.h);
L writestackn(aa.l);
}

auto R65816::op_per_e() {
  aa.l = readpc();
  aa.h = readpc();
  io();
  rd.w = regs.pc.d + (int16)aa.w;
  writestackn(rd.h);
L writestackn(rd.l);
  regs.s.h = 0x01;
}

auto R65816::op_per_n() {
  aa.l = readpc();
  aa.h = readpc();
  io();
  rd.w = regs.pc.d + (int16)aa.w;
  writestackn(rd.h);
L writestackn(rd.l);
}
