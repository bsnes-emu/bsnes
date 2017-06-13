auto WDC65816::instructionBankWrite8(uint16& reg) -> void {
  aa.l = fetch();
  aa.h = fetch();
L writeBank(aa.w, reg);
}

auto WDC65816::instructionBankWrite16(uint16& reg) -> void {
  aa.l = fetch();
  aa.h = fetch();
  writeBank(aa.w + 0, reg >> 0);
L writeBank(aa.w + 1, reg >> 8);
}

auto WDC65816::instructionBankWrite8(uint16& reg, uint16 index) -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle();
L writeBank(aa.w + index, reg);
}

auto WDC65816::instructionBankWrite16(uint16& reg, uint16 index) -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle();
  writeBank(aa.w + index + 0, reg >> 0);
L writeBank(aa.w + index + 1, reg >> 8);
}

auto WDC65816::instructionLongWrite8(uint16 index) -> void {
  aa.l = fetch();
  aa.h = fetch();
  aa.b = fetch();
L write(aa.d + index, r.a.l);
}

auto WDC65816::instructionLongWrite16(uint16 index) -> void {
  aa.l = fetch();
  aa.h = fetch();
  aa.b = fetch();
  write(aa.d + index + 0, r.a.l);
L write(aa.d + index + 1, r.a.h);
}

auto WDC65816::instructionDirectWrite8(uint16& reg) -> void {
  dp = fetch();
  idle2();
L writeDirect(dp, reg);
}

auto WDC65816::instructionDirectWrite16(uint16& reg) -> void {
  dp = fetch();
  idle2();
  writeDirect(dp + 0, reg >> 0);
L writeDirect(dp + 1, reg >> 8);
}

auto WDC65816::instructionDirectWrite8(uint16& reg, uint16 index) -> void {
  dp = fetch();
  idle2();
  idle();
L writeDirect(dp + index, reg);
}

auto WDC65816::instructionDirectWrite16(uint16& reg, uint16 index) -> void {
  dp = fetch();
  idle2();
  idle();
  writeDirect(dp + index + 0, reg >> 0);
L writeDirect(dp + index + 1, reg >> 8);
}

auto WDC65816::instructionIndirectWrite8() -> void {
  dp = fetch();
  idle2();
  aa.l = readDirect(dp + 0);
  aa.h = readDirect(dp + 1);
L writeBank(aa.w, r.a.l);
}

auto WDC65816::instructionIndirectWrite16() -> void {
  dp = fetch();
  idle2();
  aa.l = readDirect(dp + 0);
  aa.h = readDirect(dp + 1);
  writeBank(aa.w + 0, r.a.l);
L writeBank(aa.w + 1, r.a.h);
}

auto WDC65816::instructionIndexedIndirectWrite8() -> void {
  dp = fetch();
  idle2();
  idle();
  aa.l = readDirect(dp + r.x.w + 0);
  aa.h = readDirect(dp + r.x.w + 1);
L writeBank(aa.w, r.a.l);
}

auto WDC65816::instructionIndexedIndirectWrite16() -> void {
  dp = fetch();
  idle2();
  idle();
  aa.l = readDirect(dp + r.x.w + 0);
  aa.h = readDirect(dp + r.x.w + 1);
  writeBank(aa.w + 0, r.a.l);
L writeBank(aa.w + 1, r.a.h);
}

auto WDC65816::instructionIndirectIndexedWrite8() -> void {
  dp = fetch();
  idle2();
  aa.l = readDirect(dp + 0);
  aa.h = readDirect(dp + 1);
  idle();
L writeBank(aa.w + r.y.w, r.a.l);
}

auto WDC65816::instructionIndirectIndexedWrite16() -> void {
  dp = fetch();
  idle2();
  aa.l = readDirect(dp + 0);
  aa.h = readDirect(dp + 1);
  idle();
  writeBank(aa.w + r.y.w + 0, r.a.l);
L writeBank(aa.w + r.y.w + 1, r.a.h);
}

auto WDC65816::instructionIndirectLongWrite8(uint16 index) -> void {
  dp = fetch();
  idle2();
  aa.l = readDirectN(dp + 0);
  aa.h = readDirectN(dp + 1);
  aa.b = readDirectN(dp + 2);
L write(aa.d + index, r.a.l);
}

auto WDC65816::instructionIndirectLongWrite16(uint16 index) -> void {
  dp = fetch();
  idle2();
  aa.l = readDirectN(dp + 0);
  aa.h = readDirectN(dp + 1);
  aa.b = readDirectN(dp + 2);
  write(aa.d + index + 0, r.a.l);
L write(aa.d + index + 1, r.a.h);
}

auto WDC65816::instructionStackWrite8() -> void {
  sp = fetch();
  idle();
L writeStack(sp, r.a.l);
}

auto WDC65816::instructionStackWrite16() -> void {
  sp = fetch();
  idle();
  writeStack(sp + 0, r.a.l);
L writeStack(sp + 1, r.a.h);
}

auto WDC65816::instructionIndirectStackWrite8() -> void {
  sp = fetch();
  idle();
  aa.l = readStack(sp + 0);
  aa.h = readStack(sp + 1);
  idle();
L writeBank(aa.w + r.y.w, r.a.l);
}

auto WDC65816::instructionIndirectStackWrite16() -> void {
  sp = fetch();
  idle();
  aa.l = readStack(sp + 0);
  aa.h = readStack(sp + 1);
  idle();
  writeBank(aa.w + r.y.w + 0, r.a.l);
L writeBank(aa.w + r.y.w + 1, r.a.h);
}
