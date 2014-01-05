#ifndef NALL_RANDOM_HPP
#define NALL_RANDOM_HPP

#include <nall/serializer.hpp>
#include <nall/stdint.hpp>

namespace nall {

struct RandomNumberGenerator {
  virtual void seed(uint64_t) = 0;
  virtual uint64_t operator()() = 0;
  virtual void serialize(serializer&) = 0;
};

//Galois LFSR using CRC64 polynomials
struct LinearFeedbackShiftRegisterGenerator : RandomNumberGenerator {
  void seed(uint64_t seed) {
    lfsr = seed;
    for(unsigned n = 0; n < 8; n++) operator()();
  }

  uint64_t operator()() {
    return lfsr = (lfsr >> 1) ^ (-(lfsr & 1) & crc64jones);
  }

  void serialize(serializer& s) {
    s.integer(lfsr);
  }

private:
  static const uint64_t crc64ecma  = 0x42f0e1eba9ea3693;
  static const uint64_t crc64jones = 0xad93d23594c935a9;
  uint64_t lfsr = crc64ecma;
};

inline uint64_t random() {
  static LinearFeedbackShiftRegisterGenerator lfsr;
  return lfsr();
}

}

#endif
