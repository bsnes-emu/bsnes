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
  T result = target + source + carry;
  T carries = target ^ source ^ result;
  T overflow = (target ^ result) & (source ^ result);
  CF = T(carries ^ overflow).bit(-1);
  NF = 0;
  VF = overflow.bit(-1);
  HF = carries.bit(4);
  if constexpr(T::bits() == 32) HF = Undefined;
  ZF = result == 0;
  SF = result.bit(-1);
  return result;
}

template<typename T> auto TLCS900H::algorithmAnd(T target, T source) -> T {
  T result = target & source;
  CF = 0;
  NF = 0;
  PF = parity(result);
  HF = 1;
  ZF = result == 0;
  SF = result.bit(-1);
  return result;
}

template<typename T> auto TLCS900H::algorithmDecrement(T target, T source) -> T {
  T result = target - source;
  if constexpr(T::bits() == 8) {
    NF = 1;
    VF = T((target ^ source) & (target ^ result)).bit(-1);
    HF = T(target ^ source ^ result).bit(4);
    ZF = result == 0;
    SF = result.bit(-1);
  }
  return result;
}

template<typename T> auto TLCS900H::algorithmIncrement(T target, T source) -> T {
  T result = target + source;
  if constexpr(T::bits() == 8) {
    NF = 0;
    VF = T((target ^ result) & (source ^ result)).bit(-1);
    HF = T(target ^ source ^ result).bit(4);
    ZF = result == 0;
    SF = result.bit(-1);
  }
  return result;
}

template<typename T> auto TLCS900H::algorithmOr(T target, T source) -> T {
  T result = target | source;
  CF = 0;
  NF = 0;
  PF = parity(result);
  HF = 0;
  ZF = result == 0;
  SF = result.bit(-1);
  return result;
}

template<typename T> auto TLCS900H::algorithmRotated(T result) -> T {
  NF = 0;
  PF = parity(result);
  HF = 0;
  ZF = result == 0;
  SF = result.bit(-1);
  return result;
}

template<typename T> auto TLCS900H::algorithmShifted(T result) -> T {
  NF = 0;
  PF = parity(result);
  HF = 0;
  ZF = result == 0;
  SF = result.bit(-1);
  return result;
}

template<typename T> auto TLCS900H::algorithmSubtract(T target, T source, uint1 carry) -> T {
  T result = target - source - carry;
  T carries = target ^ source ^ result;
  T overflow = (target ^ result) & (source ^ target);
  CF = T(carries ^ overflow).bit(-1);
  NF = 1;
  VF = overflow.bit(-1);
  HF = carries.bit(4);
  if constexpr(T::bits() == 32) HF = Undefined;
  ZF = result == 0;
  SF = result.bit(-1);
  return result;
}

template<typename T> auto TLCS900H::algorithmXor(T target, T source) -> T {
  T result = target ^ source;
  CF = 0;
  NF = 0;
  PF = parity(result);
  HF = 0;
  ZF = result == 0;
  SF = result.bit(-1);
  return result;
}
