#ifndef NALL_BITARRAY_HPP
#define NALL_BITARRAY_HPP

#include <nall/stdint.hpp>

//statically-sized bit array
//no bounds-checking on read/write
//packed into uint8_t array (8 bits per byte)

namespace nall {

struct bitarray {
  uint8_t *pool;
  unsigned poolsize;

  uint8_t* data() { return pool; }
  const uint8_t* data() const { return pool; }
  unsigned size() const { return poolsize; }
  unsigned bytesize() const { return (poolsize >> 3) + ((poolsize & 7) > 0); }

  void reset() {
    if(pool) free(pool);
    pool = nullptr;
    poolsize = 0u;
  }

  void resize(unsigned allocsize) {
    if(allocsize == poolsize) return;
    pool = (uint8_t*)realloc(pool, allocsize);
    poolsize = allocsize;
  }

  bool operator[](unsigned offset) const {
    return pool[offset >> 3] & (0x80 >> (offset & 7));
  }

  void set() {
    memset(pool, 0xff, (poolsize >> 3) + ((poolsize & 7) > 0));
  }

  void set(unsigned offset) {
    pool[offset >> 3] |= 0x80 >> (offset & 7);
  }

  void clear() {
    memset(pool, 0, (poolsize >> 3) + ((poolsize & 7) > 0));
  }

  void clear(unsigned offset) {
    pool[offset >> 3] &=~0x80 >> (offset & 7);
  }

  void set(unsigned offset, bool data) {
    data ? set(offset) : clear(offset);
  }

  struct bit {
    bitarray &array;
    unsigned offset;
    operator bool() const { return const_cast<const bitarray&>(array)[offset]; }
    bit& operator=(bool data) { array.set(offset, data); return *this; }
    bit& operator=(const bit& data) { return operator=((bool)data); }
    bit(bitarray &array, unsigned offset) : array(array), offset(offset) {}
  };

  bit operator[](unsigned offset) {
    return bit(*this, offset);
  }

  bitarray() : pool(nullptr), poolsize(0u) {}
  bitarray(unsigned allocsize) {
    pool = (uint8_t*)malloc((allocsize >> 3) + ((allocsize & 7) > 0));
    poolsize = allocsize;
  }
};

}

#endif
