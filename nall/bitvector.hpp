#ifndef NALL_BITVECTOR_HPP
#define NALL_BITVECTOR_HPP

namespace nall {

struct bitvector {
protected:
  uint8_t* pool = nullptr;
  unsigned bits = 0;

public:
  bitvector() {}
  bitvector(unsigned size) { resize(size); }
  bitvector(const bitvector& source) { operator=(source); }
  bitvector(bitvector&& source) { operator=(std::move(source)); }
  ~bitvector() { reset(); }

  bitvector& operator=(const bitvector& source) {
    bits = source.bits;
    pool = (uint8_t*)realloc(pool, bytes());
    memcpy(pool, source.pool, bytes());
    return *this;
  }

  bitvector& operator=(bitvector&& source) {
    pool = source.pool;
    bits = source.bits;
    source.pool = nullptr;
    source.bits = 0;
    return *this;
  }

  explicit operator bool() const { return bits > 0; }
  bool empty() const { return bits == 0; }
  unsigned size() const { return bits; }
  unsigned bytes() const { return (bits + 7) / 8; }
  uint8_t* data() { return pool; }
  const uint8_t* data() const { return pool; }

  void reset() {
    if(pool) free(pool);
    pool = nullptr;
    bits = 0;
  }

  void resize(unsigned size) {
    unsigned from = bits;
    bits = size;
    for(unsigned n = size; n < from; n++) clear(n);  //on reduce
    pool = (uint8_t*)realloc(pool, bytes());
    for(unsigned n = from; n < size; n++) clear(n);  //on expand
  }

  bool get(unsigned position) const {
    return pool[position >> 3] & (0x80 >> (position & 7));
  }

  void clear() {
    memset(pool, 0x00, bytes());
  }

  void set() {
    memset(pool, 0xff, bytes());
    for(unsigned n = bits; n < bytes() * 8; n++) clear(n);
  }

  void clear(unsigned position) {
    pool[position >> 3] &= ~(0x80 >> (position & 7));
  }

  void set(unsigned position) {
    pool[position >> 3] |=  (0x80 >> (position & 7));
  }

  void invert(unsigned position) {
    get(position) ? clear(position) : set(position);
  }

  void set(unsigned position, bool value) {
    value ? set(position) : clear(position);
  }

  struct reference {
    reference(bitvector& self, unsigned position) : self(self), position(position) {}
    operator bool() const { return self.get(position); }
    void operator=(bool value) { self.set(position, value); }

  protected:
    bitvector& self;
    unsigned position;
  };

  reference operator[](unsigned position) {
    return reference(*this, position);
  }

  bool operator[](unsigned position) const {
    return get(position);
  }

  struct iterator {
    iterator(bitvector& self, unsigned position) : self(self), position(position) {}
    bool operator!=(const iterator& source) const { return position != source.position; }
    iterator& operator++() { position++; return *this; }
    reference operator*() { return self.operator[](position); }

  protected:
    bitvector& self;
    unsigned position;
  };

  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, bits); }
};

}

#endif
