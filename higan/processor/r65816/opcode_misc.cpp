auto R65816::op_nop() {
L op_io_irq();
}

auto R65816::op_wdm() {
L op_readpc();
}

auto R65816::op_xba() {
  op_io();
L op_io();
  regs.a.l ^= regs.a.h;
  regs.a.h ^= regs.a.l;
  regs.a.l ^= regs.a.h;
  regs.p.n = (regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

auto R65816::op_move_b(signed adjust) {
  dp = op_readpc();
  sp = op_readpc();
  regs.db = dp;
  rd.l = op_readlong((sp << 16) | regs.x.w);
  op_writelong((dp << 16) | regs.y.w, rd.l);
  op_io();
  regs.x.l += adjust;
  regs.y.l += adjust;
L op_io();
  if(regs.a.w--) regs.pc.w -= 3;
}

auto R65816::op_move_w(signed adjust) {
  dp = op_readpc();
  sp = op_readpc();
  regs.db = dp;
  rd.l = op_readlong((sp << 16) | regs.x.w);
  op_writelong((dp << 16) | regs.y.w, rd.l);
  op_io();
  regs.x.w += adjust;
  regs.y.w += adjust;
L op_io();
  if(regs.a.w--) regs.pc.w -= 3;
}

auto R65816::op_interrupt_e(uint16 vector) {
  op_readpc();
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.p);
  rd.l = op_readlong(vector + 0);
  regs.pc.b = 0;
  regs.p.i = 1;
  regs.p.d = 0;
L rd.h = op_readlong(vector + 1);
  regs.pc.w = rd.w;
}

auto R65816::op_interrupt_n(uint16 vector) {
  op_readpc();
  op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.p);
  rd.l = op_readlong(vector + 0);
  regs.pc.b = 0x00;
  regs.p.i = 1;
  regs.p.d = 0;
L rd.h = op_readlong(vector + 1);
  regs.pc.w = rd.w;
}

auto R65816::op_stp() {
  while(regs.wai = true) {
L   op_io();
  }
}

auto R65816::op_wai() {
  regs.wai = true;
  while(regs.wai) {
L   op_io();
  }
  op_io();
}

auto R65816::op_xce() {
L op_io_irq();
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
L op_io_irq();
  flag = value;
}

auto R65816::op_pflag_e(bool mode) {
  rd.l = op_readpc();
L op_io();
  regs.p = (mode ? regs.p | rd.l : regs.p & ~rd.l);
  regs.p.m = 1;
  regs.p.x = 1;
  regs.x.h = 0x00;
  regs.y.h = 0x00;
}

auto R65816::op_pflag_n(bool mode) {
  rd.l = op_readpc();
L op_io();
  regs.p = (mode ? regs.p | rd.l : regs.p & ~rd.l);
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

auto R65816::op_transfer_b(reg16_t& from, reg16_t& to) {
L op_io_irq();
  to.l = from.l;
  regs.p.n = (to.l & 0x80);
  regs.p.z = (to.l == 0);
}

auto R65816::op_transfer_w(reg16_t& from, reg16_t& to) {
L op_io_irq();
  to.w = from.w;
  regs.p.n = (to.w & 0x8000);
  regs.p.z = (to.w == 0);
}

auto R65816::op_tcs_e() {
L op_io_irq();
  regs.s.l = regs.a.l;
}

auto R65816::op_tcs_n() {
L op_io_irq();
  regs.s.w = regs.a.w;
}

auto R65816::op_tsx_b() {
L op_io_irq();
  regs.x.l = regs.s.l;
  regs.p.n = (regs.x.l & 0x80);
  regs.p.z = (regs.x.l == 0);
}

auto R65816::op_tsx_w() {
L op_io_irq();
  regs.x.w = regs.s.w;
  regs.p.n = (regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

auto R65816::op_txs_e() {
L op_io_irq();
  regs.s.l = regs.x.l;
}

auto R65816::op_txs_n() {
L op_io_irq();
  regs.s.w = regs.x.w;
}

auto R65816::op_push_b(reg16_t& reg) {
  op_io();
L op_writestack(reg.l);
}

auto R65816::op_push_w(reg16_t& reg) {
  op_io();
  op_writestack(reg.h);
L op_writestack(reg.l);
}

auto R65816::op_phd_e() {
  op_io();
  op_writestackn(regs.d.h);
L op_writestackn(regs.d.l);
  regs.s.h = 0x01;
}

auto R65816::op_phd_n() {
  op_io();
  op_writestackn(regs.d.h);
L op_writestackn(regs.d.l);
}

auto R65816::op_phb() {
  op_io();
L op_writestack(regs.db);
}

auto R65816::op_phk() {
  op_io();
L op_writestack(regs.pc.b);
}

auto R65816::op_php() {
  op_io();
L op_writestack(regs.p);
}

auto R65816::op_pull_b(reg16_t& reg) {
  op_io();
  op_io();
L reg.l = op_readstack();
  regs.p.n = (reg.l & 0x80);
  regs.p.z = (reg.l == 0);
}

auto R65816::op_pull_w(reg16_t& reg) {
  op_io();
  op_io();
  reg.l = op_readstack();
L reg.h = op_readstack();
  regs.p.n = (reg.w & 0x8000);
  regs.p.z = (reg.w == 0);
}

auto R65816::op_pld_e() {
  op_io();
  op_io();
  regs.d.l = op_readstackn();
L regs.d.h = op_readstackn();
  regs.p.n = (regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
  regs.s.h = 0x01;
}

auto R65816::op_pld_n() {
  op_io();
  op_io();
  regs.d.l = op_readstackn();
L regs.d.h = op_readstackn();
  regs.p.n = (regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
}

auto R65816::op_plb() {
  op_io();
  op_io();
L regs.db = op_readstack();
  regs.p.n = (regs.db & 0x80);
  regs.p.z = (regs.db == 0);
}

auto R65816::op_plp_e() {
  op_io();
  op_io();
L regs.p = op_readstack() | 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

auto R65816::op_plp_n() {
  op_io();
  op_io();
L regs.p = op_readstack();
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

auto R65816::op_pea_e() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_writestackn(aa.h);
L op_writestackn(aa.l);
  regs.s.h = 0x01;
}

auto R65816::op_pea_n() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_writestackn(aa.h);
L op_writestackn(aa.l);
}

auto R65816::op_pei_e() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp + 0);
  aa.h = op_readdp(dp + 1);
  op_writestackn(aa.h);
L op_writestackn(aa.l);
  regs.s.h = 0x01;
}

auto R65816::op_pei_n() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp + 0);
  aa.h = op_readdp(dp + 1);
  op_writestackn(aa.h);
L op_writestackn(aa.l);
}

auto R65816::op_per_e() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.w = regs.pc.d + (int16)aa.w;
  op_writestackn(rd.h);
L op_writestackn(rd.l);
  regs.s.h = 0x01;
}

auto R65816::op_per_n() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.w = regs.pc.d + (int16)aa.w;
  op_writestackn(rd.h);
L op_writestackn(rd.l);
}
