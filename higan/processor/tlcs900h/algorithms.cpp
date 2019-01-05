template<> auto TLCS900H::parity(Byte data) const -> bool {
  data ^= data >> 4;
  data ^= data >> 2;
  data ^= data >> 1;
  return !(data & 1);
}

template<> auto TLCS900H::parity(Word data) const -> bool {
  data ^= data >> 8;
  data ^= data >> 4;
  data ^= data >> 2;
  data ^= data >> 1;
  return !(data & 1);
}

template<> auto TLCS900H::parity(Long data) const -> bool {
  return Undefined;
}

//

template<typename Size> auto TLCS900H::algorithmAdd(Size target, Size source, uint1 carry) -> Size {
  uint64 result = target + source + carry;
  CF = result.bit(Size::bits());
  NF = 0;
  VF = Size(~(target ^ source) & (target ^ result)).negative();
  HF = Size(target ^ source ^ result).bit(4);
  if constexpr(isLong<Size>()) HF = Undefined;
  ZF = Size(result).zero();
  SF = result.negative();
  return result;
}

template<typename Size> auto TLCS900H::algorithmAnd(Size target, Size source) -> Size {
  Size result = target & source;
  CF = 0;
  NF = 0;
  VF = parity(result);
  HF = 1;
  ZF = result.zero();
  SF = result.negative();
  return result;
}

template<typename Size> auto TLCS900H::algorithmOr(Size target, Size source) -> Size {
  Size result = target | source;
  CF = 0;
  NF = 0;
  VF = parity(result);
  HF = 0;
  ZF = result.zero();
  SF = result.negative();
  return result;
}

template<typename Size> auto TLCS900H::algorithmSubtract(Size target, Size source, uint1 carry) -> Size {
  uint64 result = target - source - carry;
  CF = result.bit(Size::bits());
  NF = 1;
  VF = Size((target ^ source) & (target ^ result)).negative();
  HF = Size(target ^ source ^ result).bit(4);
  if constexpr(isLong<Size>()) HF = Undefined;
  ZF = Size(result).zero();
  SF = result.negative();
  return result;
}

template<typename Size> auto TLCS900H::algorithmXor(Size target, Size source) -> Size {
  Size result = target ^ source;
  CF = 0;
  NF = 0;
  VF = parity(result);
  HF = 0;
  ZF = result.zero();
  SF = result.negative();
  return result;
}
