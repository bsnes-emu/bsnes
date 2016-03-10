//00  addb mem,reg
//01  addw mem,reg
auto V30MZ::opAddMemReg(Size size) {
  modRM();
  setMem(size, alAdd(size, getMem(size), getReg(size)));
}

//02  addb reg,mem
//03  addw reg,mem
auto V30MZ::opAddRegMem(Size size) {
  modRM();
  setReg(size, alAdd(size, getReg(size), getMem(size)));
}

//04  add al,#imm
//05  add ax,#imm
auto V30MZ::opAddAccImm(Size size) {
  setAcc(size, alAdd(size, getAcc(size), fetch(size)));
}

//08  orb mem,reg
//09  orb mem,reg
auto V30MZ::opOrMemReg(Size size) {
  modRM();
  setMem(size, alOr(size, getMem(size), getReg(size)));
}

//0a  orb reg,mem
//0b  orb reg,mem
auto V30MZ::opOrRegMem(Size size) {
  modRM();
  setReg(size, alOr(size, getReg(size), getMem(size)));
}

//0c  or al,#imm
//0d  or ax,#imm
auto V30MZ::opOrAccImm(Size size) {
  setAcc(size, alOr(size, getAcc(size), fetch(size)));
}

auto V30MZ::opAdcMemReg(Size size) {
  modRM();
  setMem(size, alAdc(size, getMem(size), getReg(size)));
}

auto V30MZ::opAdcRegMem(Size size) {
  modRM();
  setReg(size, alAdc(size, getReg(size), getMem(size)));
}

auto V30MZ::opAdcAccImm(Size size) {
  setAcc(size, alAdc(size, getAcc(size), fetch(size)));
}

auto V30MZ::opSbbMemReg(Size size) {
  modRM();
  setMem(size, alSbb(size, getMem(size), getReg(size)));
}

auto V30MZ::opSbbRegMem(Size size) {
  modRM();
  setReg(size, alSbb(size, getReg(size), getMem(size)));
}

auto V30MZ::opSbbAccImm(Size size) {
  setAcc(size, alSbb(size, getAcc(size), fetch(size)));
}

auto V30MZ::opAndMemReg(Size size) {
  modRM();
  setMem(size, alAnd(size, getMem(size), getReg(size)));
}

auto V30MZ::opAndRegMem(Size size) {
  modRM();
  setReg(size, alAnd(size, getReg(size), getMem(size)));
}

auto V30MZ::opAndAccImm(Size size) {
  setAcc(size, alAnd(size, getAcc(size), fetch(size)));
}

auto V30MZ::opSubMemReg(Size size) {
  modRM();
  setMem(size, alSub(size, getMem(size), getReg(size)));
}

auto V30MZ::opSubRegMem(Size size) {
  modRM();
  setReg(size, alSub(size, getReg(size), getMem(size)));
}

auto V30MZ::opSubAccImm(Size size) {
  setAcc(size, alSub(size, getAcc(size), fetch(size)));
}

auto V30MZ::opXorMemReg(Size size) {
  modRM();
  setMem(size, alXor(size, getMem(size), getReg(size)));
}

auto V30MZ::opXorRegMem(Size size) {
  modRM();
  setReg(size, alXor(size, getReg(size), getMem(size)));
}

auto V30MZ::opXorAccImm(Size size) {
  setAcc(size, alXor(size, getAcc(size), fetch(size)));
}

auto V30MZ::opCmpMemReg(Size size) {
  modRM();
  alSub(size, getMem(size), getReg(size));
}

auto V30MZ::opCmpRegMem(Size size) {
  modRM();
  alSub(size, getReg(size), getMem(size));
}

auto V30MZ::opCmpAccImm(Size size) {
  alSub(size, getAcc(size), fetch(size));
}

auto V30MZ::opTestAcc(Size size) {
  alAnd(size, getAcc(size), fetch(size));
}

auto V30MZ::opTestMemReg(Size size) {
  modRM();
  alAnd(size, getMem(size), getReg(size));
}

auto V30MZ::opMultiplySignedRegMemImm(Size size) {
  wait(2);
  modRM();
  setReg(size, alMuli(size, getMem(size), size == Word ? (int16_t)fetch(Word) : (int8_t)fetch(Byte)));
}

//40  inc ax
//41  inc cx
//42  inc dx
//43  inc bx
//44  inc sp
//45  inc bp
//46  inc si
//47  inc di
auto V30MZ::opIncReg(uint16_t& reg) {
  reg = alInc(Word, reg);
}

//48  dec ax
//49  dec cx
//4a  dec dx
//4b  dec bx
//4c  dec sp
//4d  dec bp
//4e  dec si
//4f  dec di
auto V30MZ::opDecReg(uint16_t& reg) {
  reg = alDec(Word, reg);
}

//98  cbw
auto V30MZ::opSignExtendByte() {
  setAcc(Word, (int8)getAcc(Byte));
}

//99  cwd
auto V30MZ::opSignExtendWord() {
  setAcc(Long, (int16)getAcc(Word));
}
