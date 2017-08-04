auto LR35902::instructionADC_Direct_Data(uint8& target) -> void {
  target = ADC(target, operand());
}

auto LR35902::instructionADC_Direct_Direct(uint8& target, uint8& source) -> void {
  target = ADC(target, source);
}

auto LR35902::instructionADC_Direct_Indirect(uint8& target, uint16& source) -> void {
  target = ADC(target, read(source));
}

auto LR35902::instructionADD_Direct_Data(uint8& target) -> void {
  target = ADD(target, operand());
}

auto LR35902::instructionADD_Direct_Direct(uint8& target, uint8& source) -> void {
  target = ADD(target, source);
}

auto LR35902::instructionADD_Direct_Direct(uint16& target, uint16& source) -> void {
  idle();
  uint32 x = target + source;
  uint32 y = (uint12)target + (uint12)source;
  target = x;
  CF = x > 0xffff;
  HF = y > 0x0fff;
  NF = 0;
}

auto LR35902::instructionADD_Direct_Indirect(uint8& target, uint16& source) -> void {
  target = ADD(target, read(source));
}

auto LR35902::instructionAND_Direct_Data(uint8& target) -> void {
  target = AND(target, operand());
}

auto LR35902::instructionAND_Direct_Direct(uint8& target, uint8& source) -> void {
  target = AND(target, source);
}

auto LR35902::instructionAND_Direct_Indirect(uint8& target, uint16& source) -> void {
  target = AND(target, read(source));
}

auto LR35902::instructionCCF() -> void {
  CF = !CF;
  HF = NF = 0;
}

auto LR35902::instructionCP_Direct_Data(uint8& target) -> void {
  CP(target, operand());
}

auto LR35902::instructionCP_Direct_Direct(uint8& target, uint8& source) -> void {
  CP(target, source);
}

auto LR35902::instructionCP_Direct_Indirect(uint8& target, uint16& source) -> void {
  CP(target, read(source));
}

auto LR35902::instructionCPL() -> void {
  A = ~A;
  HF = NF = 1;
}

auto LR35902::instructionDAA() -> void {
  uint16 a = A;
  if(!NF) {
    if(HF || (uint4)a > 0x09) a += 0x06;
    if(CF || (uint8)a > 0x9f) a += 0x60;
  } else {
    if(HF) {
      a -= 0x06;
      if(!CF) a &= 0xff;
    }
    if(CF) a -= 0x60;
  }
  A = a;
  CF |= a.bit(8);
  HF = 0;
  ZF = A == 0;
}

auto LR35902::instructionDEC_Direct(uint8& target) -> void {
  target--;
  HF = (uint4)target == 15;
  NF = 0;
  ZF = target == 0;
}

auto LR35902::instructionDEC_Direct(uint16& target) -> void {
  idle();
  target--;
}

auto LR35902::instructionDEC_Indirect(uint16& target) -> void {
  auto data = read(target);
  write(target, ++data);
  HF = (uint4)data == 15;
  NF = 0;
  ZF = data == 0;
}

auto LR35902::instructionDI() -> void {
  r.ime = 0;
}

auto LR35902::instructionEI() -> void {
  r.ei = 1;
}

auto LR35902::instructionHALT() -> void {
  r.halt = 1;
  while(r.halt) idle();
}

auto LR35902::instructionINC_Direct(uint8& target) -> void {
  target++;
  HF = (uint4)target == 0;
  NF = 0;
  ZF = target == 0;
}

auto LR35902::instructionINC_Direct(uint16& target) -> void {
  idle();
  target++;
}

auto LR35902::instructionINC_Indirect(uint16& target) -> void {
  auto data = read(target);
  write(target, ++data);
  HF = (uint4)data == 0;
  NF = 0;
  ZF = data == 0;
}

auto LR35902::instructionJR_Condition_Relative(bool take) -> void {
  auto data = operand();
  if(!take) return;
  idle();
  PC += (int8)data;
}

auto LR35902::instructionLD_Address_Direct(uint16& data) -> void {
  store(operands(), data);
}

auto LR35902::instructionLD_Direct_Data(uint8& target) -> void {
  target = operand();
}

