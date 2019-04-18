auto WDC65816::instructionImmediateRead8(alu8 op) -> void {
L uint8 data = fetch();
  alu(data);
}

auto WDC65816::instructionImmediateRead16(alu16 op) -> void {
  uint16 data = fetch();
L hi(data) = fetch();
  alu(data);
}

auto WDC65816::instructionBankRead8(alu8 op) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
L uint8 data = readBank(absolute);
  alu(data);
}

auto WDC65816::instructionBankRead16(alu16 op) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  uint16 data = readBank(absolute + 0);
L hi(data) = readBank(absolute + 1);
  alu(data);
}

auto WDC65816::instructionBankRead8(alu8 op, uint16 index) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  idle4(absolute, absolute + index);
L uint8 data = readBank(absolute + index);
  alu(data);
}

auto WDC65816::instructionBankRead16(alu16 op, uint16 index) -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  idle4(absolute, absolute + index);
  uint16 data = readBank(absolute + index + 0);
L hi(data) = readBank(absolute + index + 1);
  alu(data);
}

auto WDC65816::instructionLongRead8(alu8 op, uint16 index) -> void {
  uint24 address = fetch();
  hi(address) = fetch();
  db(address) = fetch();
L uint8 data = read(address + index);
  alu(data);
}

auto WDC65816::instructionLongRead16(alu16 op, uint16 index) -> void {
  uint24 address = fetch();
  hi(address) = fetch();
  db(address) = fetch();
  uint16 data = read(address + index + 0);
L hi(data) = read(address + index + 1);
  alu(data);
}

auto WDC65816::instructionDirectRead8(alu8 op) -> void {
  uint8 direct = fetch();
  idle2();
L uint8 data = readDirect(direct);
  alu(data);
}

auto WDC65816::instructionDirectRead16(alu16 op) -> void {
  uint8 direct = fetch();
  idle2();
  uint16 data = readDirect(direct + 0);
L hi(data) = readDirect(direct + 1);
  alu(data);
}

auto WDC65816::instructionDirectRead8(alu8 op, uint16 index) -> void {
  uint8 direct = fetch();
  idle2();
  idle();
L uint8 data = readDirect(direct + index);
  alu(data);
}

auto WDC65816::instructionDirectRead16(alu16 op, uint16 index) -> void {
  uint8 direct = fetch();
  idle2();
  idle();
  uint16 data = readDirect(direct + index + 0);
L hi(data) = readDirect(direct + index + 1);
  alu(data);
}

auto WDC65816::instructionIndirectRead8(alu8 op) -> void {
  uint8 direct = fetch();
  idle2();
  uint16 absolute = readDirect(direct + 0);
  hi(absolute) = readDirect(direct + 1);
L uint8 data = readBank(absolute);
  alu(data);
}

auto WDC65816::instructionIndirectRead16(alu16 op) -> void {
  uint8 direct = fetch();
  idle2();
  uint16 absolute = readDirect(direct + 0);
  hi(absolute) = readDirect(direct + 1);
  uint16 data = readBank(absolute + 0);
L hi(data) = readBank(absolute + 1);
  alu(data);
}

auto WDC65816::instructionIndexedIndirectRead8(alu8 op) -> void {
  uint8 direct = fetch();
  idle2();
  idle();
  uint16 absolute = readDirect(direct + X + 0);
  hi(absolute) = readDirect(direct + X + 1);
L uint8 data = readBank(absolute);
  alu(data);
}

auto WDC65816::instructionIndexedIndirectRead16(alu16 op) -> void {
  uint8 direct = fetch();
  idle2();
  idle();
  uint16 absolute = readDirect(direct + X + 0);
  hi(absolute) = readDirect(direct + X + 1);
  uint16 data = readBank(absolute + 0);
L hi(data) = readBank(absolute + 1);
  alu(data);
}

auto WDC65816::instructionIndirectIndexedRead8(alu8 op) -> void {
  uint8 direct = fetch();
  idle2();
  uint16 absolute = readDirect(direct + 0);
  hi(absolute) = readDirect(direct + 1);
  idle4(absolute, absolute + Y);
L uint8 data = readBank(absolute + Y);
  alu(data);
}

auto WDC65816::instructionIndirectIndexedRead16(alu16 op) -> void {
  uint8 direct = fetch();
  idle2();
  uint16 absolute = readDirect(direct + 0);
  hi(absolute) = readDirect(direct + 1);
  idle4(absolute, absolute + Y);
  uint16 data = readBank(absolute + Y + 0);
L hi(data) = readBank(absolute + Y + 1);
  alu(data);
}

auto WDC65816::instructionIndirectLongRead8(alu8 op, uint16 index) -> void {
  uint8 direct = fetch();
  idle2();
  uint24 address = readDirectN(direct + 0);
  hi(address) = readDirectN(direct + 1);
  db(address) = readDirectN(direct + 2);
L uint8 data = read(address + index);
  alu(data);
}

auto WDC65816::instructionIndirectLongRead16(alu16 op, uint16 index) -> void {
  uint8 direct = fetch();
  idle2();
  uint24 address = readDirectN(direct + 0);
  hi(address) = readDirectN(direct + 1);
  db(address) = readDirectN(direct + 2);
  uint16 data = read(address + index + 0);
L hi(data) = read(address + index + 1);
  alu(data);
}

auto WDC65816::instructionStackRead8(alu8 op) -> void {
  uint8 stack = fetch();
  idle();
L uint8 data = readStack(stack);
  alu(data);
}

auto WDC65816::instructionStackRead16(alu16 op) -> void {
  uint8 stack = fetch();
  idle();
  uint16 data = readStack(stack + 0);
L hi(data) = readStack(stack + 1);
  alu(data);
}

auto WDC65816::instructionIndirectStackRead8(alu8 op) -> void {
  uint8 stack = fetch();
  idle();
  uint16 absolute = readStack(stack + 0);
  hi(absolute) = readStack(stack + 1);
  idle();
L uint8 data = readBank(absolute + Y);
  alu(data);
}

auto WDC65816::instructionIndirectStackRead16(alu16 op) -> void {
  uint8 stack = fetch();
  idle();
  uint16 absolute = readStack(stack + 0);
  hi(absolute) = readStack(stack + 1);
  idle();
  uint16 data = readBank(absolute + Y + 0);
L hi(data) = readBank(absolute + Y + 1);
  alu(data);
}
