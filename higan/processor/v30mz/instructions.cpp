auto V30MZ::opJumpFar() {
  auto ip = readWord();
  auto cs = readWord();
  r.ip = ip;
  r.cs = cs;
}

auto V30MZ::opInByte() {
  auto port = readByte();
  r.al = in(port);
}

auto V30MZ::opInWord() {
  auto port = readByte();
  r.ax = in(port);
}

auto V30MZ::opOutByte() {
  auto port = readByte();
  out(port, r.al);
}

auto V30MZ::opOutWord() {
  auto port = readByte();
  out(port, r.ax);
}

auto V30MZ::opInDXByte() {
  r.al = in(r.dx);
}

auto V30MZ::opInDXWord() {
  r.ax = in(r.dx);
}

auto V30MZ::opOutDXByte() {
  out(r.dx, r.al);
}

auto V30MZ::opOutDXWord() {
  out(r.dx, r.ax);
}

auto V30MZ::opMoveRegisterImmediateByte(uint8& breg) {
  breg = readByte();
}

auto V30MZ::opMoveRegisterImmediateWord(uint16& wreg) {
  wreg = readWord();
}

auto V30MZ::opNoOperation() {
}

auto V30MZ::opClearFlag(bool& flag) {
  flag = false;
}

auto V30MZ::opSetFlag(bool& flag) {
  flag = true;
}
