#pragma once

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
    for(uint n = 0; n < 8; n++) operator()();
  }

  auto operator()() -> uint64_t {
    return lfsr = (lfsr >> 1) ^ (-(lfsr & 1) & crc64);
  }

  auto serialize(serializer& s) -> void {
    s.integer(lfsr);
  }

private:
  static const uint64_t crc64 = 0xc96c'5795'd787'0f42;
  uint64_t lfsr = crc64;
};

inline auto random() -> uint64_t {
  static LinearFeedbackShiftRegisterGenerator lfsr;
  return lfsr();
}

}
