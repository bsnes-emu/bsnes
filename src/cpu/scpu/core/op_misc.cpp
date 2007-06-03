void sCPU::op_nop() {
  last_cycle();
  op_io();
}

void sCPU::op_wdm() {
  last_cycle();
  op_readpc();
}

void sCPU::op_xba() {
  op_io();
  last_cycle();
  op_io();
  regs.a.l ^= regs.a.h;
  regs.a.h ^= regs.a.l;
  regs.a.l ^= regs.a.h;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void sCPU::op_mvn() {
  dp = op_readpc();
  sp = op_readpc();
  regs.db = dp;
  rd.l = op_readlong((sp << 16) | regs.x.w);
  op_writelong((dp << 16) | regs.y.w, rd.l);
  op_io();
  if(regs.p.x) {
    regs.x.l ++;
    regs.y.l ++;
  } else {
    regs.x.w ++;
    regs.y.w ++;
  }
  last_cycle();
  op_io();
  if(regs.a.w--)regs.pc.w -= 3;
}

void sCPU::op_mvp() {
  dp = op_readpc();
  sp = op_readpc();
  regs.db = dp;
  rd.l = op_readlong((sp << 16) | regs.x.w);
  op_writelong((dp << 16) | regs.y.w, rd.l);
  op_io();
  if(regs.p.x) {
    regs.x.l --;
    regs.y.l --;
  } else {
    regs.x.w --;
    regs.y.w --;
  }
  last_cycle();
  op_io();
  if(regs.a.w--)regs.pc.w -= 3;
}

void sCPU::op_brk() {
  op_readpc();
  if(!regs.e)op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.p);
  rd.l = op_readlong((regs.e) ? 0xfffe : 0xffe6);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  last_cycle();
  rd.h = op_readlong((regs.e) ? 0xffff : 0xffe7);
  regs.pc.w = rd.w;
}

void sCPU::op_cop() {
  op_readpc();
  if(!regs.e)op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.p);
  rd.l = op_readlong((regs.e) ? 0xfff4 : 0xffe4);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  last_cycle();
  rd.h = op_readlong((regs.e) ? 0xfff5 : 0xffe5);
  regs.pc.w = rd.w;
}

void sCPU::op_stp() {
  op_io();
  last_cycle();
  while(1) { op_io(); }
}

void sCPU::op_wai() {
  //last_cycle() will set event.wai to false
//once an NMI / IRQ edge is reached
  event.wai = true;
  while(event.wai) {
    last_cycle();
    op_io();
  }
  op_io();
}

void sCPU::op_xce() {
  last_cycle();
  op_io();
bool carry = regs.p.c;
  regs.p.c = regs.e;
  regs.e = carry;
  if(regs.e) {
    regs.p |= 0x30;
    regs.s.h = 0x01;
  }
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

void sCPU::op_clc() {
  last_cycle();
  op_io();
  regs.p.c = 0;
}

void sCPU::op_cld() {
  last_cycle();
  op_io();
  regs.p.d = 0;
}

void sCPU::op_cli() {
  last_cycle();
  op_io();
  regs.p.i = 0;
}

void sCPU::op_clv() {
  last_cycle();
  op_io();
  regs.p.v = 0;
}

void sCPU::op_sec() {
  last_cycle();
  op_io();
  regs.p.c = 1;
}

void sCPU::op_sed() {
  last_cycle();
  op_io();
  regs.p.d = 1;
}

void sCPU::op_sei() {
  last_cycle();
  op_io();
  regs.p.i = 1;
}

void sCPU::op_rep() {
  rd.l = op_readpc();
  last_cycle();
  op_io();
  regs.p &=~ rd.l;
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

void sCPU::op_sep() {
  rd.l = op_readpc();
  last_cycle();
  op_io();
  regs.p |= rd.l;
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

void sCPU::op_tax() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l = regs.a.l;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w = regs.a.w;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
}

void sCPU::op_tay() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.y.l = regs.a.l;
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  } else {
    regs.y.w = regs.a.w;
    regs.p.n = !!(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }
}

void sCPU::op_txa() {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.a.l = regs.x.l;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.a.w = regs.x.w;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_txy() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.y.l = regs.x.l;
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
  } else {
    regs.y.w = regs.x.w;
    regs.p.n = !!(regs.y.w & 0x8000);
    regs.p.z = (regs.y.w == 0);
  }
}

