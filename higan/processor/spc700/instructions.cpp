auto SPC700::instructionAbsoluteBitModify(uint3 mode) -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  uint3 bit = address >> 13;
  address &= 0x1fff;
  uint8 data = read(address);
  switch(mode) {
  case 0:  //or addr:bit
    idle(address);
    CF |= data.bit(bit);
    break;
  case 1:  //or !addr:bit
    idle(address);
    CF |= !data.bit(bit);
    break;
  case 2:  //and addr:bit
    CF &= data.bit(bit);
    break;
  case 3:  //and !addr:bit
    CF &= !data.bit(bit);
    break;
  case 4:  //eor addr:bit
    idle(address);
    CF ^= data.bit(bit);
    break;
  case 5:  //ld addr:bit
    CF = data.bit(bit);
    break;
  case 6:  //st addr:bit
    idle(address);
    data.bit(bit) = CF;
    write(address, data);
    break;
  case 7:  //not addr:bit
    data.bit(bit) ^= 1;
    write(address, data);
    break;
  }
}

auto SPC700::instructionAbsoluteBitSet(uint3 bit, bool value) -> void {
  uint8 address = read(PC++);
  uint8 data = read(page(address));
  data.bit(bit) = value;
  write(page(address), data);
}

auto SPC700::instructionAbsoluteRead(fpb op, uint8& target) -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  uint8 data = read(address);
  target = alu(target, data);
}

auto SPC700::instructionAbsoluteModify(fps op) -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  uint8 data = read(address);
  write(address, alu(data));
}

auto SPC700::instructionAbsoluteWrite(uint8& data) -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  read(address);
  write(address, data);
}

auto SPC700::instructionAbsoluteIndexedRead(fpb op, uint8& index) -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  idle(PC - 1);
  uint8 data = read(address + index);
  A = alu(A, data);
}

auto SPC700::instructionAbsoluteIndexedWrite(uint8& index) -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  idle(PC - 1);
  read(address + index);
  write(address + index, A);
}

auto SPC700::instructionBranch(bool take) -> void {
  uint8 data = read(PC++);
  if(!take) return;
  idle(PC - 1);
  idle(PC - 1);
  PC += (int8)data;
}

auto SPC700::instructionBranchBit(uint3 bit, bool match) -> void {
  uint8 address = read(PC++);
  uint8 data = read(page(address));
  idle(page(address));
  uint8 displacement = read(PC++);
  if(data.bit(bit) != match) return;
  idle(PC - 1);
  idle(PC - 1);
  PC += (int8)displacement;
}

auto SPC700::instructionBranchNotDirect() -> void {
  uint8 address = read(PC++);
  uint8 data = read(page(address));
  idle(page(address));
  uint8 displacement = read(PC++);
  if(A == data) return;
  idle(PC - 1);
  idle(PC - 1);
  PC += (int8)displacement;
}

auto SPC700::instructionBranchNotDirectDecrement() -> void {
  uint8 address = read(PC++);
  uint8 data = read(page(address));
  write(page(address), --data);
  uint8 displacement = read(PC++);
  if(data == 0) return;
  idle(PC - 1);
  idle(PC - 1);
  PC += (int8)displacement;
}

auto SPC700::instructionBranchNotDirectX() -> void {
  uint8 address = read(PC++);
  idle(PC - 1);
  uint8 data = read(page(address + X));
  idle(page(address + X));
  uint8 displacement = read(PC++);
  if(A == data) return;
  idle(PC - 1);
  idle(PC - 1);
  PC += (int8)displacement;
}

auto SPC700::instructionBranchNotYDecrement() -> void {
  idle(PC);
  idle(PC);
  uint8 displacement = read(PC++);
  if(--Y == 0) return;
  idle(PC - 1);
  idle(PC - 1);
  PC += (int8)displacement;
}

auto SPC700::instructionBreak() -> void {
  idle(PC);
  write(stack(S--), PC >> 8);
  write(stack(S--), PC >> 0);
  write(stack(S--), P);
  idle(stack(S + 1));
  uint16 address = read(0xffde + 0);
  address |= read(0xffde + 1) << 8;
  PC = address;
  IF = 0;
  BF = 1;
}

auto SPC700::instructionCallAbsolute() -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  idle(stack(S));
  write(stack(S--), PC >> 8);
  write(stack(S--), PC >> 0);
  idle(stack(S + 1));
  idle(stack(S + 1));
  PC = address;
}

auto SPC700::instructionCallPage() -> void {
  uint8 address = read(PC++);
  idle(PC - 1);
  write(stack(S--), PC >> 8);
  write(stack(S--), PC >> 0);
  idle(stack(S + 1));
  PC = 0xff00 | address;
}

auto SPC700::instructionCallTable(uint4 vector) -> void {
  idle(PC);
  idle(stack(S));
  write(stack(S--), PC >> 8);
  write(stack(S--), PC >> 0);
  idle(stack(S + 1));
  uint16 address = 0xffde - (vector << 1);
  uint16 pc = read(address + 0);
  pc |= read(address + 1) << 8;
  PC = pc;
}

