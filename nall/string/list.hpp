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

template<typename... P> auto lstring::append(const string& data, P&&... p) -> lstring& {
  vector::append(data);
  append(forward<P>(p)...);
  return *this;
}

auto lstring::append() -> lstring& {
  return *this;
}

auto lstring::find(rstring source) const -> maybe<unsigned> {
  for(unsigned n = 0; n < size(); n++) {
    if(operator[](n).equals(source)) return n;
  }
  return nothing;
}

auto lstring::ifind(rstring source) const -> maybe<unsigned> {
  for(unsigned n = 0; n < size(); n++) {
    if(operator[](n).iequals(source)) return n;
  }
  return nothing;
}

auto lstring::match(rstring pattern) const -> lstring {
  lstring result;
  for(unsigned n = 0; n < size(); n++) {
    if(operator[](n).match(pattern)) result.append(operator[](n));
  }
  return result;
}

auto lstring::merge(rstring separator) const -> string {
  string output;
  for(unsigned n = 0; n < size(); n++) {
    output.append(operator[](n));
    if(n < size() - 1) output.append(separator.data());
  }
  return output;
}

auto lstring::strip() -> lstring& {
  for(unsigned n = 0; n < size(); n++) {
    operator[](n).strip();
  }
  return *this;
}

}

#endif