void sCPU::op_tya() {
  last_cycle();
  op_io();
  if(regs.p.m) {
    regs.a.l = regs.y.l;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.a.w = regs.y.w;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_tyx() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l = regs.y.l;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w = regs.y.w;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
}

void sCPU::op_tcd() {
  last_cycle();
  op_io();
  regs.d.w = regs.a.w;
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
}

void sCPU::op_tcs() {
  last_cycle();
  op_io();
  regs.s.w = regs.a.w;
  if(regs.e)regs.s.h = 0x01;
}

void sCPU::op_tdc() {
  last_cycle();
  op_io();
  regs.a.w = regs.d.w;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void sCPU::op_tsc() {
  last_cycle();
  op_io();
  regs.a.w = regs.s.w;
  if(regs.e) {
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void sCPU::op_tsx() {
  last_cycle();
  op_io();
  if(regs.p.x) {
    regs.x.l = regs.s.l;
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
  } else {
    regs.x.w = regs.s.w;
    regs.p.n = !!(regs.x.w & 0x8000);
    regs.p.z = (regs.x.w == 0);
  }
}

void sCPU::op_txs() {
  last_cycle();
  op_io();
  if(regs.e) {
    regs.s.l = regs.x.l;
  } else {
    regs.s.w = regs.x.w;
  }
}

void sCPU::op_pha() {
  op_io();
  if(!regs.p.m)op_writestack(regs.a.h);
  last_cycle();
  op_writestack(regs.a.l);
}

void sCPU::op_phx() {
  op_io();
  if(!regs.p.x)op_writestack(regs.x.h);
  last_cycle();
  op_writestack(regs.x.l);
}

void sCPU::op_phy() {
  op_io();
  if(!regs.p.x)op_writestack(regs.y.h);
  last_cycle();
  op_writestack(regs.y.l);
}

void sCPU::op_phd() {
  op_io();
  op_writestackn(regs.d.h);
  last_cycle();
  op_writestackn(regs.d.l);
  if(regs.e)regs.s.h = 0x01;
}

void sCPU::op_phb() {
  op_io();
  last_cycle();
  op_writestack(regs.db);
}

void sCPU::op_phk() {
  op_io();
  last_cycle();
  op_writestack(regs.pc.b);
}

void sCPU::op_php() {
  op_io();
  last_cycle();
  op_writestack(regs.p);
}

void sCPU::op_pla() {
  op_io();
  op_io();
  if(regs.p.m)last_cycle();
  regs.a.l = op_readstack();
  if(regs.p.m) {
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
    return;
  }
  last_cycle();
  regs.a.h = op_readstack();
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void sCPU::op_plx() {
  op_io();
  op_io();
  if(regs.p.x)last_cycle();
  regs.x.l = op_readstack();
  if(regs.p.x) {
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
    return;
  }
  last_cycle();
  regs.x.h = op_readstack();
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

void sCPU::op_ply() {
  op_io();
  op_io();
  if(regs.p.x)last_cycle();
  regs.y.l = op_readstack();
  if(regs.p.x) {
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
    return;
  }
  last_cycle();
  regs.y.h = op_readstack();
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

void sCPU::op_pld() {
  op_io();
  op_io();
  regs.d.l = op_readstackn();
  last_cycle();
  regs.d.h = op_readstackn();
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
  if(regs.e)regs.s.h = 0x01;
}

void sCPU::op_plb() {
  op_io();
  op_io();
  last_cycle();
  regs.db = op_readstack();
  regs.p.n = !!(regs.db & 0x80);
  regs.p.z = (regs.db == 0);
}

void sCPU::op_plp() {
  op_io();
  op_io();
  last_cycle();
  regs.p = op_readstack();
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

void sCPU::op_pea() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_writestackn(aa.h);
  last_cycle();
  op_writestackn(aa.l);
  if(regs.e)regs.s.h = 0x01;
}

void sCPU::op_pei() {
  dp = op_readpc();
  op_io_cond2();
  aa.l = op_readdp(dp);
  aa.h = op_readdp(dp + 1);
  op_writestackn(aa.h);
  last_cycle();
  op_writestackn(aa.l);
  if(regs.e)regs.s.h = 0x01;
}

void sCPU::op_per() {
  aa.l = op_readpc();
  aa.h = op_readpc();
  op_io();
  rd.w = regs.pc.d + (int16)aa.w;
  op_writestackn(rd.h);
  last_cycle();
  op_writestackn(rd.l);
  if(regs.e)regs.s.h = 0x01;
}

