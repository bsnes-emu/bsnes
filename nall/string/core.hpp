#ifdef NALL_STRING_INTERNAL_HPP

//only allocators may access _data or modify _size and _capacity
//all other functions must use data(), size(), capacity()

#if defined(NALL_STRING_ALLOCATOR_COPY_ON_WRITE)
  #include <nall/string/allocator/copy-on-write.hpp>
#elif defined(NALL_STRING_ALLOCATOR_SMALL_STRING_OPTIMIZATION)
  #include <nall/string/allocator/small-string-optimization.hpp>
#elif defined(NALL_STRING_ALLOCATOR_VECTOR)
  #include <nall/string/allocator/vector.hpp>
#endif

namespace nall {

unsigned string::length() const { return strlen(data()); }
unsigned string::size() const { return _size; }
unsigned string::capacity() const { return _capacity; }
bool string::empty() const { return _size == 0; }

void string::clear(char c) {
  for(unsigned n = 0; n < size(); n++) data()[n] = c;
}

unsigned string::hash() const {
  const char* p = data();
  unsigned result = 5381;
  while(*p) result = (result << 5) + result + *p++;
  return result;
}

template<typename... Args> string& string::assign(Args&&... args) {
  resize(0);
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

char& string::operator[](signed position) {
  if(position > size() + 1) throw exception_out_of_bounds{};
  return data()[position];
}

const char& string::operator[](signed position) const {
  if(position > size() + 1) throw exception_out_of_bounds{};
  return data()[position];
}

bool string::operator==(const char* str) const { return strcmp(data(), str) == 0; }
bool string::operator!=(const char* str) const { return strcmp(data(), str) != 0; }
bool string::operator< (const char* str) const { return strcmp(data(), str)  < 0; }
bool string::operator<=(const char* str) const { return strcmp(data(), str) <= 0; }
bool string::operator> (const char* str) const { return strcmp(data(), str)  > 0; }
bool string::operator>=(const char* str) const { return strcmp(data(), str) >= 0; }

string::string(const string& source) {
  construct();
  operator=(source);
}

string::string(string&& source) {
  construct();
  operator=(std::move(source));
}

}

#endif
