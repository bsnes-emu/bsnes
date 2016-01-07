#pragma once

namespace nall {

struct string_view {
  string_view() {
    _string = nullptr;
    _data = "";
    _size = 0;
  }

  string_view(const char* data) {
    _string = nullptr;
    _data = data;
    _size = -1;  //defer length calculation, as it is often unnecessary
  }

  string_view(const char* data, unsigned size) {
    _string = nullptr;
    _data = data;
    _size = size;
  }

  string_view(const string& source) {
    _string = nullptr;
    _data = source.data();
    _size = source.size();
  }

  template<typename... P>
  string_view(P&&... p) {
    _string = new string{forward<P>(p)...};
    _data = _string->data();
    _size = _string->size();
  }

  ~string_view() {
    if(_string) delete _string;
  }

  string_view(const string_view& source) {
    _string = nullptr;
    _data = source._data;
    _size = source._size;
  }

  string_view(string_view&& source) {
    _string = source._string;
    _data = source._data;
    _size = source._size;
    source._string = nullptr;
  }

  auto operator=(const string_view& source) -> string_view& {
    _string = nullptr;
    _data = source._data;
    _size = source._size;
    return *this;
  };

  auto operator=(string_view&& source) -> string_view& {
    _string = source._string;
    _data = source._data;
    _size = source._size;
    source._string = nullptr;
    return *this;
  };

  operator const char*() const {
    return _data;
  }

  auto data() const -> const char* {
    return _data;
  }

  auto size() const -> unsigned {
    if(_size < 0) _size = strlen(_data);
    return _size;
  }

protected:
  string* _string;
  const char* _data;
  mutable signed _size;
};

}
