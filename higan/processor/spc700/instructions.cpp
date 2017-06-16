#define alu (this->*op)

auto SPC700::instructionImpliedModify(fps op, uint8& target) -> void {
  idle();
  target = alu(target);
}

auto SPC700::instructionAbsoluteModify(fps op) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  uint8 data = read(absolute);
  write(absolute, alu(data));
}

auto SPC700::instructionDirectPageModify(fps op) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  store(direct, alu(data));
}

auto SPC700::instructionDirectPageModifyWord(int adjust) -> void {
  uint8 direct = fetch();
  uint16 data = load(direct) + adjust;
  store(direct++, data >> 0);
  data += load(direct) << 8;
  store(direct++, data >> 8);
  ZF = data == 0;
  NF = data & 0x8000;
}

auto SPC700::instructionDirectPageXModify(fps op) -> void {
  uint8 direct = fetch();
  idle();
  uint8 data = load(direct + X);
  store(direct + X, alu(data));
}

auto SPC700::instructionBranch(bool take) -> void {
  uint8 data = fetch();
  if(!take) return;
  idle();
  idle();
  PC += (int8)data;
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

auto SPC700::instructionAbsoluteRead(fpb op, uint8& target) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  uint8 data = read(absolute);
  target = alu(target, data);
}

auto SPC700::instructionAbsoluteIndexedRead(fpb op, uint8& index) -> void {
  uint16 absolute = fetch();
  absolute |= fetch() << 8;
  idle();
  uint8 data = read(absolute + index);
  A = alu(A, data);
}

auto SPC700::instructionImmediateRead(fpb op, uint8& target) -> void {
  uint8 data = fetch();
  target = alu(target, data);
}

auto SPC700::instructionDirectPageRead(fpb op, uint8& target) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  target = alu(target, data);
}

auto SPC700::instructionDirectPageIndexedRead(fpb op, uint8& data, uint8& index) -> void {
  dp = fetch();
  idle();
  rd = load(dp + index);
  data = alu(data, rd);
}

auto SPC700::instructionDirectPageReadWord(fpw op) -> void {
  dp = fetch();
  rd.l = load(dp++);
  if(op != &SPC700::algorithmCPW) idle();
  rd.h = load(dp++);
  YA = alu(YA, rd);
}

auto SPC700::instructionIndirectPageXRead(fpb op) -> void {
  dp = fetch() + X;
  idle();
  sp.l = load(dp++);
  sp.h = load(dp++);
  rd = read(sp);
  A = alu(A, rd);
}

auto SPC700::instructionIndirectPageYRead(fpb op) -> void {
  dp = fetch();
  idle();
  sp.l = load(dp++);
  sp.h = load(dp++);
  rd = read(sp + Y);
  A = alu(A, rd);
}

auto SPC700::instructionIndirectXRead(fpb op) -> void {
  idle();
  rd = load(X);
  A = alu(A, rd);
}

