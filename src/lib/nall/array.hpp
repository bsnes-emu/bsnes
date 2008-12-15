#ifndef NALL_ARRAY_HPP
#define NALL_ARRAY_HPP

#include <stdlib.h>
#include <nall/algorithm.hpp>

namespace nall {

//dynamic vector array
//neither constructor nor destructor is ever invoked;
//this this should only be used for POD objects.

template<typename T> class array {
protected:
  T *pool;
  unsigned poolsize, buffersize;

  unsigned findsize(unsigned size) const {
    if(size <=    0x100) return    0x100;
    if(size <=    0x400) return    0x400;
    if(size <=   0x1000) return   0x1000;
    if(size <=   0x4000) return   0x4000;
    if(size <=  0x10000) return  0x10000;
    if(size <=  0x40000) return  0x40000;
    if(size <= 0x100000) return 0x100000;

    return (size + 0x100000) & ~0xfffff;
  }

public:
  unsigned size() const { return buffersize; }
  unsigned capacity() const { return poolsize; }

  void reset() {
    if(pool) free(pool);
    pool = 0;
    poolsize = 0;
    buffersize = 0;
  }

  void reserve(unsigned newsize) {
    if(newsize == poolsize) return;

    pool = (T*)realloc(pool, newsize * sizeof(T));
    poolsize = newsize;
    buffersize = min(buffersize, newsize);
  }

  void resize(unsigned newsize) {
    if(newsize > poolsize) reserve(findsize(newsize));
    buffersize = newsize;
  }

  T* get(unsigned minsize = 0) {
    if(minsize > buffersize) resize(minsize);
    if(minsize > buffersize) throw "array[] out of bounds";
    return pool;
  }

  void add(const T data) { operator[](buffersize) = data; }
  void clear() { memset(pool, 0, buffersize * sizeof(T)); }

  array() {
    pool = 0;
    poolsize = 0;
    buffersize = 0;
  }

  ~array() { reset(); }

  array& operator=(const array &source) {
    if(pool) free(pool);
    buffersize = source.buffersize;
    poolsize = source.poolsize;
    pool = (T*)realloc(pool, sizeof(T) * poolsize);     //allocate entire pool size,
    memcpy(pool, source.pool, sizeof(T) * buffersize);  //... but only copy used pool objects
    return *this;
  }

  inline T& operator[](unsigned index) {
    if(index >= buffersize) resize(index + 1);
    if(index >= buffersize) throw "array[] out of bounds";
    return pool[index];
  }

  inline const T& operator[](unsigned index) const {
    if(index >= buffersize) throw "array[] out of bounds";
    return pool[index];
  }
};

} //namespace nall

#endif //ifndef NALL_ARRAY_HPP
