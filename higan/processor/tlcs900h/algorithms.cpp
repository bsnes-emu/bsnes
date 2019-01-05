template<> auto TLCS900H::algorithmAdd<Byte>(uint8 target, uint8 source, uint1 carry) -> uint8 {
  uint64 result = target + source + carry;
  CF = result.bit(8);
  NF = 0;
  VF = uint8(~(target ^ source) & (target ^ result)).bit(7);
  HF = uint8(target ^ source ^ result).bit(4);
  ZF = uint8(result) == 0;
  SF = result.bit(7);
  return result;
}

template<> auto TLCS900H::algorithmAdd<Word>(uint16 target, uint16 source, uint1 carry) -> uint16 {
  uint64 result = target + source + carry;
  CF = result.bit(16);
  NF = 0;
  VF = uint16(~(target ^ source) & (target ^ result)).bit(15);
  HF = uint16(target ^ source ^ result).bit(4);
  ZF = uint16(result) == 0;
  SF = result.bit(15);
  return result;
}

template<> auto TLCS900H::algorithmAdd<Long>(uint32 target, uint32 source, uint1 carry) -> uint32 {
  uint64 result = target + source + carry;
  CF = result.bit(32);
  NF = 0;
  VF = uint32(~(target ^ source) & (target ^ result)).bit(31);
  HF = undefined;
  ZF = uint32(result) == 0;
  SF = result.bit(31);
  return result;
}
