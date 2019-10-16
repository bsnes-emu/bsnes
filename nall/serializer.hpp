#pragma once

//serializer: a class designed to save and restore the state of classes.
//
//benefits:
//- data() will be portable in size (it is not necessary to specify type sizes.)
//- data() will be portable in endianness (always stored internally as little-endian.)
//- one serialize function can both save and restore class states.
//
//caveats:
//- only plain-old-data can be stored. complex classes must provide serialize(serializer&);
//- floating-point usage is not portable across different implementations

#include <nall/array.hpp>
#include <nall/range.hpp>
#include <nall/stdint.hpp>
#include <nall/traits.hpp>
#include <nall/utility.hpp>

namespace nall {

struct serializer;

template<typename T>
struct has_serialize {
  template<typename C> static auto test(decltype(std::declval<C>().serialize(std::declval<serializer&>()))*) -> char;
  template<typename C> static auto test(...) -> long;
  static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

struct serializer {
  enum Mode : uint { Load, Save, Size };

  explicit operator bool() const {
    return _size;
  }

  auto mode() const -> Mode {
    return _mode;
  }

  auto data() const -> const uint8_t* {
    return _data;
  }

  auto size() const -> uint {
    return _size;
  }

  auto capacity() const -> uint {
    return _capacity;
  }

  auto setMode(Mode mode) -> bool {
    if(_mode == Mode::Save && mode == Mode::Load) {
      _mode = mode;
      _size = 0;
      return true;
    }
    return false;
  }

  template<typename T> auto real(T& value) -> serializer& {
    enum : uint { size = sizeof(T) };
    //this is rather dangerous, and not cross-platform safe;
    //but there is no standardized way to export FP-values
    auto p = (uint8_t*)&value;
    if(_mode == Save) {
      for(uint n : range(size)) _data[_size++] = p[n];
    } else if(_mode == Load) {
      for(uint n : range(size)) p[n] = _data[_size++];
    } else {
      _size += size;
    }
    return *this;
  }

  template<typename T> auto boolean(T& value) -> serializer& {
    if(_mode == Save) {
      _data[_size++] = (bool)value;
    } else if(_mode == Load) {
      value = (bool)_data[_size++];
    } else if(_mode == Size) {
      _size += 1;
    }
    return *this;
  }

  template<typename T> auto integer(T& value) -> serializer& {
    enum : uint { size = std::is_same<bool, T>::value ? 1 : sizeof(T) };
    if(_mode == Save) {
      T copy = value;
      for(uint n : range(size)) _data[_size++] = copy, copy >>= 8;
    } else if(_mode == Load) {
      value = 0;
      for(uint n : range(size)) value |= (T)_data[_size++] << (n << 3);
    } else if(_mode == Size) {
      _size += size;
    }
    return *this;
  }

  template<typename T, int N> auto array(T (&array)[N]) -> serializer& {
    for(uint n : range(N)) operator()(array[n]);
    return *this;
  }

  template<typename T> auto array(T array, uint size) -> serializer& {
    for(uint n : range(size)) operator()(array[n]);
    return *this;
  }

  template<int N> auto array(uint8_t (&array_)[N]) -> serializer& {
    array(array_, N);
    return *this;
  }
  auto array(uint8_t* array, uint size) -> serializer& {
    if(_mode == Save) {
      memcpy(_data+_size, array, size);
    } else if(_mode == Load) {
      memcpy(array, _data+_size, size);
    }
    _size += size;
    return *this;
  }
#ifdef ENDIAN_LSB
  template<int N> auto array(uint16_t (&array_)[N]) -> serializer& {
    array(array_, N);
    return *this;
  }
  auto array(uint16_t* array_, uint size) -> serializer& {
    array((uint8_t*)array_, size*2);
    return *this;
  }
#endif

  template<typename T, uint Size> auto array(nall::array<T[Size]>& array) -> serializer& {
    for(auto& value : array) operator()(value);
    return *this;
  }

  template<typename T> auto operator()(T& value, typename std::enable_if<has_serialize<T>::value>::type* = 0) -> serializer& { value.serialize(*this); return *this; }
  template<typename T> auto operator()(T& value, typename std::enable_if<std::is_integral<T>::value>::type* = 0) -> serializer& { return integer(value); }
  template<typename T> auto operator()(T& value, typename std::enable_if<std::is_floating_point<T>::value>::type* = 0) -> serializer& { return real(value); }
  template<typename T> auto operator()(T& value, typename std::enable_if<std::is_array<T>::value>::type* = 0) -> serializer& { return array(value); }
  template<typename T> auto operator()(T& value, uint size, typename std::enable_if<std::is_pointer<T>::value>::type* = 0) -> serializer& { return array(value, size); }

  auto operator=(const serializer& s) -> serializer& {
    if(_data) delete[] _data;

    _mode = s._mode;
    _data = new uint8_t[s._capacity];
    _size = s._size;
    _capacity = s._capacity;

    memcpy(_data, s._data, s._capacity);
    return *this;
  }

  auto operator=(serializer&& s) -> serializer& {
    if(_data) delete[] _data;

    _mode = s._mode;
    _data = s._data;
    _size = s._size;
    _capacity = s._capacity;

    s._data = nullptr;
    return *this;
  }

  serializer() = default;
  serializer(const serializer& s) { operator=(s); }
  serializer(serializer&& s) { operator=(move(s)); }

  serializer(uint capacity) {
    _mode = Save;
    _data = new uint8_t[capacity]();
    _size = 0;
    _capacity = capacity;
  }

  serializer(const uint8_t* data, uint capacity) {
    _mode = Load;
    _data = new uint8_t[capacity];
    _size = 0;
    _capacity = capacity;
    memcpy(_data, data, capacity);
  }

  ~serializer() {
    if(_data) delete[] _data;
  }

private:
  Mode _mode = Size;
  uint8_t* _data = nullptr;
  uint _size = 0;
  uint _capacity = 0;
};

}
