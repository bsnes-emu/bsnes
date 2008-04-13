#ifndef NALL_BIT_HPP
#define NALL_BIT_HPP

namespace nall {

template<int bits> inline unsigned uclamp(const unsigned x) {
  enum { y = (1U << bits) - 1 };
  return y + ((x - y) & -(x < y)); //min(x, y);
}

template<int bits> inline unsigned uclip(const unsigned x) {
  enum { m = (1U << bits) - 1 };
  return (x & m);
}

template<int bits> inline signed sclamp(const signed x) {
  enum { b = 1U << (bits - 1), m = (1U << (bits - 1)) - 1 };
  return (x > m) ? m : (x < -b) ? -b : x;
}

template<int bits> inline signed sclip(const signed x) {
  enum { b = 1U << (bits - 1), m = (1U << bits) - 1 };
  return ((x & m) ^ b) - b;
}

} //namespace nall

#endif //ifndef NALL_BIT_HPP
