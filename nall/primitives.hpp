#ifndef NALL_PRIMITIVES_HPP
#define NALL_PRIMITIVES_HPP

#include <nall/serializer.hpp>
#include <nall/traits.hpp>

namespace nall {

struct Boolean {
  inline Boolean() : data(false) {}
  inline Boolean(bool value) : data(value) {}

  inline operator bool() const { return data; }
  inline auto& operator=(const bool value) { data = value; return *this; }

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
  inline Natural(const type value) : data(clip(value)) {}
  template<uint B> inline Natural(const Natural<B> value) : data(clip(value)) {}

  inline operator type() const { return data; }
  inline auto& operator=(const type value) { data = clip(value); return *this; }
  template<uint B> inline auto& operator=(const Natural<B> value) { data = clip(value); return *this; }

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
  inline Integer(const type value) : data(clip(value)) {}
  template<uint B> inline Integer(const Integer<B> value) : data(clip(value)) {}

  inline operator type() const { return data; }
  inline auto& operator=(const type value) { data = clip(value); return *this; }
  template<uint B> inline auto& operator=(const Integer<B> value) { data = clip(value); return *this; }

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

  template<uint Lo, uint Hi> struct Range {
    enum : utype { RangeBits = Hi - Lo + 1, RangeMask = (((1ull << RangeBits) - 1) << Lo) & Mask };

    inline operator utype() const {
      return (self() & RangeMask) >> Lo;
    }

    inline auto& operator=(const utype value) {
      self() = (((self() & ~RangeMask) | ((value << Lo) & RangeMask)) ^ Sign) - Sign;
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
  inline Real(const type value) : data(value) {}
  template<uint B> inline Real(const Real<B> value) : data((type)value) {}

  inline operator type() const { return data; }
  inline auto& operator=(const type value) { data = value; return *this; }
  template<uint B> inline auto& operator=(const Real<B> value) { data = (type)value; return *this; }

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

#endif
