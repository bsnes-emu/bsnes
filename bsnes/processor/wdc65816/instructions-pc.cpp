auto WDC65816::instructionBranch(bool take) -> void {
  if(!take) {
L   fetch();
  } else {
    uint8 displacement = fetch();
    uint16 absolute = PC + (int8)displacement;
    idle6(absolute);
L   idle();
    aa(PC) = absolute;
    idleBranch();
  }
}

auto WDC65816::instructionBranchLong() -> void {
  uint16 displacement = fetch();
  hi(displacement) = fetch();
  uint16 absolute = PC + (int16)displacement;
L idle();
  aa(PC) = absolute;
  idleBranch();
}

auto WDC65816::instructionJumpShort() -> void {
  uint16 data = fetch();
L hi(data) = fetch();
  aa(PC) = data;
  idleJump();
}

auto WDC65816::instructionJumpLong() -> void {
  uint24 data = fetch();
  hi(data) = fetch();
L db(data) = fetch();
  PC = data;
  idleJump();
}

auto WDC65816::instructionJumpIndirect() -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  uint16 data = read(uint16(absolute + 0));
L hi(data) = read(uint16(absolute + 1));
  aa(PC) = data;
  idleJump();
}

auto WDC65816::instructionJumpIndexedIndirect() -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  idle();
  uint16 data = read(db(PC) << 16 | uint16(absolute + X + 0));
L hi(data) = read(db(PC) << 16 | uint16(absolute + X + 1));
  aa(PC) = data;
  idleJump();
}

auto WDC65816::instructionJumpIndirectLong() -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  uint24 data = read(uint16(absolute + 0));
  hi(data) = read(uint16(absolute + 1));
L db(data) = read(uint16(absolute + 2));
  PC = data;
  idleJump();
}

auto WDC65816::instructionCallShort() -> void {
  uint16 data = fetch();
  hi(data) = fetch();
  idle();
  aa(PC)--;
  push(hi(PC));
L push(lo(PC));
  aa(PC) = data;
  idleJump();
}

auto WDC65816::instructionCallLong() -> void {
  uint24 data = fetch();
  hi(data) = fetch();
  pushN(db(PC));
  idle();
  db(data) = fetch();
  aa(PC)--;
  pushN(hi(PC));
L pushN(lo(PC));
  PC = data;
E hi(S) = 0x01;
  idleJump();
}

auto WDC65816::instructionCallIndexedIndirect() -> void {
  uint16 absolute = fetch();
  pushN(hi(PC));
  pushN(lo(PC));
  hi(absolute) = fetch();
  idle();
  uint16 data = read(db(PC) << 16 | uint16(absolute + X + 0));
L hi(data) = read(db(PC) << 16 | uint16(absolute + X + 1));
  aa(PC) = data;
E hi(S) = 0x01;
  idleJump();
}

auto WDC65816::instructionReturnInterrupt() -> void {
  idle();
  idle();
  P = pull();
E XF = 1, MF = 1;
  if(XF) hi(X) = 0x00, hi(Y) = 0x00;
  lo(PC) = pull();
  if(EF) {
  L hi(PC) = pull();
  } else {
    hi(PC) = pull();
  L db(PC) = pull();
  }
  idleJump();
}

auto WDC65816::instructionReturnShort() -> void {
  idle();
  idle();
  uint16 data = pull();
  hi(data) = pull();
L idle();
  aa(PC) = data;
  aa(PC)++;
  idleJump();
}

auto WDC65816::instructionReturnLong() -> void {
  idle();
  idle();
  uint24 data = pullN();
  hi(data) = pullN();
L db(data) = pullN();
  PC = data;
  aa(PC)++;
E hi(S) = 0x01;
  idleJump();
}
