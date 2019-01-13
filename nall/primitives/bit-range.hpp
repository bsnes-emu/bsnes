#pragma once

namespace nall {

template<int RequestedPrecision> struct BitRange {
  enum : uint { Precision = RequestedPrecision < 1 ? 1 : RequestedPrecision > 64 ? 64 : RequestedPrecision };
  static inline constexpr auto bits() -> uint { return Precision; }
  using type =
    typename conditional<bits() <=  8,  uint8_t,
    typename conditional<bits() <= 16, uint16_t,
    typename conditional<bits() <= 32, uint32_t,
    typename conditional<bits() <= 64, uint64_t,
    void>::type>::type>::type>::type;
  static inline constexpr auto mask() -> type { return ~0ull >> 64 - bits(); }

  inline BitRange(type& source, uint low, uint high)
  : source(source), lo(low < high ? low : high), hi(high > low ? high : low) {}
  inline auto& operator=(BitRange& source) { return set(source.get()); }

  inline auto operator++(int) { auto value = get(); set(value + 1); return value; }
  inline auto operator--(int) { auto value = get(); set(value - 1); return value; }

  inline auto& operator++() { return set(get() + 1); }
  inline auto& operator--() { return set(get() - 1); }

  inline operator uint64_t() const { return get(); }
  template<typename T> inline auto& operator  =(const T& value) { return set(         value); }
  template<typename T> inline auto& operator *=(const T& value) { return set(get()  * value); }
  template<typename T> inline auto& operator /=(const T& value) { return set(get()  / value); }
  template<typename T> inline auto& operator %=(const T& value) { return set(get()  % value); }
  template<typename T> inline auto& operator +=(const T& value) { return set(get()  + value); }
  template<typename T> inline auto& operator -=(const T& value) { return set(get()  - value); }
  template<typename T> inline auto& operator<<=(const T& value) { return set(get() << value); }
  template<typename T> inline auto& operator>>=(const T& value) { return set(get() >> value); }
  template<typename T> inline auto& operator &=(const T& value) { return set(get()  & value); }
  template<typename T> inline auto& operator ^=(const T& value) { return set(get()  ^ value); }
  template<typename T> inline auto& operator |=(const T& value) { return set(get()  | value); }

private:
  inline auto get() const -> uint64_t {
    const type rangeBits = hi - lo + 1;
    const type rangeMask = (1ull << rangeBits) - 1 << lo & mask();
    return (source & rangeMask) >> lo;
  }

  inline auto& set(const type& value) {
    const type rangeBits = hi - lo + 1;
    const type rangeMask = (1ull << rangeBits) - 1 << lo & mask();
    source = source & ~rangeMask | value << lo & rangeMask;
    return *this;
  }

  type& source;
  const uint lo;
  const uint hi;
};

}
