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

auto string::get() -> char* {
  if(_capacity == 0) reserve(1);
  return _data;
}

auto string::data() const -> const char* {
  if(_capacity == 0) return "";
  return _data;
}

auto string::reset() -> type& {
  if(_data) { memory::free(_data); _data = nullptr; }
  _capacity = 0;
  _size = 0;
  return *this;
}

auto string::reserve(uint capacity) -> type& {
  if(capacity > _capacity) {
    _capacity = bit::round(capacity + 1) - 1;
    _data = (char*)memory::resize(_data, _capacity + 1);
    _data[_capacity] = 0;
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
  _data = (char*)memory::allocate(source._size + 1);
  _capacity = source._size;
  _size = source._size;
  memory::copy(_data, source.data(), source.size() + 1);
  return *this;
}

auto string::operator=(string&& source) -> type& {
  if(&source == this) return *this;
  reset();
  _data = source._data;
  _capacity = source._capacity;
  _size = source._size;
  source._data = nullptr;
  source._capacity = 0;
  source._size = 0;
  return *this;
}

string::string() {
  _data = nullptr;
  _capacity = 0;
  _size = 0;
}

}

#endif
