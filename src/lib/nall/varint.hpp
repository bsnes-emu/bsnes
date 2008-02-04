#ifndef NALL_VARINT_HPP
#define NALL_VARINT_HPP

#include <nall/bit.hpp>
#include <nall/static.hpp>

namespace nall {

template<unsigned bits> class uint_t {
private:
  enum { bytes = (bits + 7) >> 3 }; //minimum number of bytes needed to store value
  typedef typename static_if<
    sizeof(int) >= bytes,
    unsigned int,
    typename static_if<
      sizeof(long) >= bytes,
      unsigned long,
      typename static_if<
        sizeof(long long) >= bytes,
        unsigned long long,
        void
      >::type
    >::type
  >::type T;
  static_assert<!is_void<T>::value> uint_assert;
  T data;

public:
  inline operator T() const { return data; }
  inline T  operator ++(int) { T r = data; data = uclip<bits>(data + 1); return r; }
  inline T  operator --(int) { T r = data; data = uclip<bits>(data - 1); return r; }
  inline T& operator ++() { data = uclip<bits>(data + 1); return *this; }
  inline T& operator --() { data = uclip<bits>(data - 1); return *this; }
  inline T& operator  =(const T i) { data = uclip<bits>(i); return *this; }
  inline T& operator |=(const T i) { data = uclip<bits>(data  | i); return *this; }
  inline T& operator ^=(const T i) { data = uclip<bits>(data  ^ i); return *this; }
  inline T& operator &=(const T i) { data = uclip<bits>(data  & i); return *this; }
  inline T& operator<<=(const T i) { data = uclip<bits>(data << i); return *this; }
  inline T& operator>>=(const T i) { data = uclip<bits>(data >> i); return *this; }
  inline T& operator +=(const T i) { data = uclip<bits>(data  + i); return *this; }
  inline T& operator -=(const T i) { data = uclip<bits>(data  - i); return *this; }
  inline T& operator *=(const T i) { data = uclip<bits>(data  * i); return *this; }
  inline T& operator /=(const T i) { data = uclip<bits>(data  / i); return *this; }
  inline T& operator %=(const T i) { data = uclip<bits>(data  % i); return *this; }

  inline uint_t() : data(0) {}
  inline uint_t(const T i) : data(uclip<bits>(i)) {}
};

template<unsigned bits> class int_t {
private:
  enum { bytes = (bits + 7) >> 3 }; //minimum number of bytes needed to store value
  typedef typename static_if<
    sizeof(int) >= bytes,
    signed int,
    typename static_if<
      sizeof(long) >= bytes,
      signed long,
      typename static_if<
        sizeof(long long) >= bytes,
        signed long long,
        void
      >::type
    >::type
  >::type T;
  static_assert<!is_void<T>::value> int_assert;
  T data;

public:
  inline operator T() const { return data; }
  inline T  operator ++(int) { T r = data; data = sclip<bits>(data + 1); return r; }
  inline T  operator --(int) { T r = data; data = sclip<bits>(data - 1); return r; }
  inline T& operator ++() { data = sclip<bits>(data + 1); return *this; }
  inline T& operator --() { data = sclip<bits>(data - 1); return *this; }
  inline T& operator  =(const T i) { data = sclip<bits>(i); return *this; }
  inline T& operator |=(const T i) { data = sclip<bits>(data  | i); return *this; }
  inline T& operator ^=(const T i) { data = sclip<bits>(data  ^ i); return *this; }
  inline T& operator &=(const T i) { data = sclip<bits>(data  & i); return *this; }
  inline T& operator<<=(const T i) { data = sclip<bits>(data << i); return *this; }
  inline T& operator>>=(const T i) { data = sclip<bits>(data >> i); return *this; }
  inline T& operator +=(const T i) { data = sclip<bits>(data  + i); return *this; }
  inline T& operator -=(const T i) { data = sclip<bits>(data  - i); return *this; }
  inline T& operator *=(const T i) { data = sclip<bits>(data  * i); return *this; }
  inline T& operator /=(const T i) { data = sclip<bits>(data  / i); return *this; }
  inline T& operator %=(const T i) { data = sclip<bits>(data  % i); return *this; }

  inline int_t() : data(0) {}
  inline int_t(const T i) : data(sclip<bits>(i)) {}
};

typedef int_t<24> int24_t;
typedef uint_t<24> uint24_t;
typedef int_t<48> int48_t;
typedef uint_t<48> uint48_t;

} //namespace nall

using nall::int24_t;
using nall::uint24_t;
using nall::int48_t;
using nall::uint48_t;

#endif //ifndef NALL_VARINT_HPP
