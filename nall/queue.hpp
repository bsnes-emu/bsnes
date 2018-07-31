#pragma once

//simple circular ring buffer

#include <nall/range.hpp>
#include <nall/serializer.hpp>

namespace nall {

template<typename T>
struct queue {
  queue() = default;
  queue(const queue& source) { operator=(source); }
  queue(queue&& source) { operator=(move(source)); }

  auto operator=(const queue& source) -> queue& {
    if(this == &source) return *this;
    reset();
    _size = source._size;
    _data = new T[_size];
    for(uint n : range(_size)) _data[n] = source._data[n];
    _read = source._read;
    _write = source._write;
    return *this;
  }

  auto operator=(queue&& source) -> queue& {
    if(this == &source) return *this;
    _data = source._data;
    _size = source._size;
    _read = source._read;
    _write = source._write;
    source._data = nullptr;
    source.reset();
    return *this;
  }

  ~queue() {
    reset();
  }

  explicit operator bool() const { return _size; }
  template<typename U = T> auto size() const -> uint { return _size * sizeof(T) / sizeof(U); }
  auto empty() const -> bool { return _read == _write; }
  auto pending() const -> bool { return _read != _write; }
  auto full() const -> bool { return _write == _size; }

  auto data() -> T* { return _data; }
  auto data() const -> const T* { return _data; }

  auto reset() {
    delete[] _data;
    _data = nullptr;
    _size = 0;
    _read = 0;
    _write = 0;
  }

  auto resize(uint size, const T& value = {}) -> void {
    reset();
    _size = size;
    _data = new T[_size];
    for(uint n : range(_size)) _data[n] = value;
  }

  auto flush() -> void {
    _read = 0;
    _write = 0;
  }

  auto read() -> T {
    if(_read >= _size) _read = 0;
    return _data[_read++];
  }

  auto write(const T& value) -> void {
    if(_write >= _size) _write = 0;
    _data[_write++] = value;
  }

  auto serialize(serializer& s) -> void {
    s.array(_data, _size);
    s.integer(_size);
    s.integer(_read);
    s.integer(_write);
  }

private:
  T* _data = nullptr;
  uint _size = 0;
  uint _read = 0;
  uint _write = 0;
};

}
