#ifndef NALL_VECTOR_HPP
#define NALL_VECTOR_HPP

#include <new>

namespace nall {

/*****
 * Dynamic vector allocation template class library
 *
 * Implements auto-allocating, auto-resizing vectors that allow for
 * theoretically infinitely-sized arrays of objects.
 * Array indexes are not bounds checked, and reallocate to requested size when
 * accessing out of bounds.
 * These class are intended only for use with objects that require
 * construction. Significant overhead will result if used for raw memory
 * management. See libarray for a far more suitable raw memory vector array
 * class.
 *
 * Vectors also reserve an internal pool of memory that can be larger to
 * minimize memory allocation requests, but by default the pools only grow to
 * the requested vector index, as some objects may have substantial memory
 * requirements per object. The reserve() function may be used to allocate
 * memory in advance when possible.
 *
 * Two types of vectors are implemented. Their public interfaces are identical,
 * however they operate very differently internally.
 *
 * [linear_vector]
 * Works by allocating one giant heap of memory to contain all objects.
 * Constructors and destructors are called during resize() as needed.
 *
 * Pros:
 * - memory is allocated and objects are prebuilt in advance, so accessing
 *   array objects will be very fast.
 * - all objects are in contigious memory, which helps for processor caching.
 * Cons:
 * - accessing object 15 will require objects 0-14 to be constructed first.
 * - resizing array may invalidate pointers to member objects inside classes
 *   (eg object "this" pointer may change).
 *
 * Use when:
 * You need an array of objects that do not rely on pointers to member
 * variables, you have an idea of how many objects you will need, and
 * speed is critical.
 *
 * [ptr_vector]
 * Works by allocating a smaller heap of memory used as an array of pointers to
 * individual objects.
 *
 * Pros:
 * - accessing object 15 does not require objects 0-14 to be constructed first.
 * - resizing array will not invalidate pointers to member objects inside
 *   classes.
 * Cons:
 * - memory for individual objects cannot be allocated in advance. Each "first"
 *   access to an array object will require memory allocation and object
 *   construction.
 * - objects are not guaranteed to be aligned in contigious memory, and
 *   individual array indexes require two indirect memory lookups. Will affect
 *   speed adversely when indexing very large arrays of objects.
 *
 * Use when:
 * Objects rely on pointers to member variables, you do not know how many
 * objects you will need in advance, you need to access vector elements out of
 * order and prefer not to have all previous objects constructed in advance,
 * and speed is less critical.
 *****/

template<typename T> class linear_vector {
protected:
  T *pool;
  unsigned poolsize, objectsize;

public:
  unsigned size() const { return objectsize; }
  unsigned capacity() const { return poolsize; }

  void reset() {
    for(unsigned i = 0; i < objectsize; i++) { pool[i].~T(); }

    if(pool) {
      free(pool);
      pool = 0;
    }

    poolsize = 0;
    objectsize = 0;
  }

  void reserve(unsigned size) {
    if(size == poolsize)return;

    if(size < poolsize) {
      for(unsigned i = size; i < objectsize; i++) { pool[i].~T(); }
      objectsize = size;
    }

    pool = (T*)realloc(pool, sizeof(T) * size);
    poolsize = size;
  }

  void resize(int size) {
    if(size == objectsize)return;
    if(size > poolsize)reserve(size);

    if(size < objectsize) {
      for(unsigned i = size; i < objectsize; i++) { pool[i].~T(); }
    } else {
      for(unsigned i = objectsize; i < size; i++) { new(pool + i) T; }
    }

    objectsize = size;
  }

  linear_vector() {
    pool = 0;
    poolsize = 0;
    objectsize = 0;
  }

  ~linear_vector() { reset(); }

  inline operator T&() {
    if(objectsize == 0)resize(1);
    if(objectsize == 0)throw "vector[] out of bounds";
    return pool[0];
  }

  inline T &operator[](int index) {
    if(index >= objectsize)resize(index + 1);
    if(index >= objectsize)throw "vector[] out of bounds";
    return pool[index];
  }
};

template<typename T> class ptr_vector {
protected:
  T **pool;
  unsigned poolsize, objectsize;

public:
  unsigned size() const { return objectsize; }
  unsigned capacity() const { return poolsize; }

  void reset() {
    for(unsigned i = 0; i < objectsize; i++) { if(pool[i])delete pool[i]; }

    if(pool) {
      free(pool);
      pool = 0;
    }

    poolsize = 0;
    objectsize = 0;
  }

  void reserve(unsigned size) {
    if(size == poolsize)return;

    if(size < poolsize) {
      for(unsigned i = size; i < objectsize; i++) { if(pool[i])delete pool[i]; }
      objectsize = size;
    }

    pool = (T**)realloc(pool, sizeof(T*) * size);
    if(size > poolsize) {
      memset(pool + poolsize, 0, sizeof(T*) * (size - poolsize));
    }
    poolsize = size;
  }

  void resize(int size) {
    if(size == objectsize)return;
    if(size > poolsize)reserve(size);

    if(size < objectsize) {
      for(unsigned i = size; i < objectsize; i++) { if(pool[i])delete pool[i]; }
    }

    objectsize = size;
  }

  ptr_vector() {
    pool = 0;
    poolsize = 0;
    objectsize = 0;
  }

  ~ptr_vector() { reset(); }

  inline operator T&() {
    if(objectsize == 0)resize(1);
    if(objectsize == 0)throw "vector[] out of bounds";
    if(!pool[0])pool[0] = new T;
    return *pool[0];
  }

  inline T &operator[](int index) {
    if(index >= objectsize)resize(index + 1);
    if(index >= objectsize)throw "vector[] out of bounds";
    if(!pool[index])pool[index] = new T;
    return *pool[index];
  }
};

template<typename T> class vector : public linear_vector<T> {};

} //namespace nall

#endif //ifndef NALL_VECTOR_HPP
