auto WDC65816::instructionBITImmediate8() -> void {
L uint8 immediate = fetch();
  ZF = (immediate & lo(A)) == 0;
}

auto WDC65816::instructionBITImmediate16() -> void {
  uint16 immediate = fetch();
L immediate |= fetch() << 8;
  ZF = (immediate & A) == 0;
}

auto WDC65816::instructionNOP() -> void {
L idleIRQ();
}

auto WDC65816::instructionWDM() -> void {
L fetch();
}

auto WDC65816::instructionXBA() -> void {
  idle();
L idle();
  A = A >> 8 | A << 8;
  ZF = lo(A) == 0;
  NF = A & 0x80;
}

auto WDC65816::instructionBlockMove8(int adjust) -> void {
  auto targetBank = fetch();
  auto sourceBank = fetch();
  B = targetBank;
  auto data = read(sourceBank << 16 | X);
  write(targetBank << 16 | Y, data);
  idle();
  lo(X) += adjust;
  lo(Y) += adjust;
L idle();
  if(A--) aa(PC) -= 3;
}

auto WDC65816::instructionBlockMove16(int adjust) -> void {
  auto targetBank = fetch();
  auto sourceBank = fetch();
  B = targetBank;
  auto data = read(sourceBank << 16 | X);
  write(targetBank << 16 | Y, data);
  idle();
  X += adjust;
  Y += adjust;
L idle();
  if(A--) aa(PC) -= 3;
}

auto WDC65816::instructionInterrupt(uint16 vector) -> void {
  fetch();
N push(db(PC));
  push(hi(PC));
  push(lo(PC));
  push(P);
  IF = 1;
  DF = 0;
  lo(PC) = read(vector++);
L hi(PC) = read(vector++);
  db(PC) = 0x00;
}

auto WDC65816::instructionSTP() -> void {
  r.stp = true;
  while(r.stp && !synchronizing()) {
L   idle();
  }
}

auto WDC65816::instructionWAI() -> void {
  r.wai = true;
  while(r.wai && !synchronizing()) {
L   idle();
  }
  idle();
}

auto WDC65816::instructionXCE() -> void {
L idleIRQ();
  swap(CF, EF);
  if(EF) {
    XF = 1;
    MF = 1;
    hi(X) = 0x00;
    hi(Y) = 0x00;
    hi(S) = 0x01;
  }
}

auto WDC65816::instructionSetFlag(bool& flag) -> void {
L idleIRQ();
  flag = 1;
}

auto WDC65816::instructionClearFlag(bool& flag) -> void {
L idleIRQ();
  flag = 0;
}

auto WDC65816::instructionREP() -> void {
  auto data = fetch();
L idle();
  P = P & ~data;
E XF = 1, MF = 1;
  if(XF) hi(X) = 0x00, hi(Y) = 0x00;
}

auto WDC65816::instructionSEP() -> void {
  auto data = fetch();
L idle();
  P = P | data;
E XF = 1, MF = 1;
  if(XF) hi(X) = 0x00, hi(Y) = 0x00;
}

auto WDC65816::instructionTransfer8(uint16& from, uint16& to) -> void {
L idleIRQ();
  lo(to) = lo(from);
  ZF = lo(to) == 0;
  NF = to & 0x80;
}

auto WDC65816::instructionTransfer16(uint16& from, uint16& to) -> void {
L idleIRQ();
  to = from;
  ZF = to == 0;
  NF = to & 0x8000;
}

auto WDC65816::instructionTCS() -> void {
L idleIRQ();
  S = A;
E hi(S) = 0x01;
}

auto WDC65816::instructionTSX8() -> void {
L idleIRQ();
  lo(X) = lo(S);
  ZF = lo(X) == 0;
  NF = X & 0x80;
}

auto WDC65816::instructionTSX16() -> void {
L idleIRQ();
  X = S;
  ZF = X == 0;
  NF = X & 0x8000;
}

auto WDC65816::instructionTXS() -> void {
L idleIRQ();
E lo(S) = lo(X);
N S = X;
}

auto WDC65816::instructionPush8(uint8 data) -> void {
  idle();
L push(data);
}

auto WDC65816::instructionPush16(uint16 data) -> void {
  idle();
  push(hi(data));
L push(lo(data));
}

auto WDC65816::instructionPHD() -> void {
  idle();
  pushN(hi(D));
L pushN(lo(D));
E hi(S) = 0x01;
}

auto WDC65816::instructionPull8(uint16& data) -> void {
  idle();
  idle();
L lo(data) = pull();
  ZF = lo(data) == 0;
  NF = data & 0x80;
}

auto WDC65816::instructionPull16(uint16& data) -> void {
  idle();
  idle();
  lo(data) = pull();
L hi(data) = pull();
  ZF = data == 0;
  NF = data & 0x8000;
}

auto WDC65816::instructionPLD() -> void {
  idle();
  idle();
  lo(D) = pullN();
L hi(D) = pullN();
  ZF = D == 0;
  NF = D & 0x8000;
E hi(S) = 0x01;
}

auto WDC65816::instructionPLB() -> void {
  idle();
  idle();
L B = pull();
  ZF = B == 0;
  NF = B & 0x80;
}

auto WDC65816::instructionPLP() -> void {
  idle();
  idle();
L P = pull();
E XF = 1, MF = 1;
  if(XF) hi(X) = 0x00, hi(Y) = 0x00;
}

auto WDC65816::instructionPEA() -> void {
  uint16 data = fetch();
  hi(data) = fetch();
  pushN(hi(data));
L pushN(lo(data));
E hi(S) = 0x01;
}

auto WDC65816::instructionPEI() -> void {
  auto direct = fetch();
  idle2();
  uint16 data = readDirectN(direct + 0);
  hi(data) = readDirectN(direct + 1);
  pushN(hi(data));
L pushN(lo(data));
E hi(S) = 0x01;
}

auto WDC65816::instructionPER() -> void {
  uint16 displacement = fetch();
  hi(displacement) = fetch();
  idle();
  uint16 data = PC + (int16)displacement;
  pushN(hi(data));
L pushN(lo(data));
E hi(S) = 0x01;
}
