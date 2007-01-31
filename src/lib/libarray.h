/*
  libarray : version 0.08 ~byuu (2006-12-16)
*/

#ifndef __LIBARRAY
#define __LIBARRAY

template<typename T> class array {
protected:
T *pool;
uint poolsize, buffersize;

protected:
  uint findsize(uint size) {
    if(size <=    0x100) { return    0x100; }
    if(size <=    0x400) { return    0x400; }
    if(size <=   0x1000) { return   0x1000; }
    if(size <=   0x4000) { return   0x4000; }
    if(size <=  0x10000) { return  0x10000; }
    if(size <=  0x40000) { return  0x40000; }
    if(size <= 0x100000) { return 0x100000; }

    return (size + 0x100000) & ~0xfffff;
  }

public:
  uint size() { return buffersize; }
  uint capacity() { return poolsize; }

  void reset() {
    safe_free(pool);
    poolsize = 0;
    buffersize = 0;
  }

  void reserve(uint size) {
    if(size == poolsize)return;

    if(size < poolsize) {
      buffersize = size;
    }

    pool = (T*)realloc(pool, sizeof(T) * size);
    poolsize = size;
  }

  T *get(uint size = 0) {
    if(size > buffersize)resize(size);
    if(size > buffersize)throw "array[] out of bounds";
    return pool;
  }

  void clear() { memset(pool, 0, sizeof(T) * buffersize); }

  void resize(uint size) {
    reserve(findsize(size));
    buffersize = size;
  }

  array() {
    pool = 0;
    poolsize = 0;
    buffersize = 0;
  }

  ~array() { reset(); }

  array &operator=(array &source) {
    safe_free(pool);
    buffersize = source.buffersize;
    poolsize = source.poolsize;
  //allocate entire pool size ...
    pool = (T*)realloc(pool, sizeof(T) * poolsize);
  //... but only copy used pool objects
    memcpy(pool, source.pool, sizeof(T) * buffersize);
    return *this;
  }

  inline T &operator[](int index) {
    if(index >= buffersize)resize(index + 1);
    if(index >= buffersize)throw "array[] out of bounds";
    return pool[index];
  }
};

#endif
