#ifndef NALL_SERIALIZER_HPP
#define NALL_SERIALIZER_HPP

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

#include <type_traits>
#include <utility>
#include <nall/stdint.hpp>
#include <nall/utility.hpp>

namespace nall {

struct serializer;

template<typename T>
struct has_serialize {
  template<typename C> static char test(decltype(std::declval<C>().serialize(std::declval<serializer&>()))*);
  template<typename C> static long test(...);
  static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

struct serializer {
  enum mode_t { Load, Save, Size };

  mode_t mode() const {
    return _mode;
  }

  const uint8_t* data() const {
    return _data;
  }

  unsigned size() const {
    return _size;
  }

  unsigned capacity() const {
    return _capacity;
  }

  template<typename T> serializer& floatingpoint(T& value) {
    enum { size = sizeof(T) };
    //this is rather dangerous, and not cross-platform safe;
    //but there is no standardized way to export FP-values
    uint8_t* p = (uint8_t*)&value;
    if(_mode == Save) {
      for(unsigned n = 0; n < size; n++) _data[_size++] = p[n];
    } else if(_mode == Load) {
      for(unsigned n = 0; n < size; n++) p[n] = _data[_size++];
    } else {
      _size += size;
    }
    return *this;
  }

  template<typename T> serializer& integer(T& value) {
    enum { size = std::is_same<bool, T>::value ? 1 : sizeof(T) };
    if(_mode == Save) {
      for(unsigned n = 0; n < size; n++) _data[_size++] = (uintmax_t)value >> (n << 3);
    } else if(_mode == Load) {
      value = 0;
      for(unsigned n = 0; n < size; n++) value |= (uintmax_t)_data[_size++] << (n << 3);
    } else if(_mode == Size) {
      _size += size;
    }
    return *this;
  }

  template<typename T, int N> serializer& array(T (&array)[N]) {
    for(unsigned n = 0; n < N; n++) operator()(array[n]);
    return *this;
  }

  template<typename T> serializer& array(T array, unsigned size) {
    for(unsigned n = 0; n < size; n++) operator()(array[n]);
    return *this;
  }

  template<typename T> serializer& operator()(T& value, typename std::enable_if<has_serialize<T>::value>::type* = 0) { value.serialize(*this); return *this; }
  template<typename T> serializer& operator()(T& value, typename std::enable_if<std::is_integral<T>::value>::type* = 0) { return integer(value); }
  template<typename T> serializer& operator()(T& value, typename std::enable_if<std::is_floating_point<T>::value>::type* = 0) { return floatingpoint(value); }
  template<typename T> serializer& operator()(T& value, typename std::enable_if<std::is_array<T>::value>::type* = 0) { return array(value); }
  template<typename T> serializer& operator()(T& value, unsigned size, typename std::enable_if<std::is_pointer<T>::value>::type* = 0) { return array(value, size); }

  serializer& operator=(const serializer& s) {
    if(_data) delete[] _data;

    _mode = s._mode;
    _data = new uint8_t[s._capacity];
    _size = s._size;
    _capacity = s._capacity;

    memcpy(_data, s._data, s._capacity);
    return *this;
  }

  serializer& operator=(serializer&& s) {
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
  serializer(serializer&& s) { operator=(std::move(s)); }

  serializer(unsigned capacity) {
    _mode = Save;
    _data = new uint8_t[capacity]();
    _size = 0;
    _capacity = capacity;
  }

  serializer(const uint8_t* data, unsigned capacity) {
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
  mode_t _mode = Size;
  uint8_t* _data = nullptr;
  unsigned _size = 0;
  unsigned _capacity = 0;
};

};

#endif
