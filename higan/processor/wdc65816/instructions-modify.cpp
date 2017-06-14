auto WDC65816::instructionImpliedModify8(alu8 op, uint16& data) -> void {
L idleIRQ();
  lo(data) = alu(lo(data));
}

auto WDC65816::instructionImpliedModify16(alu16 op, uint16& data) -> void {
L idleIRQ();
  data = alu(data);
}

auto WDC65816::instructionBankModify8(alu8 op) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  uint8 data = readBank(absolute);
  idle();
  data = alu(data);
L writeBank(absolute, data);
}

auto WDC65816::instructionBankModify16(alu16 op) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  uint16 data = readBank(absolute + 0);
  hi(data) = readBank(absolute + 1);
  idle();
  data = alu(data);
  writeBank(absolute + 1, hi(data));
L writeBank(absolute + 0, lo(data));
}

auto WDC65816::instructionBankIndexedModify8(alu8 op) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  idle();
  uint8 data = readBank(absolute + X);
  idle();
  data = alu(data);
L writeBank(absolute + X, data);
}

auto WDC65816::instructionBankIndexedModify16(alu16 op) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  idle();
  uint16 data = readBank(absolute + X + 0);
  hi(data) = readBank(absolute + X + 1);
  idle();
  data = alu(data);
  writeBank(absolute + X + 1, hi(data));
L writeBank(absolute + X + 0, lo(data));
}

auto WDC65816::instructionDirectModify8(alu8 op) -> void {
  uint8 direct = fetch();
  idle2();
  uint8 data = readDirect(direct);
  idle();
  data = alu(data);
L writeDirect(direct, data);
}

auto WDC65816::instructionDirectModify16(alu16 op) -> void {
  uint8 direct = fetch();
  idle2();
  uint16 data = readDirect(direct + 0);
  hi(data) = readDirect(direct + 1);
  idle();
  data = alu(data);
  writeDirect(direct + 1, hi(data));
L writeDirect(direct + 0, lo(data));
}

auto WDC65816::instructionDirectIndexedModify8(alu8 op) -> void {
  uint8 direct = fetch();
  idle2();
  idle();
  uint8 data = readDirect(direct + X);
  idle();
  data = alu(data);
L writeDirect(direct + X, data);
}

auto WDC65816::instructionDirectIndexedModify16(alu16 op) -> void {
  uint8 direct = fetch();
  idle2();
  idle();
  uint16 data = readDirect(direct + X + 0);
  hi(data) = readDirect(direct + X + 1);
  idle();
  data = alu(data);
  writeDirect(direct + X + 1, hi(data));
L writeDirect(direct + X + 0, lo(data));
}
