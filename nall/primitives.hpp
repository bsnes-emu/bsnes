#pragma once

#include <nall/serializer.hpp>
#include <nall/traits.hpp>

namespace nall {

struct Boolean {
  inline Boolean() : data(false) {}
  template<typename T> inline Boolean(const T& value) : data(value) {}

  inline operator bool() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { data = value; return *this; }

  inline auto flip() { return data ^= 1; }
  inline auto raise() { return data == 0 ? data = 1, true : false; }
  inline auto lower() { return data == 1 ? data = 0, true : false; }

  inline auto serialize(serializer& s) { s(data); }

private:
  bool data;
};

template<uint Bits> struct Natural {
  using type =
    type_if<expression<Bits <=  8>,  uint8_t,
    type_if<expression<Bits <= 16>, uint16_t,
    type_if<expression<Bits <= 32>, uint32_t,
    type_if<expression<Bits <= 64>, uint64_t,
    void>>>>;

  enum : type { Mask = ~0ull >> (64 - Bits) };

  inline Natural() : data(0) {}
  template<typename T> inline Natural(const T& value) { set(value); }

  inline operator type() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { set(value); return *this; }

  inline auto operator++(int) { type value = data; set(data + 1); return value; }
  inline auto operator--(int) { type value = data; set(data - 1); return value; }

  inline auto& operator++() { set(data + 1); return *this; }
  inline auto& operator--() { set(data - 1); return *this; }

  inline auto& operator &=(const type value) { set(data  & value); return *this; }
  inline auto& operator |=(const type value) { set(data  | value); return *this; }
  inline auto& operator ^=(const type value) { set(data  ^ value); return *this; }
  inline auto& operator<<=(const type value) { set(data << value); return *this; }
  inline auto& operator>>=(const type value) { set(data >> value); return *this; }
  inline auto& operator +=(const type value) { set(data  + value); return *this; }
  inline auto& operator -=(const type value) { set(data  - value); return *this; }
  inline auto& operator *=(const type value) { set(data  * value); return *this; }
  inline auto& operator /=(const type value) { set(data  / value); return *this; }
  inline auto& operator %=(const type value) { set(data  % value); return *this; }

  inline auto serialize(serializer& s) { s(data); }

  struct Reference {
    inline Reference(Natural& source, uint lo, uint hi) : source(source), Lo(lo), Hi(hi) {}
    inline auto& operator=(Reference& source) { return set(source.get()); }

    inline auto get() const -> type {
      const type RangeBits = Hi - Lo + 1;
      const type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
      return (source & RangeMask) >> Lo;
    }

    inline auto& set(const type value) {
      const type RangeBits = Hi - Lo + 1;
      const type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
      source = (source & ~RangeMask) | ((value << Lo) & RangeMask);
      return *this;
    }

    inline operator type() const { return get(); }
    inline auto& operator  =(const type value) { return set(         value); }
    inline auto& operator &=(const type value) { return set(get()  & value); }
    inline auto& operator |=(const type value) { return set(get()  | value); }
    inline auto& operator ^=(const type value) { return set(get()  ^ value); }
    inline auto& operator<<=(const type value) { return set(get() << value); }
    inline auto& operator>>=(const type value) { return set(get() >> value); }
    inline auto& operator +=(const type value) { return set(get()  + value); }
    inline auto& operator -=(const type value) { return set(get()  - value); }
    inline auto& operator *=(const type value) { return set(get()  * value); }
    inline auto& operator /=(const type value) { return set(get()  / value); }
    inline auto& operator %=(const type value) { return set(get()  % value); }
    inline auto& operator++(int) { auto value = get(); set(value + 1); return value; }
    inline auto& operator--(int) { auto value = get(); set(value - 1); return value; }
    inline auto& operator++() { return set(get() + 1); }
    inline auto& operator--() { return set(get() - 1); }

  private:
    Natural& source;
    const type Lo;
    const type Hi;
  };

  inline auto bits(uint lo, uint hi) -> Reference { return {*this, lo < hi ? lo : hi, hi > lo ? hi : lo}; }
  inline auto bit(uint index) -> Reference { return {*this, index, index}; }
  inline auto byte(uint index) -> Reference { return {*this, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(uint lo, uint hi) const -> const Reference { return {(Natural&)*this, lo < hi ? lo : hi, hi > lo ? hi : lo}; }
  inline auto bit(uint index) const -> const Reference { return {(Natural&)*this, index, index}; }
  inline auto byte(uint index) const -> const Reference { return {(Natural&)*this, index * 8 + 0, index * 8 + 7}; }

  inline auto clamp(uint bits) -> uintmax {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return data < m ? data : m;
  }

  inline auto clip(uint bits) -> uintmax {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return data & m;
  }

private:
  auto set(type value) -> void {
    data = value & Mask;
  }

  type data;
};

template<uint Bits> struct Integer {
  using type =
    type_if<expression<Bits <=  8>,  int8_t,
    type_if<expression<Bits <= 16>, int16_t,
    type_if<expression<Bits <= 32>, int32_t,
    type_if<expression<Bits <= 64>, int64_t,
    void>>>>;
  using utype = typename Natural<Bits>::type;

  enum : utype { Mask = ~0ull >> (64 - Bits), Sign = 1ull << (Bits - 1) };

  inline Integer() : data(0) {}
  template<typename T> inline Integer(const T& value) { set(value); }

  inline operator type() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { set(value); return *this; }

  inline auto operator++(int) { type value = data; set(data + 1); return value; }
  inline auto operator--(int) { type value = data; set(data - 1); return value; }

  inline auto& operator++() { set(data + 1); return *this; }
  inline auto& operator--() { set(data - 1); return *this; }

  inline auto& operator &=(const type value) { set(data  & value); return *this; }
  inline auto& operator |=(const type value) { set(data  | value); return *this; }
  inline auto& operator ^=(const type value) { set(data  ^ value); return *this; }
  inline auto& operator<<=(const type value) { set(data << value); return *this; }
  inline auto& operator>>=(const type value) { set(data >> value); return *this; }
  inline auto& operator +=(const type value) { set(data  + value); return *this; }
  inline auto& operator -=(const type value) { set(data  - value); return *this; }
  inline auto& operator *=(const type value) { set(data  * value); return *this; }
  inline auto& operator /=(const type value) { set(data  / value); return *this; }
  inline auto& operator %=(const type value) { set(data  % value); return *this; }

  inline auto serialize(serializer& s) { s(data); }

  struct Reference {
    inline Reference(Integer& source, uint lo, uint hi) : source(source), Lo(lo), Hi(hi) {}
    inline auto& operator=(const Reference& source) { return set(source.get()); }

    inline auto get() const -> utype {
      const type RangeBits = Hi - Lo + 1;
      const type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
      return ((utype)source & RangeMask) >> Lo;
    }

    inline auto& set(const utype value) {
      const type RangeBits = Hi - Lo + 1;
      const type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
      source = ((utype)source & ~RangeMask) | ((value << Lo) & RangeMask);
      return *this;
    }

    inline operator utype() const { return get(); }
    inline auto& operator  =(const utype value) { return set(         value); }
    inline auto& operator &=(const utype value) { return set(get()  & value); }
    inline auto& operator |=(const utype value) { return set(get()  | value); }
    inline auto& operator ^=(const utype value) { return set(get()  ^ value); }
    inline auto& operator<<=(const utype value) { return set(get() << value); }
    inline auto& operator>>=(const utype value) { return set(get() >> value); }
    inline auto& operator +=(const utype value) { return set(get()  + value); }
    inline auto& operator -=(const utype value) { return set(get()  - value); }
    inline auto& operator *=(const utype value) { return set(get()  * value); }
    inline auto& operator /=(const utype value) { return set(get()  / value); }
    inline auto& operator %=(const utype value) { return set(get()  % value); }
    inline auto& operator++(int) { auto value = get(); set(value + 1); return value; }
    inline auto& operator--(int) { auto value = get(); set(value - 1); return value; }
    inline auto& operator++() { return set(get() + 1); }
    inline auto& operator--() { return set(get() - 1); }

  private:
    Integer& source;
    const uint Lo;
    const uint Hi;
  };

  inline auto bits(uint lo, uint hi) -> Reference { return {*this, lo < hi ? lo : hi, hi > lo ? hi : lo}; }
  inline auto bit(uint index) -> Reference { return {*this, index, index}; }
  inline auto byte(uint index) -> Reference { return {*this, index * 8 + 0, index * 8 + 7}; }

  inline auto bits(uint lo, uint hi) const -> const Reference { return {(Integer&)*this, lo < hi ? lo : hi, hi > lo ? hi : lo}; }
  inline auto bit(uint index) const -> const Reference { return {(Integer&)*this, index, index}; }
  inline auto byte(uint index) const -> const Reference { return {(Integer&)*this, index * 8 + 0, index * 8 + 7}; }

  inline auto clamp(uint bits) -> intmax {
    const intmax b = 1ull << (bits - 1);
    const intmax m = b - 1;
    return data > m ? m : data < -b ? -b : data;
  }

  inline auto clip(uint bits) -> intmax {
    const uintmax b = 1ull << (bits - 1);
    const uintmax m = b * 2 - 1;
    return ((data & m) ^ b) - b;
  }

private:
  auto set(type value) -> void {
    data = ((value & Mask) ^ Sign) - Sign;
  }

  type data;
};

template<uint Bits> struct Real {
  using type =
    type_if<expression<Bits == 32>, float32_t,
    type_if<expression<Bits == 64>, float64_t,
  //type_if<expression<Bits == 80>, float80_t,
    void>>;

  inline Real() : data(0.0) {}
  template<typename T> inline Real(const T& value) : data((type)value) {}

  inline operator type() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { data = (type)value; return *this; }

  inline auto operator++(int) { type value = data; ++data; return value; }
  inline auto operator--(int) { type value = data; --data; return value; }

  inline auto& operator++() { data++; return *this; }
  inline auto& operator--() { data--; return *this; }

  inline auto& operator+=(const type value) { data = data + value; return *this; }
  inline auto& operator-=(const type value) { data = data - value; return *this; }
  inline auto& operator*=(const type value) { data = data * value; return *this; }
  inline auto& operator/=(const type value) { data = data / value; return *this; }
  inline auto& operator%=(const type value) { data = data % value; return *this; }

  inline auto serialize(serializer& s) { s(data); }

private:
  type data;
};

using boolean = nall::Boolean;
using natural = nall::Natural<sizeof(uint) * 8>;
using integer = nall::Integer<sizeof(int) * 8>;
using real    = nall::Real<sizeof(double) * 8>;

}
