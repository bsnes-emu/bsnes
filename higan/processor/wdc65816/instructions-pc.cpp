auto WDC65816::instructionBranch(bool take) -> void {
  if(!take) {
L   rd.l = fetch();
  } else {
    rd.l = fetch();
    aa.w = r.pc.d + (int8)rd.l;
    idle6(aa.w);
L   idle();
    r.pc.w = aa.w;
  }
}

auto WDC65816::instructionBRL() -> void {
  rd.l = fetch();
  rd.h = fetch();
L idle();
  r.pc.w = r.pc.d + (int16)rd.w;
}

auto WDC65816::instructionJMPShort() -> void {
  rd.l = fetch();
L rd.h = fetch();
  r.pc.w = rd.w;
}

auto WDC65816::instructionJMPLong() -> void {
  rd.l = fetch();
  rd.h = fetch();
L rd.b = fetch();
  r.pc.d = rd.d;
}

auto WDC65816::instructionJMPIndirect() -> void {
  aa.l = fetch();
  aa.h = fetch();
  rd.l = read(uint16(aa.w + 0));
L rd.h = read(uint16(aa.w + 1));
  r.pc.w = rd.w;
}

auto WDC65816::instructionJMPIndexedIndirect() -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle();
  rd.l = read(PCB << 16 | uint16(aa.w + r.x.w + 0));
L rd.h = read(PCB << 16 | uint16(aa.w + r.x.w + 1));
  r.pc.w = rd.w;
}

auto WDC65816::instructionJMPIndirectLong() -> void {
  aa.l = fetch();
  aa.h = fetch();
  rd.l = read(uint16(aa.w + 0));
  rd.h = read(uint16(aa.w + 1));
L rd.b = read(uint16(aa.w + 2));
  r.pc.d = rd.d;
}

auto WDC65816::instructionJSRShort() -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle();
  r.pc.w--;
  push(r.pc.h);
L push(r.pc.l);
  r.pc.w = aa.w;
}

auto WDC65816::instructionJSRLong() -> void {
  aa.l = fetch();
  aa.h = fetch();
  pushN(r.pc.b);
  idle();
  aa.b = fetch();
  r.pc.w--;
  pushN(r.pc.h);
L pushN(r.pc.l);
  r.pc.d = aa.d;
E r.s.h = 0x01;
}

auto WDC65816::instructionJSRIndexedIndirect() -> void {
  aa.l = fetch();
  pushN(r.pc.h);
  pushN(r.pc.l);
  aa.h = fetch();
  idle();
  rd.l = read(PCB << 16 | uint16(aa.w + r.x.w + 0));
L rd.h = read(PCB << 16 | uint16(aa.w + r.x.w + 1));
  r.pc.w = rd.w;
E r.s.h = 0x01;
}

auto WDC65816::instructionRTI() -> void {
  idle();
  idle();
  r.p = pull();
E r.p.m = 1, r.p.x = 1;
  if(r.p.x) {
    r.x.h = 0x00;
    r.y.h = 0x00;
  }
  r.pc.l = pull();
  if(r.e) {
  L r.pc.h = pull();
  } else {
    r.pc.h = pull();
  L r.pc.b = pull();
  }
}

auto WDC65816::instructionRTS() -> void {
  idle();
  idle();
  rd.l = pull();
  rd.h = pull();
L idle();
  r.pc.w = ++rd.w;
}

auto WDC65816::instructionRTL() -> void {
  idle();
  idle();
  rd.l = pullN();
  rd.h = pullN();
L rd.b = pullN();
  r.pc.b = rd.b;
  r.pc.w = ++rd.w;
E r.s.h = 0x01;
}
