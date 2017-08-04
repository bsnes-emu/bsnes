auto LR35902::ADC(uint8 target, uint8 source) -> uint8 {
  uint16 x = target + source + CF;
  uint16 y = (uint4)target + (uint4)source + CF;
  CF = x > 0xff;
  HF = y > 0x0f;
  NF = 0;
  ZF = (uint8)x == 0;
  return x;
}

auto LR35902::ADD(uint8 target, uint8 source) -> uint8 {
  uint16 x = target + source;
  uint16 y = (uint4)target + (uint4)source;
  CF = x > 0xff;
  HF = y > 0x0f;
  NF = 0;
  ZF = (uint8)x == 0;
  return x;
}

auto LR35902::AND(uint8 target, uint8 source) -> uint8 {
  target &= source;
  CF = HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::CP(uint8 target, uint8 source) -> void {
  uint16 x = target - source;
  uint16 y = (uint4)target - (uint4)source;
  CF = x > 0xff;
  HF = y > 0x0f;
  NF = 1;
  ZF = (uint8)x == 0;
}

auto LR35902::OR(uint8 target, uint8 source) -> uint8 {
  target |= source;
  CF = HF = NF = 0;
  ZF = target == 0;
  return target;
}

auto LR35902::SBC(uint8 target, uint8 source) -> uint8 {
  uint16 x = target - source - CF;
  uint16 y = (uint4)target - (uint4)source - CF;
  CF = x > 0xff;
  HF = y > 0x0f;
  NF = 1;
  ZF = (uint8)x == 0;
  return x;
}

auto LR35902::SUB(uint8 target, uint8 source) -> uint8 {
  uint16 x = target - source;
  uint16 y = (uint4)target - (uint4)source;
  CF = x > 0xff;
  HF = y > 0x0f;
  NF = 1;
  ZF = (uint8)x == 0;
  return x;
}

auto LR35902::XOR(uint8 target, uint8 source) -> uint8 {
  target ^= source;
  CF = HF = NF = 0;
  ZF = target == 0;
  return target;
}
