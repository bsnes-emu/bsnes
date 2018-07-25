#pragma once

namespace nall {

template<typename... P> auto vector<string>::append(const string& data, P&&... p) -> type& {
  vector_base::append(data);
  append(forward<P>(p)...);
  return *this;
}

auto vector<string>::append() -> type& {
  return *this;
}

auto vector<string>::isort() -> type& {
  sort([](const string& x, const string& y) {
    return memory::icompare(x.data(), x.size(), y.data(), y.size()) < 0;
  });
  return *this;
}

auto vector<string>::find(view<string> source) const -> maybe<uint> {
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).equals(source)) return n;
  }
  return {};
}

auto vector<string>::ifind(view<string> source) const -> maybe<uint> {
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).iequals(source)) return n;
  }
  return {};
}

auto vector<string>::match(view<string> pattern) const -> vector<string> {
  vector<string> result;
  for(uint n = 0; n < size(); n++) {
    if(operator[](n).match(pattern)) result.append(operator[](n));
  }
  return result;
}

auto vector<string>::merge(view<string> separator) const -> string {
  string output;
  for(uint n = 0; n < size(); n++) {
    output.append(operator[](n));
    if(n < size() - 1) output.append(separator.data());
  }
  return output;
}

auto vector<string>::strip() -> type& {
  for(uint n = 0; n < size(); n++) {
    operator[](n).strip();
  }
  return *this;
}

}
