#ifndef NALL_HASHSET_HPP
#define NALL_HASHSET_HPP

//hashset
//
//search: O(1) average; O(n) worst
//insert: O(1) average; O(n) worst
//remove: O(1) average; O(n) worst
//
//requirements:
//  unsigned T::hash() const;
//  bool T::operator==(const T&) const;

namespace nall {

template<typename T>
struct hashset {
protected:
  T** pool = nullptr;
  unsigned length = 8;  //length of pool
  unsigned count = 0;   //number of objects inside of the pool

public:
  hashset() {}
  hashset(unsigned length) : length(bit::round(length)) {}
  hashset(const hashset& source) { operator=(source); }
  hashset(hashset&& source) { operator=(std::move(source)); }
  ~hashset() { reset(); }

  hashset& operator=(const hashset& source) {
    reset();
    if(source.pool) {
      for(unsigned n = 0; n < source.count; n++) {
        insert(*source.pool[n]);
      }
    }
    return *this;
  }

  hashset& operator=(hashset&& source) {
    reset();
    pool = source.pool;
    length = source.length;
    count = source.count;
    source.pool = nullptr;
    source.length = 8;
    source.count = 0;
    return *this;
  }

  unsigned capacity() const { return length; }
  unsigned size() const { return count; }
  bool empty() const { return count == 0; }

  void reset() {
    if(pool) {
      for(unsigned n = 0; n < length; n++) {
        if(pool[n]) {
          delete pool[n];
          pool[n] = nullptr;
        }
      }
      delete pool;
      pool = nullptr;
    }
    length = 8;
    count = 0;
  }

  void reserve(unsigned size) {
    //ensure all items will fit into pool (with <= 50% load) and amortize growth
    size = bit::round(max(size, count << 1));
    T** copy = new T*[size]();

    if(pool) {
      for(unsigned n = 0; n < length; n++) {
        if(pool[n]) {
          unsigned hash = (*pool[n]).hash() & (size - 1);
          while(copy[hash]) if(++hash >= size) hash = 0;
          copy[hash] = pool[n];
          pool[n] = nullptr;
        }
      }
    }

    delete pool;
    pool = copy;
    length = size;
  }

  optional<T&> find(const T& value) {
    if(!pool) return false;

    unsigned hash = value.hash() & (length - 1);
    while(pool[hash]) {
      if(value == *pool[hash]) return {true, *pool[hash]};
      if(++hash >= length) hash = 0;
    }

    return false;
  }

  optional<T&> insert(const T& value) {
    if(!pool) pool = new T*[length]();

    //double pool size when load is >= 50%
    if(count >= (length >> 1)) reserve(length << 1);
    count++;

    unsigned hash = value.hash() & (length - 1);
    while(pool[hash]) if(++hash >= length) hash = 0;
    pool[hash] = new T(value);

    return {true, *pool[hash]};
  }

  bool remove(const T& value) {
    if(!pool) return false;

    unsigned hash = value.hash() & (length - 1);
    while(pool[hash]) {
      if(value == *pool[hash]) {
        delete pool[hash];
        pool[hash] = nullptr;
        count--;
        return true;
      }
      if(++hash >= length) hash = 0;
    }

    return false;
  }
};

}

#endif
