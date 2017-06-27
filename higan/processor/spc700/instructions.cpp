auto SPC700::instructionAbsoluteBitModify(uint3 mode) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  uint3 bit = absolute >> 13;
  absolute &= 0x1fff;
  uint8 data = read(absolute);
  switch(mode) {
  case 0:  //orc  addr:bit
    idle();
    CF |= data.bit(bit);
    break;
  case 1:  //orc !addr:bit
    idle();
    CF |= !data.bit(bit);
    break;
  case 2:  //and  addr:bit
    CF &= data.bit(bit);
    break;
  case 3:  //and !addr:bit
    CF &= !data.bit(bit);
    break;
  case 4:  //eor  addr:bit
    idle();
    CF ^= data.bit(bit);
    break;
  case 5:  //ldc  addr:bit
    CF = data.bit(bit);
    break;
  case 6:  //stc  addr:bit
    idle();
    data.bit(bit) = CF;
    write(absolute, data);
    break;
  case 7:  //not  addr:bit
    data.bit(bit) ^= 1;
    write(absolute, data);
    break;
  }
}

auto SPC700::instructionAbsoluteRead(fpb op, uint8& target) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  uint8 data = read(absolute);
  target = alu(target, data);
}

auto SPC700::instructionAbsoluteModify(fps op) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  uint8 data = read(absolute);
  write(absolute, alu(data));
}

auto SPC700::instructionAbsoluteWrite(uint8& data) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  read(absolute);
  write(absolute, data);
}

auto SPC700::instructionAbsoluteIndexedRead(fpb op, uint8& index) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  idle();
  uint8 data = read(absolute + index);
  A = alu(A, data);
}

auto SPC700::instructionAbsoluteIndexedWrite(uint8& index) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  idle();
  absolute += index;
  read(absolute);
  write(absolute, A);
}

auto SPC700::instructionBranch(bool take) -> void {
  uint8 data = fetch();
  if(!take) return;
  idle();
  idle();
  PC += (int8)data;
}

auto SPC700::instructionDirectRead(fpb op, uint8& target) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  target = alu(target, data);
}

auto SPC700::instructionDirectModify(fps op) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  store(direct, alu(data));
}

auto SPC700::instructionDirectWrite(uint8& data) -> void {
  uint8 direct = fetch();
  load(direct);
  store(direct, data);
}

auto SPC700::instructionDirectWriteDirect(fpb op) -> void {
  uint8 source = fetch();
  uint8 rhs = load(source);
  uint8 target = fetch();
  uint8 lhs;
  if(op != &SPC700::algorithmST) lhs = load(target);
  lhs = alu(lhs, rhs);
  op != &SPC700::algorithmCMP ? store(target, lhs) : idle();
}

auto SPC700::instructionDirectWriteImmediate(fpb op) -> void {
  uint8 immediate = fetch();
  uint8 direct = fetch();
  uint8 data = load(direct);
  data = alu(data, immediate);
  op != &SPC700::algorithmCMP ? store(direct, data) : idle();
}

auto SPC700::instructionDirectReadWord(fpw op) -> void {
  uint8 direct = fetch();
  uint16 data = load(direct++);
  if(op != &SPC700::algorithmCPW) idle();
  data |= load(direct++) << 8;
  YA = alu(YA, data);
}

auto SPC700::instructionDirectModifyWord(int adjust) -> void {
  uint8 direct = fetch();
  uint16 data = load(direct) + adjust;
  store(direct++, data >> 0);
  data += load(direct) << 8;
  store(direct++, data >> 8);
  ZF = data == 0;
  NF = data & 0x8000;
}

auto SPC700::instructionDirectIndexedRead(fpb op, uint8& target, uint8& index) -> void {
  uint8 direct = fetch();
  idle();
  uint8 data = load(direct + index);
  target = alu(target, data);
}

auto SPC700::instructionDirectIndexedModify(fps op, uint8& index) -> void {
  uint8 direct = fetch();
  idle();
  uint8 data = load(direct + index);
  store(direct + index, alu(data));
}

