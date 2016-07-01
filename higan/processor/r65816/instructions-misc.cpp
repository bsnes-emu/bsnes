auto R65816::op_nop() {
L idleIRQ();
}

auto R65816::op_wdm() {
L readPC();
}

auto R65816::op_xba() {
  idle();
L idle();
  r.a.w = r.a.w >> 8 | r.a.w << 8;
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto R65816::op_move_b(int adjust) {
  dp = readPC();
  sp = readPC();
  r.db = dp;
  rd.l = readLong(sp << 16 | r.x.w);
  writeLong(dp << 16 | r.y.w, rd.l);
  idle();
  r.x.l += adjust;
  r.y.l += adjust;
L idle();
  if(r.a.w--) r.pc.w -= 3;
}

auto R65816::op_move_w(int adjust) {
  dp = readPC();
  sp = readPC();
  r.db = dp;
  rd.l = readLong(sp << 16 | r.x.w);
  writeLong(dp << 16 | r.y.w, rd.l);
  idle();
  r.x.w += adjust;
  r.y.w += adjust;
L idle();
  if(r.a.w--) r.pc.w -= 3;
}

auto R65816::op_interrupt(uint16 vector) {
  readPC();
N writeSP(r.pc.b);
  writeSP(r.pc.h);
  writeSP(r.pc.l);
  writeSP(r.p);
  r.pc.l = readLong(vector + 0);
  r.p.i = 1;
  r.p.d = 0;
L r.pc.h = readLong(vector + 1);
  r.pc.b = 0x00;
}

auto R65816::op_stp() {
  while(r.wai = true) {
L   idle();
  }
}

auto R65816::op_wai() {
  r.wai = true;
  while(r.wai) {
L   idle();
  }
  idle();
}

auto R65816::op_xce() {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = r.e;
  r.e = carry;
  if(r.e) {
    r.p.m = 1;
    r.p.x = 1;
    r.x.h = 0x00;
    r.y.h = 0x00;
    r.s.h = 0x01;
  }
}

auto R65816::op_set_flag(uint bit) {
L idleIRQ();
  r.p |= 1 << bit;
}

auto R65816::op_clear_flag(uint bit) {
L idleIRQ();
  r.p &= ~(1 << bit);
}

auto R65816::op_pflag(bool mode) {
  rd.l = readPC();
L idle();
  r.p = (mode ? r.p | rd.l : r.p & ~rd.l);
E r.p.m = 1, r.p.x = 1;
  if(r.p.x) {
    r.x.h = 0x00;
    r.y.h = 0x00;
  }
}

auto R65816::op_transfer_b(Reg16& from, Reg16& to) {
L idleIRQ();
  to.l = from.l;
  r.p.n = (to.l & 0x80);
  r.p.z = (to.l == 0);
}

auto R65816::op_transfer_w(Reg16& from, Reg16& to) {
L idleIRQ();
  to.w = from.w;
  r.p.n = (to.w & 0x8000);
  r.p.z = (to.w == 0);
}

auto R65816::op_tcs() {
L idleIRQ();
  r.s.w = r.a.w;
E r.s.h = 0x01;
}

auto R65816::op_tsx_b() {
L idleIRQ();
  r.x.l = r.s.l;
  r.p.n = (r.x.l & 0x80);
  r.p.z = (r.x.l == 0);
}

auto R65816::op_tsx_w() {
L idleIRQ();
  r.x.w = r.s.w;
  r.p.n = (r.x.w & 0x8000);
  r.p.z = (r.x.w == 0);
}

auto R65816::op_txs() {
L idleIRQ();
E r.s.l = r.x.l;
N r.s.w = r.x.w;
}

auto R65816::op_push_b(Reg16& reg) {
  idle();
L writeSP(reg.l);
}

auto R65816::op_push_w(Reg16& reg) {
  idle();
  writeSP(reg.h);
L writeSP(reg.l);
}

auto R65816::op_phd() {
  idle();
  writeSPn(r.d.h);
L writeSPn(r.d.l);
E r.s.h = 0x01;
}

auto R65816::op_phb() {
  idle();
L writeSP(r.db);
}

auto R65816::op_phk() {
  idle();
L writeSP(r.pc.b);
}

auto R65816::op_php() {
  idle();
L writeSP(r.p);
}

auto R65816::op_pull_b(Reg16& reg) {
  idle();
  idle();
L reg.l = readSP();
  r.p.n = (reg.l & 0x80);
  r.p.z = (reg.l == 0);
}

auto R65816::op_pull_w(Reg16& reg) {
  idle();
  idle();
  reg.l = readSP();
L reg.h = readSP();
  r.p.n = (reg.w & 0x8000);
  r.p.z = (reg.w == 0);
}

auto R65816::op_pld() {
  idle();
  idle();
  r.d.l = readSPn();
L r.d.h = readSPn();
  r.p.n = (r.d.w & 0x8000);
  r.p.z = (r.d.w == 0);
E r.s.h = 0x01;
}

auto R65816::op_plb() {
  idle();
  idle();
L r.db = readSP();
  r.p.n = (r.db & 0x80);
  r.p.z = (r.db == 0);
}

auto R65816::op_plp() {
  idle();
  idle();
L r.p = readSP();
E r.p.m = 1, r.p.x = 1;
  if(r.p.x) {
    r.x.h = 0x00;
    r.y.h = 0x00;
  }
}

auto R65816::op_pea() {
  aa.l = readPC();
  aa.h = readPC();
  writeSPn(aa.h);
L writeSPn(aa.l);
E r.s.h = 0x01;
}

auto R65816::op_pei() {
  dp = readPC();
  idle2();
  aa.l = readDPn(dp + 0);
  aa.h = readDPn(dp + 1);
  writeSPn(aa.h);
L writeSPn(aa.l);
E r.s.h = 0x01;
}

auto R65816::op_per() {
  aa.l = readPC();
  aa.h = readPC();
  idle();
  rd.w = r.pc.d + (int16)aa.w;
  writeSPn(rd.h);
L writeSPn(rd.l);
E r.s.h = 0x01;
}
