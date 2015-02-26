#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

auto lstring::operator==(const lstring& source) const -> bool {
  if(this == &source) return true;
  if(size() != source.size()) return false;
  for(unsigned n = 0; n < size(); n++) {
    if(operator[](n) != source[n]) return false;
  }
  return true;
}

auto lstring::operator!=(const lstring& source) const -> bool {
  return !operator==(source);
}

auto lstring::isort() -> lstring& {
  nall::sort(pool, objectsize, [](const string& x, const string& y) {
    return memory::icompare(x.data(), x.size(), y.data(), y.size()) < 0;
  });
  return *this;
}

template<typename... P> auto append(lstring& self, const string& data, P&&... p) -> lstring& {
  self.vector::append(data);
  append(self, std::forward<P>(p)...);
  return self;
}

auto append(lstring& self) -> lstring& {
  return self;
}

auto find(const lstring& self, const string& source) -> maybe<unsigned> {
  for(unsigned n = 0; n < self.size(); n++) {
    if(self[n].equals(source)) return n;
  }
  return nothing;
}

auto ifind(const lstring& self, const string& source) -> maybe<unsigned> {
  for(unsigned n = 0; n < self.size(); n++) {
    if(self[n].iequals(source)) return n;
  }
  return nothing;
}

auto merge(const lstring& self, const string& separator) -> string {
  string output;
  for(unsigned n = 0; n < self.size(); n++) {
    output.append(self[n]);
    if(n < self.size() - 1) output.append(separator);
  }
  return output;
}

auto strip(lstring& self) -> lstring& {
  for(unsigned n = 0; n < self.size(); n++) {
    self[n].strip();
  }
  return self;
}

}

#endif
