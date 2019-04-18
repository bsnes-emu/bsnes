auto MOS6502::instructionAbsoluteModify(fp alu) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  auto data = read(absolute);
  write(absolute, data);
L write(absolute, ALU(data));
}

auto MOS6502::instructionAbsoluteModify(fp alu, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  idlePageAlways(absolute, absolute + index);
  auto data = read(absolute + index);
  write(absolute + index, data);
L write(absolute + index, ALU(data));
}

auto MOS6502::instructionAbsoluteRead(fp alu, uint8& data) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
L data = ALU(read(absolute));
}

auto MOS6502::instructionAbsoluteRead(fp alu, uint8& data, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  idlePageCrossed(absolute, absolute + index);
L data = ALU(read(absolute + index));
}

auto MOS6502::instructionAbsoluteWrite(uint8& data) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
L write(absolute, data);
}

auto MOS6502::instructionAbsoluteWrite(uint8& data, uint8 index) -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  idlePageAlways(absolute, absolute + index);
L write(absolute + index, data);
}

auto MOS6502::instructionBranch(bool take) -> void {
  if(!take) {
  L operand();
  } else {
    int8 displacement = operand();
    idlePageCrossed(PC, PC + displacement);
  L idle();
    PC = PC + displacement;
  }
}

auto MOS6502::instructionBreak() -> void {
  operand();
  push(PCH);
  push(PCL);
  uint16 vector = 0xfffe;
  nmi(vector);
  push(P | 0x30);
  I = 1;
  PCL = read(vector++);
L PCH = read(vector++);
}

auto MOS6502::instructionCallAbsolute() -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  idle();
  PC--;
  push(PCH);
L push(PCL);
  PC = absolute;
}

auto MOS6502::instructionClear(bool& flag) -> void {
L idle();
  flag = 0;
}

auto MOS6502::instructionImmediate(fp alu, uint8& data) -> void {
L data = ALU(operand());
}

auto MOS6502::instructionImplied(fp alu, uint8& data) -> void {
L idle();
  data = ALU(data);
}

auto MOS6502::instructionIndirectXRead(fp alu, uint8& data) -> void {
  auto zeroPage = operand();
  load(zeroPage);
  uint16 absolute = load(zeroPage + X + 0);
  absolute |= load(zeroPage + X + 1) << 8;
L data = ALU(read(absolute));
}

auto MOS6502::instructionIndirectXWrite(uint8& data) -> void {
  auto zeroPage = operand();
  load(zeroPage);
  uint16 absolute = load(zeroPage + X + 0);
  absolute |= load(zeroPage + X + 1) << 8;
L write(absolute, data);
}

auto MOS6502::instructionIndirectYRead(fp alu, uint8& data) -> void {
  auto zeroPage = operand();
  uint16 absolute = load(zeroPage + 0);
  absolute |= load(zeroPage + 1) << 8;
  idlePageCrossed(absolute, absolute + Y);
L data = ALU(read(absolute + Y));
}

auto MOS6502::instructionIndirectYWrite(uint8& data) -> void {
  auto zeroPage = operand();
  uint16 absolute = load(zeroPage + 0);
  absolute |= load(zeroPage + 1) << 8;
  idlePageAlways(absolute, absolute + Y);
L write(absolute + Y, data);
}

auto MOS6502::instructionJumpAbsolute() -> void {
  uint16 absolute = operand();
L absolute |= operand() << 8;
  PC = absolute;
}

auto MOS6502::instructionJumpIndirect() -> void {
  uint16 absolute = operand();
  absolute |= operand() << 8;
  uint16 pc = read(absolute);
  absolute.byte(0)++;  //MOS6502: $00ff wraps here to $0000; not $0100
L pc |= read(absolute) << 8;
  PC = pc;
}

auto MOS6502::instructionNoOperation() -> void {
L idle();
}

auto MOS6502::instructionPull(uint8& data) -> void {
  idle();
  idle();
L data = pull();
  Z = data == 0;
  N = data.bit(7);
}

auto MOS6502::instructionPullP() -> void {
  idle();
  idle();
L P = pull();
}

auto MOS6502::instructionPush(uint8& data) -> void {
  idle();
L push(data);
}

auto MOS6502::instructionPushP() -> void {
  idle();
L push(P | 0x30);
}

auto MOS6502::instructionReturnInterrupt() -> void {
  idle();
  idle();
  P = pull();
  PCL = pull();
L PCH = pull();
}

auto MOS6502::instructionReturnSubroutine() -> void {
  idle();
  idle();
  PCL = pull();
  PCH = pull();
L idle();
  PC++;
}

auto MOS6502::instructionSet(bool& flag) -> void {
L idle();
  flag = 1;
}

auto MOS6502::instructionTransfer(uint8& source, uint8& target, bool flag) -> void {
L idle();
  target = source;
  if(!flag) return;
  Z = target == 0;
  N = target.bit(7);
}

auto MOS6502::instructionZeroPageModify(fp alu) -> void {
  auto zeroPage = operand();
  auto data = load(zeroPage);
  store(zeroPage, data);
L store(zeroPage, ALU(data));
}

auto MOS6502::instructionZeroPageModify(fp alu, uint8 index) -> void {
  auto zeroPage = operand();
  load(zeroPage);
  auto data = load(zeroPage + index);
  store(zeroPage + index, data);
L store(zeroPage + index, ALU(data));
}

auto MOS6502::instructionZeroPageRead(fp alu, uint8& data) -> void {
  auto zeroPage = operand();
L data = ALU(load(zeroPage));
}

auto MOS6502::instructionZeroPageRead(fp alu, uint8& data, uint8 index) -> void {
  auto zeroPage = operand();
  load(zeroPage);
L data = ALU(load(zeroPage + index));
}

auto MOS6502::instructionZeroPageWrite(uint8& data) -> void {
  auto zeroPage = operand();
L store(zeroPage, data);
}

auto MOS6502::instructionZeroPageWrite(uint8& data, uint8 index) -> void {
  auto zeroPage = operand();
  read(zeroPage);
L store(zeroPage + index, data);
}
