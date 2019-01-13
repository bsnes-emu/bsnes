#pragma once

namespace nall {

template<int RequestedPrecision> struct Real {
  enum : uint { Precision = RequestedPrecision <= 32 ? 32 : 64 };
  static inline constexpr auto bits() -> uint { return Precision; }
  using type =
    typename conditional<bits() == 32, float32_t,
    typename conditional<bits() == 64, float64_t,
    void>::type>::type;

  inline Real() : data(0.0) {}
  template<typename T> inline Real(const T& value) : data((type)value) {}

  explicit operator bool() const { return (bool)data; }
  inline operator type() const { return data; }

  inline auto operator++(int) { auto value = *this; ++data; return value; }
  inline auto operator--(int) { auto value = *this; --data; return value; }

  inline auto& operator++() { data++; return *this; }
  inline auto& operator--() { data--; return *this; }

  template<typename T> inline auto& operator =(const T& value) { data =        value; return *this; }
  template<typename T> inline auto& operator+=(const T& value) { data = data + value; return *this; }
  template<typename T> inline auto& operator-=(const T& value) { data = data - value; return *this; }
  template<typename T> inline auto& operator*=(const T& value) { data = data * value; return *this; }
  template<typename T> inline auto& operator/=(const T& value) { data = data / value; return *this; }
  template<typename T> inline auto& operator%=(const T& value) { data = data % value; return *this; }

  inline auto serialize(serializer& s) { s(data); }

private:
  type data;
};

}
