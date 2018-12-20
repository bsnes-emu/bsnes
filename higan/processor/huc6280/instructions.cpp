auto HuC6280::instructionAbsoluteModify(fp alu, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
  io();
  auto data = ALU(load16(absolute + index));
L store16(absolute + index, data);
}

auto HuC6280::instructionAbsoluteRead(fp alu, uint8& data, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L data = ALU(load16(absolute + index));
}

auto HuC6280::instructionAbsoluteWrite(uint8 data, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
L store16(absolute + index, data);
}

auto HuC6280::instructionBlockMove(bp alu) -> void {
  uint16 source = operand();
  source |= operand() << 8;
  uint16 target = operand();
  target |= operand() << 8;
  uint16 length = operand();
  length |= operand() << 8;
  push(Y);
  push(A);
  push(X);
  io();
  io();
  io();
  io();
  bool alternate = 0;
  do {
    auto data = load16(source);
    store16(target, data);
    ALU(source, target, alternate);
    alternate ^= 1;
    io();
    io();
    io();
    io();
  } while(--length);
  X = pull();
  A = pull();
L Y = pull();
}

auto HuC6280::instructionBranch(bool take) -> void {
  if(!take) {
  L operand();
  } else {
    auto displacement = operand();
    io();
  L io();
    PC += (int8)displacement;
  }
}

auto HuC6280::instructionBranchIfBitReset(uint3 index) -> void {
  auto zeropage = operand();
  auto displacement = operand();
  io();
  io();
L auto data = load8(zeropage);
  if(data.bit(index) == 0) {
    PC += (int8)displacement;
  }
}

auto HuC6280::instructionBranchIfBitSet(uint3 index) -> void {
  auto zeropage = operand();
  auto displacement = operand();
  io();
  io();
L auto data = load8(zeropage);
  if(data.bit(index) == 1) {
    PC += (int8)displacement;
  }
}

auto HuC6280::instructionBranchSubroutine() -> void {
  auto displacement = operand();
  io();
  io();
  io();
  io();
  push((PC - 1) >> 8);
L push((PC - 1) >> 0);
  PC += (int8)displacement;
}

auto HuC6280::instructionBreak() -> void {
  operand();
  io();
  push(PC >> 8);
  push(PC >> 0);
  uint8 p = P;
  push(p | 0x10);  //B flag set on push
  D = 0;
  I = 1;
  PC.byte(0) = load16(0xfff6);
L PC.byte(1) = load16(0xfff7);
}

auto HuC6280::instructionCallAbsolute() -> void {
  uint16 address = operand();
  address |= operand() << 8;
  io();
  io();
  push((PC - 1) >> 8);
L push((PC - 1) >> 0);
  PC = address;
}

auto HuC6280::instructionChangeSpeedLow() -> void {
L io();
  r.cs = 4;
}

auto HuC6280::instructionChangeSpeedHigh() -> void {
L io();
  r.cs = 1;
}

auto HuC6280::instructionClear(uint8& data) -> void {
L io();
  data = 0;
}

auto HuC6280::instructionClear(bool& flag) -> void {
L io();
  flag = 0;
}

auto HuC6280::instructionImmediate(fp alu, uint8& data) -> void {
L io();
  data = ALU(operand());
}

auto HuC6280::instructionImplied(fp alu, uint8& data) -> void {
L io();
  data = ALU(data);
}

auto HuC6280::instructionIndirectRead(fp alu, uint8& data, uint8 index) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load8(zeropage + index + 0);
  absolute |= load8(zeropage + index + 1) << 8;
  io();
L data = ALU(load16(absolute));
}

auto HuC6280::instructionIndirectWrite(uint8 data, uint8 index) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load8(zeropage + index + 0);
  absolute |= load8(zeropage + index + 1) << 8;
L store16(absolute, data);
}

auto HuC6280::instructionIndirectYRead(fp alu, uint8& data) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load8(zeropage + 0);
  absolute |= load8(zeropage + 1) << 8;
  io();
L data = ALU(load16(absolute + Y));
}

auto HuC6280::instructionIndirectYWrite(uint8 data) -> void {
  auto zeropage = operand();
  io();
  uint16 absolute = load8(zeropage + 0);
  absolute |= load8(zeropage + 1) << 8;
L store16(absolute + Y, data);
}

