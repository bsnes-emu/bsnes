//26  es:
//2e  cs:
//36  ss:
//3e  ds:
auto V30MZ::opSegment(uint16 segment) {
  if(prefixes.size() >= 7) prefixes.removeLast();
  prefixes.prepend(opcode);
  state.prefix = true;
  state.poll = false;
}

//f2  repnz:
//f3  repz:
auto V30MZ::opRepeat(bool flag) {
  if(prefixes.size() >= 7) prefixes.removeLast();
  prefixes.prepend(opcode);
  wait(4);
  state.prefix = true;
  state.poll = false;
}

//f0  lock:
auto V30MZ::opLock() {
  if(prefixes.size() >= 7) prefixes.removeLast();
  prefixes.prepend(opcode);
  state.prefix = true;
  state.poll = false;
}

//9b  wait
auto V30MZ::opWait() {
}

//f4  hlt
auto V30MZ::opHalt() {
  wait(8);
  state.halt = true;
}

//90  nop
auto V30MZ::opNop() {
}

auto V30MZ::opIn(Size size) {
  wait(5);
  setAcc(size, in(size, fetch()));
}

auto V30MZ::opOut(Size size) {
  wait(5);
  out(size, fetch(), getAcc(size));
}

auto V30MZ::opInDX(Size size) {
  wait(5);
  setAcc(size, in(size, r.dx));
}

auto V30MZ::opOutDX(Size size) {
  wait(5);
  out(size, r.dx, getAcc(size));
}

//d7  xlat
auto V30MZ::opTranslate() {
  wait(4);
  r.al = read(Byte, segment(r.ds), r.bx + r.al);
}

//62  bound reg,mem,mem
auto V30MZ::opBound() {
  wait(12);
  modRM();
  auto lo = getMem(Word, 0);
  auto hi = getMem(Word, 2);
  auto reg = getReg(Word);
  if(reg < lo || reg > hi) interrupt(5);
}
