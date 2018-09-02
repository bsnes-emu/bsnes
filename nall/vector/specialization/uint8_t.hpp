#pragma once

namespace nall {

template<> struct vector<uint8_t> : vector_base<uint8_t> {
  using type = vector<uint8_t>;
  using vector_base<uint8_t>::vector_base;

  template<typename U> auto appendl(U value, uint size) -> void {
    for(uint byte : range(size)) append(uint8_t(value >> byte * 8));
  }

  template<typename U> auto appendm(U value, uint size) -> void {
    for(uint byte : reverse(range(size))) append(uint8_t(value >> byte * 8));
  }
};

}
