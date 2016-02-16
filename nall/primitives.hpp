#pragma once

#include <nall/serializer.hpp>
#include <nall/traits.hpp>

namespace nall {

struct Boolean {
  inline Boolean() : data(false) {}
  template<typename T> inline Boolean(const T& value) : data(value) {}

  inline operator bool() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { data = value; return *this; }

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
  template<typename T> inline Natural(const T& value) { assign(value); }

  inline operator type() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { assign(value); return *this; }

  inline auto operator++(int) { type value = data; assign(data + 1); return value; }
  inline auto operator--(int) { type value = data; assign(data - 1); return value; }

  inline auto& operator++() { assign(data + 1); return *this; }
  inline auto& operator--() { assign(data - 1); return *this; }

  inline auto& operator &=(const type value) { assign(data  & value); return *this; }
  inline auto& operator |=(const type value) { assign(data  | value); return *this; }
  inline auto& operator ^=(const type value) { assign(data  ^ value); return *this; }
  inline auto& operator<<=(const type value) { assign(data << value); return *this; }
  inline auto& operator>>=(const type value) { assign(data >> value); return *this; }
  inline auto& operator +=(const type value) { assign(data  + value); return *this; }
  inline auto& operator -=(const type value) { assign(data  - value); return *this; }
  inline auto& operator *=(const type value) { assign(data  * value); return *this; }
  inline auto& operator /=(const type value) { assign(data  / value); return *this; }
  inline auto& operator %=(const type value) { assign(data  % value); return *this; }

  inline auto serialize(serializer& s) { s(data); }

  struct Reference {
    inline Reference(Natural& source, uint lo, uint hi) : source(source), Lo(lo), Hi(hi) {}

    inline operator type() const {
      const type RangeBits = Hi - Lo + 1;
      const type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
      return (source & RangeMask) >> Lo;
    }

    inline auto& operator=(const type value) {
      const type RangeBits = Hi - Lo + 1;
      const type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
      source = (source & ~RangeMask) | ((value << Lo) & RangeMask);
      return *this;
    }

  private:
    Natural& source;
    const type Lo;
    const type Hi;
  };

  inline auto bits(uint lo, uint hi) -> Reference { return {*this, lo < hi ? lo : hi, hi > lo ? hi : lo}; }
  inline auto bit(uint index) -> Reference { return {*this, index, index}; }
  inline auto byte(uint index) -> Reference { return {*this, index * 8 + 0, index * 8 + 7}; }

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
  auto assign(type value) -> void {
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
  template<typename T> inline Integer(const T& value) { assign(value); }

  inline operator type() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { assign(value); return *this; }

  inline auto operator++(int) { type value = data; assign(data + 1); return value; }
  inline auto operator--(int) { type value = data; assign(data - 1); return value; }

  inline auto& operator++() { assign(data + 1); return *this; }
  inline auto& operator--() { assign(data - 1); return *this; }

  inline auto& operator &=(const type value) { assign(data  & value); return *this; }
  inline auto& operator |=(const type value) { assign(data  | value); return *this; }
  inline auto& operator ^=(const type value) { assign(data  ^ value); return *this; }
  inline auto& operator<<=(const type value) { assign(data << value); return *this; }
  inline auto& operator>>=(const type value) { assign(data >> value); return *this; }
  inline auto& operator +=(const type value) { assign(data  + value); return *this; }
  inline auto& operator -=(const type value) { assign(data  - value); return *this; }
  inline auto& operator *=(const type value) { assign(data  * value); return *this; }
  inline auto& operator /=(const type value) { assign(data  / value); return *this; }
  inline auto& operator %=(const type value) { assign(data  % value); return *this; }

  inline auto serialize(serializer& s) { s(data); }

  struct Reference {
    inline Reference(Integer& source, uint lo, uint hi) : source(source), Lo(lo), Hi(hi) {}

    inline operator utype() const {
      const type RangeBits = Hi - Lo + 1;
      const type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
      return ((utype)source & RangeMask) >> Lo;
    }

    inline auto& operator=(const utype value) {
      const type RangeBits = Hi - Lo + 1;
      const type RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask;
      source = ((utype)source & ~RangeMask) | ((value << Lo) & RangeMask);
      return *this;
    }

  private:
    Integer& source;
    const uint Lo;
    const uint Hi;
  };

  inline auto bits(uint lo, uint hi) -> Reference { return {*this, lo, hi}; }
  inline auto bit(uint index) -> Reference { return {*this, index, index}; }
  inline auto byte(uint index) -> Reference { return {*this, index * 8 + 0, index * 8 + 7}; }

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
  auto assign(type value) -> void {
    data = ((value & Mask) ^ Sign) - Sign;
  }

  type data;
};

template<uint Bits> struct Real {
  using type =
    type_if<expression<Bits == 32>, float32_t,
    type_if<expression<Bits == 64>, float64_t,
    type_if<expression<Bits == 80>, float80_t,
    void>>>;

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

  type data;
};

}