auto LR35902::instructionLD_Direct_Data(uint16& target) -> void {
  target = operands();
}

auto LR35902::instructionLD_Direct_Direct(uint8& target, uint8& source) -> void {
  target = source;
}

auto LR35902::instructionLD_Direct_Indirect(uint8& target, uint8& source) -> void {
  target = read(0xff00 | source);
}

auto LR35902::instructionLD_Direct_Indirect(uint8& target, uint16& source) -> void {
  target = read(source);
}

auto LR35902::instructionLD_Direct_IndirectDecrement(uint8& target, uint16& source) -> void {
  target = read(source--);
}

auto LR35902::instructionLD_Direct_IndirectIncrement(uint8& target, uint16& source) -> void {
  target = read(source++);
}

auto LR35902::instructionLD_Indirect_Data(uint16& target) -> void {
  write(target, operand());
}

auto LR35902::instructionLD_Indirect_Direct(uint8& target, uint8& source) -> void {
  write(0xff00 | target, source);
}

auto LR35902::instructionLD_Indirect_Direct(uint16& target, uint8& source) -> void {
  write(target, source);
}

auto LR35902::instructionLD_IndirectDecrement_Direct(uint16& target, uint8& source) -> void {
  write(target--, source);
}

auto LR35902::instructionLD_IndirectIncrement_Direct(uint16& target, uint8& source) -> void {
  write(target++, source);
}

auto LR35902::instructionNOP() -> void {
}

auto LR35902::instructionOR_Direct_Data(uint8& target) -> void {
  target = OR(target, operand());
}

auto LR35902::instructionOR_Direct_Direct(uint8& target, uint8& source) -> void {
  target = OR(target, source);
}

auto LR35902::instructionOR_Direct_Indirect(uint8& target, uint16& source) -> void {
  target = OR(target, read(source));
}

auto LR35902::instructionRET_Condition(bool take) -> void {
  idle();
  if(!take) return;
  PC = pop();
  idle();
}

auto LR35902::instructionRL_Direct(uint8& data) -> void {
  bool carry = data.bit(7);
  data = data << 1 | CF;
  CF = carry;
  HF = NF = ZF = 0;
}

auto LR35902::instructionRLC_Direct(uint8& data) -> void {
  data = data << 1 | data >> 7;
  CF = data.bit(0);
  HF = NF = ZF = 0;
}

auto LR35902::instructionRR_Direct(uint8& data) -> void {
  bool carry = data.bit(0);
  data = data >> 1 | CF << 7;
  CF = carry;
  HF = NF = ZF = 0;
}

auto LR35902::instructionRRC_Direct(uint8& data) -> void {
  data = data >> 1 | data << 7;
  CF = data.bit(7);
  HF = NF = ZF = 0;
}

auto LR35902::instructionSBC_Direct_Data(uint8& target) -> void {
  target = SBC(target, operand());
}

auto LR35902::instructionSBC_Direct_Direct(uint8& target, uint8& source) -> void {
  target = SBC(target, source);
}

auto LR35902::instructionSBC_Direct_Indirect(uint8& target, uint16& source) -> void {
  target = SBC(target, read(source));
}

auto LR35902::instructionSCF() -> void {
  CF = 1;
  HF = NF = 0;
}

auto LR35902::instructionSTOP() -> void {
  if(stop()) return;
  r.stop = 1;
  while(r.stop) idle();
}

auto LR35902::instructionSUB_Direct_Data(uint8& target) -> void {
  target = SUB(target, operand());
}

auto LR35902::instructionSUB_Direct_Direct(uint8& target, uint8& source) -> void {
  target = SUB(target, source);
}

auto LR35902::instructionSUB_Direct_Indirect(uint8& target, uint16& source) -> void {
  target = SUB(target, read(source));
}

auto LR35902::instructionXOR_Direct_Data(uint8& target) -> void {
  target = XOR(target, operand());
}

auto LR35902::instructionXOR_Direct_Direct(uint8& target, uint8& source) -> void {
  target = XOR(target, source);
}

auto LR35902::instructionXOR_Direct_Indirect(uint8& target, uint16& source) -> void {
  target = XOR(target, read(source));
}
