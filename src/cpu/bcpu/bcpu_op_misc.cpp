void bCPU::op_nop() {
l1:
  cpu_io();
}

void bCPU::op_wdm() {
l1:
  cpu_io();
  regs.pc.w++;
}

void bCPU::op_xba() {
l1:
  cpu_io();
l2:
  cpu_io();
  regs.a.l ^= regs.a.h;
  regs.a.h ^= regs.a.l;
  regs.a.l ^= regs.a.h;
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::op_mvn() {
l1:
  dp = op_read();
l2:
  sp = op_read();
l3:
  regs.db = dp;
  rd.l = op_read(OPMODE_LONG, (sp << 16) | regs.x.w);
l4:
  op_write(OPMODE_LONG, (dp << 16) | regs.y.w, rd.l);
l5:
  cpu_io();
  if(regs.p.x) { regs.x.l++; regs.y.l++; }
  else         { regs.x.w++; regs.y.w++; }
l6:
  cpu_io();
  if(regs.a.w--)regs.pc.w -= 3;
}

void bCPU::op_mvp() {
l1:
  dp = op_read();
l2:
  sp = op_read();
l3:
  regs.db = dp;
  rd.l = op_read(OPMODE_LONG, (sp << 16) | regs.x.w);
l4:
  op_write(OPMODE_LONG, (dp << 16) | regs.y.w, rd.l);
l5:
  cpu_io();
  if(regs.p.x) { regs.x.l--; regs.y.l--; }
  else         { regs.x.w--; regs.y.w--; }
l6:
  cpu_io();
  if(regs.a.w--)regs.pc.w -= 3;
}

void bCPU::op_brk() {
l1:
  op_read();
  if(regs.e)goto l3;
l2:
  stack_write(regs.pc.b);
l3:
  stack_write(regs.pc.h);
l4:
  stack_write(regs.pc.l);
l5:
  stack_write(regs.p);
l6:
  rd.l = op_read(OPMODE_LONG, (regs.e)?0xfffe:0xffe6);
l7:
  rd.h = op_read(OPMODE_LONG, (regs.e)?0xffff:0xffe7);
  regs.pc.b = 0x00;
  regs.pc.w = rd.w;
  regs.p.i  = 1;
  regs.p.d  = 0;
}

void bCPU::op_cop() {
l1:
  op_read();
  if(regs.e)goto l3;
l2:
  stack_write(regs.pc.b);
l3:
  stack_write(regs.pc.h);
l4:
  stack_write(regs.pc.l);
l5:
  stack_write(regs.p);
l6:
  rd.l = op_read(OPMODE_LONG, (regs.e)?0xfff4:0xffe4);
l7:
  rd.h = op_read(OPMODE_LONG, (regs.e)?0xfff5:0xffe5);
  regs.pc.b = 0x00;
  regs.pc.w = rd.w;
  regs.p.i  = 1;
  regs.p.d  = 0;
}

void bCPU::op_stp() {
l1:
  cpu_io();
  status.cpu_state = CPUSTATE_STP;
l2:
  cpu_io();
  regs.pc.w--;
}

void bCPU::op_wai() {
l1:
  cpu_io();
  status.cpu_state = CPUSTATE_WAI;
l2:
  cpu_io();
  regs.pc.w--;
}

void bCPU::op_xce() {
l1:
  cpu_io();
bool c = regs.p.c;
  regs.p.c = regs.e;
  regs.e = c;
  if(regs.e) {
    regs.p  |= 0x30;
    regs.x.h = 0x00;
    regs.y.h = 0x00;
    regs.s.h = 0x01;
  }
}

void bCPU::op_clc() {
l1:
  cpu_io();
  regs.p.c = 0;
}

void bCPU::op_cld() {
l1:
  cpu_io();
  regs.p.d = 0;
}

void bCPU::op_cli() {
l1:
  cpu_io();
  regs.p.i = 0;
}

void bCPU::op_clv() {
l1:
  cpu_io();
  regs.p.v = 0;
}

void bCPU::op_sec() {
l1:
  cpu_io();
  regs.p.c = 1;
}

void bCPU::op_sed() {
l1:
  cpu_io();
  regs.p.d = 1;
}

void bCPU::op_sei() {
l1:
  cpu_io();
  regs.p.i = 1;
}

void bCPU::op_rep() {
l1:
  rd.l = op_read();
l2:
  cpu_io();
  regs.p &=~ rd.l;
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

void bCPU::op_sep() {
l1:
  rd.l = op_read();
l2:
  cpu_io();
  regs.p |= rd.l;
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

void bCPU::op_tax() {
l1:
  cpu_io();
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

void bCPU::op_tay() {
l1:
  cpu_io();
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

void bCPU::op_txa() {
l1:
  cpu_io();
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

void bCPU::op_txy() {
l1:
  cpu_io();
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

void bCPU::op_tya() {
l1:
  cpu_io();
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

void bCPU::op_tyx() {
l1:
  cpu_io();
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

void bCPU::op_tcd() {
l1:
  cpu_io();
  regs.d.w = regs.a.w;
  regs.p.n = !!(regs.d.w & 0x8000);
  regs.p.z = (regs.d.w == 0);
}

void bCPU::op_tcs() {
l1:
  cpu_io();
  regs.s.w = regs.a.w;
  if(regs.e)regs.s.h = 0x01;
}

void bCPU::op_tdc() {
l1:
  cpu_io();
  regs.a.w = regs.d.w;
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_tsc() {
l1:
  cpu_io();
  regs.a.w = regs.s.w;
  if(regs.e) {
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

void bCPU::op_tsx() {
l1:
  cpu_io();
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

void bCPU::op_txs() {
l1:
  cpu_io();
  if(regs.e) {
    regs.s.l = regs.x.l;
    regs.p.n = !!(regs.s.l & 0x80);
    regs.p.z = (regs.s.l == 0);
  } else {
    regs.s.w = regs.x.w;
    regs.p.n = !!(regs.s.w & 0x8000);
    regs.p.z = (regs.s.w == 0);
  }
}

void bCPU::op_pha() {
l1:
  cpu_io();
  if(regs.p.m)goto l3;
l2:
  stack_write(regs.a.h);
l3:
  stack_write(regs.a.l);
}

void bCPU::op_phx() {
l1:
  cpu_io();
  if(regs.p.x)goto l3;
l2:
  stack_write(regs.x.h);
l3:
  stack_write(regs.x.l);
}

void bCPU::op_phy() {
l1:
  cpu_io();
  if(regs.p.x)goto l3;
l2:
  stack_write(regs.y.h);
l3:
  stack_write(regs.y.l);
}

void bCPU::op_phd() {
l1:
  cpu_io();
  if(0)goto l3;
l2:
  stack_write(regs.       d.h);
l3:
  stack_write(regs.       d.l);
}

void bCPU::op_phb() {
l1:
  cpu_io();
l2:
  stack_write(regs.db);
}

void bCPU::op_phk() {
l1:
  cpu_io();
l2:
  stack_write(regs.pc.b);
}

void bCPU::op_php() {
l1:
  cpu_io();
l2:
  stack_write(regs.p);
}

void bCPU::op_pla() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  regs.a.l = stack_read();
  if(regs.p.m) {
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
    return;
  }
l4:
  regs.a.h = stack_read();
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

void bCPU::op_plx() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  regs.x.l = stack_read();
  if(regs.p.x) {
    regs.p.n = !!(regs.x.l & 0x80);
    regs.p.z = (regs.x.l == 0);
    return;
  }
l4:
  regs.x.h = stack_read();
  regs.p.n = !!(regs.x.w & 0x8000);
  regs.p.z = (regs.x.w == 0);
}

void bCPU::op_ply() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  regs.y.l = stack_read();
  if(regs.p.x) {
    regs.p.n = !!(regs.y.l & 0x80);
    regs.p.z = (regs.y.l == 0);
    return;
  }
l4:
  regs.y.h = stack_read();
  regs.p.n = !!(regs.y.w & 0x8000);
  regs.p.z = (regs.y.w == 0);
}

void bCPU::op_pld() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  regs.       d.l = stack_read();
  if(0) {
    regs.p.n = !!(regs.       d.l & 0x80);
    regs.p.z = (regs.       d.l == 0);
    return;
  }
l4:
  regs.       d.h = stack_read();
  regs.p.n = !!(regs.       d.w & 0x8000);
  regs.p.z = (regs.       d.w == 0);
}

void bCPU::op_plb() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  regs.db = stack_read();
  regs.p.n = !!(regs.db & 0x80);
  regs.p.z = (regs.db == 0);
}

void bCPU::op_plp() {
l1:
  cpu_io();
l2:
  cpu_io();
l3:
  regs.p = stack_read();
  if(regs.e)regs.p |= 0x30;
  if(regs.p.x) {
    regs.x.h = 0x00;
    regs.y.h = 0x00;
  }
}

void bCPU::op_pea() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  stack_write(aa.h);
l4:
  stack_write(aa.l);
}

void bCPU::op_pei() {
l1:
  dp = op_read();
l2:
  cpu_c2();
l3:
  aa.l = op_read(OPMODE_DP, dp);
l4:
  aa.h = op_read(OPMODE_DP, dp + 1);
l5:
  stack_write(aa.h);
l6:
  stack_write(aa.l);
}

void bCPU::op_per() {
l1:
  aa.l = op_read();
l2:
  aa.h = op_read();
l3:
  cpu_io();
  rd.w = regs.pc.d + (int16)aa.w;
l4:
  stack_write(rd.h);
l5:
  stack_write(rd.l);
}