auto SPC700::instructionComplementCarry() -> void {
  idle(PC);
  idle(PC);
  CF = !CF;
}

auto SPC700::instructionDecimalAdjustAdd() -> void {
  idle(PC);
  idle(PC);
  if(CF || A > 0x99) {
    A += 0x60;
    CF = 1;
  }
  if(HF || (A & 15) > 0x09) {
    A += 0x06;
  }
  ZF = A == 0;
  NF = A & 0x80;
}

auto SPC700::instructionDecimalAdjustSub() -> void {
  idle(PC);
  idle(PC);
  if(!CF || A > 0x99) {
    A -= 0x60;
    CF = 0;
  }
  if(!HF || (A & 15) > 0x09) {
    A -= 0x06;
  }
  ZF = A == 0;
  NF = A & 0x80;
}

auto SPC700::instructionDirectRead(fpb op, uint8& target) -> void {
  uint8 address = read(PC++);
  uint8 data = read(page(address));
  target = alu(target, data);
}

auto SPC700::instructionDirectModify(fps op) -> void {
  uint8 address = read(PC++);
  uint8 data = read(page(address));
  write(page(address), alu(data));
}

auto SPC700::instructionDirectWriteWord() -> void {
  uint8 address = read(PC++);
  idle(page(address + 0));
  write(page(address + 0), A);
  write(page(address + 1), Y);
}

auto SPC700::instructionDirectWriteDirect(fpb op) -> void {
  uint8 source = read(PC++);
  uint8 rhs = read(page(source));
  uint8 target = read(PC++);
  uint8 lhs;
  if(op != &SPC700::algorithmST) lhs = read(page(target));
  lhs = alu(lhs, rhs);
  op != &SPC700::algorithmCMP ? write(page(target), lhs) : idle(page(target));
}

auto SPC700::instructionDirectWriteImmediate(fpb op) -> void {
  uint8 immediate = read(PC++);
  uint8 address = read(PC++);
  uint8 data = read(page(address));
  data = alu(data, immediate);
  op != &SPC700::algorithmCMP ? write(page(address), data) : idle(page(address));
}

auto SPC700::instructionDirectReadWord(fpw op) -> void {
  uint8 address = read(PC++);
  uint16 data = read(page(address + 0));
  if(op != &SPC700::algorithmCPW) idle(page(address + 0));
  data |= read(page(address + 1)) << 8;
  YA = alu(YA, data);
}

auto SPC700::instructionDirectModifyWord(int adjust) -> void {
  uint8 address = read(PC++);
  uint16 data = read(page(address + 0)) + adjust;
  write(page(address + 0), data >> 0);
  data += read(page(address + 1)) << 8;
  write(page(address + 1), data >> 8);
  ZF = data == 0;
  NF = data & 0x8000;
}

auto SPC700::instructionDirectWrite(uint8& data) -> void {
  uint8 address = read(PC++);
  idle(page(address));
  write(page(address), data);
}

auto SPC700::instructionDirectIndexedRead(fpb op, uint8& target, uint8& index) -> void {
  uint8 address = read(PC++);
  idle(PC - 1);
  uint8 data = read(page(address + index));
  target = alu(target, data);
}

auto SPC700::instructionDirectIndexedModify(fps op, uint8& index) -> void {
  uint8 address = read(PC++);
  idle(PC - 1);
  uint8 data = read(page(address + index));
  write(page(address + index), alu(data));
}

auto SPC700::instructionDirectIndexedWrite(uint8& data, uint8& index) -> void {
  uint8 address = read(PC++);
  idle(PC - 1);
  idle(page(address + index));
  write(page(address + index), data);
}

auto SPC700::instructionDivide() -> void {
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  uint16 ya = YA;
  //overflow set if quotient >= 256
  HF = (Y & 15) >= (X & 15);
  VF = Y >= X;
  if(Y < (X << 1)) {
    //if quotient is <= 511 (will fit into 9-bit result)
    A = ya / X;
    Y = ya % X;
  } else {
    //otherwise, the quotient won't fit into VF + A
    //this emulates the odd behavior of the S-SMP in this case
    A = 255 - (ya - (X << 9)) / (256 - X);
    Y = X   + (ya - (X << 9)) % (256 - X);
  }
  //result is set based on a (quotient) only
  ZF = A == 0;
  NF = A & 0x80;
}

auto SPC700::instructionExchangeNibble() -> void {
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  A = A >> 4 | A << 4;
  ZF = A == 0;
  NF = A & 0x80;
}

auto SPC700::instructionFlagSet(bool& flag, bool value) -> void {
  idle(PC);
  if(&flag == &IF) idle(PC);
  flag = value;
}

auto SPC700::instructionImmediateRead(fpb op, uint8& target) -> void {
  uint8 data = read(PC++);
  target = alu(target, data);
}

auto SPC700::instructionImpliedModify(fps op, uint8& target) -> void {
  idle(PC);
  target = alu(target);
}

