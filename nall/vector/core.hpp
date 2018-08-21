#pragma once

namespace nall {

template<typename T> vector<T>::vector(Literal::Capacity capacity) {
  reserve(capacity.value);
}

template<typename T> vector<T>::vector(Literal::Size size) {
  resize(size.value);
}

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

template<typename T> template<typename Cast> auto vector<T>::capacity() const -> uint {
  return (_left + _size + _right) * sizeof(T) / sizeof(Cast);
}

template<typename T> template<typename Cast> auto vector<T>::size() const -> uint {
  return _size * sizeof(T) / sizeof(Cast);
}

template<typename T> template<typename Cast> auto vector<T>::data(uint offset) -> Cast* {
  return (Cast*)_pool + offset;
}

template<typename T> template<typename Cast> auto vector<T>::data(uint offset) const -> const Cast* {
  return (const Cast*)_pool + offset;
}

}
