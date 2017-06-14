auto WDC65816::instructionBankWrite8(uint16& data) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
L writeBank(absolute, data);
}

auto WDC65816::instructionBankWrite16(uint16& data) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  writeBank(absolute + 0, lo(data));
L writeBank(absolute + 1, hi(data));
}

auto WDC65816::instructionBankWrite8(uint16& data, uint16 index) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  idle();
L writeBank(absolute + index, data);
}

auto WDC65816::instructionBankWrite16(uint16& data, uint16 index) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  idle();
  writeBank(absolute + index + 0, lo(data));
L writeBank(absolute + index + 1, hi(data));
}

auto WDC65816::instructionLongWrite8(uint16 index) -> void {
  uint24 address = fetch();
  hi(address) = fetch();
  db(address) = fetch();
L write(address + index, lo(A));
}

auto WDC65816::instructionLongWrite16(uint16 index) -> void {
  uint24 address = fetch();
  hi(address) = fetch();
  db(address) = fetch();
  write(address + index + 0, lo(A));
L write(address + index + 1, hi(A));
}

auto WDC65816::instructionDirectWrite8(uint16& data) -> void {
  uint8 direct = fetch();
  idle2();
L writeDirect(direct, data);
}

auto WDC65816::instructionDirectWrite16(uint16& data) -> void {
  uint8 direct = fetch();
  idle2();
  writeDirect(direct + 0, lo(data));
L writeDirect(direct + 1, hi(data));
}

auto WDC65816::instructionDirectWrite8(uint16& data, uint16 index) -> void {
  uint8 direct = fetch();
  idle2();
  idle();
L writeDirect(direct + index, lo(data));
}

auto WDC65816::instructionDirectWrite16(uint16& data, uint16 index) -> void {
  uint8 direct = fetch();
  idle2();
  idle();
  writeDirect(direct + index + 0, lo(data));
L writeDirect(direct + index + 1, hi(data));
}

auto WDC65816::instructionIndirectWrite8() -> void {
  uint8 direct = fetch();
  idle2();
  uint16 absolute = readDirect(direct + 0);
  hi(absolute) = readDirect(direct + 1);
L writeBank(absolute, lo(A));
}

auto WDC65816::instructionIndirectWrite16() -> void {
  uint8 direct = fetch();
  idle2();
  uint16 absolute = readDirect(direct + 0);
  hi(absolute) = readDirect(direct + 1);
  writeBank(absolute + 0, lo(A));
L writeBank(absolute + 1, hi(A));
}

auto WDC65816::instructionIndexedIndirectWrite8() -> void {
  uint8 direct = fetch();
  idle2();
  idle();
  uint16 absolute = readDirect(direct + X + 0);
  hi(absolute) = readDirect(direct + X + 1);
L writeBank(absolute, lo(A));
}

auto WDC65816::instructionIndexedIndirectWrite16() -> void {
  uint8 direct = fetch();
  idle2();
  idle();
  uint16 absolute = readDirect(direct + X + 0);
  hi(absolute) = readDirect(direct + X + 1);
  writeBank(absolute + 0, lo(A));
L writeBank(absolute + 1, hi(A));
}

auto WDC65816::instructionIndirectIndexedWrite8() -> void {
  uint8 direct = fetch();
  idle2();
  uint16 absolute = readDirect(direct + 0);
  hi(absolute) = readDirect(direct + 1);
  idle();
L writeBank(absolute + Y, lo(A));
}

auto WDC65816::instructionIndirectIndexedWrite16() -> void {
  uint8 direct = fetch();
  idle2();
  uint16 absolute = readDirect(direct + 0);
  hi(absolute) = readDirect(direct + 1);
  idle();
  writeBank(absolute + Y + 0, lo(A));
L writeBank(absolute + Y + 1, hi(A));
}

auto WDC65816::instructionIndirectLongWrite8(uint16 index) -> void {
  uint8 direct = fetch();
  idle2();
  uint24 address = readDirectN(direct + 0);
  hi(address) = readDirectN(direct + 1);
  db(address) = readDirectN(direct + 2);
L write(address + index, lo(A));
}

auto WDC65816::instructionIndirectLongWrite16(uint16 index) -> void {
  uint8 direct = fetch();
  idle2();
  uint24 address = readDirectN(direct + 0);
  hi(address) = readDirectN(direct + 1);
  db(address) = readDirectN(direct + 2);
  write(address + index + 0, lo(A));
L write(address + index + 1, hi(A));
}

auto WDC65816::instructionStackWrite8() -> void {
  uint8 stack = fetch();
  idle();
L writeStack(stack, lo(A));
}

auto WDC65816::instructionStackWrite16() -> void {
  uint8 stack = fetch();
  idle();
  writeStack(stack + 0, lo(A));
L writeStack(stack + 1, hi(A));
}

auto WDC65816::instructionIndirectStackWrite8() -> void {
  uint8 stack = fetch();
  idle();
  uint16 absolute = readStack(stack + 0);
  hi(absolute) = readStack(stack + 1);
  idle();
L writeBank(absolute + Y, lo(A));
}

auto WDC65816::instructionIndirectStackWrite16() -> void {
  uint8 stack = fetch();
  idle();
  uint16 absolute = readStack(stack + 0);
  hi(absolute) = readStack(stack + 1);
  idle();
  writeBank(absolute + Y + 0, lo(A));
L writeBank(absolute + Y + 1, hi(A));
}
