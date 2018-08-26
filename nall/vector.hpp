#pragma once

#include <new>

#include <nall/array-view.hpp>
#include <nall/bit.hpp>
#include <nall/function.hpp>
#include <nall/iterator.hpp>
#include <nall/literals.hpp>
#include <nall/maybe.hpp>
#include <nall/memory.hpp>
#include <nall/merge-sort.hpp>
#include <nall/range.hpp>
#include <nall/traits.hpp>
#include <nall/view.hpp>

namespace nall {

template<typename T>
struct vector_base {
  using type = vector_base;

  //core.hpp
  vector_base() = default;
  vector_base(Literal::Capacity capacity);
  vector_base(Literal::Size size);
  vector_base(const initializer_list<T>& values);
  vector_base(const type& source);
  vector_base(type&& source);
  ~vector_base();

  explicit operator bool() const;
  operator array_view<T>() const;
  template<typename Cast = T> auto capacity() const -> uint;
  template<typename Cast = T> auto size() const -> uint;
  template<typename Cast = T> auto data() -> Cast*;
  template<typename Cast = T> auto data() const -> const Cast*;

  //assign.hpp
  auto operator=(const type& source) -> type&;
  auto operator=(type&& source) -> type&;

  //compare.hpp
  auto operator==(const type& source) const -> bool;
  auto operator!=(const type& source) const -> bool;

  //memory.hpp
  auto reset() -> void;
  auto acquire(const T* data, uint size, uint capacity = 0) -> void;
  auto release() -> T*;

  auto reserveLeft(uint capacity) -> bool;
  auto reserveRight(uint capacity) -> bool;
  auto reserve(uint capacity) -> bool { return reserveRight(capacity); }

  auto reallocateLeft(uint size) -> bool;
  auto reallocateRight(uint size) -> bool;
  auto reallocate(uint size) -> bool { return reallocateRight(size); }

  auto resizeLeft(uint size, const T& value = T()) -> bool;
  auto resizeRight(uint size, const T& value = T()) -> bool;
  auto resize(uint size, const T& value = T()) -> bool { return resizeRight(size, value); }

  //access.hpp
  alwaysinline auto operator[](uint offset) -> T&;
  alwaysinline auto operator[](uint offset) const -> const T&;

  alwaysinline auto operator()(uint offset) -> T&;
  alwaysinline auto operator()(uint offset, const T& value) const -> const T&;

  alwaysinline auto left() -> T&;
  alwaysinline auto first() -> T& { return left(); }
  alwaysinline auto left() const -> const T&;
  alwaysinline auto first() const -> const T& { return left(); }

  alwaysinline auto right() -> T&;
  alwaysinline auto last() -> T& { return right(); }
  alwaysinline auto right() const -> const T&;
  alwaysinline auto last() const -> const T& { return right(); }

  //modify.hpp
  auto prepend(const T& value) -> void;
  auto prepend(T&& value) -> void;
  auto prepend(const type& values) -> void;
  auto prepend(type&& values) -> void;

  auto append(const T& value) -> void;
  auto append(T&& value) -> void;
  auto append(const type& values) -> void;
  auto append(type&& values) -> void;

  auto insert(uint offset, const T& value) -> void;

  auto removeLeft(uint length = 1) -> void;
  auto removeFirst(uint length = 1) -> void { return removeLeft(length); }
  auto removeRight(uint length = 1) -> void;
  auto removeLast(uint length = 1) -> void { return removeRight(length); }
  auto remove(uint offset, uint length = 1) -> void;

  auto takeLeft() -> T;
  auto takeFirst() -> T { return move(takeLeft()); }
  auto takeRight() -> T;
  auto takeLast() -> T { return move(takeRight()); }
  auto take(uint offset) -> T;

  //iterator.hpp
  auto begin() -> iterator<T> { return {data(), 0}; }
  auto end() -> iterator<T> { return {data(), size()}; }

  auto begin() const -> iterator_const<T> { return {data(), 0}; }
  auto end() const -> iterator_const<T> { return {data(), size()}; }

  auto rbegin() -> reverse_iterator<T> { return {data(), size() - 1}; }
  auto rend() -> reverse_iterator<T> { return {data(), (uint)-1}; }

  auto rbegin() const -> reverse_iterator_const<T> { return {data(), size() - 1}; }
  auto rend() const -> reverse_iterator_const<T> { return {data(), (uint)-1}; }

  //utility.hpp
  auto fill(const T& value = {}) -> void;
  auto sort(const function<bool (const T& lhs, const T& rhs)>& comparator = [](auto& lhs, auto& rhs) { return lhs < rhs; }) -> void;
  auto find(const function<bool (const T& lhs)>& comparator) -> maybe<uint>;
  auto find(const T& value) const -> maybe<uint>;
  auto findSorted(const T& value) const -> maybe<uint>;
  auto foreach(const function<void (const T&)>& callback) -> void;
  auto foreach(const function<void (uint, const T&)>& callback) -> void;

private:
  T* _pool = nullptr;  //pointer to first initialized element in pool
  uint _size = 0;      //number of initialized elements in pool
  uint _left = 0;      //number of allocated elements free on the left of pool
  uint _right = 0;     //number of allocated elements free on the right of pool
};

}

#define vector vector_base
#include <nall/vector/core.hpp>
#include <nall/vector/assign.hpp>
#include <nall/vector/compare.hpp>
#include <nall/vector/memory.hpp>
#include <nall/vector/access.hpp>
#include <nall/vector/modify.hpp>
#include <nall/vector/iterator.hpp>
#include <nall/vector/utility.hpp>
#undef vector

namespace nall {
  template<typename T> struct vector : vector_base<T> {
    using vector_base<T>::vector_base;
  };
}
