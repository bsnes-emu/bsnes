#ifndef NALL_VECTOR_HPP
#define NALL_VECTOR_HPP

#include <initializer_list>
#include <new>
#include <type_traits>
#include <utility>
#include <nall/algorithm.hpp>
#include <nall/bit.hpp>
#include <nall/concept.hpp>
#include <nall/utility.hpp>

namespace nall {
  //linear_vector
  //memory: O(capacity * 2)
  //
  //linear_vector uses placement new + manual destructor calls to create a
  //contiguous block of memory for all objects. accessing individual elements
  //is fast, though resizing the array incurs significant overhead.
  //reserve() overhead is reduced from quadratic time to amortized constant time
  //by resizing twice as much as requested.
  //
  //if objects hold memory address references to themselves (introspection), a
  //valid copy constructor will be needed to keep pointers valid.

  template<typename T> class linear_vector {
  protected:
    T *pool;
    unsigned poolsize, objectsize;

  public:
    unsigned size() const { return objectsize; }
    unsigned capacity() const { return poolsize; }

    void reset() {
      if(pool) {
        for(unsigned i = 0; i < objectsize; i++) pool[i].~T();
        free(pool);
      }
      pool = 0;
      poolsize = 0;
      objectsize = 0;
    }

    void reserve(unsigned newsize) {
      newsize = bit::round(newsize);  //round to nearest power of two (for amortized growth)

      T *poolcopy = (T*)malloc(newsize * sizeof(T));
      for(unsigned i = 0; i < min(objectsize, newsize); i++) new(poolcopy + i) T(pool[i]);
      for(unsigned i = 0; i < objectsize; i++) pool[i].~T();
      free(pool);
      pool = poolcopy;
      poolsize = newsize;
      objectsize = min(objectsize, newsize);
    }

    void resize(unsigned newsize) {
      if(newsize > poolsize) reserve(newsize);

      if(newsize < objectsize) {
        //vector is shrinking; destroy excess objects
        for(unsigned i = newsize; i < objectsize; i++) pool[i].~T();
      } else if(newsize > objectsize) {
        //vector is expanding; allocate new objects
        for(unsigned i = objectsize; i < newsize; i++) new(pool + i) T;
      }

      objectsize = newsize;
    }

    void add(const T data) {
      if(objectsize + 1 > poolsize) reserve(objectsize + 1);
      new(pool + objectsize++) T(data);
    }

    inline T& operator[](unsigned index) {
      if(index >= objectsize) resize(index + 1);
      return pool[index];
    }

    inline const T& operator[](unsigned index) const {
      if(index >= objectsize) throw "vector[] out of bounds";
      return pool[index];
    }

    //copy
    inline linear_vector<T>& operator=(const linear_vector<T> &source) {
      reset();
      reserve(source.capacity());
      resize(source.size());
      for(unsigned i = 0; i < source.size(); i++) operator[](i) = source.operator[](i);
      return *this;
    }

    linear_vector(const linear_vector<T> &source) : pool(0), poolsize(0), objectsize(0) {
      operator=(source);
    }

    //move
    inline linear_vector<T>& operator=(linear_vector<T> &&source) {
      reset();
      pool = source.pool;
      poolsize = source.poolsize;
      objectsize = source.objectsize;
      source.pool = 0;
      source.reset();
      return *this;
    }

    linear_vector(linear_vector<T> &&source) : pool(0), poolsize(0), objectsize(0) {
      operator=(std::move(source));
    }

    //construction
    linear_vector() : pool(0), poolsize(0), objectsize(0) {
    }

    linear_vector(std::initializer_list<T> list) : pool(0), poolsize(0), objectsize(0) {
      for(const T *p = list.begin(); p != list.end(); ++p) add(*p);
    }

    ~linear_vector() {
      reset();
    }
  };

  //pointer_vector
  //memory: O(1)
  //
  //pointer_vector keeps an array of pointers to each vector object. this adds
  //significant overhead to individual accesses, but allows for optimal memory
  //utilization.
  //
  //by guaranteeing that the base memory address of each objects never changes,
  //this avoids the need for an object to have a valid copy constructor.

  template<typename T> class pointer_vector {
  protected:
    T **pool;
    unsigned poolsize, objectsize;

  public:
    unsigned size() const { return objectsize; }
    unsigned capacity() const { return poolsize; }

    void reset() {
      if(pool) {
        for(unsigned i = 0; i < objectsize; i++) { if(pool[i]) delete pool[i]; }
        free(pool);
      }
      pool = 0;
      poolsize = 0;
      objectsize = 0;
    }

    void reserve(unsigned newsize) {
      newsize = bit::round(newsize);  //round to nearest power of two (for amortized growth)

      for(unsigned i = newsize; i < objectsize; i++) {
        if(pool[i]) { delete pool[i]; pool[i] = 0; }
      }

      pool = (T**)realloc(pool, newsize * sizeof(T*));
      for(unsigned i = poolsize; i < newsize; i++) pool[i] = 0;
      poolsize = newsize;
      objectsize = min(objectsize, newsize);
    }

    void resize(unsigned newsize) {
      if(newsize > poolsize) reserve(newsize);

      for(unsigned i = newsize; i < objectsize; i++) {
        if(pool[i]) { delete pool[i]; pool[i] = 0; }
      }

      objectsize = newsize;
    }

    void add(const T data) {
      if(objectsize + 1 > poolsize) reserve(objectsize + 1);
      pool[objectsize++] = new T(data);
    }

    inline T& operator[](unsigned index) {
      if(index >= objectsize) resize(index + 1);
      if(!pool[index]) pool[index] = new T;
      return *pool[index];
    }

    inline const T& operator[](unsigned index) const {
      if(index >= objectsize || !pool[index]) throw "vector[] out of bounds";
      return *pool[index];
    }

    //copy
    inline pointer_vector<T>& operator=(const pointer_vector<T> &source) {
      reset();
      reserve(source.capacity());
      resize(source.size());
      for(unsigned i = 0; i < source.size(); i++) operator[](i) = source.operator[](i);
      return *this;
    }

    pointer_vector(const pointer_vector<T> &source) : pool(0), poolsize(0), objectsize(0) {
      operator=(source);
    }

    //move
    inline pointer_vector<T>& operator=(pointer_vector<T> &&source) {
      reset();
      pool = source.pool;
      poolsize = source.poolsize;
      objectsize = source.objectsize;
      source.pool = 0;
      source.reset();
      return *this;
    }

    pointer_vector(pointer_vector<T> &&source) : pool(0), poolsize(0), objectsize(0) {
      operator=(std::move(source));
    }

    //construction
    pointer_vector() : pool(0), poolsize(0), objectsize(0) {
    }

    pointer_vector(std::initializer_list<T> list) : pool(0), poolsize(0), objectsize(0) {
      for(const T *p = list.begin(); p != list.end(); ++p) add(*p);
    }

    ~pointer_vector() {
      reset();
    }
  };

  template<typename T> struct has_size<linear_vector<T>> { enum { value = true }; };
  template<typename T> struct has_size<pointer_vector<T>> { enum { value = true }; };
}

#endif
