auto V30MZ::opXorRegisterModRM(Size size) {
  auto modRM = readIP();
  setRegister(size, modRM, getRegister(size, modRM) ^ readModRM(size, modRM));
}

auto V30MZ::opJumpIf(bool condition) {
  auto displacement = (int8)readIP();
  if(condition) r.ip += displacement;
}

auto V30MZ::opMoveRegisterModRM(Size size) {
  auto modRM = readIP();
  setRegister(size, modRM, readModRM(size, modRM));
}

auto V30MZ::opMoveSegmentRegisterModRM() {
  wait(1);
  auto modRM = readIP();
  setSegment(modRM, readModRM(Word, modRM));
}

auto V30MZ::opNoOperation() {
}

auto V30MZ::opMoveString(Size size) {
  wait(4);
  write(size, r.es, r.di, read(size, r.ds, r.si));
  r.si += r.f.d ? -size : size;
  r.di += r.f.d ? -size : size;
}

auto V30MZ::opTestAX(Size size) {
  alAnd(size, r.ax, readIP(size));
}

auto V30MZ::opMoveRegisterImmediate(uint8& rd) {
  rd = readIP(Byte);
}

auto V30MZ::opMoveRegisterImmediate(uint16& rd) {
  rd = readIP(Word);
}

auto V30MZ::opReturn() {
  wait(5);
  r.ip = readSP();
}

auto V30MZ::opIn(Size size) {
  wait(5);
  auto port = readIP();
  r.al = in(port++);
  if(size != Word) return;
  r.ah = in(port++);
}

auto V30MZ::opOut(Size size) {
  wait(5);
  auto port = readIP();
  out(port++, r.al);
  if(size != Word) return;
  out(port++, r.ah);
}

auto V30MZ::opCallNear() {
  wait(4);
  auto displacement = (int16)readIP(Word);
  writeSP(r.ip);
  r.ip += displacement;
}

auto V30MZ::opJumpFar() {
  wait(6);
  auto ip = readIP(Word);
  auto cs = readIP(Word);
  r.ip = ip;
  r.cs = cs;
}

auto V30MZ::opInDX(Size size) {
  wait(5);
  r.al = in(r.dx + 0);
  if(size != Word) return;
  r.ah = in(r.dx + 1);
}

auto V30MZ::opOutDX(Size size) {
  wait(5);
  out(r.dx + 0, r.al);
  if(size != Word) return;
  out(r.dx + 1, r.ah);
}

auto V30MZ::opRepeat(bool flag) {
  wait(4);
  auto opcode = readIP();
  if((opcode & 0xfc) != 0x6c && (opcode & 0xfc) != 0xa4
  && (opcode & 0xfe) != 0xaa && (opcode & 0xfc) != 0xac) {
    //invalid argument
    r.ip--;
    return;
  }
  if(r.cx == 0) return;
  r.cx--;

  switch(opcode) {
  case 0xa4: opMoveString(Byte); r.ip -= 2; break;
  case 0xa5: opMoveString(Word); r.ip -= 2; break;
  }
}

auto V30MZ::opClearFlag(bool& flag) {
  wait(3);
  flag = false;
}

auto V30MZ::opSetFlag(bool& flag) {
  wait(3);
  flag = true;
}
