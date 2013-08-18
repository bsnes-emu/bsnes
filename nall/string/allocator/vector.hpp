#ifdef NALL_STRING_INTERNAL_HPP

/*
vector allocator
sizeof(string) == 16 (amd64)

utilizes a raw string pointer
always allocates memory onto the heap when string is not empty

pros:
* potential for in-place resize
* simplicity

cons:
* always allocates heap memory on (capacity > 0)
* pass-by-value requires heap allocation

*/

namespace nall {

char* string::data() {
  if(_capacity == 0) reserve(1);
  return _data;
}

const char* string::data() const {
  if(_capacity == 0) return "";
  return _data;
}

void string::reserve(unsigned capacity) {
  if(capacity > _capacity) {
    _capacity = bit::round(capacity + 1) - 1;
    _data = (char*)realloc(_data, _capacity + 1);
    _data[_capacity] = 0;
  }
}

void string::resize(unsigned size) {
  reserve(size);
  data()[_size = size] = 0;
}

void string::reset() {
  if(_data) { free(_data); _data = nullptr; }
  _capacity = 0;
  _size = 0;
}

string& string::operator=(const string& source) {
  if(&source == this) return *this;
  _data = (char*)malloc(source._size + 1);
  _capacity = source._size;
  _size = source._size;
  memcpy(_data, source.data(), source.size() + 1);
  return *this;
}

string& string::operator=(string&& source) {
  if(&source == this) return *this;
  _data = source._data;
  _capacity = source._capacity;
  _size = source._size;
  source._data = nullptr;
  source._capacity = 0;
  source._size = 0;
  return *this;
}

template<typename T, typename... Args> string::string(T&& source, Args&&... args) {
  _data = nullptr;
  _capacity = 0;
  _size = 0;
  sprint(*this, std::forward<T>(source), std::forward<Args>(args)...);
}

string::string() {
  _data = nullptr;
  _capacity = 0;
  _size = 0;
}

string::~string() {
  if(_data) free(_data);
}

}

#endif
