#pragma once

namespace nall {

template<int Precision> struct BitRange {
  static_assert(Precision >= 1 && Precision <= 64);
  static inline constexpr auto bits() -> uint { return Precision; }
  using utype =
    typename conditional<bits() <=  8,  uint8_t,
    typename conditional<bits() <= 16, uint16_t,
    typename conditional<bits() <= 32, uint32_t,
    typename conditional<bits() <= 64, uint64_t,
    void>::type>::type>::type>::type;
  static inline constexpr auto mask() -> utype { return ~0ull >> 64 - bits(); }

  inline BitRange(utype& source, int lo, int hi) : source(source) {
    if(lo < 0) lo = Precision + lo;
    if(hi < 0) hi = Precision + hi;
    if(lo > hi) swap(lo, hi);
    this->lo = lo;
    this->hi = hi;
  }
  inline auto& operator=(BitRange& source) { return set(source.get()); }

  inline auto operator++(int) { auto value = get(); set(value + 1); return value; }
  inline auto operator--(int) { auto value = get(); set(value - 1); return value; }

  inline auto& operator++() { return set(get() + 1); }
  inline auto& operator--() { return set(get() - 1); }

  inline operator utype() const { return get(); }
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
  inline auto get() const -> utype {
    const utype rangeBits = hi - lo + 1;
    const utype rangeMask = (1ull << rangeBits) - 1 << lo & mask();
    return (source & rangeMask) >> lo;
  }

  inline auto& set(const utype& value) {
    const utype rangeBits = hi - lo + 1;
    const utype rangeMask = (1ull << rangeBits) - 1 << lo & mask();
    source = source & ~rangeMask | value << lo & rangeMask;
    return *this;
  }

  utype& source;
  uint lo;
  uint hi;
};

}
