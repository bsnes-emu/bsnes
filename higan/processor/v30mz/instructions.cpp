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
  //todo
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

auto V30MZ::opMoveMemReg(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, getReg(size, modRM));
}

auto V30MZ::opMoveRegMem(Size size) {
  auto modRM = fetch();
  setReg(size, modRM, getMem(size, modRM));
}

auto V30MZ::opMoveSegMem() {
  wait(1);
  auto modRM = fetch();
  setSeg(modRM, getMem(Word, modRM));
}

auto V30MZ::opNop() {
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

auto V30MZ::opMoveString(Size size) {
  wait(4);
  write(size, r.es, r.di, read(size, r.ds, r.si));
  r.si += r.f.d ? -size : size;
  r.di += r.f.d ? -size : size;
}

auto V30MZ::opTestAcc(Size size) {
  alAnd(size, getAcc(size), fetch(size));
}

auto V30MZ::opStoreString(Size size) {
  wait(2);
  write(size, r.es, r.di, getAcc(size));
  r.di += r.f.d ? -size : size;
}

auto V30MZ::opMoveRegImm(uint8& reg) {
  reg = fetch(Byte);
}

auto V30MZ::opMoveRegImm(uint16& reg) {
  reg = fetch(Word);
}

auto V30MZ::opReturn() {
  wait(5);
  r.ip = pop();
}

auto V30MZ::opMoveMemImm(Size size) {
  auto modRM = fetch();
  setMem(size, modRM, fetch(size));
}

auto V30MZ::opRetFar() {
  wait(7);
  auto ip = pop();
  auto cs = pop();
  r.cs = cs;
  r.ip = ip;
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

auto V30MZ::opRepeat(bool flag) {
  wait(4);
  auto opcode = fetch();
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
  case 0xaa: opStoreString(Byte); r.ip -= 2; break;
  case 0xab: opStoreString(Word); r.ip -= 2; break;
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
