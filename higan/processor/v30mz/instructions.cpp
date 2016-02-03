//00  addb mem,reg
//01  addw mem,reg
auto V30MZ::opAddMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, alAdd(size, getMem(size, modRM), getReg(size, modRM)));
}

//02  addb reg,mem
//03  addw reg,mem
auto V30MZ::opAddRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, alAdd(size, getReg(size, modRM), getMem(size, modRM)));
}

//04  add al,#imm
//05  add ax,#imm
auto V30MZ::opAddAccImm(Size size) {
  setAcc(size, alAdd(size, getAcc(size), fetch(size)));
}

//08  orb mem,reg
//09  orb mem,reg
auto V30MZ::opOrMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, alOr(size, getMem(size, modRM), getReg(size, modRM)));
}

//0a  orb reg,mem
//0b  orb reg,mem
auto V30MZ::opOrRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, alOr(size, getReg(size, modRM), getMem(size, modRM)));
}

//0c  or al,#imm
//0d  or ax,#imm
auto V30MZ::opOrAccImm(Size size) {
  setAcc(size, alOr(size, getAcc(size), fetch(size)));
}

auto V30MZ::opAdcMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, alAdc(size, getMem(size, modRM), getReg(size, modRM)));
}

auto V30MZ::opAdcRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, alAdc(size, getReg(size, modRM), getMem(size, modRM)));
}

auto V30MZ::opAdcAccImm(Size size) {
  setAcc(size, alAdc(size, getAcc(size), fetch(size)));
}

auto V30MZ::opSbbMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, alSbb(size, getMem(size, modRM), getReg(size, modRM)));
}

auto V30MZ::opSbbRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, alSbb(size, getReg(size, modRM), getMem(size, modRM)));
}

auto V30MZ::opSbbAccImm(Size size) {
  setAcc(size, alSbb(size, getAcc(size), fetch(size)));
}

auto V30MZ::opAndMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, alAnd(size, getMem(size, modRM), getReg(size, modRM)));
}

auto V30MZ::opAndRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, alAnd(size, getReg(size, modRM), getMem(size, modRM)));
}

auto V30MZ::opAndAccImm(Size size) {
  setAcc(size, alAnd(size, getAcc(size), fetch(size)));
}

auto V30MZ::opSubMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, alSub(size, getMem(size, modRM), getReg(size, modRM)));
}

auto V30MZ::opSubRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, alSub(size, getReg(size, modRM), getMem(size, modRM)));
}

auto V30MZ::opSubAccImm(Size size) {
  setAcc(size, alSub(size, getAcc(size), fetch(size)));
}

//26  es:
//2e  cs:
//36  ss:
//3e  ds:
auto V30MZ::opPrefix(uint16& segment) {
  prefix.hold = true;
  prefix.segment = segment;
}

//27  daa
//2f  das
auto V30MZ::opDecimalAdjust(bool negate) {
  wait(9);
  uint8 al = r.al;
  if(r.f.h || ((al & 0x0f) > 0x09)) {
    r.al += negate ? -0x06 : 0x06;
    r.f.h = 1;
  }
  if(r.f.c || (al > 0x99)) {
    r.al += negate ? -0x06 : 0x60;
    r.f.c = 1;
  }
  r.f.s = r.al & 0x80;
  r.f.z = r.al == 0;
  r.f.p = parity(r.al);
}

//37  aaa
//3f  aas
auto V30MZ::opAsciiAdjust(bool negate) {
  wait(8);
  if(r.f.h || ((r.al & 0x0f) > 0x09)) {
    r.al += negate ? -0x06 : 0x06;
    r.ah += negate ? -0x01 : 0x01;
    r.f.h = 1;
    r.f.c = 1;
  } else {
    r.f.h = 0;
    r.f.c = 0;
  }
  r.al &= 0x0f;
}

auto V30MZ::opXorMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, alXor(size, getMem(size, modRM), getReg(size, modRM)));
}

auto V30MZ::opXorRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, alXor(size, getReg(size, modRM), getMem(size, modRM)));
}

auto V30MZ::opXorAccImm(Size size) {
  setAcc(size, alXor(size, getAcc(size), fetch(size)));
}

auto V30MZ::opCmpMemReg(Size size) {
  auto modRM = fetch();
  alSub(size, getMem(size, modRM), getReg(size, modRM));
}

