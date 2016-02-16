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
  template<typename T> inline Natural(const T& value) : data(clip(value)) {}

  inline operator type() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { data = clip(value); return *this; }

  inline auto operator++(int) { type value = data; data = clip(data + 1); return value; }
  inline auto operator--(int) { type value = data; data = clip(data - 1); return value; }

  inline auto& operator++() { data = clip(data + 1); return *this; }
  inline auto& operator--() { data = clip(data - 1); return *this; }

  inline auto& operator &=(const type value) { data = clip(data  & value); return *this; }
  inline auto& operator |=(const type value) { data = clip(data  | value); return *this; }
  inline auto& operator ^=(const type value) { data = clip(data  ^ value); return *this; }
  inline auto& operator<<=(const type value) { data = clip(data << value); return *this; }
  inline auto& operator>>=(const type value) { data = clip(data >> value); return *this; }
  inline auto& operator +=(const type value) { data = clip(data  + value); return *this; }
  inline auto& operator -=(const type value) { data = clip(data  - value); return *this; }
  inline auto& operator *=(const type value) { data = clip(data  * value); return *this; }
  inline auto& operator /=(const type value) { data = clip(data  / value); return *this; }
  inline auto& operator %=(const type value) { data = clip(data  % value); return *this; }

  inline auto serialize(serializer& s) { s(data); }

  struct Reference {
    inline Reference(Natural& source, uint Lo, uint Hi) : source(source), Lo(Lo), Hi(Hi) {}

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
    const uint Lo;
    const uint Hi;
  };

  inline auto bits(uint lo, uint hi) -> Reference { return {*this, lo, hi}; }
  inline auto bit(uint index) -> Reference { return {*this, index, index}; }
  inline auto byte(uint index) -> Reference { return {*this, index * 8 + 0, index * 8 + 7}; }

  template<uint Lo, uint Hi> struct Range {
    enum : type { RangeBits = Hi - Lo + 1, RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask };

    inline operator type() const {
      return (self() & RangeMask) >> Lo;
    }

    inline auto& operator=(const type value) {
      self() = (self() & ~RangeMask) | ((value << Lo) & RangeMask);
      return *this;
    }

  private:
    inline auto self() const -> type& { return *(type*)this; }
  };

  union {
    type data;

    Range< 0,  7> b0;
    Range< 8, 15> b1;
    Range<16, 23> b2;
    Range<24, 31> b3;
    Range<32, 39> b4;
    Range<40, 47> b5;
    Range<48, 55> b6;
    Range<56, 63> b7;

    Range< 0, 15> h0;
    Range<16, 31> h1;
    Range<32, 47> h2;
    Range<48, 63> h3;

    Range< 0, 31> w0;
    Range<32, 63> w1;
  };

private:
  auto clip(type value) const -> type {
    return value & Mask;
  }
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
  template<typename T> inline Integer(const T& value) : data(clip(value)) {}

  inline operator type() const { return data; }
  template<typename T> inline auto& operator=(const T& value) { data = clip(value); return *this; }

  inline auto operator++(int) { type value = data; data = clip(data + 1); return value; }
  inline auto operator--(int) { type value = data; data = clip(data - 1); return value; }

  inline auto& operator++() { data = clip(data + 1); return *this; }
  inline auto& operator--() { data = clip(data - 1); return *this; }

  inline auto& operator &=(const type value) { data = clip(data  & value); return *this; }
  inline auto& operator |=(const type value) { data = clip(data  | value); return *this; }
  inline auto& operator ^=(const type value) { data = clip(data  ^ value); return *this; }
  inline auto& operator<<=(const type value) { data = clip(data << value); return *this; }
  inline auto& operator>>=(const type value) { data = clip(data >> value); return *this; }
  inline auto& operator +=(const type value) { data = clip(data  + value); return *this; }
  inline auto& operator -=(const type value) { data = clip(data  - value); return *this; }
  inline auto& operator *=(const type value) { data = clip(data  * value); return *this; }
  inline auto& operator /=(const type value) { data = clip(data  / value); return *this; }
  inline auto& operator %=(const type value) { data = clip(data  % value); return *this; }

  inline auto serialize(serializer& s) { s(data); }

  struct Reference {
    inline Reference(Integer& source, uint Lo, uint Hi) : source(source), Lo(Lo), Hi(Hi) {}

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
    uint Lo;
    uint Hi;
  };

  inline auto bits(uint lo, uint hi) -> Reference { return {*this, lo, hi}; }
  inline auto bit(uint index) -> Reference { return {*this, index, index}; }
  inline auto byte(uint index) -> Reference { return {*this, index * 8 + 0, index * 8 + 7}; }

  template<uint Lo, uint Hi> struct Range {
    enum : utype { RangeBits = Hi - Lo + 1, RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask };

    inline operator utype() const {
      return ((utype)self() & RangeMask) >> Lo;
    }

    inline auto& operator=(const utype value) {
      self() = ((((utype)self() & ~RangeMask) | ((value << Lo) & RangeMask)) ^ Sign) - Sign;
      return *this;
    }

  private:
    inline auto self() const -> utype& { return *(utype*)this; }
  };

  union {
    type data;

    Range< 0,  7> b0;
    Range< 8, 15> b1;
    Range<16, 23> b2;
    Range<24, 31> b3;
    Range<32, 39> b4;
    Range<40, 47> b5;
    Range<48, 55> b6;
    Range<56, 63> b7;

    Range< 0, 15> h0;
    Range<16, 31> h1;
    Range<32, 47> h2;
    Range<48, 63> h3;

    Range< 0, 31> w0;
    Range<32, 63> w1;
  };

private:
  auto clip(type value) const -> type {
    return ((value & Mask) ^ Sign) - Sign;
  }
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
