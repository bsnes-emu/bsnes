auto WDC65816::instructionINCImplied8(uint16& reg) -> void {
L idleIRQ();
  LO(reg)++;
  r.p.n = (LO(reg) & 0x80);
  r.p.z = (LO(reg) == 0);
}

auto WDC65816::instructionINCImplied16(uint16& reg) -> void {
L idleIRQ();
  reg++;
  r.p.n = (reg & 0x8000);
  r.p.z = (reg == 0);
}

auto WDC65816::instructionDECImplied8(uint16& reg) -> void {
L idleIRQ();
  LO(reg)--;
  r.p.n = (LO(reg) & 0x80);
  r.p.z = (LO(reg) == 0);
}

auto WDC65816::instructionDECImplied16(uint16& reg) -> void {
L idleIRQ();
  reg--;
  r.p.n = (reg & 0x8000);
  r.p.z = (reg == 0);
}

auto WDC65816::instructionASLImplied8() -> void {
L idleIRQ();
  r.p.c = (r.a.l & 0x80);
  r.a.l <<= 1;
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto WDC65816::instructionASLImplied16() -> void {
L idleIRQ();
  r.p.c = (r.a.w & 0x8000);
  r.a.w <<= 1;
  r.p.n = (r.a.w & 0x8000);
  r.p.z = (r.a.w == 0);
}

auto WDC65816::instructionLSRImplied8() -> void {
L idleIRQ();
  r.p.c = (r.a.l & 0x01);
  r.a.l >>= 1;
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto WDC65816::instructionLSRImplied16() -> void {
L idleIRQ();
  r.p.c = (r.a.w & 0x0001);
  r.a.w >>= 1;
  r.p.n = (r.a.w & 0x8000);
  r.p.z = (r.a.w == 0);
}

auto WDC65816::instructionROLImplied8() -> void {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = (r.a.l & 0x80);
  r.a.l = (r.a.l << 1) | carry;
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto WDC65816::instructionROLImplied16() -> void {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = (r.a.w & 0x8000);
  r.a.w = (r.a.w << 1) | carry;
  r.p.n = (r.a.w & 0x8000);
  r.p.z = (r.a.w == 0);
}

auto WDC65816::instructionRORImplied8() -> void {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = (r.a.l & 0x01);
  r.a.l = (carry << 7) | (r.a.l >> 1);
  r.p.n = (r.a.l & 0x80);
  r.p.z = (r.a.l == 0);
}

auto WDC65816::instructionRORImplied16() -> void {
L idleIRQ();
  bool carry = r.p.c;
  r.p.c = (r.a.w & 0x0001);
  r.a.w = (carry << 15) | (r.a.w >> 1);
  r.p.n = (r.a.w & 0x8000);
  r.p.z = (r.a.w == 0);
}

auto WDC65816::instructionBankModify8(fp op) -> void {
  aa.l = fetch();
  aa.h = fetch();
  rd.l = readBank(aa.w);
  idle();
  call(op);
L writeBank(aa.w, rd.l);
}

auto WDC65816::instructionBankModify16(fp op) -> void {
  aa.l = fetch();
  aa.h = fetch();
  rd.l = readBank(aa.w + 0);
  rd.h = readBank(aa.w + 1);
  idle();
  call(op);
  writeBank(aa.w + 1, rd.h);
L writeBank(aa.w + 0, rd.l);
}

auto WDC65816::instructionBankIndexedModify8(fp op) -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle();
  rd.l = readBank(aa.w + r.x.w);
  idle();
  call(op);
L writeBank(aa.w + r.x.w, rd.l);
}

auto WDC65816::instructionBankIndexedModify16(fp op) -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle();
  rd.l = readBank(aa.w + r.x.w + 0);
  rd.h = readBank(aa.w + r.x.w + 1);
  idle();
  call(op);
  writeBank(aa.w + r.x.w + 1, rd.h);
L writeBank(aa.w + r.x.w + 0, rd.l);
}

auto WDC65816::instructionDirectModify8(fp op) -> void {
  dp = fetch();
  idle2();
  rd.l = readDirect(dp);
  idle();
  call(op);
L writeDirect(dp, rd.l);
}

auto WDC65816::instructionDirectModify16(fp op) -> void {
  dp = fetch();
  idle2();
  rd.l = readDirect(dp + 0);
  rd.h = readDirect(dp + 1);
  idle();
  call(op);
  writeDirect(dp + 1, rd.h);
L writeDirect(dp + 0, rd.l);
}

auto WDC65816::instructionDirectIndexedModify8(fp op) -> void {
  dp = fetch();
  idle2();
  idle();
  rd.l = readDirect(dp + r.x.w);
  idle();
  call(op);
L writeDirect(dp + r.x.w, rd.l);
}

auto WDC65816::instructionDirectIndexedModify16(fp op) -> void {
  dp = fetch();
  idle2();
  idle();
  rd.l = readDirect(dp + r.x.w + 0);
  rd.h = readDirect(dp + r.x.w + 1);
  idle();
  call(op);
  writeDirect(dp + r.x.w + 1, rd.h);
L writeDirect(dp + r.x.w + 0, rd.l);
}