auto V30MZ::opCmpRegMem(Size size) {
  auto modRM = fetch();
  alSub(size, getReg(size, modRM), getMem(size, modRM));
}

auto V30MZ::opCmpAccImm(Size size) {
  alSub(size, getAcc(size), fetch(size));
}

//40  inc ax
//41  inc cx
//42  inc dx
//43  inc bx
//44  inc sp
//45  inc bp
//46  inc si
//47  inc di
auto V30MZ::opIncReg(uint16& reg) {
  reg++;
}

//48  dec ax
//49  dec cx
//4a  dec dx
//4b  dec bx
//4c  dec sp
//4d  dec bp
//4e  dec si
//4f  dec di
auto V30MZ::opDecReg(uint16& reg) {
  reg--;
}

auto V30MZ::opPushReg(uint16& reg) {
  push(reg);
}

auto V30MZ::opPopReg(uint16& reg) {
  reg = pop();
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

//62  bound reg,mem,mem
auto V30MZ::opBound() {
  wait(12);
  auto modRM = fetch();
  auto bound = getMem(Long, modRM);
  auto reg = getReg(Word, modRM);
  uint16 lo = bound >>  0;
  uint16 hi = bound >> 16;
  if(reg < lo || reg > hi) {
    //todo
  }
}

//68  push imm16
//6a  push imm8s
auto V30MZ::opPushImm(Size size) {
  push(size == Word ? fetch(Word) : (int8)fetch(Byte));
}

auto V30MZ::opMultiplySignedRegMemImm(Size size) {
  wait(2);
  auto modRM = fetch();
  auto mem = getMem(size, modRM);
  auto imm = size == Word ? fetch(Word) : (int8)fetch(Byte);
  setReg(size, modRM, alMuli(size, mem, imm));
}

//9e  sahf
auto V30MZ::opStoreFlagsAcc() {
  wait(3);
  r.f = (r.f & 0xff00) | r.ah;
}

//9f  lahf
auto V30MZ::opLoadAccFlags() {
  wait(1);
  r.ah = (r.f & 0x00ff);
}

auto V30MZ::opJumpIf(bool condition) {
  auto offset = (int8)fetch();
  if(condition) r.ip += offset;
}

//80  grp1 memb,imm8
//81  grp1 memw,imm16
//82  grp1 memb,imm8s
//83  grp1 memw,imm8s
auto V30MZ::opGroup1MemImm(Size size, bool sign) {
  auto modRM = fetch();
  auto imm = sign ? (int8)fetch() : size == Byte ? fetch() : fetch(Word);
  auto mem = getMem(size, modRM);
  switch((uint3)(modRM >> 3)) {
  case 0: setMem(size, modRM, alAdd(size, mem, imm)); break;
  case 1: setMem(size, modRM, alOr (size, mem, imm)); break;
  case 2: setMem(size, modRM, alAdc(size, mem, imm)); break;
  case 3: setMem(size, modRM, alSbb(size, mem, imm)); break;
  case 4: setMem(size, modRM, alAnd(size, mem, imm)); break;
  case 5: setMem(size, modRM, alSub(size, mem, imm)); break;
  case 6: setMem(size, modRM, alXor(size, mem, imm)); break;
  case 7:                     alSub(size, mem, imm);  break;
  }
}

auto V30MZ::opTestMemReg(Size size) {
  auto modRM = fetch();
  auto mem = getMem(size, modRM);
  auto reg = getReg(size, modRM);
  alAnd(size, mem, reg);
}

auto V30MZ::opExchangeMemReg(Size size) {
  wait(2);
  auto modRM = fetch();
  auto mem = getMem(size, modRM);
  auto reg = getReg(size, modRM);
  setMem(size, modRM, reg);
  setReg(size, modRM, mem);
}

auto V30MZ::opMoveMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, getReg(size, modRM));
}

auto V30MZ::opMoveRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, getMem(size, modRM));
}

auto V30MZ::opLoadEffectiveAddressRegMem() {
  auto modRM = fetch();
  setReg(Word, modRM, getMemAddress(modRM));
}

auto V30MZ::opMoveMemSeg() {
  auto modRM = fetch();
  setMem(Word, modRM, getSeg(modRM));
}

