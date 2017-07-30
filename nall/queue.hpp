#pragma once

//simple circular ring buffer

namespace nall {

template<typename T>
struct queue {
  queue() = default;

  queue(const queue& source) {
    operator=(source);
  }

  queue(queue&& source) {
    operator=(move(source));
  }

  auto operator=(const queue& source) -> queue& {
    if(this == &source) return *this;
    reset();
    _size = source._size;
    _data = new T[_size];
    for(auto n : range(_size)) _data[n] = source._data[n];
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

  explicit operator bool() const {
    return _size;
  }

  auto size() const -> uint {
    return _size;
  }

  auto data() -> T* {
    return _data;
  }

  auto data() const -> const T* {
    return _data;
  }

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
    for(auto n : range(_size)) _data[n] = value;
  }

  auto pending() const -> bool {
    return _read != _write;
  }

  auto read() -> T {
    T result = _data[_read];
    if(++_read >= _size) _read = 0;
    return result;
  }

  auto last() const -> T {
    return _data[_write];
  }

  auto write(const T& value) -> void {
    _data[_write] = value;
    if(++_write >= _size) _write = 0;
  }

private:
  T* _data = nullptr;
  uint _size = 0;
  uint _read = 0;
  uint _write = 0;
};

}
