#pragma once

#include <nall/arithmetic.hpp>

namespace nall { namespace Cipher {

struct ChaCha20 {
  auto initialize(uint256_t key, uint64_t nonce, uint64_t counter = 0) -> void {
    static const uint256_t sigma = 0x6b20657479622d323320646e61707865_u256;  //"expand 32-byte k"

    input[ 0] = sigma   >>   0;
    input[ 1] = sigma   >>  32;
    input[ 2] = sigma   >>  64;
    input[ 3] = sigma   >>  96;
    input[ 4] = key     >>   0;
    input[ 5] = key     >>  32;
    input[ 6] = key     >>  64;
    input[ 7] = key     >>  96;
    input[ 8] = key     >> 128;
    input[ 9] = key     >> 160;
    input[10] = key     >> 192;
    input[11] = key     >> 224;
    input[12] = counter >>   0;
    input[13] = counter >>  32;
    input[14] = nonce   >>   0;
    input[15] = nonce   >>  32;
    offset = 0;
  }

  auto encrypt(const uint8_t* input, uint8_t* output, uint64_t length) -> void {
    while(length--) {
      if(!offset) cipher();
      auto byte = offset++;
      *output++ = *input++ ^ (block[byte >> 2] >> (byte & 3) * 8);
      offset &= 63;
    }
  }

  auto decrypt(const uint8_t* input, uint8_t* output, uint64_t length) -> void {
    encrypt(input, output, length);  //reciprocal cipher
  }

private:
  inline auto rol(uint32_t value, uint bits) -> uint32_t {
    return value << bits | value >> (32 - bits);
  }

  auto quarterRound(uint32_t x[16], uint a, uint b, uint c, uint d) -> void {
    x[a] += x[b]; x[d] = rol(x[d] ^ x[a], 16);
    x[c] += x[d]; x[b] = rol(x[b] ^ x[c], 12);
    x[a] += x[b]; x[d] = rol(x[d] ^ x[a],  8);
    x[c] += x[d]; x[b] = rol(x[b] ^ x[c],  7);
  }

  auto cipher() -> void {
    memory::copy(block, input, 64);
    for(auto n : range(10)) {
      quarterRound(block, 0, 4,  8, 12);
      quarterRound(block, 1, 5,  9, 13);
      quarterRound(block, 2, 6, 10, 14);
      quarterRound(block, 3, 7, 11, 15);
      quarterRound(block, 0, 5, 10, 15);
      quarterRound(block, 1, 6, 11, 12);
      quarterRound(block, 2, 7,  8, 13);
      quarterRound(block, 3, 4,  9, 14);
    }
    for(auto n : range(16)) {
      block[n] += input[n];
    }
    if(!++input[12]) ++input[13];
  }

  uint32_t input[16];
  uint32_t block[16];
  uint64_t offset;
};

}}
