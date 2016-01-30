auto V30MZ::opbXorRegisterModRM() {
  auto md = readbIP();
  regb(md) ^= readbModRM(md);
}

auto V30MZ::opwXorRegisterModRM() {
  auto md = readbIP();
  regw(md) ^= readwModRM(md);
}

auto V30MZ::opJumpIf(bool condition) {
  auto displacement = (int8)readbIP();
  if(condition) r.ip += displacement;
}

auto V30MZ::opbMoveRegisterModRM() {
  auto md = readbIP();
  regb(md) = readbModRM(md);
}

auto V30MZ::opwMoveRegisterModRM() {
  auto md = readbIP();
  regw(md) = readwModRM(md);
}

auto V30MZ::opMoveSegmentRegisterModRM() {
  wait(1);
  auto md = readbIP();
  sreg(md) = readwModRM(md);
}

auto V30MZ::opNoOperation() {
}

auto V30MZ::opTestAL() {
  albAnd(r.al, readbIP());
}

auto V30MZ::opTestAX() {
  alwAnd(r.ax, readwIP());
}

auto V30MZ::opbMoveRegisterImmediate(uint8& rd) {
  rd = readbIP();
}

auto V30MZ::opwMoveRegisterImmediate(uint16& rd) {
  rd = readwIP();
}

auto V30MZ::opReturn() {
  wait(5);
  r.ip = readSP();
}

auto V30MZ::opbIn() {
  wait(5);
  auto port = readbIP();
  r.al = in(port);
}

auto V30MZ::opwIn() {
  wait(5);
  auto port = readbIP();
  r.al = in(port + 0);
  r.ah = in(port + 1);
}

auto V30MZ::opbOut() {
  wait(5);
  auto port = readbIP();
  out(port, r.al);
}

auto V30MZ::opwOut() {
  wait(5);
  auto port = readbIP();
  out(port + 0, r.al);
  out(port + 1, r.ah);
}

auto V30MZ::opCallNear() {
  wait(4);
  auto displacement = (int16)readwIP();
  writeSP(r.ip);
  r.ip += displacement;
}

auto V30MZ::opJumpFar() {
  wait(6);
  auto ip = readwIP();
  auto cs = readwIP();
  r.ip = ip;
  r.cs = cs;
}

auto V30MZ::opbInDX() {
  wait(5);
  r.al = in(r.dx);
}

auto V30MZ::opwInDX() {
  wait(5);
  r.al = in(r.dx + 0);
  r.ah = in(r.dx + 1);
}

auto V30MZ::opbOutDX() {
  wait(5);
  out(r.dx, r.al);
}

auto V30MZ::opwOutDX() {
  wait(5);
  out(r.dx + 0, r.al);
  out(r.dx + 1, r.ah);
}

auto V30MZ::opClearFlag(bool& flag) {
  wait(3);
  flag = false;
}

auto V30MZ::opSetFlag(bool& flag) {
  wait(3);
  flag = true;
}
