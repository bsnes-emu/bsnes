#pragma once

namespace nall {

auto lstring::operator==(const lstring& source) const -> bool {
  if(this == &source) return true;
  if(size() != source.size()) return false;
  for(uint n = 0; n < size(); n++) {
    if(operator[](n) != source[n]) return false;
  }
  return true;
}

auto lstring::operator!=(const lstring& source) const -> bool {
  return !operator==(source);
}

auto lstring::isort() -> lstring& {
  sort([](const string& x, const string& y) {
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

auto lstring::find(string_view source) const -> maybe<uint> {
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).equals(source)) return n;
  }
  return nothing;
}

auto lstring::ifind(string_view source) const -> maybe<uint> {
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).iequals(source)) return n;
  }
  return nothing;
}

auto lstring::match(string_view pattern) const -> lstring {
  lstring result;
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).match(pattern)) result.append(operator[](n));
  }
  return result;
}

auto lstring::merge(string_view separator) const -> string {
  string output;
  for(uint n = 0; n < size(); n++) {
    output.append(operator[](n));
    if(n < size() - 1) output.append(separator.data());
  }
  return output;
}

auto lstring::strip() -> lstring& {
  for(uint n = 0; n < size(); n++) {
    operator[](n).strip();
  }
  return *this;
}

}
