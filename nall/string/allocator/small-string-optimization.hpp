#ifdef NALL_STRING_INTERNAL_HPP

/*
small string optimization (SSO) allocator
sizeof(string) == 16 (amd64)

utilizes a union to store small strings directly into text pointer
bypasses the need to allocate heap memory for small strings
requires extra computations, which can be slower for large strings

pros:
* potential for in-place resize
* no heap allocation when (capacity < 8)

cons:
* added overhead to fetch data()
* 32-bit platforms limited to (capacity < 4)
* pass-by-value requires heap allocation

*/

namespace nall {

char* string::data() {
  if(_capacity < SSO) return _text;
  return _data;
}

const char* string::data() const {
  if(_capacity < SSO) return _text;
  return _data;
}

void string::reserve(unsigned capacity) {
  if(capacity > _capacity) {
    if(capacity >= SSO) {
      capacity = bit::round(capacity + 1) - 1;
      if(_capacity < SSO) {
        char temp[SSO];
        memcpy(temp, _text, SSO);
        _data = (char*)malloc(capacity + 1);
        memcpy(_data, temp, SSO);
      } else {
        _data = (char*)realloc(_data, capacity + 1);
      }
    }
    _capacity = capacity;
    data()[_capacity] = 0;
  }
}

void string::resize(unsigned size) {
  reserve(size);
  data()[_size = size] = 0;
}

void string::reset() {
  if(_capacity >= SSO) free(_data);
  _data = nullptr;
  _capacity = SSO - 1;
  _size = 0;
}

string& string::operator=(const string& source) {
  if(&source == this) return *this;
  reset();
  if(source._capacity >= SSO) {
    _data = (char*)malloc(source._capacity + 1);
    _capacity = source._capacity;
    _size = source._size;
    memcpy(_data, source.data(), source.size() + 1);
  } else {
    memcpy(_text, source._text, SSO);
    _capacity = SSO - 1;
    _size = strlen(_text);
  }
  return *this;
}

string& string::operator=(string&& source) {
  if(&source == this) return *this;
  reset();
  memcpy(this, &source, sizeof(string));
  source._data = nullptr;
  source._capacity = SSO - 1;
  source._size = 0;
  return *this;
}

template<typename T, typename... Args> string::string(T&& source, Args&&... args) {
  construct();
  sprint(*this, std::forward<T>(source), std::forward<Args>(args)...);
}

string::string() {
  construct();
}

string::~string() {
  reset();
}

void string::construct() {
  _data = nullptr;
  _capacity = SSO - 1;
  _size = 0;
}

}

#endif
