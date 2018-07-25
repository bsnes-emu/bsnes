#pragma once

namespace nall {

view<string>::view() {
  _string = nullptr;
  _data = "";
  _size = 0;
}

view<string>::view(const view& source) {
  if(this == &source) return;
  _string = nullptr;
  _data = source._data;
  _size = source._size;
}

view<string>::view(view&& source) {
  if(this == &source) return;
  _string = source._string;
  _data = source._data;
  _size = source._size;
  source._string = nullptr;
}

view<string>::view(const char* data) {
  _string = nullptr;
  _data = data;
  _size = -1;  //defer length calculation, as it is often unnecessary
}

view<string>::view(const char* data, uint size) {
  _string = nullptr;
  _data = data;
  _size = size;
}

view<string>::view(const string& source) {
  _string = nullptr;
  _data = source.data();
  _size = source.size();
}

template<typename... P>
view<string>::view(P&&... p) {
  _string = new string{forward<P>(p)...};
  _data = _string->data();
  _size = _string->size();
}

view<string>::~view() {
  if(_string) delete _string;
}

auto view<string>::operator=(const view& source) -> view& {
  if(this == &source) return *this;
  _string = nullptr;
  _data = source._data;
  _size = source._size;
  return *this;
};

auto view<string>::operator=(view&& source) -> view& {
  if(this == &source) return *this;
  _string = source._string;
  _data = source._data;
  _size = source._size;
  source._string = nullptr;
  return *this;
};

view<string>::operator const char*() const {
  return _data;
}

auto view<string>::data() const -> const char* {
  return _data;
}

auto view<string>::size() const -> uint {
  if(_size < 0) _size = strlen(_data);
  return _size;
}

}
