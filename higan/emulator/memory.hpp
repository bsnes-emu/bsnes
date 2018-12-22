#pragma once

namespace Emulator {

template<typename type>
struct WritableMemory {
  inline auto reset() -> void {
    delete[] self.data;
    self.data = nullptr;
    self.size = 0;
    self.mask = 0;
  }

  inline auto allocate(uint size, type fill = ~0ull) -> void {
    delete[] self.data;
    self.size = size;
    self.mask = bit::round(size) - 1;
    self.data = new uint8[self.mask];
    memory::fill<type>(self.data, size, fill);
  }

  explicit operator bool() const { return (bool)self.data; }
  inline auto data() -> type* { return self.data; }
  inline auto size() const -> uint { return self.size; }
  inline auto mask() const -> uint { return self.mask; }

  inline auto operator[](uint address) -> type& {
    return self.data[address & self.mask];
  }

  inline auto read(uint address) -> type {
    return self.data[address & self.mask];
  }

  inline auto write(uint address, type data) -> void {
    self.data[address & self.mask] = data;
  }

  struct {
    type* data = nullptr;
    uint size = 0;
    uint mask = 0;
  } self;
};

}