auto V30MZ::opMoveSegMem() {
  wait(1);
  auto modRM = fetch();
  setSeg(modRM, getMem(Word, modRM));
}

auto V30MZ::opPopMem() {
  auto modRM = fetch();
  setMem(Word, modRM, pop());
}

auto V30MZ::opNop() {
}

auto V30MZ::opExchange(uint16& x, uint16& y) {
  wait(2);
  uint16 z = x;
  x = y;
  y = z;
}

//98  cbw
auto V30MZ::opSignExtendByte() {
  setAcc(Word, (int8)getAcc(Byte));
}

//99  cwd
auto V30MZ::opSignExtendWord() {
  setAcc(Long, (int16)getAcc(Word));
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

auto V30MZ::opMoveAccMem(Size size) {
  setAcc(size, read(size, r.ds, fetch(Word)));
}

auto V30MZ::opMoveMemAcc(Size size) {
  write(size, r.ds, fetch(Word), getAcc(size));
}

auto V30MZ::opInString(Size size) {
  wait(5);
  auto data = in(size, r.dx);
  write(size, r.es, r.di, data);
  r.di += r.f.d ? -size : size;

  if(prefix.repeat && --r.cx) {
    prefix.hold = true;
    r.ip--;
  }
}

auto V30MZ::opOutString(Size size) {
  wait(6);
  auto data = read(size, segment(r.ds), r.si);
  out(size, r.dx, data);
  r.si += r.f.d ? -size : size;

  if(prefix.repeat && --r.cx) {
    prefix.hold = true;
    r.ip--;
  }
}

auto V30MZ::opMoveString(Size size) {
  wait(4);
  auto data = read(size, segment(r.ds), r.si);
  write(size, r.es, r.di, data);
  r.si += r.f.d ? -size : size;
  r.di += r.f.d ? -size : size;

  if(prefix.repeat && --r.cx) {
    prefix.hold = true;
    r.ip--;
  }
}

auto V30MZ::opCompareString(Size size) {
  wait(5);
  auto x = read(size, segment(r.ds), r.si);
  auto y = read(size, r.es, r.di);
  r.si += r.f.d ? -size : size;
  r.di += r.f.d ? -size : size;
  alSub(size, x, y);

  if(prefix.repeat && prefix.repeat() == r.f.z && --r.cx) {
    prefix.hold = true;
    r.ip--;
  }
}

auto V30MZ::opTestAcc(Size size) {
  alAnd(size, getAcc(size), fetch(size));
}

auto V30MZ::opStoreString(Size size) {
  wait(2);
  write(size, r.es, r.di, getAcc(size));
  r.di += r.f.d ? -size : size;

  if(prefix.repeat && --r.cx) {
    prefix.hold = true;
    r.ip--;
  }
}

//ac  lodsb
//ad  lodsw
auto V30MZ::opLoadString(Size size) {
  wait(2);
  setAcc(size, read(size, segment(r.ds), r.si));

  if(prefix.repeat && --r.cx) {
    prefix.hold = true;
    r.ip--;
  }
}

//ae  scasb
//af  scasw
auto V30MZ::opSubtractCompareString(Size size) {
  wait(3);
  auto x = getAcc(size);
  auto y = read(size, r.es, r.di);
  alSub(size, x, y);

  if(prefix.repeat && prefix.repeat() == r.f.z && --r.cx) {
    prefix.hold = true;
    r.ip--;
  }
}

auto V30MZ::opMoveRegImm(uint8& reg) {
  reg = fetch(Byte);
}

auto V30MZ::opMoveRegImm(uint16& reg) {
  reg = fetch(Word);
}

auto V30MZ::opReturnImm() {
  wait(5);
  auto offset = fetch(Word);
  r.ip = pop();
  r.sp += offset;
}

auto V30MZ::opReturn() {
  wait(5);
  r.ip = pop();
}

auto V30MZ::opMoveMemImm(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, fetch(size));
}

auto V30MZ::opReturnFarImm() {
  wait(8);
  auto offset = fetch(Word);
  r.ip = pop();
  r.cs = pop();
  r.sp += offset;
}

auto V30MZ::opReturnFar() {
  wait(7);
  r.ip = pop();
  r.cs = pop();
}

auto V30MZ::opReturnInt() {
  wait(9);
  r.ip = pop();
  r.cs = pop();
  r.f = pop();
}

