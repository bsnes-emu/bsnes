auto V30MZ::opLoop() {
  wait(1);
  auto offset = (int8)fetch();
  if(--r.cx) {
    wait(3);
    r.ip += offset;
  }
}

auto V30MZ::opLoopWhile(bool value) {
  wait(2);
  auto offset = (int8)fetch();
  if(--r.cx && r.f.z == value) {
    wait(3);
    r.ip += offset;
  }
}

auto V30MZ::opJumpShort() {
  wait(3);
  auto offset = (int8)fetch();
  r.ip += offset;
}

auto V30MZ::opJumpIf(bool condition) {
  auto offset = (int8)fetch();
  if(condition) r.ip += offset;
}

auto V30MZ::opJumpNear() {
  wait(3);
  r.ip += (int16)fetch(Word);
}

auto V30MZ::opJumpFar() {
  wait(6);
  auto ip = fetch(Word);
  auto cs = fetch(Word);
  r.cs = cs;
  r.ip = ip;
}

auto V30MZ::opCallNear() {
  wait(4);
  auto offset = (int16)fetch(Word);
  push(r.ip);
  r.ip += offset;
}

auto V30MZ::opCallFar() {
  wait(9);
  auto ip = fetch(Word);
  auto cs = fetch(Word);
  push(r.cs);
  push(r.ip);
  r.cs = cs;
  r.ip = ip;
}

auto V30MZ::opReturn() {
  wait(5);
  r.ip = pop();
}

auto V30MZ::opReturnImm() {
  wait(5);
  auto offset = fetch(Word);
  r.ip = pop();
  r.sp += offset;
}

auto V30MZ::opReturnFar() {
  wait(7);
  r.ip = pop();
  r.cs = pop();
}

auto V30MZ::opReturnFarImm() {
  wait(8);
  auto offset = fetch(Word);
  r.ip = pop();
  r.cs = pop();
  r.sp += offset;
}

//cf  iret
auto V30MZ::opReturnInt() {
  wait(9);
  r.ip = pop();
  r.cs = pop();
  r.f = pop();
  state.poll = false;
}

auto V30MZ::opInt3() {
  wait(8);
  interrupt(3);
}

auto V30MZ::opIntImm() {
  wait(9);
  interrupt(fetch());
}

auto V30MZ::opInto() {
  wait(5);
  interrupt(4);
}

auto V30MZ::opEnter() {
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

auto V30MZ::opLeave() {
  wait(1);
  r.sp = r.bp;
  r.bp = pop();
}

auto V30MZ::opPushReg(uint16& reg) {
  push(reg);
}

auto V30MZ::opPopReg(uint16& reg) {
  reg = pop();
  if(&reg == &r.ss) state.poll = false;
}

//9c  pushf
auto V30MZ::opPushFlags() {
  wait(1);
  push(r.f);
}

//9d  popf
auto V30MZ::opPopFlags() {
  wait(2);
  r.f = pop();
  state.poll = false;
}

//60  pusha
auto V30MZ::opPushAll() {
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

//61  popa
auto V30MZ::opPopAll() {
  wait(7);
  r.di = pop();
  r.si = pop();
  r.bp = pop();
  auto sp = pop();
  r.bx = pop();
  r.dx = pop();
  r.cx = pop();
  r.ax = pop();
  r.sp = sp;
}

//68  push imm16
//6a  push imm8s
auto V30MZ::opPushImm(Size size) {
  push(size == Word ? fetch(Word) : (int8)fetch(Byte));
}

auto V30MZ::opPopMem() {
  modRM();
  setMem(Word, pop());
}

