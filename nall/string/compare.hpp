#pragma once

namespace nall {

template<bool Insensitive>
auto string::_compare(const char* target, uint capacity, const char* source, uint size) -> int {
  if(Insensitive) return memory::icompare(target, capacity, source, size);
  return memory::compare(target, capacity, source, size);
}

//size() + 1 includes null-terminator; required to properly compare strings of differing lengths
auto string::compare(view<string> x, view<string> y) -> int {
  return memory::compare(x.data(), x.size() + 1, y.data(), y.size() + 1);
}

auto string::icompare(view<string> x, view<string> y) -> int {
  return memory::icompare(x.data(), x.size() + 1, y.data(), y.size() + 1);
}

auto string::compare(view<string> source) const -> int {
  return memory::compare(data(), size() + 1, source.data(), source.size() + 1);
}

auto string::icompare(view<string> source) const -> int {
  return memory::icompare(data(), size() + 1, source.data(), source.size() + 1);
}

auto string::equals(view<string> source) const -> bool {
  if(size() != source.size()) return false;
  return memory::compare(data(), source.data(), source.size()) == 0;
}

auto string::iequals(view<string> source) const -> bool {
  if(size() != source.size()) return false;
  return memory::icompare(data(), source.data(), source.size()) == 0;
}

auto string::beginsWith(view<string> source) const -> bool {
  if(source.size() > size()) return false;
  return memory::compare(data(), source.data(), source.size()) == 0;
}

auto string::ibeginsWith(view<string> source) const -> bool {
  if(source.size() > size()) return false;
  return memory::icompare(data(), source.data(), source.size()) == 0;
}

auto string::endsWith(view<string> source) const -> bool {
  if(source.size() > size()) return false;
  return memory::compare(data() + size() - source.size(), source.data(), source.size()) == 0;
}

auto string::iendsWith(view<string> source) const -> bool {
  if(source.size() > size()) return false;
  return memory::icompare(data() + size() - source.size(), source.data(), source.size()) == 0;
}

}
