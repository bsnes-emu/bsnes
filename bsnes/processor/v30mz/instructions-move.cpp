auto V30MZ::instructionMoveMemReg(Size size) -> void {
  modRM();
  setMem(size, getReg(size));
}

auto V30MZ::instructionMoveRegMem(Size size) -> void {
  modRM();
  setReg(size, getMem(size));
}

auto V30MZ::instructionMoveMemSeg() -> void {
  modRM();
  setMem(Word, getSeg());
  state.poll = false;
}

auto V30MZ::instructionMoveSegMem() -> void {
  wait(1);
  modRM();
  setSeg(getMem(Word));
  if((modrm.reg & 3) == 3) state.poll = false;
}

auto V30MZ::instructionMoveAccMem(Size size) -> void {
  setAcc(size, read(size, segment(r.ds), fetch(Word)));
}

auto V30MZ::instructionMoveMemAcc(Size size) -> void {
  write(size, segment(r.ds), fetch(Word), getAcc(size));
}

auto V30MZ::instructionMoveRegImm(uint8_t& reg) -> void {
  reg = fetch(Byte);
}

auto V30MZ::instructionMoveRegImm(uint16_t& reg) -> void {
  reg = fetch(Word);
}

auto V30MZ::instructionMoveMemImm(Size size) -> void {
  modRM();
  setMem(size, fetch(size));
}

auto V30MZ::instructionExchange(uint16_t& x, uint16_t& y) -> void {
  wait(2);
  uint16 z = x;
  x = y;
  y = z;
}

auto V30MZ::instructionExchangeMemReg(Size size) -> void {
  wait(2);
  modRM();
  auto mem = getMem(size);
  auto reg = getReg(size);
  setMem(size, reg);
  setReg(size, mem);
}

auto V30MZ::instructionLoadEffectiveAddressRegMem() -> void {
  modRM();
  setReg(Word, modrm.address);
}

auto V30MZ::instructionLoadSegmentMem(uint16_t& segment) -> void {
  wait(5);
  modRM();
  setReg(Word, getMem(Word));
  segment = getMem(Word, 2);
}
