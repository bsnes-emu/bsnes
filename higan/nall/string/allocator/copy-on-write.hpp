#ifdef NALL_STRING_INTERNAL_HPP

/*
copy on write (COW) allocator
sizeof(string) == 24 (amd64)

utilizes a shared_ptr to reference count strings
allows string copies to execute as fast as string moves
requires extra computations, which will be slower for all string sizes

pros:
* lower memory usage
* pass-by-value does not require heap allocation; obviates pass-by-const-reference

cons:
* added overhead to fetch data()
* added heap allocation for reference-count pool
* no potential for in-place resize (always copies)
* larger sizeof(string)

*/

namespace nall {

char* string::data() {
  if(!_data.unique()) _copy();
  return _data.get();
}

const char* string::data() const {
  if(!_data) return "";
  return _data.get();
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