auto V30MZ::opInto() {
  wait(5);
  //todo
}

auto V30MZ::opLeave() {
  wait(1);
  r.sp = r.bp;
  r.bp = pop();
}

auto V30MZ::opGroup2MemImm(Size size, maybe<uint8> imm) {
  auto modRM = fetch();
  if(!imm) {
    wait(2);
    imm = fetch();
  }
  auto mem = getMem(size, modRM);
  switch((uint3)(modRM >> 3)) {
  case 0: setMem(size, modRM, alRol(size, mem, *imm)); break;
  case 1: setMem(size, modRM, alRor(size, mem, *imm)); break;
  case 2: setMem(size, modRM, alRcl(size, mem, *imm)); break;
  case 3: setMem(size, modRM, alRcr(size, mem, *imm)); break;
  case 4: setMem(size, modRM, alShl(size, mem, *imm)); break;
  case 5: setMem(size, modRM, alShr(size, mem, *imm)); break;
  case 6: setMem(size, modRM, alSal(size, mem, *imm)); break;
  case 7: setMem(size, modRM, alSar(size, mem, *imm)); break;
  }
}

auto V30MZ::opGroup3MemImm(Size size) {
  auto modRM = fetch();
  auto mem = getMem(size, modRM);
  switch((uint3)(modRM >> 3)) {
  case 0: alAnd(size, mem, fetch(size)); break;
  case 1: break;
  case 2: wait(2); setMem(size, modRM, alNot(size, mem)); break;
  case 3: wait(2); setMem(size, modRM, alNeg(size, mem)); break;
  case 4: wait(2); setAcc(size * 2, alMul(size, getAcc(size), mem)); break;
  case 5: wait(2); setAcc(size * 2, alMuli(size, getAcc(size), mem)); break; break;
  case 6: wait(size == Byte ? 15 : 23); setAcc(size * 2, alDiv(size, getAcc(size * 2), mem)); break;
  case 7: wait(size == Byte ? 17 : 24); setAcc(size * 2, alDivi(size, getAcc(size * 2), mem)); break;
  }
}

auto V30MZ::opGroup4MemImm(Size size) {
  auto modRM = fetch();
  auto mem = getMem(size, modRM);
  switch((uint3)(modRM >> 3)) {
  case 0: wait(2); setMem(size, modRM, alInc(size, mem)); break;
  case 1: wait(2); setMem(size, modRM, alDec(size, mem)); break;
  case 2: break;
  case 3: break;
  case 4: break;
  case 5: break;
  case 6: break;
  case 7: break;
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

auto V30MZ::opLoop() {
  wait(1);
  auto offset = (int8)fetch();
  if(--r.cx) {
    wait(3);
    r.ip += offset;
  }
}

auto V30MZ::opIn(Size size) {
  wait(5);
  auto port = fetch();
  r.al = in(port++);
  if(size != Word) return;
  r.ah = in(port++);
}

auto V30MZ::opOut(Size size) {
  wait(5);
  auto port = fetch();
  out(port++, r.al);
  if(size != Word) return;
  out(port++, r.ah);
}

auto V30MZ::opCallNear() {
  wait(4);
  auto offset = (int16)fetch(Word);
  push(r.ip);
  r.ip += offset;
}

auto V30MZ::opJumpFar() {
  wait(6);
  auto ip = fetch(Word);
  auto cs = fetch(Word);
  r.cs = cs;
  r.ip = ip;
}

auto V30MZ::opJumpShort() {
  wait(3);
  auto offset = (int8)fetch();
  r.ip += offset;
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

//f0  lock
auto V30MZ::opLock() {
  prefix.hold = true;
}

//f2  repnz
//f3  repz
auto V30MZ::opRepeat(bool flag) {
  wait(4);
  if(r.cx == 0) return;
  prefix.hold  = true;
  prefix.repeat = flag;
}

//f4  halt
auto V30MZ::opHalt() {
  wait(8);
  //todo
}

//f5  cmc
auto V30MZ::opComplementCarry() {
  wait(3);
  r.f.c = !r.f.c;
}

auto V30MZ::opClearFlag(bool& flag) {
  wait(3);
  flag = false;
}

auto V30MZ::opSetFlag(bool& flag) {
  wait(3);
  flag = true;
}
