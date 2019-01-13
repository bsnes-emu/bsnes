#pragma once

#include <nall/primitives.hpp>

namespace nall {

template<typename Type, uint Bit> struct BooleanBitField {
  enum : uint { bit = Bit };
  enum : uint { mask = 1ull << bit };
  using type = Type;
  using utype = typename std::make_unsigned<type>::type;
  static_assert(bit < sizeof(type) * 8, "");

  inline BooleanBitField() = default;
  inline BooleanBitField(const BooleanBitField& value) { set(value.get()); }
  template<typename T> inline BooleanBitField(const bool value) { set(value); }

  inline operator bool() const { return get(); }

  inline auto& operator=(const BooleanBitField& value) { return set(value.get()); }
  inline auto& operator=(const bool value) { return set(value); }

  inline auto& operator&=(const bool value) { return set(get() & value); }
  inline auto& operator|=(const bool value) { return set(get() | value); }
  inline auto& operator^=(const bool value) { return set(get() ^ value); }

  inline auto raise() { return get() == 0 ? set(1), true : false; }
  inline auto lower() { return get() == 1 ? set(0), true : false; }
  inline auto& invert() { return set(get() ^ 1); }

private:
  utype data;

  inline auto get() const -> bool {
    return data & mask;
  }

  inline auto set(bool value) -> BooleanBitField& {
    return data = (data & ~mask) | (value << bit), *this;
  }
};

template<typename Type, uint Lo, uint Hi> struct NaturalBitField {
  enum : uint { lo = Lo <= Hi ? Lo : Hi };
  enum : uint { hi = Hi >= Lo ? Hi : Lo };
  enum : uint { bits = hi - lo + 1 };
  enum : uint { mask = (~0ull >> (64 - bits)) << lo };
  using type = Type;
  using utype = typename std::make_unsigned<type>::type;
  static_assert(hi < sizeof(type) * 8, "");

  inline NaturalBitField() = default;
  inline NaturalBitField(const NaturalBitField& value) { set(value.data); }
  template<typename T> inline NaturalBitField(const T& value) { set(value << lo); }

  inline explicit operator bool() const { return data & mask; }
  inline operator utype() const { return get(); }

  inline auto& operator=(const NaturalBitField& value) { return set(value.data); }
  template<typename T> inline auto& operator=(T value) { return set(value << lo); }

  inline auto operator++(int) { utype value = get(); set(data + (1 << lo)); return value; }
  inline auto operator--(int) { utype value = get(); set(data - (1 << lo)); return value; }

  inline auto& operator++() { return set(data + (1 << lo)); }
  inline auto& operator--() { return set(data - (1 << lo)); }

  inline auto& operator &=(const utype value) { return set(data  & (value << lo)); }
  inline auto& operator |=(const utype value) { return set(data  | (value << lo)); }
  inline auto& operator ^=(const utype value) { return set(data  ^ (value << lo)); }
  inline auto& operator<<=(const utype value) { return set((data & mask) << value); }
  inline auto& operator>>=(const utype value) { return set((data & mask) >> value); }
  inline auto& operator +=(const utype value) { return set(data  + (value << lo)); }
  inline auto& operator -=(const utype value) { return set(data  - (value << lo)); }
  inline auto& operator *=(const utype value) { return set((get() * value) << lo); }
  inline auto& operator /=(const utype value) { return set((get() / value) << lo); }
  inline auto& operator %=(const utype value) { return set((get() % value) << lo); }

private:
  utype data;

  inline auto get() const -> utype {
    return (data & mask) >> lo;
  }

  inline auto set(utype value) -> NaturalBitField& {
    return data = (data & ~mask) | (value & mask), *this;
  }
};

template<typename Type, uint Lo, uint Hi> struct IntegerBitField {
  enum : uint { lo = Lo <= Hi ? Lo : Hi };
  enum : uint { hi = Hi >= Lo ? Hi : Lo };
  enum : uint { bits = hi - lo + 1 };
  enum : uint { mask = (~0ull >> (64 - bits)) << lo };
  using type = Type;
  using stype = typename std::make_signed<type>::type;
  using utype = typename std::make_unsigned<type>::type;
  static_assert(hi < sizeof(type) * 8, "");

  inline IntegerBitField() = default;
  inline IntegerBitField(const IntegerBitField& value) { set(value.get()); }
  template<typename T> inline IntegerBitField(const T& value) { set(value); }

  inline explicit operator bool() const { return data & mask; }
  inline operator stype() const { return get(); }

  inline auto& operator=(const IntegerBitField& value) { return set(value.get()); }
  template<typename T> inline auto& operator=(const T& value) { return set(value); }

  inline auto operator++(int) { stype value = get(); set(value + 1); return value; }
  inline auto operator--(int) { stype value = get(); set(value - 1); return value; }

  inline auto& operator++() { return set(get() + 1); }
  inline auto& operator--() { return set(get() - 1); }

  inline auto& operator &=(const stype value) { return set(get()  & value); }
  inline auto& operator |=(const stype value) { return set(get()  | value); }
  inline auto& operator ^=(const stype value) { return set(get()  ^ value); }
  inline auto& operator<<=(const stype value) { return set(get() << value); }
  inline auto& operator>>=(const stype value) { return set(get() >> value); }
  inline auto& operator +=(const stype value) { return set(get()  + value); }
  inline auto& operator -=(const stype value) { return set(get()  - value); }
  inline auto& operator *=(const stype value) { return set(get()  * value); }
  inline auto& operator /=(const stype value) { return set(get()  / value); }
  inline auto& operator %=(const stype value) { return set(get()  % value); }

private:
  utype data;

  inline auto get() const -> stype {
    enum : utype { b = 1ull << (bits - 1) };
    enum : utype { m = b * 2 - 1 };
    return ((((data & mask) >> lo) & m) ^ b) - b;
  }

  inline auto set(utype value) -> IntegerBitField& {
    return data = (data & ~mask) | ((value << lo) & mask), *this;
  }
};

}
