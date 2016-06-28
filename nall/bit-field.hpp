#pragma once

namespace nall {

template<typename type, uint Lo, uint Hi> struct NaturalBitField {
  enum : uint { lo = Lo <= Hi ? Lo : Hi };
  enum : uint { hi = Hi >= Lo ? Hi : Lo };
  enum : uint { bits = hi - lo + 1 };
  enum : uint { mask = (~0ull >> (64 - bits)) << lo };
  static_assert(hi < sizeof(type) * 8, "");

  inline NaturalBitField() = default;
  inline NaturalBitField(const NaturalBitField& value) { set(value.data); }
  template<typename T> inline NaturalBitField(const T& value) { set(value << lo); }

  inline explicit operator bool() const { return data & mask; }
  inline operator type() const { return get(); }

  inline auto& operator=(const NaturalBitField& value) { return set(value.data); }
  template<typename T> inline auto& operator=(const T& value) { return set(value << lo); }

  inline auto operator++(int) { type value = get(); set(data + (1 << lo)); return value; }
  inline auto operator--(int) { type value = get(); set(data - (1 << lo)); return value; }

  inline auto& operator++() { return set(data + (1 << lo)); }
  inline auto& operator--() { return set(data - (1 << lo)); }

  inline auto& operator &=(const type value) { return set(data  & (value << lo)); }
  inline auto& operator |=(const type value) { return set(data  | (value << lo)); }
  inline auto& operator ^=(const type value) { return set(data  ^ (value << lo)); }
  inline auto& operator<<=(const type value) { return set((data & mask) << value); }
  inline auto& operator>>=(const type value) { return set((data & mask) >> value); }
  inline auto& operator +=(const type value) { return set(data  + (value << lo)); }
  inline auto& operator -=(const type value) { return set(data  - (value << lo)); }
  inline auto& operator *=(const type value) { return set((get() * value) << lo); }
  inline auto& operator /=(const type value) { return set((get() / value) << lo); }
  inline auto& operator %=(const type value) { return set((get() % value) << lo); }

private:
  type data;

  inline auto get() const -> type {
    return (data & mask) >> lo;
  }

  inline auto set(type value) -> NaturalBitField& {
    return data = (data & ~mask) | (value & mask), *this;
  }
};

template<typename type, uint Bit> struct BooleanBitField {
  enum : uint { bit = Bit };
  enum : uint { mask = 1ull << bit };
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
  type data;

  inline auto get() const -> bool {
    return data & mask;
  }

  inline auto set(bool value) -> BooleanBitField& {
    return data = (data & ~mask) | (value << bit), *this;
  }
};

}
