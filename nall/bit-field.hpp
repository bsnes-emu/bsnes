#pragma once

namespace nall {

template<typename type> struct BitFieldReference {
  BitFieldReference(type& data, uint lo, uint hi)
  : data(data), lo(lo), hi(hi), bits(hi + lo - 1), mask((~0ull >> (64 - bits)) << lo) {
  }

  inline explicit operator bool() const { return data & mask; }
  inline operator type() const { return get(); }

  inline auto& operator=(const BitFieldReference& value) { return set(value.data); }
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

  const uint lo;
  const uint hi;
  const uint bits;
  const uint mask;

private:
  type& data;

  inline auto get() const -> type {
    return (data & mask) >> lo;
  }

  inline auto set(type value) -> BitFieldReference& {
    return data = (data & ~mask) | (value & mask), *this;
  }
};

template<typename type, uint Lo, uint Hi = ~0U> struct BitField {
  enum : uint { lo = Lo <= Hi ? Lo : Hi };
  enum : uint { hi = Hi >= Lo ? Hi : Lo };
  enum : uint { bits = hi - lo + 1 };
  enum : uint { mask = (~0ull >> (64 - bits)) << lo };
  static_assert(hi < sizeof(type) * 8, "");

  inline BitField() = default;
  inline BitField(const BitField& value) { set(value.data); }
  template<typename T> inline BitField(const T& value) { set(value << lo); }

  inline explicit operator bool() const { return data & mask; }
  inline operator type() const { return get(); }
  inline operator BitFieldReference<type>() { return {data, lo, hi}; }

  inline auto& operator=(const BitField& value) { return set(value.data); }
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

  inline auto set(type value) -> BitField& {
    return data = (data & ~mask) | (value & mask), *this;
  }
};

template<typename type, uint Bit> struct BitField<type, Bit, ~0U> {
  enum : uint { bit = Bit };
  enum : uint { mask = 1ull << bit };
  static_assert(bit < sizeof(type) * 8, "");

  inline BitField() = default;
  inline BitField(const BitField& value) { set(value.get()); }
  template<typename T> inline BitField(const bool value) { set(value); }

  inline operator bool() const { return get(); }
  inline operator BitFieldReference<type>() { return {data, bit, bit}; }

  inline auto& operator=(const BitField& value) { return set(value.get()); }
  inline auto& operator=(const bool value) { return set(value); }

  inline auto& operator&=(const bool value) { return set(get() & value); }
  inline auto& operator|=(const bool value) { return set(get() | value); }
  inline auto& operator^=(const bool value) { return set(get() ^ value); }

private:
  type data;

  inline auto get() const -> bool {
    return data & mask;
  }

  inline auto set(bool value) -> BitField& {
    return data = (data & ~mask) | (value << bit), *this;
  }
};

}
