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
  setCarry(result.bit(T::bits()));
  setNegative(0);
  setOverflow(T(~(target ^ source) & (target ^ result)).negative());
  setHalfCarry(T(target ^ source ^ result).bit(4));
  if constexpr(is_same<T, uint32>::value) setHalfCarry(Undefined);
  setZero(T(result).zero());
  setSign(result.negative());
  return result;
}

template<typename T> auto TLCS900H::algorithmAnd(T target, T source) -> T {
  T result = target & source;
  setCarry(0);
  setNegative(0);
  setParity(parity(result));
  setHalfCarry(1);
  setZero(result.zero());
  setSign(result.negative());
  return result;
}

template<typename T> auto TLCS900H::algorithmOr(T target, T source) -> T {
  T result = target | source;
  setCarry(0);
  setNegative(0);
  setParity(parity(result));
  setHalfCarry(0);
  setZero(result.zero());
  setSign(result.negative());
  return result;
}

template<typename T> auto TLCS900H::algorithmSubtract(T target, T source, uint1 carry) -> T {
  uint64 result = target - source - carry;
  setCarry(result.bit(T::bits()));
  setNegative(1);
  setOverflow(T((target ^ source) & (target ^ result)).negative());
  setHalfCarry(T(target ^ source ^ result).bit(4));
  if constexpr(is_same<T, uint32>::value) setHalfCarry(Undefined);
  setZero(T(result).zero());
  setSign(result.negative());
  return result;
}

template<typename T> auto TLCS900H::algorithmXor(T target, T source) -> T {
  T result = target ^ source;
  setCarry(0);
  setNegative(0);
  setParity(parity(result));
  setHalfCarry(0);
  setZero(result.zero());
  setSign(result.negative());
  return result;
}
