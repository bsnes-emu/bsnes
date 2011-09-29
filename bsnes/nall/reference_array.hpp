#ifndef NALL_REFERENCE_ARRAY_HPP
#define NALL_REFERENCE_ARRAY_HPP

#include <algorithm>
#include <type_traits>
#include <nall/bit.hpp>

namespace nall {
  template<typename T> struct reference_array {
  protected:
    typedef typename std::remove_reference<T>::type *Tptr;
    Tptr *pool;
    unsigned poolsize, buffersize;

  public:
    unsigned size() const { return buffersize; }
    unsigned capacity() const { return poolsize; }

    void reset() {
      if(pool) free(pool);
      pool = nullptr;
      poolsize = 0;
      buffersize = 0;
    }

    void reserve(unsigned newsize) {
      if(newsize == poolsize) return;

      pool = (Tptr*)realloc(pool, newsize * sizeof(T));
      poolsize = newsize;
      buffersize = min(buffersize, newsize);
    }

    void resize(unsigned newsize) {
      if(newsize > poolsize) reserve(bit::round(newsize));
      buffersize = newsize;
    }

    bool append(const T data) {
      for(unsigned index = 0; index < buffersize; index++) {
        if(pool[index] == &data) return false;
      }

      unsigned index = buffersize++;
      if(index >= poolsize) resize(index + 1);
      pool[index] = &data;
      return true;
    }

    bool remove(const T data) {
      for(unsigned index = 0; index < buffersize; index++) {
        if(pool[index] == &data) {
          for(unsigned i = index; i < buffersize - 1; i++) pool[i] = pool[i + 1];
          resize(buffersize - 1);
          return true;
        }
      }
      return false;
    }

    template<typename... Args> reference_array(Args&... args) : pool(nullptr), poolsize(0), buffersize(0) {
      construct(args...);
    }

    ~reference_array() {
      reset();
    }

    reference_array& operator=(const reference_array &source) {
      if(pool) free(pool);
      buffersize = source.buffersize;
      poolsize = source.poolsize;
      pool = (Tptr*)malloc(sizeof(T) * poolsize);
      memcpy(pool, source.pool, sizeof(T) * buffersize);
      return *this;
    }

    reference_array& operator=(const reference_array &&source) {
      if(pool) free(pool);
      pool = source.pool;
      poolsize = source.poolsize;
      buffersize = source.buffersize;
      source.pool = nullptr;
      source.reset();
      return *this;
    }

    inline T operator[](unsigned index) {
      if(index >= buffersize) throw "reference_array[] out of bounds";
      return *pool[index];
    }

    inline const T operator[](unsigned index) const {
      if(index >= buffersize) throw "reference_array[] out of bounds";
      return *pool[index];
    }

    //iteration
    struct iterator {
      bool operator!=(const iterator &source) const { return index != source.index; }
      T& operator*() { return array.operator[](index); }
      iterator& operator++() { index++; return *this; }
      iterator(const reference_array &array, unsigned index) : array(array), index(index) {}
    private:
      const reference_array &array;
      unsigned index;
    };

    iterator begin() { return iterator(*this, 0); }
    iterator end() { return iterator(*this, buffersize); }
    const iterator begin() const { return iterator(*this, 0); }
    const iterator end() const { return iterator(*this, buffersize); }

  private:
    void construct() {
    }

    void construct(const reference_array &source) {
      operator=(source);
    }

    void construct(const reference_array &&source) {
      operator=(std::move(source));
    }

    template<typename... Args> void construct(T data, Args&... args) {
      append(data);
      construct(args...);
    }
  };
}

#endif
