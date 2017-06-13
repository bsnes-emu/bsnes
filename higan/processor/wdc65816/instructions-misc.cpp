auto WDC65816::instructionBITImmediate8() -> void {
L uint8 immediate = fetch();
  ZF = (immediate & AL) == 0;
}

auto WDC65816::instructionBITImmediate16() -> void {
  uint16 immediate = fetch();
L immediate |= fetch() << 8;
  ZF = (immediate & AW) == 0;
}

auto WDC65816::instructionNOP() -> void {
L idleIRQ();
}

auto WDC65816::instructionWDM() -> void {
L fetch();
}

auto WDC65816::instructionXBA() -> void {
  idle();
L idle();
  r.a.w = r.a.w >> 8 | r.a.w << 8;
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto WDC65816::instructionBlockMove8(int adjust) -> void {
  dp = fetch();
  sp = fetch();
  r.db = dp;
  rd.l = read(sp << 16 | r.x.w);
  write(dp << 16 | r.y.w, rd.l);
  idle();
  r.x.l += adjust;
  r.y.l += adjust;
L idle();
  if(r.a.w--) r.pc.w -= 3;
}

auto WDC65816::instructionBlockMove16(int adjust) -> void {
  dp = fetch();
  sp = fetch();
  r.db = dp;
  rd.l = read(sp << 16 | r.x.w);
  write(dp << 16 | r.y.w, rd.l);
  idle();
  r.x.w += adjust;
  r.y.w += adjust;
L idle();
  if(r.a.w--) r.pc.w -= 3;
}

auto WDC65816::instructionInterrupt(uint16 vector) -> void {
  fetch();
N push(PCB);
  push(PCH);
  push(PCL);
  push(P);
  IF = 1;
  DF = 0;
  PCL = read(vector++);
L PCH = read(vector++);
  PCB = 0x00;
}

auto WDC65816::instructionSTP() -> void {
  r.stp = true;
  while(r.stp && !synchronizing()) {
L   idle();
  }
}

auto WDC65816::instructionWAI() -> void {
  r.wai = true;
  while(r.wai && !synchronizing()) {
L   idle();
  }
  idle();
}

auto WDC65816::instructionXCE() -> void {
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

auto WDC65816::instructionSetFlag(bool& flag) -> void {
L idleIRQ();
  flag = 1;
}

auto WDC65816::instructionClearFlag(bool& flag) -> void {
L idleIRQ();
  flag = 0;
}

auto WDC65816::instructionREP() -> void {
  auto data = fetch();
L idle();
  P = P & ~data;
E XF = 1, MF = 1;
  if(XF) XH = 0x00, YH = 0x00;
}

auto WDC65816::instructionSEP() -> void {
  auto data = fetch();
L idle();
  P = P | data;
E XF = 1, MF = 1;
  if(XF) XH = 0x00, YH = 0x00;
}

auto WDC65816::instructionTransfer8(uint16& from, uint16& to) -> void {
L idleIRQ();
  LO(to) = LO(from);
  r.p.n = (LO(to) & 0x80);
  r.p.z = (LO(to) == 0);
}

auto WDC65816::instructionTransfer16(uint16& from, uint16& to) -> void {
L idleIRQ();
  to = from;
  r.p.n = (to & 0x8000);
  r.p.z = (to == 0);
}

auto WDC65816::instructionTCS() -> void {
L idleIRQ();
  r.s.w = r.a.w;
E r.s.h = 0x01;
}

auto WDC65816::instructionTSX8() -> void {
L idleIRQ();
  r.x.l = r.s.l;
  r.p.n = (r.x.l & 0x80);
  r.p.z = (r.x.l == 0);
}

auto WDC65816::instructionTSX16() -> void {
L idleIRQ();
  r.x.w = r.s.w;
  r.p.n = (r.x.w & 0x8000);
  r.p.z = (r.x.w == 0);
}

auto WDC65816::instructionTXS() -> void {
L idleIRQ();
E r.s.l = r.x.l;
N r.s.w = r.x.w;
}

auto WDC65816::instructionPush8(uint16& reg) -> void {
  idle();
L push(reg);
}

auto WDC65816::instructionPush16(uint16& reg) -> void {
  idle();
  push(reg >> 8);
L push(reg >> 0);
}

auto WDC65816::instructionPHD() -> void {
  idle();
  pushN(r.d.h);
L pushN(r.d.l);
E r.s.h = 0x01;
}

auto WDC65816::instructionPHB() -> void {
  idle();
L push(r.db);
}

auto WDC65816::instructionPHK() -> void {
  idle();
L push(r.pc.b);
}

auto WDC65816::instructionPHP() -> void {
  idle();
L push(r.p);
}

auto WDC65816::instructionPull8(uint16& reg) -> void {
  idle();
  idle();
L LO(reg) = pull();
  r.p.n = (LO(reg) & 0x80);
  r.p.z = (LO(reg) == 0);
}

auto WDC65816::instructionPull16(uint16& reg) -> void {
  idle();
  idle();
  LO(reg) = pull();
L HI(reg) = pull();
  r.p.n = (reg & 0x8000);
  r.p.z = (reg == 0);
}

auto WDC65816::instructionPLD() -> void {
  idle();
  idle();
  r.d.l = pullN();
L r.d.h = pullN();
  r.p.n = (r.d.w & 0x8000);
  r.p.z = (r.d.w == 0);
E r.s.h = 0x01;
}

auto WDC65816::instructionPLB() -> void {
  idle();
  idle();
L r.db = pull();
  r.p.n = (r.db & 0x80);
  r.p.z = (r.db == 0);
}

auto WDC65816::instructionPLP() -> void {
  idle();
  idle();
L r.p = pull();
E r.p.m = 1, r.p.x = 1;
  if(r.p.x) {
    r.x.h = 0x00;
    r.y.h = 0x00;
  }
}

auto WDC65816::instructionPEA() -> void {
  aa.l = fetch();
  aa.h = fetch();
  pushN(aa.h);
L pushN(aa.l);
E r.s.h = 0x01;
}

auto WDC65816::instructionPEI() -> void {
  dp = fetch();
  idle2();
  aa.l = readDirectN(dp + 0);
  aa.h = readDirectN(dp + 1);
  pushN(aa.h);
L pushN(aa.l);
E r.s.h = 0x01;
}

auto WDC65816::instructionPER() -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle();
  rd.w = r.pc.d + (int16)aa.w;
  pushN(rd.h);
L pushN(rd.l);
E r.s.h = 0x01;
}