auto SPC700::instructionDirectIndexedWrite(uint8& data, uint8& index) -> void {
  uint8 direct = fetch() + index;
  idle();
  load(direct);
  store(direct, data);
}

auto SPC700::instructionFlagClear(bool& flag) -> void {
  idle();
  if(&flag == &IF) idle();
  flag = 0;
}

auto SPC700::instructionFlagSet(bool& flag) -> void {
  idle();
  if(&flag == &IF) idle();
  flag = 1;
}

auto SPC700::instructionImmediateRead(fpb op, uint8& target) -> void {
  uint8 data = fetch();
  target = alu(target, data);
}

auto SPC700::instructionImpliedModify(fps op, uint8& target) -> void {
  idle();
  target = alu(target);
}

auto SPC700::instructionIndexedIndirectRead(fpb op, uint8& index) -> void {
  uint8 direct = fetch() + index;
  idle();
  uint16 absolute = load(direct++);
  absolute |= load(direct++) << 8;
  uint8 data = read(absolute);
  A = alu(A, data);
}

auto SPC700::instructionIndexedIndirectWrite(uint8& data, uint8& index) -> void {
  uint8 direct = fetch() + index;
  idle();
  uint16 absolute = load(direct++);
  absolute |= load(direct++) << 8;
  read(absolute);
  write(absolute, data);
}

auto SPC700::instructionIndirectIndexedRead(fpb op, uint8& index) -> void {
  uint8 direct = fetch();
  uint16 absolute = load(direct++);
  absolute |= load(direct++) << 8;
  idle();
  uint8 data = read(absolute + index);
  A = alu(A, data);
}

auto SPC700::instructionIndirectIndexedWrite(uint8& data, uint8& index) -> void {
  uint8 direct = fetch();
  uint16 absolute = load(direct++);
  absolute |= load(direct++) << 8;
  idle();
  read(absolute + index);
  write(absolute + index, data);
}

auto SPC700::instructionIndirectXRead(fpb op) -> void {
  idle();
  uint8 data = load(X);
  A = alu(A, data);
}

auto SPC700::instructionIndirectXWrite(uint8& data) -> void {
  idle();
  load(X);
  store(X, data);
}

auto SPC700::instructionIndirectXIncrementRead(uint8& data) -> void {
  idle();
  data = load(X++);
  idle();
  ZF = data == 0;
  NF = data & 0x80;
}

auto SPC700::instructionIndirectXIncrementWrite(uint8& data) -> void {
  idle();
  idle();
  store(X++, data);
}

auto SPC700::instructionIndirectXWriteIndirectY(fpb op) -> void {
  idle();
  uint8 rhs = load(Y);
  uint8 lhs = load(X);
  lhs = alu(lhs, rhs);
  op != &SPC700::algorithmCMP ? store(X, lhs) : idle();
}

auto SPC700::instructionPull(uint8& data) -> void {
  idle();
  idle();
  data = pull();
}

auto SPC700::instructionPush(uint8 data) -> void {
  idle();
  idle();
  push(data);
}

auto SPC700::instructionTransfer(uint8& from, uint8& to) -> void {
  idle();
  to = from;
  if(&to == &S) return;
  ZF = to == 0;
  NF = to & 0x80;
}

//

auto SPC700::instructionBBC(uint3 bit) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  uint8 displacement = fetch();
  idle();
  if(data.bit(bit) == 1) return;
  idle();
  idle();
  PC += (int8)displacement;
}

auto SPC700::instructionBBS(uint3 bit) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  uint8 displacement = fetch();
  idle();
  if(data.bit(bit) == 0) return;
  idle();
  idle();
  PC += (int8)displacement;
}

auto SPC700::instructionBNEDirect() -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  uint8 displacement = fetch();
  idle();
  if(A == data) return;
  idle();
  idle();
  PC += (int8)displacement;
}

auto SPC700::instructionBNEDirectDecrement() -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  store(direct, --data);
  uint8 displacement = fetch();
  if(data == 0) return;
  idle();
  idle();
  PC += (int8)displacement;
}

