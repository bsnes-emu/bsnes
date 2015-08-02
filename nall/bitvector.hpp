#ifndef NALL_BITVECTOR_HPP
#define NALL_BITVECTOR_HPP

#include <nall/memory.hpp>

namespace nall {

struct bitvector {
  bitvector() = default;
  bitvector(unsigned size) { resize(size); }
  bitvector(const bitvector& source) { operator=(source); }
  bitvector(bitvector&& source) { operator=(move(source)); }
  ~bitvector() { reset(); }

  auto operator=(const bitvector& source) -> bitvector& {
    bits = source.bits;
    pool = (uint8_t*)memory::resize(pool, bytes());
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
  auto empty() const -> bool { return bits == 0; }
  auto size() const -> unsigned { return bits; }
  auto bytes() const -> unsigned { return (bits + 7) / 8; }
  auto data() -> uint8_t* { return pool; }
  auto data() const -> const uint8_t* { return pool; }

  auto reset() -> void {
    if(pool) free(pool);
    pool = nullptr;
    bits = 0;
  }

  auto resize(unsigned size) -> void {
    unsigned from = bits;
    bits = size;
    for(unsigned n = size; n < from; n++) clear(n);  //on reduce
    pool = (uint8_t*)memory::resize(pool, bytes());
    for(unsigned n = from; n < size; n++) clear(n);  //on expand
  }

  auto get(unsigned position) const -> bool {
    return pool[position >> 3] & (0x80 >> (position & 7));
  }

  auto clear() -> void {
    memory::fill(pool, bytes(), 0x00);
  }

  auto set() -> void {
    memory::fill(pool, bytes(), 0xff);
    for(unsigned n = bits; n < bytes() * 8; n++) clear(n);
  }

  auto clear(unsigned position) -> void {
    pool[position >> 3] &= ~(0x80 >> (position & 7));
  }

  auto set(unsigned position) -> void {
    pool[position >> 3] |=  (0x80 >> (position & 7));
  }

  auto invert(unsigned position) -> void {
    get(position) ? clear(position) : set(position);
  }

  auto set(unsigned position, bool value) -> void {
    value ? set(position) : clear(position);
  }

  struct reference {
    reference(bitvector& self, unsigned position) : self(self), position(position) {}
    operator bool() const { return self.get(position); }
    auto operator=(bool value) -> reference& { self.set(position, value); return *this; }

  protected:
    bitvector& self;
    unsigned position;
  };

  auto operator[](unsigned position) -> reference {
    return reference(*this, position);
  }

  auto operator[](unsigned position) const -> bool {
    return get(position);
  }

  struct iterator {
    iterator(bitvector& self, unsigned position) : self(self), position(position) {}
    auto operator!=(const iterator& source) const -> bool { return position != source.position; }
    auto operator++() -> iterator& { position++; return *this; }
    auto operator*() -> reference { return self.operator[](position); }

  protected:
    bitvector& self;
    unsigned position;
  };

  auto begin() -> iterator { return iterator(*this, 0); }
  auto end() -> iterator { return iterator(*this, bits); }

protected:
  uint8_t* pool = nullptr;
  unsigned bits = 0;
};

}

#endif
