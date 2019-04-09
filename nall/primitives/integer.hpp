#pragma once

namespace nall {

template<uint Precision> struct Integer {
  static_assert(Precision >= 1 && Precision <= 64);
  static inline constexpr auto bits() -> uint { return Precision; }
  using stype =
    conditional_t<bits() <=  8,  int8_t,
    conditional_t<bits() <= 16, int16_t,
    conditional_t<bits() <= 32, int32_t,
    conditional_t<bits() <= 64, int64_t,
    void>>>>;
  using utype = typename Natural<Precision>::utype;
  static inline constexpr auto mask() -> utype { return ~0ull >> 64 - bits(); }
  static inline constexpr auto sign() -> utype { return 1ull << Precision - 1; }

  inline Integer() : data(0) {}
  template<uint Bits> inline Integer(Integer<Bits> value) { data = mask(value); }
  template<typename T> inline Integer(const T& value) { data = mask(value); }
  explicit inline Integer(const char* value) { data = mask(toInteger(value)); }

  inline operator stype() const { return data; }

  inline auto operator++(int) { auto value = *this; data = mask(data + 1); return value; }
  inline auto operator--(int) { auto value = *this; data = mask(data - 1); return value; }

  inline auto& operator++() { data = mask(data + 1); return *this; }
  inline auto& operator--() { data = mask(data - 1); return *this; }

  template<typename T> inline auto& operator  =(const T& value) { data = mask(        value); return *this; }
  template<typename T> inline auto& operator *=(const T& value) { data = mask(data  * value); return *this; }
  template<typename T> inline auto& operator /=(const T& value) { data = mask(data  / value); return *this; }
  template<typename T> inline auto& operator %=(const T& value) { data = mask(data  % value); return *this; }
  template<typename T> inline auto& operator +=(const T& value) { data = mask(data  + value); return *this; }
  template<typename T> inline auto& operator -=(const T& value) { data = mask(data  - value); return *this; }
  template<typename T> inline auto& operator<<=(const T& value) { data = mask(data << value); return *this; }
  template<typename T> inline auto& operator>>=(const T& value) { data = mask(data >> value); return *this; }
  template<typename T> inline auto& operator &=(const T& value) { data = mask(data  & value); return *this; }
  template<typename T> inline auto& operator ^=(const T& value) { data = mask(data  ^ value); return *this; }
  template<typename T> inline auto& operator |=(const T& value) { data = mask(data  | value); return *this; }

  inline auto bits(int lo, int hi) -> BitRange<Precision> { return {(utype&)data, lo, hi}; }
  inline auto bit(int index) -> BitRange<Precision> { return {(utype&)data, index, index}; }
  inline auto byte(int index) -> BitRange<Precision> { return {(utype&)data, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(int lo, int hi) const -> const BitRange<Precision> { return {(utype&)*this, lo, hi}; }
  inline auto bit(int index) const -> const BitRange<Precision> { return {(utype&)*this, index, index}; }
  inline auto byte(int index) const -> const BitRange<Precision> { return {(utype&)*this, index * 8 + 0, index * 8 + 7}; }

  inline auto slice(int index) const { return Natural<>{bit(index)}; }
  inline auto slice(int lo, int hi) const { return Natural<>{bit(lo, hi)}; }

  inline auto clamp(uint bits) -> stype {
    const int64_t b = 1ull << (bits - 1);
    const int64_t m = b - 1;
    return data > m ? m : data < -b ? -b : data;
  }

  inline auto clip(uint bits) -> stype {
    const uint64_t b = 1ull << (bits - 1);
    const uint64_t m = b * 2 - 1;
    return (data & m ^ b) - b;
  }

  inline auto serialize(serializer& s) { s(data); }
  inline auto natural() const -> Natural<Precision>;

private:
  inline auto mask(stype value) const -> stype {
    return (value & mask() ^ sign()) - sign();
  }

  stype data;
};

}
