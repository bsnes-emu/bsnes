#ifndef NALL_VECTOR_HPP
#define NALL_VECTOR_HPP

#include <algorithm>
#include <initializer_list>
#include <new>
#include <utility>
#include <nall/algorithm.hpp>
#include <nall/bit.hpp>
#include <nall/sort.hpp>
#include <nall/utility.hpp>

namespace nall {

template<typename T> struct vector {
  struct exception_out_of_bounds{};

protected:
  T* pool = nullptr;
  unsigned poolbase = 0;
  unsigned poolsize = 0;
  unsigned objectsize = 0;

public:
  explicit operator bool() const { return objectsize; }
  T* data() { return pool + poolbase; }
  const T* data() const { return pool + poolbase; }

  bool empty() const { return objectsize == 0; }
  unsigned size() const { return objectsize; }
  unsigned capacity() const { return poolsize; }

  T* move() {
    T* result = pool + poolbase;
    pool = nullptr;
    poolbase = 0;
    poolsize = 0;
    objectsize = 0;
    return result;
  }

  void reset() {
    if(pool) {
      for(unsigned n = 0; n < objectsize; n++) pool[poolbase + n].~T();
      free(pool);
    }
    pool = nullptr;
    poolbase = 0;
    poolsize = 0;
    objectsize = 0;
  }

  void reserve(unsigned size) {
    if(size <= poolsize) return;
    size = bit::round(size);  //amortize growth

    T* copy = (T*)calloc(size, sizeof(T));
    for(unsigned n = 0; n < objectsize; n++) new(copy + n) T(std::move(pool[poolbase + n]));
    free(pool);
    pool = copy;
    poolbase = 0;
    poolsize = size;
  }

  void resize(unsigned size) {
    T* copy = (T*)calloc(size, sizeof(T));
    for(unsigned n = 0; n < size && n < objectsize; n++) new(copy + n) T(std::move(pool[poolbase + n]));
    reset();
    pool = copy;
    poolbase = 0;
    poolsize = size;
    objectsize = size;
  }

  template<typename... Args> void prepend(const T& data, Args&&... args) {
    prepend(std::forward<Args>(args)...);
    prepend(data);
  }

  T& prepend(const T& data) {
    reserve(objectsize + 1);
    if(poolbase == 0) {
      unsigned available = poolsize - objectsize;
      poolbase = max(1u, available >> 1);
      for(signed n = objectsize - 1; n >= 0; n--) {
        pool[poolbase + n] = std::move(pool[n]);
      }
    }
    new(pool + --poolbase) T(data);
    objectsize++;
    return first();
  }

  template<typename... Args> void append(const T& data, Args&&... args) {
    append(data);
    append(std::forward<Args>(args)...);
  }

  T& append(const T& data) {
    reserve(poolbase + objectsize + 1);
    new(pool + poolbase + objectsize++) T(data);
    return last();
  }

  bool appendOnce(const T& data) {
    if(find(data)) return false;
    return append(data), true;
  }

  void insert(unsigned position, const T& data) {
    if(position == 0) return prepend(data);
    append(data);
    if(position == ~0u) return;
    for(signed n = objectsize - 1; n > position; n--) {
      pool[poolbase + n] = std::move(pool[poolbase + n - 1]);
    }
    pool[poolbase + position] = data;
  }

  void remove(unsigned position = ~0u, unsigned length = 1) {
    if(position == ~0u) position = objectsize - 1;
    if(position + length > objectsize) throw exception_out_of_bounds{};

    if(position == 0) {
      for(unsigned n = 0; n < length; n++) pool[poolbase + n].~T();
      poolbase += length;
    } else {
      for(unsigned n = position; n < objectsize; n++) {
        if(n + length < objectsize) {
          pool[poolbase + n] = std::move(pool[poolbase + n + length]);
        } else {
          pool[poolbase + n].~T();
        }
      }
    }
    objectsize -= length;
  }

  void removeFirst() { return remove(0); }
  void removeLast() { return remove(~0u); }

  T take(unsigned position = ~0u) {
    if(position == ~0u) position = objectsize - 1;
    T object = pool[poolbase + position];
    remove(position);
    return object;
  }

  T takeFirst() { return take(0); }
  T takeLast() { return take(~0u); }

  void reverse() {
    unsigned pivot = size() / 2;
    for(unsigned l = 0, r = size() - 1; l < pivot; l++, r--) {
      std::swap(pool[poolbase + l], pool[poolbase + r]);
    }
  }

  void sort() {
    nall::sort(pool + poolbase, objectsize);
  }

  template<typename Comparator> void sort(const Comparator &lessthan) {
    nall::sort(pool + poolbase, objectsize, lessthan);
  }

  optional<unsigned> find(const T& data) {
    for(unsigned n = 0; n < objectsize; n++) if(pool[poolbase + n] == data) return {true, n};
    return false;
  }

  T& first() {
    if(objectsize == 0) throw exception_out_of_bounds();
    return pool[poolbase];
  }

  const T& first() const {
    if(objectsize == 0) throw exception_out_of_bounds();
    return pool[poolbase];
  }

  T& last() {
    if(objectsize == 0) throw exception_out_of_bounds();
    return pool[poolbase + objectsize - 1];
  }

  const T& last() const {
    if(objectsize == 0) throw exception_out_of_bounds();
    return pool[poolbase + objectsize - 1];
  }

  //access
  inline T& operator[](unsigned position) {
    if(position >= objectsize) throw exception_out_of_bounds();
    return pool[poolbase + position];
  }

  inline const T& operator[](unsigned position) const {
    if(position >= objectsize) throw exception_out_of_bounds();
    return pool[poolbase + position];
  }

  inline T& operator()(unsigned position) {
    if(position >= poolsize) reserve(position + 1);
    while(position >= objectsize) append(T());
    return pool[poolbase + position];
  }

  inline const T& operator()(unsigned position, const T& data) const {
    if(position >= objectsize) return data;
    return pool[poolbase + position];
  }

  //iteration
  struct iterator {
    T& operator*() { return source.operator[](position); }
    bool operator!=(const iterator& source) const { return position != source.position; }
    iterator& operator++() { position++; return *this; }
    iterator(vector& source, unsigned position) : source(source), position(position) {}

  private:
    vector& source;
    unsigned position;
  };

  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, size()); }

  struct constIterator {
    const T& operator*() const { return source.operator[](position); }
    bool operator!=(const constIterator& source) const { return position != source.position; }
    constIterator& operator++() { position++; return *this; }
    constIterator(const vector& source, unsigned position) : source(source), position(position) {}

  private:
    const vector& source;
    unsigned position;
  };

  const constIterator begin() const { return constIterator(*this, 0); }
  const constIterator end() const { return constIterator(*this, size()); }

  //copy
  inline vector& operator=(const vector& source) {
    reset();
    reserve(source.size());
    for(auto& data : source) append(data);
    return *this;
  }

  //move
  inline vector& operator=(vector&& source) {
    reset();
    pool = source.pool;
    poolbase = source.poolbase;
    poolsize = source.poolsize;
    objectsize = source.objectsize;
    source.pool = nullptr;
    source.poolbase = 0;
    source.poolsize = 0;
    source.objectsize = 0;
    return *this;
  }

  //construction and destruction
  vector() = default;
  vector(std::initializer_list<T> list) { for(auto& data : list) append(data); }
  vector(const vector& source) { operator=(source); }
  vector(vector&& source) { operator=(std::move(source)); }
  ~vector() { reset(); }
};

}

#endif
