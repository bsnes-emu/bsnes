#pragma once

#include <nall/memory.hpp>

namespace nall {

struct bitvector {
  bitvector() = default;
  bitvector(uint size) { resize(size); }
  bitvector(const bitvector& source) { operator=(source); }
  bitvector(bitvector&& source) { operator=(move(source)); }
  ~bitvector() { reset(); }

  auto operator=(const bitvector& source) -> bitvector& {
    bits = source.bits;
    pool = memory::resize<uint8_t>(pool, bytes());
    memory::copy(pool, source.pool, bytes());
    return *this;
  }

  auto operator=(bitvector&& source) -> bitvector& {
    pool = source.pool;
    bits = source.bits;
    source.pool = nullptr;
    source.bits = 0;
    return *this;
  }

  explicit operator bool() const { return bits > 0; }
  auto size() const -> uint { return bits; }
  auto bytes() const -> uint { return (bits + 7) / 8; }
  auto data() -> uint8_t* { return pool; }
  auto data() const -> const uint8_t* { return pool; }

  auto reset() -> void {
    if(pool) free(pool);
    pool = nullptr;
    bits = 0;
  }

  auto resize(uint size) -> void {
    uint from = bits;
    bits = size;
    for(uint n = size; n < from; n++) clear(n);  //on reduce
    pool = memory::resize<uint8_t>(pool, bytes());
    for(uint n = from; n < size; n++) clear(n);  //on expand
  }

  auto get(uint position) const -> bool {
    #ifdef DEBUG
    struct out_of_bounds {};
    if(position >= bits) throw out_of_bounds{};
    #endif
    return pool[position >> 3] & (0x80 >> (position & 7));
  }

  auto clear() -> void {
    memory::fill<uint8_t>(pool, bytes(), 0x00);
  }

  auto set() -> void {
    memory::fill<uint8_t>(pool, bytes(), 0xff);
    for(uint n = bits; n < bytes() * 8; n++) clear(n);
  }

  auto clear(uint position) -> void {
    pool[position >> 3] &= ~(0x80 >> (position & 7));
  }

  auto set(uint position) -> void {
    pool[position >> 3] |=  (0x80 >> (position & 7));
  }

  auto invert(uint position) -> void {
    get(position) ? clear(position) : set(position);
  }

  auto set(uint position, bool value) -> void {
    value ? set(position) : clear(position);
  }

  struct reference {
    reference(bitvector& self, uint position) : self(self), position(position) {}
    operator bool() const { return self.get(position); }
    auto operator=(bool value) -> reference& { self.set(position, value); return *this; }

  protected:
    bitvector& self;
    uint position;
  };

  auto operator[](uint position) -> reference {
    return reference(*this, position);
  }

  auto operator[](uint position) const -> bool {
    return get(position);
  }

  struct iterator {
    iterator(bitvector& self, uint position) : self(self), position(position) {}
    auto operator!=(const iterator& source) const -> bool { return position != source.position; }
    auto operator++() -> iterator& { position++; return *this; }
    auto operator*() -> reference { return self.operator[](position); }

  protected:
    bitvector& self;
    uint position;
  };

  auto begin() -> iterator { return iterator(*this, 0); }
  auto end() -> iterator { return iterator(*this, bits); }

protected:
  uint8_t* pool = nullptr;
  uint bits = 0;
};

}