auto SPC700::instructionBNEDirectX() -> void {
  uint8 direct = fetch();
  idle();
  uint8 data = load(direct + X);
  uint8 displacement = fetch();
  idle();
  if(A == data) return;
  idle();
  idle();
  PC += (int8)displacement;
}

auto SPC700::instructionBNEYDecrement() -> void {
  uint8 displacement = fetch();
  idle();
  idle();
  if(--Y == 0) return;
  idle();
  idle();
  PC += (int8)displacement;
}

auto SPC700::instructionBRK() -> void {
  uint16 absolute = read(0xffde);
  absolute |= read(0xffdf) << 8;
  idle();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  push(P);
  PC = absolute;
  IF = 0;
  BF = 1;
}

auto SPC700::instructionCLR(uint3 bit) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  data.bit(bit) = 0;
  store(direct, data);
}

auto SPC700::instructionCLV() -> void {
  idle();
  HF = 0;
  VF = 0;
}

auto SPC700::instructionCMC() -> void {
  idle();
  idle();
  CF = !CF;
}

auto SPC700::instructionDAA() -> void {
  idle();
  idle();
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

auto SPC700::instructionDAS() -> void {
  idle();
  idle();
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

auto SPC700::instructionDIV() -> void {
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
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

auto SPC700::instructionJMPAbsolute() -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  PC = absolute;
}

auto SPC700::instructionJMPIndirectX() -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  idle();
  absolute += X;
  uint16 pc = read(absolute++);
  pc |= read(absolute++) << 8;
  PC = pc;
}

auto SPC700::instructionJSPDirect() -> void {
  uint8 direct = fetch();
  idle();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  PC = 0xff00 | direct;
}

auto SPC700::instructionJSRAbsolute() -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  idle();
  idle();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  PC = absolute;
}

auto SPC700::instructionJST(uint4 vector) -> void {
  uint16 absolute = 0xffde - (vector << 1);
  uint16 pc = read(absolute++);
  pc |= read(absolute++) << 8;
  idle();
  idle();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  PC = pc;
}

auto SPC700::instructionMUL() -> void {
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
  idle();
  uint16 ya = Y * A;
  A = ya >> 0;
  Y = ya >> 8;
  //result is set based on y (high-byte) only
  ZF = Y == 0;
  NF = Y & 0x80;
}

auto SPC700::instructionNOP() -> void {
  idle();
}

auto SPC700::instructionPLP() -> void {
  idle();
  idle();
  P = pull();
}

auto SPC700::instructionRTI() -> void {
  P = pull();
  uint16 absolute = pull();
  absolute |= pull() << 8;
  idle();
  idle();
  PC = absolute;
}

auto SPC700::instructionRTS() -> void {
  uint16 absolute = pull();
  absolute |= pull() << 8;
  idle();
  idle();
  PC = absolute;
}

auto SPC700::instructionSET(uint3 bit) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  data.bit(bit) = 1;
  store(direct, data);
}

auto SPC700::instructionSTP() -> void {
  r.stp = true;
  while(r.stp && !synchronizing()) {
    idle();
    idle();
  }
}

auto SPC700::instructionSTWDirect() -> void {
  uint8 direct = fetch();
  load(direct);
  store(direct++, A);
  store(direct++, Y);
}

auto SPC700::instructionTRBAbsolute() -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  uint8 data = read(absolute);
  ZF = (A - data) == 0;
  NF = (A - data) & 0x80;
  read(absolute);
  write(absolute, data & ~A);
}

auto SPC700::instructionTSBAbsolute() -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  uint8 data = read(absolute);
  ZF = (A - data) == 0;
  NF = (A - data) & 0x80;
  read(absolute);
  write(absolute, data | A);
}

auto SPC700::instructionWAI() -> void {
  r.wai = true;
  while(r.wai && !synchronizing()) {
    idle();
    idle();
  }
}

auto SPC700::instructionXCN() -> void {
  idle();
  idle();
  idle();
  idle();
  A = A >> 4 | A << 4;
  ZF = A == 0;
  NF = A & 0x80;
}
