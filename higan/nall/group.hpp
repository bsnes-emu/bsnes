#ifndef NALL_GROUP_HPP
#define NALL_GROUP_HPP

//group: a vector of unique references

#include <stdlib.h>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <nall/bit.hpp>
#include <nall/traits.hpp>

namespace nall {

template<typename TR> struct group {
  struct exception_out_of_bounds{};
  typedef typename std::remove_reference<TR>::type T;

protected:
  T** pool = nullptr;
  unsigned poolsize = 0;
  unsigned objectsize = 0;

public:
  unsigned size() const { return objectsize; }
  unsigned capacity() const { return poolsize; }

  void reset() {
    if(pool) free(pool);
    pool = nullptr;
    poolsize = 0;
    objectsize = 0;
  }

  void reserve(unsigned size) {
    if(size == poolsize) return;
    pool = (T**)realloc(pool, sizeof(T*) * size);
    poolsize = size;
    objectsize = min(objectsize, size);
  }

  void resize(unsigned size) {
    if(size > poolsize) reserve(bit::round(size));  //amortize growth
    objectsize = size;
  }

  bool append(T& data) {
    if(find(data)) return false;
    unsigned offset = objectsize++;
    if(offset >= poolsize) resize(offset + 1);
    pool[offset] = &data;
    return true;
  }

  template<typename... Args>
  bool append(T& data, Args&&... args) {
    bool result = append(data);
    append(std::forward<Args>(args)...);
    return result;
  }

  bool remove(T& data) {
    if(auto position = find(data)) {
      for(signed i = position(); i < objectsize - 1; i++) pool[i] = pool[i + 1];
      resize(objectsize - 1);
      return true;
    }
    return false;
  }

  optional<unsigned> find(const T& data) {
    for(unsigned n = 0; n < objectsize; n++) if(pool[n] == &data) return {true, n};
    return {false, 0u};
  }

  template<typename... Args> group(Args&&... args) {
    construct(std::forward<Args>(args)...);
  }

  ~group() {
    reset();
  }

  group& operator=(const group& source) {
    if(&source == this) return *this;
    reset();
    reserve(source.poolsize);
    resize(source.objectsize);
    memcpy(pool, source.pool, sizeof(T*) * objectsize);
    return *this;
  }

  group& operator=(const group&& source) {
    if(&source == this) return *this;
    reset();
    pool = source.pool;
    poolsize = source.poolsize;
    objectsize = source.objectsize;
    source.pool = nullptr;
    source.reset();
    return *this;
  }

  T& operator[](unsigned position) const {
    if(position >= objectsize) throw exception_out_of_bounds();
    return *pool[position];
  }

  struct iterator {
    bool operator!=(const iterator& source) const { return position != source.position; }
    T& operator*() { return source.operator[](position); }
    iterator& operator++() { position++; return *this; }
    iterator(const group& source, unsigned position) : source(source), position(position) {}
  private:
    const group& source;
    unsigned position;
  };

  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, objectsize); }
  const iterator begin() const { return iterator(*this, 0); }
  const iterator end() const { return iterator(*this, objectsize); }

private:
  void construct() {}
  void construct(const group& source) { operator=(source); }
  void construct(const group&& source) { operator=(std::move(source)); }
  template<typename... Args> void construct(T& data, Args&&... args) {
    append(data);
    construct(std::forward<Args>(args)...);
  }
};

}

#endif
