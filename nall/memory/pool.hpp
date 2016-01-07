#pragma once

namespace nall {
namespace memory {

template<unsigned Capacity, unsigned Size>
struct pool_spsc {
  signed* list = nullptr;
  uint8_t* data = nullptr;
  unsigned slot = 0;

  pool_spsc() {
    list = (signed*)memory::allocate(Capacity * sizeof(signed));
    data = (uint8_t*)memory::allocate(Capacity * Size);
    for(unsigned n = 0; n < Capacity; n++) list[n] = n;
  }

  ~pool_spsc() {
    memory::free(list);
    memory::free(data);
  }

  auto allocate(unsigned size) -> void* {
    if(size == 0) return nullptr;
    if(size > Size) return memory::allocate(size);
    signed offset = list[slot];
    if(offset < 0) return memory::allocate(size);
    list[slot] = -1;
    slot = (slot + 1) % Capacity;
    return (void*)(data + offset * Size);
  }

  auto allocate(unsigned size, uint8_t data) -> void* {
    auto result = allocate(size);
    memset(result, data, size);
    return result;
  }

  auto resize(void* target, unsigned size) -> void* {
    if(target == nullptr) return allocate(size);
    signed offset = ((uint8_t*)target - data) / Size;
    if(offset < 0 || offset >= Capacity) return memory::resize(target, size);
    if(size <= Size) return target;
    slot = (slot - 1) % Capacity;
    list[slot] = offset;
    return memory::allocate(size);
  }

  auto free(void* target) -> void {
    if(target == nullptr) return;
    signed offset = ((uint8_t*)target - data) / Size;
    if(offset < 0 || offset >= Capacity) return memory::free(target);
    slot = (slot - 1) % Capacity;
    list[slot] = offset;
  }

  pool_spsc(const pool_spsc&) = delete;
  pool_spsc& operator=(const pool_spsc&) = delete;
};

}
}
