#pragma once

namespace nall {

auto string_vector::operator==(const string_vector& source) const -> bool {
  if(this == &source) return true;
  if(size() != source.size()) return false;
  for(uint n = 0; n < size(); n++) {
    if(operator[](n) != source[n]) return false;
  }
  return true;
}

auto string_vector::operator!=(const string_vector& source) const -> bool {
  return !operator==(source);
}

auto string_vector::isort() -> type& {
  sort([](const string& x, const string& y) {
    return memory::icompare(x.data(), x.size(), y.data(), y.size()) < 0;
  });
  return *this;
}

template<typename... P> auto string_vector::append(const string& data, P&&... p) -> type& {
  vector::append(data);
  append(forward<P>(p)...);
  return *this;
}

auto string_vector::append() -> type& {
  return *this;
}

auto string_vector::find(string_view source) const -> maybe<uint> {
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).equals(source)) return n;
  }
  return nothing;
}

auto string_vector::ifind(string_view source) const -> maybe<uint> {
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).iequals(source)) return n;
  }
  return nothing;
}

auto string_vector::match(string_view pattern) const -> type {
  string_vector result;
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).match(pattern)) result.append(operator[](n));
  }
  return result;
}

auto string_vector::merge(string_view separator) const -> string {
  string output;
  for(uint n = 0; n < size(); n++) {
    output.append(operator[](n));
    if(n < size() - 1) output.append(separator.data());
  }
  return output;
}

auto string_vector::strip() -> type& {
  for(uint n = 0; n < size(); n++) {
    operator[](n).strip();
  }
  return *this;
}

}