auto SPC700::instructionAbsoluteModifyBit() -> void {
  dp.l = fetch();
  dp.h = fetch();
  bit = dp >> 13;
  dp &= 0x1fff;
  rd = read(dp);
  switch(opcode >> 5) {
  case 0:  //orc  addr:bit
  case 1:  //orc !addr:bit
    idle();
    CF |= (rd & (1 << bit)) ^ (bool)(opcode & 0x20);
    break;
  case 2:  //and  addr:bit
  case 3:  //and !addr:bit
    CF &= (rd & (1 << bit)) ^ (bool)(opcode & 0x20);
    break;
  case 4:  //eor  addr:bit
    idle();
    CF ^= (bool)(rd & (1 << bit));
    break;
  case 5:  //ldc  addr:bit
    CF  = (rd & (1 << bit));
    break;
  case 6:  //stc  addr:bit
    idle();
    rd = (rd & ~(1 << bit)) | (CF << bit);
    write(dp, rd);
    break;
  case 7:  //not  addr:bit
    rd ^= 1 << bit;
    write(dp, rd);
    break;
  }
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

auto SPC700::instructionTransfer(uint8& from, uint8& to) -> void {
  idle();
  to = from;
  if(&to == &S) return;
  ZF = to == 0;
  NF = to & 0x80;
}

auto SPC700::instructionAbsoluteWrite(uint8& data) -> void {
  dp.l = fetch();
  dp.h = fetch();
  read(dp);
  write(dp, data);
}

auto SPC700::instructionAbsoluteIndexedWrite(uint8& index) -> void {
  dp.l = fetch();
  dp.h = fetch();
  idle();
  dp += index;
  read(dp);
  write(dp, A);
}

auto SPC700::instructionDirectPageWrite(uint8& data) -> void {
  dp = fetch();
  load(dp);
  store(dp, data);
}

auto SPC700::instructionDirectPageIndexedWrite(uint8& data, uint8& index) -> void {
  dp = fetch() + index;
  idle();
  load(dp);
  store(dp, data);
}

auto SPC700::instructionDirectPageWriteImmediate(fpb op) -> void {
  rd = fetch();
  dp = fetch();
  wr = load(dp);
  wr = alu(wr, rd);
  op != &SPC700::algorithmCMP ? store(dp, wr) : idle();
}

auto SPC700::instructionDirectPageWriteDirectPage(fpb op) -> void {
  sp = fetch();
  rd = load(sp);
  dp = fetch();
  if(op != &SPC700::algorithmST) wr = load(dp);
  wr = alu(wr, rd);
  op != &SPC700::algorithmCMP ? store(dp, wr) : idle();
}

auto SPC700::instructionIndirectXWriteIndirectY(fpb op) -> void {
  idle();
  rd = load(Y);
  wr = load(X);
  wr = alu(wr, rd);
  op != &SPC700::algorithmCMP ? store(X, wr) : idle();
}

//

auto SPC700::instructionBBC(uint3 bit) -> void {
  dp = fetch();
  uint8 data = load(dp);
  rd = fetch();
  idle();
  if(data.bit(bit) == 1) return;
  idle();
  idle();
  PC += (int8)rd;
}

auto SPC700::instructionBBS(uint3 bit) -> void {
  dp = fetch();
  uint8 data = load(dp);
  rd = fetch();
  idle();
  if(data.bit(bit) == 0) return;
  idle();
  idle();
  PC += (int8)rd;
}

auto SPC700::instructionBNEDirectPage() -> void {
  dp = fetch();
  sp = load(dp);
  rd = fetch();
  idle();
  if(A == sp) return;
  idle();
  idle();
  PC += (int8)rd;
}

auto SPC700::instructionBNEDirectPageDecrement() -> void {
  dp = fetch();
  wr = load(dp);
  store(dp, --wr);
  rd = fetch();
  if(wr == 0) return;
  idle();
  idle();
  PC += (int8)rd;
}

auto SPC700::instructionBNEDirectPageX() -> void {
  dp = fetch();
  idle();
  sp = load(dp + X);
  rd = fetch();
  idle();
  if(A == sp) return;
  idle();
  idle();
  PC += (int8)rd;
}

auto SPC700::instructionBNEYDecrement() -> void {
  rd = fetch();
  idle();
  idle();
  if(--Y == 0) return;
  idle();
  idle();
  PC += (int8)rd;
}

auto SPC700::instructionBRK() -> void {
  rd.l = read(0xffde);
  rd.h = read(0xffdf);
  idle();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  push(P);
  PC = rd;
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
  rd.l = fetch();
  rd.h = fetch();
  PC = rd;
}

auto SPC700::instructionJMPIndirectAbsoluteX() -> void {
  dp.l = fetch();
  dp.h = fetch();
  idle();
  dp += X;
  rd.l = read(dp++);
  rd.h = read(dp++);
  PC = rd;
}

auto SPC700::instructionJSPDirectPage() -> void {
  rd = fetch();
  idle();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  PC = 0xff00 | rd;
}

auto SPC700::instructionJSRAbsolute() -> void {
  rd.l = fetch();
  rd.h = fetch();
  idle();
  idle();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  PC = rd;
}

auto SPC700::instructionJST(uint4 vector) -> void {
  dp = 0xffde - (vector << 1);
  rd.l = read(dp++);
  rd.h = read(dp++);
  idle();
  idle();
  idle();
  push(PC >> 8);
  push(PC >> 0);
  PC = rd;
}

auto SPC700::instructionLDAIndirectXIncrement() -> void {
  idle();
  A = load(X++);
  idle();
  ZF = A == 0;
  NF = A & 0x80;
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
  rd.l = pull();
  rd.h = pull();
  idle();
  idle();
  PC = rd;
}

auto SPC700::instructionRTS() -> void {
  rd.l = pull();
  rd.h = pull();
  idle();
  idle();
  PC = rd;
}

auto SPC700::instructionSET(uint3 bit) -> void {
  uint8 direct = fetch();
  uint8 data = load(direct);
  data.bit(bit) = 1;
  store(direct, data);
}

auto SPC700::instructionSTAIndirectPageX() -> void {
  sp = fetch() + X;
  idle();
  dp.l = load(sp++);
  dp.h = load(sp++);
  read(dp);
  write(dp, A);
}

auto SPC700::instructionSTAIndirectPageY() -> void {
  sp = fetch();
  dp.l = load(sp++);
  dp.h = load(sp++);
  idle();
  dp += Y;
  read(dp);
  write(dp, A);
}

auto SPC700::instructionSTAIndirectX() -> void {
  idle();
  load(X);
  store(X, A);
}

auto SPC700::instructionSTAIndirectXIncrement() -> void {
  idle();
  idle();
  store(X++, A);
}

auto SPC700::instructionSTP() -> void {
  while(true) {
    idle();
    idle();
  }
}

auto SPC700::instructionSTWDirectPage() -> void {
  dp = fetch();
  load(dp);
  store(dp++, A);
  store(dp++, Y);
}

auto SPC700::instructionTRBAbsolute() -> void {
  dp.l = fetch();
  dp.h = fetch();
  rd = read(dp);
  ZF = (A - rd) == 0;
  NF = (A - rd) & 0x80;
  read(dp);
  write(dp, rd & ~A);
}

auto SPC700::instructionTSBAbsolute() -> void {
  dp.l = fetch();
  dp.h = fetch();
  rd = read(dp);
  ZF = (A - rd) == 0;
  NF = (A - rd) & 0x80;
  read(dp);
  write(dp, rd | A);
}

auto SPC700::instructionWAI() -> void {
  while(true) {
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

#undef alu
