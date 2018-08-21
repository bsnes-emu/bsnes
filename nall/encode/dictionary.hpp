#pragma once

#include <nall/suffix-array.hpp>

namespace nall { namespace Encode {

struct Dictionary {
  inline Dictionary(const void* data, uint size, uint capacity = 0);
  inline ~Dictionary();

  inline auto operator[](uint index) const -> uint;
  inline auto scan(uint offset = 0, uint size = 0) -> uint;
  inline auto find(uint prefix, uint& lower, uint& upper) -> void;

private:
  const uint8_t* data = nullptr;
  uint size = 0;

  uint capacity = 0;
  uint unique = 0;
  uint* suffixes = nullptr;
  uint* prefixes = nullptr;
};

Dictionary::Dictionary(const void* data, uint size, uint capacity) {
  this->data = (const uint8_t*)data;
  this->size = size;
  this->capacity = capacity ? capacity : size;
  suffixes = new uint[2 * this->capacity];
  prefixes = &suffixes[this->capacity];
}

Dictionary::~Dictionary() {
  delete[] suffixes;
}

auto Dictionary::operator[](uint index) const -> uint {
  return suffixes[index];
}

auto Dictionary::scan(uint offset, uint size) -> uint {
  size = min(size ? size : capacity, this->size - offset);
  partial_suffix_array<32, 32>(suffixes, prefixes, data + offset, size, offset);
  uint target = 0, source = 0;
  while(source < size) {
    prefixes[target] = prefixes[source];
    suffixes[target] = suffixes[source];
    uint length = 1;
    while(source + length < size) {
      if(suffixes[source + length] != suffixes[source] + length) break;
      length++;
    }
    source += length;
    target += 1;
  }
  return unique = target;
}

auto Dictionary::find(uint prefix, uint& lower, uint& upper) -> void {
  uint l = 0, r = unique - 1;
  while(l < r - 1) {
    uint m = l + r >> 1;
    prefixes[m] >= prefix ? r = m : l = m;
  }
  lower = l, r = unique - 1;
  while(l < r - 1) {
    uint m = l + r >> 1;
    prefixes[m] <= prefix ? l = m : r = m;
  }
  upper = r;
}

}}
