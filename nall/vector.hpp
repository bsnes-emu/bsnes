#ifndef NALL_VECTOR_HPP
#define NALL_VECTOR_HPP

#include <algorithm>
#include <initializer_list>
#include <new>
#include <utility>
#include <nall/algorithm.hpp>
#include <nall/bit.hpp>
#include <nall/maybe.hpp>
#include <nall/memory.hpp>
#include <nall/sort.hpp>
#include <nall/utility.hpp>

namespace nall {

template<typename T> struct vector {
  struct exception_out_of_bounds{};

  explicit operator bool() const { return objectsize; }
  auto data() -> T* { return pool + poolbase; }
  auto data() const -> const T* { return pool + poolbase; }

  auto empty() const -> bool { return objectsize == 0; }
  auto size() const -> unsigned { return objectsize; }
  auto capacity() const -> unsigned { return poolsize; }

  auto release() -> T* {
    T* result = pool + poolbase;
    pool = nullptr;
    poolbase = 0;
    poolsize = 0;
    objectsize = 0;
    return result;
  }

  auto reset() -> void {
    if(pool) {
      for(unsigned n = 0; n < objectsize; n++) pool[poolbase + n].~T();
      memory::free(pool);
    }
    pool = nullptr;
    poolbase = 0;
    poolsize = 0;
    objectsize = 0;
  }

  auto reserve(unsigned size) -> void {
    if(size <= poolsize) return;
    size = bit::round(size);  //amortize growth

    T* copy = (T*)memory::allocate(size * sizeof(T));
    for(unsigned n = 0; n < objectsize; n++) new(copy + n) T(move(pool[poolbase + n]));
    free(pool);
    pool = copy;
    poolbase = 0;
    poolsize = size;
  }

  auto resize(unsigned size, T value = T()) -> void {
    T* copy = (T*)memory::allocate(size * sizeof(T));
    for(unsigned n = 0; n < size && n < objectsize; n++) new(copy + n) T(move(pool[poolbase + n]));
    for(unsigned n = objectsize; n < size; n++) new(copy + n) T(value);
    reset();
    pool = copy;
    poolbase = 0;
    poolsize = size;
    objectsize = size;
  }

  auto reallocate(unsigned size, T value = T()) -> void {
    reset();
    resize(size, value);
  }

  template<typename... Args> auto prepend(const T& data, Args&&... args) -> void {
    prepend(forward<Args>(args)...);
    prepend(data);
  }

  auto prepend(const T& data) -> T& {
    reserve(objectsize + 1);
    if(poolbase == 0) {
      unsigned available = poolsize - objectsize;
      poolbase = max(1u, available >> 1);
      for(signed n = objectsize - 1; n >= 0; n--) {
        pool[poolbase + n] = move(pool[n]);
      }
    }
    new(pool + --poolbase) T(data);
    objectsize++;
    return first();
  }

  template<typename... Args> auto append(const T& data, Args&&... args) -> void {
    append(data);
    append(forward<Args>(args)...);
  }

  auto append(const T& data) -> T& {
    reserve(poolbase + objectsize + 1);
    new(pool + poolbase + objectsize++) T(data);
    return last();
  }

  auto appendOnce(const T& data) -> bool {
    if(find(data)) return false;
    return append(data), true;
  }

  auto insert(unsigned position, const T& data) -> void {
    if(position == 0) {
      prepend(data);
      return;
    }
    append(data);
    if(position == ~0u) return;
    for(signed n = objectsize - 1; n > position; n--) {
      pool[poolbase + n] = move(pool[poolbase + n - 1]);
    }
    pool[poolbase + position] = data;
  }

  auto remove(unsigned position = ~0u, unsigned length = 1) -> void {
    if(position == ~0u) position = objectsize - 1;
    if(position + length > objectsize) throw exception_out_of_bounds{};

    if(position == 0) {
      for(unsigned n = 0; n < length; n++) pool[poolbase + n].~T();
      poolbase += length;
    } else {
      for(unsigned n = position; n < objectsize; n++) {
        if(n + length < objectsize) {
          pool[poolbase + n] = move(pool[poolbase + n + length]);
        } else {
          pool[poolbase + n].~T();
        }
      }
    }
    objectsize -= length;
  }

  auto removeFirst() -> void { return remove(0); }
  auto removeLast() -> void { return remove(~0u); }

  auto take(unsigned position = ~0u) -> T {
    if(position == ~0u) position = objectsize - 1;
    T object = pool[poolbase + position];
    remove(position);
    return object;
  }

  auto takeFirst() -> T { return take(0); }
  auto takeLast() -> T { return take(~0u); }

  auto reverse() -> void {
    unsigned pivot = size() / 2;
    for(unsigned l = 0, r = size() - 1; l < pivot; l++, r--) {
      swap(pool[poolbase + l], pool[poolbase + r]);
    }
  }

  auto sort() -> void {
    nall::sort(pool + poolbase, objectsize);
  }

  template<typename Comparator> auto sort(const Comparator& lessthan) -> void {
    nall::sort(pool + poolbase, objectsize, lessthan);
  }

  auto find(const T& data) const -> maybe<unsigned> {
    for(unsigned n = 0; n < objectsize; n++) if(pool[poolbase + n] == data) return n;
    return nothing;
  }

  auto first() -> T& {
    if(objectsize == 0) throw exception_out_of_bounds();
    return pool[poolbase];
  }

  auto first() const -> const T& {
    if(objectsize == 0) throw exception_out_of_bounds();
    return pool[poolbase];
  }

  auto last() -> T& {
    if(objectsize == 0) throw exception_out_of_bounds();
    return pool[poolbase + objectsize - 1];
  }

  auto last() const -> const T& {
    if(objectsize == 0) throw exception_out_of_bounds();
    return pool[poolbase + objectsize - 1];
  }

  //access
  inline auto operator[](unsigned position) -> T& {
    if(position >= objectsize) throw exception_out_of_bounds();
    return pool[poolbase + position];
  }

  inline auto operator[](unsigned position) const -> const T& {
    if(position >= objectsize) throw exception_out_of_bounds();
    return pool[poolbase + position];
  }

  inline auto operator()(unsigned position) -> T& {
    if(position >= poolsize) reserve(position + 1);
    while(position >= objectsize) append(T());
    return pool[poolbase + position];
  }

  inline auto operator()(unsigned position, const T& data) const -> const T& {
    if(position >= objectsize) return data;
    return pool[poolbase + position];
  }

  //iteration
  struct iterator {
    iterator(vector& source, unsigned position) : source(source), position(position) {}
    auto operator*() -> T& { return source.operator[](position); }
    auto operator!=(const iterator& source) const -> bool { return position != source.position; }
    auto operator++() -> iterator& { position++; return *this; }

  private:
    vector& source;
    unsigned position;
  };

  auto begin() -> iterator { return iterator(*this, 0); }
  auto end() -> iterator { return iterator(*this, size()); }

  struct constIterator {
    constIterator(const vector& source, unsigned position) : source(source), position(position) {}
    auto operator*() const -> const T& { return source.operator[](position); }
    auto operator!=(const constIterator& source) const -> bool { return position != source.position; }
    auto operator++() -> constIterator& { position++; return *this; }

  private:
    const vector& source;
    unsigned position;
  };

  auto begin() const -> const constIterator { return constIterator(*this, 0); }
  auto end() const -> const constIterator { return constIterator(*this, size()); }

  //copy
  inline auto operator=(const vector& source) -> vector& {
    if(this == &source) return *this;
    reset();
    reserve(source.size());
    for(auto& data : source) append(data);
    return *this;
  }

  //move
  inline auto operator=(vector&& source) -> vector& {
    if(this == &source) return *this;
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
  vector(initializer_list<T> list) { for(auto& data : list) append(data); }
  vector(const vector& source) { operator=(source); }
  vector(vector&& source) { operator=(move(source)); }
  ~vector() { reset(); }

protected:
  T* pool = nullptr;
  unsigned poolbase = 0;
  unsigned poolsize = 0;
  unsigned objectsize = 0;
};

}

#endif