auto SPC700::instructionIndexedIndirectRead(fpb op, uint8& index) -> void {
  uint8 indirect = read(PC++);
  idle(PC - 1);
  uint16 address = read(page(indirect + index + 0));
  address |= read(page(indirect + index + 1)) << 8;
  uint8 data = read(address);
  A = alu(A, data);
}

auto SPC700::instructionIndexedIndirectWrite(uint8& data, uint8& index) -> void {
  uint8 indirect = read(PC++);
  idle(PC - 1);
  uint16 address = read(page(indirect + index + 0));
  address |= read(page(indirect + index + 1)) << 8;
  read(address);
  write(address, data);
}

auto SPC700::instructionIndirectIndexedRead(fpb op, uint8& index) -> void {
  uint8 indirect = read(PC++);
  uint16 address = read(page(indirect + 0));
  address |= read(page(indirect + 1)) << 8;
  idle(page(indirect + 1));
  uint8 data = read(address + index);
  A = alu(A, data);
}

auto SPC700::instructionIndirectIndexedWrite(uint8& data, uint8& index) -> void {
  uint8 indirect = read(PC++);
  uint16 address = read(page(indirect + 0));
  address |= read(page(indirect + 1)) << 8;
  idle(page(indirect + 1));
  read(address + index);
  write(address + index, data);
}

auto SPC700::instructionIndirectXRead(fpb op) -> void {
  idle(PC);
  uint8 data = read(page(X));
  A = alu(A, data);
}

auto SPC700::instructionIndirectXWrite(uint8& data) -> void {
  idle(PC);
  idle(page(X));
  write(page(X), data);
}

auto SPC700::instructionIndirectXIncrementRead(uint8& data) -> void {
  idle(PC);
  data = read(page(X));  //todo: $f0-ff not accessible on this cycle?
  idle(page(X++));
  ZF = data == 0;
  NF = data & 0x80;
}

auto SPC700::instructionIndirectXIncrementWrite(uint8& data) -> void {
  idle(PC);
  idle(page(X));  //todo: $f0-ff not accessible on this cycle?
  write(page(X++), data);
}

auto SPC700::instructionIndirectXWriteIndirectY(fpb op) -> void {
  idle(PC);
  uint8 rhs = read(page(Y));
  uint8 lhs = read(page(X));
  lhs = alu(lhs, rhs);
  op != &SPC700::algorithmCMP ? write(page(X), lhs) : idle(page(X));
}

auto SPC700::instructionJumpAbsolute() -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  PC = address;
}

auto SPC700::instructionJumpIndirectX() -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  idle(PC - 1);
  uint16 pc = read(address + X + 0);
  pc |= read(address + X + 1) << 8;
  PC = pc;
}

auto SPC700::instructionMultiply() -> void {
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  idle(PC);
  uint16 ya = Y * A;
  A = ya >> 0;
  Y = ya >> 8;
  //result is set based on y (high-byte) only
  ZF = Y == 0;
  NF = Y & 0x80;
}

auto SPC700::instructionNoOperation() -> void {
  idle(PC);
}

auto SPC700::instructionOverflowClear() -> void {
  idle(PC);
  HF = 0;
  VF = 0;
}

auto SPC700::instructionPull(uint8& data) -> void {
  idle(PC);
  idle(stack(S));
  data = read(stack(++S));
}

auto SPC700::instructionPullP() -> void {
  idle(PC);
  idle(stack(S));
  P = read(stack(++S));
}

auto SPC700::instructionPush(uint8 data) -> void {
  idle(PC);
  write(stack(S--), data);
  idle(stack(S + 1));
}

auto SPC700::instructionReturnInterrupt() -> void {
  idle(PC);
  idle(stack(S));
  P = read(stack(++S));
  uint16 address = read(stack(++S));
  address |= read(stack(++S)) << 8;
  PC = address;
}

auto SPC700::instructionReturnSubroutine() -> void {
  idle(PC);
  idle(stack(S));
  uint16 address = read(stack(++S));
  address |= read(stack(++S)) << 8;
  PC = address;
}

auto SPC700::instructionStop() -> void {
  r.stop = true;
  while(r.stop && !synchronizing()) {
    idle(PC);
    idle(PC);
  }
}

auto SPC700::instructionTestSetBitsAbsolute(bool set) -> void {
  uint16 address = read(PC++);
  address |= read(PC++) << 8;
  uint8 data = read(address);
  ZF = (A - data) == 0;
  NF = (A - data) & 0x80;
  idle(address);
  write(address, set ? data | A : data & ~A);
}

auto SPC700::instructionTransfer(uint8& from, uint8& to) -> void {
  idle(PC);
  to = from;
  if(&to == &S) return;
  ZF = to == 0;
  NF = to & 0x80;
}

auto SPC700::instructionWait() -> void {
  r.wait = true;
  while(r.wait && !synchronizing()) {
    idle(PC);
    idle(PC);
  }
}
