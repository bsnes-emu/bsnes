auto WDC65816::instructionImmediateRead8(fp op) -> void {
L rd.l = fetch();
  call(op);
}

auto WDC65816::instructionImmediateRead16(fp op) -> void {
  rd.l = fetch();
L rd.h = fetch();
  call(op);
}

auto WDC65816::instructionBankRead8(fp op) -> void {
  aa.l = fetch();
  aa.h = fetch();
L rd.l = readBank(aa.w);
  call(op);
}

auto WDC65816::instructionBankRead16(fp op) -> void {
  aa.l = fetch();
  aa.h = fetch();
  rd.l = readBank(aa.w + 0);
L rd.h = readBank(aa.w + 1);
  call(op);
}

auto WDC65816::instructionBankRead8(fp op, uint16 index) -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle4(aa.w, aa.w + index);
L rd.l = readBank(aa.w + index);
  call(op);
}

auto WDC65816::instructionBankRead16(fp op, uint16 index) -> void {
  aa.l = fetch();
  aa.h = fetch();
  idle4(aa.w, aa.w + index);
  rd.l = readBank(aa.w + index + 0);
L rd.h = readBank(aa.w + index + 1);
  call(op);
}

auto WDC65816::instructionLongRead8(fp op, uint16 index) -> void {
  aa.l = fetch();
  aa.h = fetch();
  aa.b = fetch();
L rd.l = read(aa.d + index);
  call(op);
}

auto WDC65816::instructionLongRead16(fp op, uint16 index) -> void {
  aa.l = fetch();
  aa.h = fetch();
  aa.b = fetch();
  rd.l = read(aa.d + index + 0);
L rd.h = read(aa.d + index + 1);
  call(op);
}

auto WDC65816::instructionDirectRead8(fp op) -> void {
  dp = fetch();
  idle2();
L rd.l = readDirect(dp);
  call(op);
}

auto WDC65816::instructionDirectRead16(fp op) -> void {
  dp = fetch();
  idle2();
  rd.l = readDirect(dp + 0);
L rd.h = readDirect(dp + 1);
  call(op);
}

auto WDC65816::instructionDirectRead8(fp op, uint16 index) -> void {
  dp = fetch();
  idle2();
  idle();
L rd.l = readDirect(dp + index);
  call(op);
}

auto WDC65816::instructionDirectRead16(fp op, uint16 index) -> void {
  dp = fetch();
  idle2();
  idle();
  rd.l = readDirect(dp + index + 0);
L rd.h = readDirect(dp + index + 1);
  call(op);
}

auto WDC65816::instructionIndirectRead8(fp op) -> void {
  dp = fetch();
  idle2();
  aa.l = readDirect(dp + 0);
  aa.h = readDirect(dp + 1);
L rd.l = readBank(aa.w);
  call(op);
}

auto WDC65816::instructionIndirectRead16(fp op) -> void {
  dp = fetch();
  idle2();
  aa.l = readDirect(dp + 0);
  aa.h = readDirect(dp + 1);
  rd.l = readBank(aa.w + 0);
L rd.h = readBank(aa.w + 1);
  call(op);
}

auto WDC65816::instructionIndexedIndirectRead8(fp op) -> void {
  dp = fetch();
  idle2();
  idle();
  aa.l = readDirect(dp + r.x.w + 0);
  aa.h = readDirect(dp + r.x.w + 1);
L rd.l = readBank(aa.w);
  call(op);
}

auto WDC65816::instructionIndexedIndirectRead16(fp op) -> void {
  dp = fetch();
  idle2();
  idle();
  aa.l = readDirect(dp + r.x.w + 0);
  aa.h = readDirect(dp + r.x.w + 1);
  rd.l = readBank(aa.w + 0);
L rd.h = readBank(aa.w + 1);
  call(op);
}

auto WDC65816::instructionIndirectIndexedRead8(fp op) -> void {
  dp = fetch();
  idle2();
  aa.l = readDirect(dp + 0);
  aa.h = readDirect(dp + 1);
  idle4(aa.w, aa.w + r.y.w);
L rd.l = readBank(aa.w + r.y.w);
  call(op);
}

auto WDC65816::instructionIndirectIndexedRead16(fp op) -> void {
  dp = fetch();
  idle2();
  aa.l = readDirect(dp + 0);
  aa.h = readDirect(dp + 1);
  idle4(aa.w, aa.w + r.y.w);
  rd.l = readBank(aa.w + r.y.w + 0);
L rd.h = readBank(aa.w + r.y.w + 1);
  call(op);
}

auto WDC65816::instructionIndirectLongRead8(fp op, uint16 index) -> void {
  dp = fetch();
  idle2();
  aa.l = readDirectN(dp + 0);
  aa.h = readDirectN(dp + 1);
  aa.b = readDirectN(dp + 2);
L rd.l = read(aa.d + index);
  call(op);
}

auto WDC65816::instructionIndirectLongRead16(fp op, uint16 index) -> void {
  dp = fetch();
  idle2();
  aa.l = readDirectN(dp + 0);
  aa.h = readDirectN(dp + 1);
  aa.b = readDirectN(dp + 2);
  rd.l = read(aa.d + index + 0);
L rd.h = read(aa.d + index + 1);
  call(op);
}

auto WDC65816::instructionStackRead8(fp op) -> void {
  sp = fetch();
  idle();
L rd.l = readStack(sp);
  call(op);
}

auto WDC65816::instructionStackRead16(fp op) -> void {
  sp = fetch();
  idle();
  rd.l = readStack(sp + 0);
L rd.h = readStack(sp + 1);
  call(op);
}

auto WDC65816::instructionIndirectStackRead8(fp op) -> void {
  sp = fetch();
  idle();
  aa.l = readStack(sp + 0);
  aa.h = readStack(sp + 1);
  idle();
L rd.l = readBank(aa.w + r.y.w);
  call(op);
}

auto WDC65816::instructionIndirectStackRead16(fp op) -> void {
  sp = fetch();
  idle();
  aa.l = readStack(sp + 0);
  aa.h = readStack(sp + 1);
  idle();
  rd.l = readBank(aa.w + r.y.w + 0);
L rd.h = readBank(aa.w + r.y.w + 1);
  call(op);
}
