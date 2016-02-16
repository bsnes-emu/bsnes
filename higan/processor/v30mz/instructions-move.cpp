auto V30MZ::opMoveMemReg(Size size) {
  modRM();
  setMem(size, getReg(size));
}

auto V30MZ::opMoveRegMem(Size size) {
  modRM();
  setReg(size, getMem(size));
}

//8c  mov memw,seg
auto V30MZ::opMoveMemSeg() {
  modRM();
  setMem(Word, getSeg());
  state.poll = false;
}

//8e  mov seg,memw
auto V30MZ::opMoveSegMem() {
  wait(1);
  modRM();
  setSeg(getMem(Word));
  if((modrm.reg & 3) == 3) state.poll = false;
}

auto V30MZ::opMoveAccMem(Size size) {
  setAcc(size, read(size, segment(r.ds), fetch(Word)));
}

auto V30MZ::opMoveMemAcc(Size size) {
  write(size, segment(r.ds), fetch(Word), getAcc(size));
}

auto V30MZ::opMoveRegImm(uint8_t& reg) {
  reg = fetch(Byte);
}

auto V30MZ::opMoveRegImm(uint16_t& reg) {
  reg = fetch(Word);
}

auto V30MZ::opMoveMemImm(Size size) {
  modRM();
  setMem(size, fetch(size));
}

auto V30MZ::opExchange(uint16_t& x, uint16_t& y) {
  wait(2);
  uint16 z = x;
  x = y;
  y = z;
}

auto V30MZ::opExchangeMemReg(Size size) {
  wait(2);
  modRM();
  auto mem = getMem(size);
  auto reg = getReg(size);
  setMem(size, reg);
  setReg(size, mem);
}

auto V30MZ::opLoadEffectiveAddressRegMem() {
  modRM();
  setReg(Word, modrm.address);
}

auto V30MZ::opLoadSegmentMem(uint16_t& reg) {
  wait(5);
  modRM();
  reg = getMem(Word);
}
