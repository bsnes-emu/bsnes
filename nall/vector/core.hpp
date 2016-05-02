#pragma once

namespace nall {

template<typename T> vector<T>::vector(const initializer_list<T>& values) {
  reserveRight(values.size());
  for(auto& value : values) append(value);
}

template<typename T> vector<T>::vector(const vector<T>& source) {
  operator=(source);
}

template<typename T> vector<T>::vector(vector<T>&& source) {
  operator=(move(source));
}

template<typename T> vector<T>::~vector() {
  reset();
}

template<typename T> vector<T>::operator bool() const {
  return _size;
}

template<typename T> auto vector<T>::capacity() const -> uint {
  return _left + _size + _right;
}

template<typename T> auto vector<T>::size() const -> uint {
  return _size;
}

template<typename T> auto vector<T>::data() -> T* {
  return _pool;
}

template<typename T> auto vector<T>::data() const -> const T* {
  return _pool;
}

}
