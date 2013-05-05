#ifdef NALL_STRING_INTERNAL_HPP

//core functionality
//only this header file may access _data, _size, _capacity directly
//all other headers must use data(), size(), capacity()

namespace nall {

char* string::data() { _unique(); return _data.get(); }
const char* string::data() const { if(!_data) return ""; return _data.get(); }
unsigned string::length() const { return strlen(data()); }
unsigned string::size() const { return _size; }
unsigned string::capacity() const { return _capacity; }
bool string::empty() const { return size() == 0; }

//ensure _data is unique
void string::_unique() {
  if(_data.unique()) return;
  _copy();
}

//copy _data (to make unique or to grow in size)
void string::_copy() {
  auto copy = new char[_capacity + 1];
  if(_data.get()) memcpy(copy, _data.get(), min(_capacity, _size));
  copy[_size] = 0;
  copy[_capacity] = 0;
  _data.reset(copy);
}

//amortize growth to O(log n)
//allocate one extra byte to always store null-terminator for libc usage
void string::reserve(unsigned capacity) {
  if(capacity > _capacity) {
    _capacity = bit::round(capacity + 1) - 1;
    _copy();
  }
}

void string::resize(unsigned size) {
  reserve(size);
  data()[_size = size] = 0;
}

void string::reset() {
  _data.reset();
  _capacity = 0;
  _size = 0;
}

void string::clear(char c) {
  for(unsigned n = 0; n < size(); n++) data()[n] = c;
}

template<typename... Args> string& string::assign(Args&&... args) {
  reset();
  sprint(*this, std::forward<Args>(args)...);
  return *this;
}

template<typename... Args> string& string::append(Args&&... args) {
  sprint(*this, std::forward<Args>(args)...);
  return *this;
}

string& string::_append(const char* s) {
  if(s == nullptr) return *this;
  unsigned basesize = size(), length = strlen(s);
  reserve(basesize + length);
  memcpy(data() + basesize, s, length);
  resize(basesize + length);
  return *this;
}

string::operator bool() const {
  return !empty();
}

string::operator const char*() const {
  return data();
}

char& string::operator[](unsigned position) {
  if(position > size() + 1) throw exception_out_of_bounds{};
  return data()[position];
}

const char& string::operator[](unsigned position) const {
  if(position > size() + 1) throw exception_out_of_bounds{};
  return data()[position];
}

bool string::operator==(const char* str) const { return strcmp(data(), str) == 0; }
bool string::operator!=(const char* str) const { return strcmp(data(), str) != 0; }
bool string::operator< (const char* str) const { return strcmp(data(), str)  < 0; }
bool string::operator<=(const char* str) const { return strcmp(data(), str) <= 0; }
bool string::operator> (const char* str) const { return strcmp(data(), str)  > 0; }
bool string::operator>=(const char* str) const { return strcmp(data(), str) >= 0; }

string& string::operator=(const string& source) {
  if(&source == this) return *this;
  _data = source._data;
  _capacity = source._capacity;
  _size = source._size;
  return *this;
}

string& string::operator=(string&& source) {
  if(&source == this) return *this;
  _data = std::move(source._data);
  _capacity = source._capacity;
  _size = source._size;
  source._capacity = 0;
  source._size = 0;
  return *this;
}

string::string(const string& source) {
  operator=(source);
}

string::string(string&& source) {
  operator=(std::move(source));
}

template<typename T, typename... Args> string::string(T&& source, Args&&... args) {
  _capacity = 0;
  _size = 0;
  sprint(*this, std::forward<T>(source), std::forward<Args>(args)...);
}

string::string() {
  _capacity = 0;
  _size = 0;
}

string::~string() {
}

}

#endif
