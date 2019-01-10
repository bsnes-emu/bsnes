template<> auto TLCS900H::parity< uint8>( uint8 data) const -> bool {
  data ^= data >> 4;
  data ^= data >> 2;
  data ^= data >> 1;
  return !(data & 1);
}

template<> auto TLCS900H::parity<uint16>(uint16 data) const -> bool {
  data ^= data >> 8;
  data ^= data >> 4;
  data ^= data >> 2;
  data ^= data >> 1;
  return !(data & 1);
}

template<> auto TLCS900H::parity<uint32>(uint32 data) const -> bool {
  return Undefined;
}

//

template<typename T> auto TLCS900H::algorithmAdd(T target, T source, uint1 carry) -> T {
  uint64 result = target + source + carry;
  CF = result.bit(T::bits());
  NF = 0;
  VF = T(~(target ^ source) & (target ^ result)).negative();
  HF = T(target ^ source ^ result).bit(4);
  if constexpr(is_same<T, uint32>::value) HF = Undefined;
  ZF = T(result).zero();
  SF = result.negative();
  return result;
}

template<typename T> auto TLCS900H::algorithmAnd(T target, T source) -> T {
  T result = target & source;
  CF = 0;
  NF = 0;
  PF = parity(result);
  HF = 1;
  ZF = result.zero();
  SF = result.negative();
  return result;
}

template<typename T> auto TLCS900H::algorithmOr(T target, T source) -> T {
  T result = target | source;
  CF = 0;
  NF = 0;
  PF = parity(result);
  HF = 0;
  ZF = result.zero();
  SF = result.negative();
  return result;
}

template<typename T> auto TLCS900H::algorithmSubtract(T target, T source, uint1 carry) -> T {
  uint64 result = target - source - carry;
  CF = result.bit(T::bits());
  NF = 1;
  VF = T((target ^ source) & (target ^ result)).negative();
  HF = T(target ^ source ^ result).bit(4);
  if constexpr(is_same<T, uint32>::value) HF = Undefined;
  ZF = T(result).zero();
  SF = result.negative();
  return result;
}

template<typename T> auto TLCS900H::algorithmXor(T target, T source) -> T {
  T result = target ^ source;
  CF = 0;
  NF = 0;
  PF = parity(result);
  HF = 0;
  ZF = result.zero();
  SF = result.negative();
  return result;
}
