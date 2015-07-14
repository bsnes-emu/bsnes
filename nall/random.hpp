#ifndef NALL_RANDOM_HPP
#define NALL_RANDOM_HPP

#include <nall/serializer.hpp>
#include <nall/stdint.hpp>

namespace nall {

struct RandomNumberGenerator {
  virtual auto seed(uint64_t) -> void = 0;
  virtual auto operator()() -> uint64_t = 0;
  virtual auto serialize(serializer&) -> void = 0;
};

//Galois LFSR using CRC64 polynomials
struct LinearFeedbackShiftRegisterGenerator : RandomNumberGenerator {
  auto seed(uint64_t seed) -> void {
    lfsr = seed;
    for(unsigned n = 0; n < 8; n++) operator()();
  }

  auto operator()() -> uint64_t {
    return lfsr = (lfsr >> 1) ^ (-(lfsr & 1) & crc64jones);
  }

  auto serialize(serializer& s) -> void {
    s.integer(lfsr);
  }

private:
  static const uint64_t crc64ecma  = 0x42f0e1eba9ea3693;
  static const uint64_t crc64jones = 0xad93d23594c935a9;
  uint64_t lfsr = crc64ecma;
};

inline auto random() -> uint64_t {
  static LinearFeedbackShiftRegisterGenerator lfsr;
  return lfsr();
}

}

#endif
