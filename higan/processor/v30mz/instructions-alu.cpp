auto V30MZ::instructionAddMemReg(Size size) -> void {
  modRM();
  setMem(size, ADD(size, getMem(size), getReg(size)));
}

auto V30MZ::instructionAddRegMem(Size size) -> void {
  modRM();
  setReg(size, ADD(size, getReg(size), getMem(size)));
}

auto V30MZ::instructionAddAccImm(Size size) -> void {
  setAcc(size, ADD(size, getAcc(size), fetch(size)));
}

auto V30MZ::instructionOrMemReg(Size size) -> void {
  modRM();
  setMem(size, OR(size, getMem(size), getReg(size)));
}

auto V30MZ::instructionOrRegMem(Size size) -> void {
  modRM();
  setReg(size, OR(size, getReg(size), getMem(size)));
}

auto V30MZ::instructionOrAccImm(Size size) -> void {
  setAcc(size, OR(size, getAcc(size), fetch(size)));
}

auto V30MZ::instructionAdcMemReg(Size size) -> void {
  modRM();
  setMem(size, ADC(size, getMem(size), getReg(size)));
}

auto V30MZ::instructionAdcRegMem(Size size) -> void {
  modRM();
  setReg(size, ADC(size, getReg(size), getMem(size)));
}

auto V30MZ::instructionAdcAccImm(Size size) -> void {
  setAcc(size, ADC(size, getAcc(size), fetch(size)));
}

auto V30MZ::instructionSbbMemReg(Size size) -> void {
  modRM();
  setMem(size, SBB(size, getMem(size), getReg(size)));
}

auto V30MZ::instructionSbbRegMem(Size size) -> void {
  modRM();
  setReg(size, SBB(size, getReg(size), getMem(size)));
}

auto V30MZ::instructionSbbAccImm(Size size) -> void {
  setAcc(size, SBB(size, getAcc(size), fetch(size)));
}

auto V30MZ::instructionAndMemReg(Size size) -> void {
  modRM();
  setMem(size, AND(size, getMem(size), getReg(size)));
}

auto V30MZ::instructionAndRegMem(Size size) -> void {
  modRM();
  setReg(size, AND(size, getReg(size), getMem(size)));
}

auto V30MZ::instructionAndAccImm(Size size) -> void {
  setAcc(size, AND(size, getAcc(size), fetch(size)));
}

auto V30MZ::instructionSubMemReg(Size size) -> void {
  modRM();
  setMem(size, SUB(size, getMem(size), getReg(size)));
}

auto V30MZ::instructionSubRegMem(Size size) -> void {
  modRM();
  setReg(size, SUB(size, getReg(size), getMem(size)));
}

auto V30MZ::instructionSubAccImm(Size size) -> void {
  setAcc(size, SUB(size, getAcc(size), fetch(size)));
}

auto V30MZ::instructionXorMemReg(Size size) -> void {
  modRM();
  setMem(size, XOR(size, getMem(size), getReg(size)));
}

auto V30MZ::instructionXorRegMem(Size size) -> void {
  modRM();
  setReg(size, XOR(size, getReg(size), getMem(size)));
}

auto V30MZ::instructionXorAccImm(Size size) -> void {
  setAcc(size, XOR(size, getAcc(size), fetch(size)));
}

auto V30MZ::instructionCmpMemReg(Size size) -> void {
  modRM();
  SUB(size, getMem(size), getReg(size));
}

auto V30MZ::instructionCmpRegMem(Size size) -> void {
  modRM();
  SUB(size, getReg(size), getMem(size));
}

auto V30MZ::instructionCmpAccImm(Size size) -> void {
  SUB(size, getAcc(size), fetch(size));
}

auto V30MZ::instructionTestAcc(Size size) -> void {
  AND(size, getAcc(size), fetch(size));
}

auto V30MZ::instructionTestMemReg(Size size) -> void {
  modRM();
  AND(size, getMem(size), getReg(size));
}

auto V30MZ::instructionMultiplySignedRegMemImm(Size size) -> void {
  wait(2);
  modRM();
  setReg(Word, MULI(Word, getMem(Word), size == Word ? (int16_t)fetch(Word) : (int8_t)fetch(Byte)));
}

auto V30MZ::instructionIncReg(uint16_t& reg) -> void {
  reg = INC(Word, reg);
}

auto V30MZ::instructionDecReg(uint16_t& reg) -> void {
  reg = DEC(Word, reg);
}

auto V30MZ::instructionSignExtendByte() -> void {
  setAcc(Word, (int8)getAcc(Byte));
}

auto V30MZ::instructionSignExtendWord() -> void {
  setAcc(Long, (int16)getAcc(Word));
}
