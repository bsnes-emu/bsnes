#pragma once

#include <new>

#include <nall/bit.hpp>
#include <nall/function.hpp>
#include <nall/maybe.hpp>
#include <nall/memory.hpp>
#include <nall/range.hpp>
#include <nall/sort.hpp>
#include <nall/traits.hpp>

namespace nall {

template<typename T> struct vector_iterator;
template<typename T> struct vector_iterator_const;

template<typename T>
struct vector {
  //core.hpp
  vector() = default;
  vector(const initializer_list<T>& values);
  vector(const vector& source);
  vector(vector&& source);
  ~vector();

  explicit operator bool() const;
  auto capacity() const -> uint;
  auto size() const -> uint;
  auto data() -> T*;
  auto data() const -> const T*;

  //assign.hpp
  auto operator=(const vector& source) -> vector&;
  auto operator=(vector&& source) -> vector&;

  //memory.hpp
  auto reset() -> void;
  auto release() -> T*;

  auto reserveLeft(uint capacity) -> bool;
  auto reserveRight(uint capacity) -> bool;
  auto reserve(uint capacity) -> bool { return reserveRight(capacity); }

  auto resizeLeft(uint size, const T& value = T()) -> bool;
  auto resizeRight(uint size, const T& value = T()) -> bool;
  auto resize(uint size, const T& value = T()) -> bool { return resizeRight(size, value); }

  //access.hpp
  alwaysinline auto operator[](uint offset) -> T&;
  alwaysinline auto operator[](uint offset) const -> const T&;

  alwaysinline auto operator()(uint offset) -> T&;
  alwaysinline auto operator()(uint offset, const T& value) const -> const T&;

  alwaysinline auto left() -> T&;
  alwaysinline auto left() const -> const T&;

  alwaysinline auto right() -> T&;
  alwaysinline auto right() const -> const T&;

  //modify.hpp
  auto prepend(const T& value) -> void;
  auto prepend(T&& value) -> void;
  auto prepend(const vector<T>& values) -> void;
  auto prepend(vector<T>&& values) -> void;

  auto append(const T& value) -> void;
  auto append(T&& value) -> void;
  auto append(const vector<T>& values) -> void;
  auto append(vector<T>&& values) -> void;

  auto insert(uint offset, const T& value) -> void;

  auto removeLeft(uint length = 1) -> void;
  auto removeRight(uint length = 1) -> void;
  auto remove(uint offset, uint length = 1) -> void;

  auto takeLeft() -> T;
  auto takeRight() -> T;
  auto take(uint offset) -> T;

  //iterator.hpp
  auto begin() { return vector_iterator<T>{*this, 0}; }
  auto end() { return vector_iterator<T>{*this, size()}; }

  auto begin() const { return vector_iterator_const<T>{*this, 0}; }
  auto end() const { return vector_iterator_const<T>{*this, size()}; }

  //utility.hpp
  auto sort(const function<bool (const T& lhs, const T& rhs)>& comparator = [](auto& lhs, auto& rhs) { return lhs < rhs; }) -> void;
  auto find(const T& value) const -> maybe<uint>;

private:
  T* _pool = nullptr;  //pointer to first initialized element in pool
  uint _size = 0;      //number of initialized elements in pool
  uint _left = 0;      //number of allocated elements free on the left of pool
  uint _right = 0;     //number of allocated elements free on the right of pool
};

}

#include <nall/vector/core.hpp>
#include <nall/vector/assign.hpp>
#include <nall/vector/memory.hpp>
#include <nall/vector/access.hpp>
#include <nall/vector/modify.hpp>
#include <nall/vector/iterator.hpp>
#include <nall/vector/utility.hpp>
