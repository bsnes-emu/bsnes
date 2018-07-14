#pragma once

namespace nall {

template<typename T>
struct vector_iterator {
  vector_iterator(vector<T>& self, uint offset) : self(self), offset(offset) {}
  auto operator*() -> T& { return self.operator[](offset); }
  auto operator!=(const vector_iterator& source) const -> bool { return offset != source.offset; }
  auto operator++() -> vector_iterator& { return offset++, *this; }

private:
  vector<T>& self;
  uint offset;
};

template<typename T>
struct vector_iterator_const {
  vector_iterator_const(const vector<T>& self, uint offset) : self(self), offset(offset) {}
  auto operator*() -> const T& { return self.operator[](offset); }
  auto operator!=(const vector_iterator_const& source) const -> bool { return offset != source.offset; }
  auto operator++() -> vector_iterator_const& { return offset++, *this; }

private:
  const vector<T>& self;
  uint offset;
};

template<typename T>
struct vector_iterator_reverse {
  vector_iterator_reverse(vector<T>& self, uint offset) : self(self), offset(offset) {}
  auto operator*() -> T& { return self.operator[](offset); }
  auto operator!=(const vector_iterator_reverse& source) const -> bool { return offset != source.offset; }
  auto operator++() -> vector_iterator_reverse& { return offset--, *this; }

private:
  vector<T>& self;
  uint offset;
};

template<typename T>
struct vector_iterator_reverse_const {
  vector_iterator_reverse_const(const vector<T>& self, uint offset) : self(self), offset(offset) {}
  auto operator*() -> const T& { return self.operator[](offset); }
  auto operator!=(const vector_iterator_reverse_const& source) const -> bool { return offset != source.offset; }
  auto operator++() -> vector_iterator_reverse_const& { return offset--, *this; }

private:
  const vector<T>& self;
  uint offset;
};

}
