auto WDC65816::instructionBranch(bool take) -> void {
  if(!take) {
L   fetch();
  } else {
    uint8 displacement = fetch();
    uint16 absolute = PC + (int8)displacement;
    idle6(absolute);
L   idle();
    aa(PC) = absolute;
  }
}

auto WDC65816::instructionBRL() -> void {
  uint16 displacement = fetch();
  hi(displacement) = fetch();
L idle();
  aa(PC) = PC + (int16)displacement;
}

auto WDC65816::instructionJMPShort() -> void {
  uint16 data = fetch();
L hi(data) = fetch();
  aa(PC) = data;
}

auto WDC65816::instructionJMPLong() -> void {
  uint24 data = fetch();
  hi(data) = fetch();
L db(data) = fetch();
  PC = data;
}

auto WDC65816::instructionJMPIndirect() -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  uint16 data = read(uint16(absolute + 0));
L hi(data) = read(uint16(absolute + 1));
  aa(PC) = data;
}

auto WDC65816::instructionJMPIndexedIndirect() -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  idle();
  uint16 data = read(db(PC) << 16 | uint16(absolute + X + 0));
L hi(data) = read(db(PC) << 16 | uint16(absolute + X + 1));
  aa(PC) = data;
}

auto WDC65816::instructionJMPIndirectLong() -> void {
  uint16 absolute = fetch();
  hi(absolute) = fetch();
  uint24 data = read(uint16(absolute + 0));
  hi(data) = read(uint16(absolute + 1));
L db(data) = read(uint16(absolute + 2));
  PC = data;
}

auto WDC65816::instructionJSRShort() -> void {
  uint16 data = fetch();
  hi(data) = fetch();
  idle();
  aa(PC)--;
  push(hi(PC));
L push(lo(PC));
  aa(PC) = data;
}

auto WDC65816::instructionJSRLong() -> void {
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
}

auto WDC65816::instructionJSRIndexedIndirect() -> void {
  uint16 absolute = fetch();
  pushN(hi(PC));
  pushN(lo(PC));
  hi(absolute) = fetch();
  idle();
  uint16 data = read(db(PC) << 16 | uint16(absolute + X + 0));
L hi(data) = read(db(PC) << 16 | uint16(absolute + X + 1));
  aa(PC) = data;
E hi(S) = 0x01;
}

auto WDC65816::instructionRTI() -> void {
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
}

auto WDC65816::instructionRTS() -> void {
  idle();
  idle();
  uint16 data = pull();
  hi(data) = pull();
L idle();
  aa(PC) = data;
  aa(PC)++;
}

auto WDC65816::instructionRTL() -> void {
  idle();
  idle();
  uint24 data = pullN();
  hi(data) = pullN();
L db(data) = pullN();
  PC = data;
  aa(PC)++;
E hi(S) = 0x01;
}
