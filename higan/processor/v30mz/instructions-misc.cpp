auto V30MZ::instructionSegment(uint16 segment) -> void {
  if(prefixes.size() >= 7) prefixes.removeRight();
  prefixes.prepend(opcode);
  state.prefix = true;
  state.poll = false;
}

auto V30MZ::instructionRepeat() -> void {
  if(prefixes.size() >= 7) prefixes.removeRight();
  prefixes.prepend(opcode);
  wait(4);
  state.prefix = true;
  state.poll = false;
}

auto V30MZ::instructionLock() -> void {
  if(prefixes.size() >= 7) prefixes.removeRight();
  prefixes.prepend(opcode);
  state.prefix = true;
  state.poll = false;
}

auto V30MZ::instructionWait() -> void {
}

auto V30MZ::instructionHalt() -> void {
  wait(8);
  state.halt = true;
}

auto V30MZ::instructionNop() -> void {
}

auto V30MZ::instructionIn(Size size) -> void {
  wait(5);
  setAcc(size, in(size, fetch()));
}

auto V30MZ::instructionOut(Size size) -> void {
  wait(5);
  out(size, fetch(), getAcc(size));
}

auto V30MZ::instructionInDX(Size size) -> void {
  wait(5);
  setAcc(size, in(size, r.dx));
}

auto V30MZ::instructionOutDX(Size size) -> void {
  wait(5);
  out(size, r.dx, getAcc(size));
}

auto V30MZ::instructionTranslate() -> void {
  wait(4);
  r.al = read(Byte, segment(r.ds), r.bx + r.al);
}

auto V30MZ::instructionBound() -> void {
  wait(12);
  modRM();
  auto lo = getMem(Word, 0);
  auto hi = getMem(Word, 2);
  auto reg = getReg(Word);
  if(reg < lo || reg > hi) interrupt(5);
}
