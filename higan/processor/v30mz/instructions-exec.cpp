auto V30MZ::instructionLoop() -> void {
  wait(1);
  auto offset = (int8)fetch();
  if(--r.cx) {
    wait(3);
    r.ip += offset;
  }
}

auto V30MZ::instructionLoopWhile(bool value) -> void {
  wait(2);
  auto offset = (int8)fetch();
  if(--r.cx && r.f.z == value) {
    wait(3);
    r.ip += offset;
  }
}

auto V30MZ::instructionJumpShort() -> void {
  wait(3);
  auto offset = (int8)fetch();
  r.ip += offset;
}

auto V30MZ::instructionJumpIf(bool condition) -> void {
  auto offset = (int8)fetch();
  if(condition) r.ip += offset;
}

auto V30MZ::instructionJumpNear() -> void {
  wait(3);
  r.ip += (int16)fetch(Word);
}

auto V30MZ::instructionJumpFar() -> void {
  wait(6);
  auto ip = fetch(Word);
  auto cs = fetch(Word);
  r.cs = cs;
  r.ip = ip;
}

auto V30MZ::instructionCallNear() -> void {
  wait(4);
  auto offset = (int16)fetch(Word);
  push(r.ip);
  r.ip += offset;
}

auto V30MZ::instructionCallFar() -> void {
  wait(9);
  auto ip = fetch(Word);
  auto cs = fetch(Word);
  push(r.cs);
  push(r.ip);
  r.cs = cs;
  r.ip = ip;
}

auto V30MZ::instructionReturn() -> void {
  wait(5);
  r.ip = pop();
}

auto V30MZ::instructionReturnImm() -> void {
  wait(5);
  auto offset = fetch(Word);
  r.ip = pop();
  r.sp += offset;
}

auto V30MZ::instructionReturnFar() -> void {
  wait(7);
  r.ip = pop();
  r.cs = pop();
}

auto V30MZ::instructionReturnFarImm() -> void {
  wait(8);
  auto offset = fetch(Word);
  r.ip = pop();
  r.cs = pop();
  r.sp += offset;
}

auto V30MZ::instructionReturnInt() -> void {
  wait(9);
  r.ip = pop();
  r.cs = pop();
  r.f = pop();
  state.poll = false;
}

auto V30MZ::instructionInt3() -> void {
  wait(8);
  interrupt(3);
}

auto V30MZ::instructionIntImm() -> void {
  wait(9);
  interrupt(fetch());
}

auto V30MZ::instructionInto() -> void {
  wait(5);
  interrupt(4);
}

auto V30MZ::instructionEnter() -> void {
  wait(7);
  auto offset = fetch(Word);
  auto length = fetch(Byte) & 0x1f;
  push(r.bp);
  r.bp = r.sp;
  r.sp -= offset;

  if(length) {
    wait(length > 1 ? 7 : 6);
    for(uint n = 1; n < length; n++) {
      wait(4);
      auto data = read(Word, segment(r.ss), r.bp - n * 2);
      push(data);
    }
    push(r.bp);
  }
}

auto V30MZ::instructionLeave() -> void {
  wait(1);
  r.sp = r.bp;
  r.bp = pop();
}

auto V30MZ::instructionPushReg(uint16_t& reg) -> void {
  push(reg);
}

auto V30MZ::instructionPopReg(uint16_t& reg) -> void {
  reg = pop();
  if(&reg == &r.ss) state.poll = false;
}

auto V30MZ::instructionPushFlags() -> void {
  wait(1);
  push(r.f);
}

auto V30MZ::instructionPopFlags() -> void {
  wait(2);
  r.f = pop();
  state.poll = false;
}

auto V30MZ::instructionPushAll() -> void {
  wait(8);
  auto sp = r.sp;
  push(r.ax);
  push(r.cx);
  push(r.dx);
  push(r.bx);
  push(sp);
  push(r.bp);
  push(r.si);
  push(r.di);
}

auto V30MZ::instructionPopAll() -> void {
  wait(7);
  r.di = pop();
  r.si = pop();
  r.bp = pop();
  auto sp = pop();
  r.bx = pop();
  r.dx = pop();
  r.cx = pop();
  r.ax = pop();
  //r.sp is not restored
}

auto V30MZ::instructionPushImm(Size size) -> void {
  if(size == Byte) push((int8)fetch(Byte));
  if(size == Word) push(fetch(Word));
}

auto V30MZ::instructionPopMem() -> void {
  modRM();
  setMem(Word, pop());
}