auto HuC6280::instructionJumpAbsolute() -> void {
  uint16 address = operand();
  address |= operand() << 8;
L io();
  PC = address;
}

auto HuC6280::instructionJumpIndirect(uint8 index) -> void {
  uint16 address = operand();
  address |= operand() << 8;
  io();
  io();
  PC.byte(0) = load16(address + index + 0);
L PC.byte(1) = load16(address + index + 1);
}

auto HuC6280::instructionMemory(fp alu) -> void {
  auto a = A;
  A = ALU(load8(X));
L store8(X, A);
  A = a;
}

auto HuC6280::instructionNoOperation() -> void {
L io();
}

auto HuC6280::instructionPull(uint8& data) -> void {
  io();
  io();
L data = pull();
  Z = data == 0;
  N = data.bit(7);
}

auto HuC6280::instructionPullP() -> void {
  io();
  io();
L P = pull();
}

auto HuC6280::instructionPush(uint8 data) -> void {
  io();
L push(data);
}

auto HuC6280::instructionResetMemoryBit(uint3 index) -> void {
  auto zeropage = operand();
  io();
  io();
  io();
  auto data = load8(zeropage);
  data.bit(index) = 0;
L store8(zeropage, data);
}

auto HuC6280::instructionReturnInterrupt() -> void {
  io();
  io();
  io();
  P = pull();
  PC.byte(0) = pull();
L PC.byte(1) = pull();
}

auto HuC6280::instructionReturnSubroutine() -> void {
  io();
  io();
  io();
  PC.byte(0) = pull();
  PC.byte(1) = pull();
L io();
  PC++;
}

auto HuC6280::instructionSet(bool& flag) -> void {
L io();
  flag = 1;
}

auto HuC6280::instructionSetMemoryBit(uint3 index) -> void {
  auto zeropage = operand();
  io();
  io();
  io();
  auto data = load8(zeropage);
  data.bit(index) = 1;
L store8(zeropage, data);
}

auto HuC6280::instructionStoreImplied(uint2 index) -> void {
  auto data = operand();
  io();
  io();
L store(index, data);
}

auto HuC6280::instructionSwap(uint8& lhs, uint8& rhs) -> void {
  io();
L io();
  swap(lhs, rhs);
}

auto HuC6280::instructionTestAbsolute(uint8 index) -> void {
  auto mask = operand();
  uint16 absolute = operand();
  absolute |= operand() << 8;
  io();
  io();
  io();
L uint8 data = load16(absolute + index);
  Z = (data & mask) == 0;
  V = data.bit(6);
  N = data.bit(7);
}

auto HuC6280::instructionTestZeroPage(uint8 index) -> void {
  auto mask = operand();
  auto zeropage = operand();
  io();
  io();
  io();
L uint8 data = load8(zeropage + index);
  Z = (data & mask) == 0;
  V = data.bit(6);
  N = data.bit(7);
}

auto HuC6280::instructionTransfer(uint8& source, uint8& target) -> void {
L io();
  target = source;
  Z = target == 0;
  N = target.bit(7);
}

auto HuC6280::instructionTransferAccumulatorToMPR() -> void {
  auto mask = operand();
  io();
  io();
L io();
  for(uint index : range(8)) {
    if(mask.bit(index)) r.mpr[index] = A;
  }
}

auto HuC6280::instructionTransferMPRToAccumulator() -> void {
  auto mask = operand();
  io();
L io();
  for(uint index : range(8)) {
    if(mask.bit(index)) { A = r.mpr[index]; break; }
  }
}

auto HuC6280::instructionTransferXS() -> void {
L io();
  S = X;
}

auto HuC6280::instructionZeroPageModify(fp alu, uint8 index) -> void {
  auto zeropage = operand();
  io();
  io();
  auto data = ALU(load8(zeropage + index));
L store8(zeropage + index, data);
}

auto HuC6280::instructionZeroPageRead(fp alu, uint8& data, uint8 index) -> void {
  auto zeropage = operand();
  io();
L data = ALU(load8(zeropage + index));
}

auto HuC6280::instructionZeroPageWrite(uint8 data, uint8 index) -> void {
  auto zeropage = operand();
  io();
L store8(zeropage + index, data);
}
