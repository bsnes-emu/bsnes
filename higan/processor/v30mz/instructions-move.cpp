auto V30MZ::opMoveMemReg(Size size) {
  modRM();
  setMem(size, getReg(size));
}

auto V30MZ::opMoveRegMem(Size size) {
  modRM();
  setReg(size, getMem(size));
}

auto V30MZ::opMoveMemSeg() {
  modRM();
  setMem(Word, getSeg());
}

auto V30MZ::opMoveSegMem() {
  wait(1);
  modRM();
  setSeg(getMem(Word));
}

auto V30MZ::opMoveAccMem(Size size) {
  setAcc(size, read(size, r.ds, fetch(Word)));
}

auto V30MZ::opMoveMemAcc(Size size) {
  write(size, r.ds, fetch(Word), getAcc(size));
}

auto V30MZ::opMoveRegImm(uint8& reg) {
  reg = fetch(Byte);
}

auto V30MZ::opMoveRegImm(uint16& reg) {
  reg = fetch(Word);
}

auto V30MZ::opMoveMemImm(Size size) {
  modRM();
  setMem(size, fetch(size));
}

auto V30MZ::opExchange(uint16& x, uint16& y) {
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

auto V30MZ::opLoadSegmentMem(uint16& reg) {
  wait(5);
  modRM();
  reg = getMem(Word);
}
