auto LR35902::ADD(uint8 target, uint8 source, bool carry) -> uint8 {
  uint16 x = target + source + carry;
  uint16 y = (uint4)target + (uint4)source + carry;
  CF = x > 0xff;
  HF = y > 0x0f;
  NF = 0;
  ZF = (uint8)x == 0;
  return x;
}

auto LR35902::AND(uint8 target, uint8 source) -> uint8 {
  target &= source;
  CF = 0;
  HF = 1;
  NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::BIT(uint3 index, uint8 target) -> void {
  HF = 1;
  NF = 0;
  ZF = target.bit(index) == 0;
}

auto LR35902::CP(uint8 target, uint8 source) -> void {
  uint16 x = target - source;
  uint16 y = (uint4)target - (uint4)source;
  CF = x > 0xff;
  HF = y > 0x0f;
  NF = 1;
  ZF = (uint8)x == 0;
}

auto LR35902::DEC(uint8 target) -> uint8 {
  target--;
  HF = (uint4)target == 0x0f;
  NF = 1;
  ZF = target == 0;
  return target;
}

auto LR35902::INC(uint8 target) -> uint8 {
  target++;
  HF = (uint4)target == 0x00;
  NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::OR(uint8 target, uint8 source) -> uint8 {
  target |= source;
  CF = HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::RL(uint8 target) -> uint8 {
  bool carry = target.bit(7);
  target = target << 1 | CF;
  CF = carry;
  HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::RLC(uint8 target) -> uint8 {
  target = target << 1 | target >> 7;
  CF = target.bit(0);
  HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::RR(uint8 target) -> uint8 {
  bool carry = target.bit(0);
  target = CF << 7 | target >> 1;
  CF = carry;
  HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::RRC(uint8 target) -> uint8 {
  target = target << 7 | target >> 1;
  CF = target.bit(7);
  HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::SLA(uint8 target) -> uint8 {
  bool carry = target.bit(7);
  target <<= 1;
  CF = carry;
  HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::SRA(uint8 target) -> uint8 {
  bool carry = target.bit(0);
  target = (int8)target >> 1;
  CF = carry;
  HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::SRL(uint8 target) -> uint8 {
  bool carry = target.bit(0);
  target >>= 1;
  CF = carry;
  HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::SUB(uint8 target, uint8 source, bool carry) -> uint8 {
  uint16 x = target - source - carry;
  uint16 y = (uint4)target - (uint4)source - carry;
  CF = x > 0xff;
  HF = y > 0x0f;
  NF = 1;
  ZF = (uint8)x == 0;
  return x;
}

auto LR35902::SWAP(uint8 target) -> uint8 {
  target = target << 4 | target >> 4;
  CF = HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::XOR(uint8 target, uint8 source) -> uint8 {
  target ^= source;
  CF = HF = NF = 0;
  ZF = target == 0;
  return target;
}
