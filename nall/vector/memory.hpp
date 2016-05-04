#pragma once

namespace nall {

template<typename T> auto vector<T>::reset() -> void {
  if(!_pool) return;

  for(uint n : range(_size)) _pool[n].~T();
  memory::free(_pool - _left);

  _pool = nullptr;
  _size = 0;
  _left = 0;
  _right = 0;
}

template<typename T> auto vector<T>::release() -> T* {
  auto pool = _pool;
  _pool = nullptr;
  _size = 0;
  _left = 0;
  _right = 0;
  return pool;
}

template<typename T> auto vector<T>::reserveLeft(uint capacity) -> bool {
  if(_size + _left >= capacity) return false;

  uint left = bit::round(capacity);
  auto pool = (T*)memory::allocate(sizeof(T) * (left + _right)) + (left - _size);
  for(uint n : range(_size)) new(pool + n) T(move(_pool[n]));
  memory::free(_pool - _left);

  _pool = pool;
  _left = left - _size;

  return true;
}

template<typename T> auto vector<T>::reserveRight(uint capacity) -> bool {
  if(_size + _right >= capacity) return false;

  uint right = bit::round(capacity);
  auto pool = (T*)memory::allocate(sizeof(T) * (_left + right)) + _left;
  for(uint n : range(_size)) new(pool + n) T(move(_pool[n]));
  memory::free(_pool - _left);

  _pool = pool;
  _right = right - _size;

  return true;
}

template<typename T> auto vector<T>::resizeLeft(uint size, const T& value) -> bool {
  if(size < _size) {  //shrink
    for(uint n : range(_size - size)) _pool[n].~T();
    _pool += _size - size;
    _left += _size - size;
    _size = size;
    return true;
  }
  if(size > _size) {  //grow
    reserveLeft(size);
    _pool -= size - _size;
    for(uint n : rrange(size - _size)) new(_pool + n) T(value);
    _left -= size - _size;
    _size = size;
    return true;
  }
  return false;
}

template<typename T> auto vector<T>::resizeRight(uint size, const T& value) -> bool {
  if(size < _size) {  //shrink
    for(uint n : range(size, _size)) _pool[n].~T();
    _right += _size - size;
    _size = size;
    return true;
  }
  if(size > _size) {  //grow
    reserveRight(size);
    for(uint n : range(_size, size)) new(_pool + n) T(value);
    _right -= size - _size;
    _size = size;
    return true;
  }
  return false;
}

}
