#pragma once

namespace nall {

template<typename T> struct iterator {
  iterator(T* self, uint offset) : self(self), offset(offset) {}
  auto operator*() -> T& { return self[offset]; }
  auto operator!=(const iterator& source) const -> bool { return offset != source.offset; }
  auto operator++() -> iterator& { return offset++, *this; }

private:
  T* self;
  uint offset;
};

template<typename T> struct iterator_const {
  iterator_const(const T* self, uint offset) : self(self), offset(offset) {}
  auto operator*() -> const T& { return self[offset]; }
  auto operator!=(const iterator_const& source) const -> bool { return offset != source.offset; }
  auto operator++() -> iterator_const& { return offset++, *this; }

private:
  const T* self;
  uint offset;
};

template<typename T> struct reverse_iterator {
  reverse_iterator(T* self, uint offset) : self(self), offset(offset) {}
  auto operator*() -> T& { return self[offset]; }
  auto operator!=(const reverse_iterator& source) const -> bool { return offset != source.offset; }
  auto operator++() -> reverse_iterator& { return offset--, *this; }

private:
  T* self;
  uint offset;
};

template<typename T> struct reverse_iterator_const {
  reverse_iterator_const(const T* self, uint offset) : self(self), offset(offset) {}
  auto operator*() -> const T& { return self[offset]; }
  auto operator!=(const reverse_iterator_const& source) const -> bool { return offset != source.offset; }
  auto operator++() -> reverse_iterator_const& { return offset--, *this; }

private:
  const T* self;
  uint offset;
};

//std::rbegin(), std::rend() is missing from GCC 4.9; which I still target

template<typename T, uint Size> auto rbegin(T (&array)[Size]) { return reverse_iterator<T>{array, Size - 1}; }
template<typename T, uint Size> auto rend(T (&array)[Size]) { return reverse_iterator<T>{array, (uint)-1}; }

template<typename T> auto rbegin(T& self) { return self.rbegin(); }
template<typename T> auto rend(T& self) { return self.rend(); }

template<typename T> struct reverse_wrapper {
  T self;

  auto begin() { return rbegin(self); }
  auto end() { return rend(self); }

  auto begin() const { return rbegin(self); }
  auto end() const { return rend(self); }
};

template<typename T> auto reverse(T& object) -> reverse_wrapper<T&> {
  return {object};
}

template<typename T> auto reverse(T&& object) -> reverse_wrapper<T> {
  return {object};
}

}
