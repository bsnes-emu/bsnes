#pragma once

/*
small string optimization (SSO) allocator
sizeof(string) == 8 + string::SSO

utilizes a union to store small strings directly into text pointer
bypasses the need to allocate heap memory for small strings
requires extra computations, which can be slower for large strings

pros:
* potential for in-place resize
* no heap allocation when (capacity < SSO)

cons:
* added overhead to fetch data()
* pass-by-value requires heap allocation when (capacity >= SSO)

*/

namespace nall {

string::string() {
  _data = nullptr;
  _capacity = SSO - 1;
  _size = 0;
}

auto string::get() -> char* {
  if(_capacity < SSO) return _text;
  return _data;
}

auto string::data() const -> const char* {
  if(_capacity < SSO) return _text;
  return _data;
}

auto string::reset() -> type& {
  if(_capacity >= SSO) memory::free(_data);
  _data = nullptr;
  _capacity = SSO - 1;
  _size = 0;
  return *this;
}

auto string::reserve(uint capacity) -> type& {
  if(capacity <= _capacity) return *this;
  capacity = bit::round(capacity + 1) - 1;
  if(_capacity < SSO) {
    char _temp[SSO];
    memory::copy(_temp, _text, SSO);
    _data = (char*)memory::allocate(_capacity = capacity + 1);
    memory::copy(_data, _temp, SSO);
  } else {
    _data = (char*)memory::resize(_data, _capacity = capacity + 1);
  }
  return *this;
}

auto string::resize(uint size) -> type& {
  reserve(size);
  get()[_size = size] = 0;
  return *this;
}

auto string::operator=(const string& source) -> type& {
  if(&source == this) return *this;
  reset();
  if(source._capacity >= SSO) {
    _data = (char*)memory::allocate(source._capacity + 1);
    _capacity = source._capacity;
    _size = source._size;
    memory::copy(_data, source._data, source._size + 1);
  } else {
    memory::copy(_text, source._text, SSO);
    _capacity = SSO - 1;
    _size = source._size;
  }
  return *this;
}

auto string::operator=(string&& source) -> type& {
  if(&source == this) return *this;
  reset();
  memory::copy(this, &source, sizeof(string));
  source._data = nullptr;
  source._capacity = SSO - 1;
  source._size = 0;
  return *this;
}

}
