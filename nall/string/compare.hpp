#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive>
auto string::_compare(const char* target, unsigned capacity, const char* source, unsigned size) -> signed {
  if(Insensitive) return memory::icompare(target, capacity, source, size);
  return memory::compare(target, capacity, source, size);
}

//size() + 1 includes null-terminator; required to properly compare strings of differing lengths
auto string::compare(rstring x, rstring y) -> signed {
  return memory::compare(x.data(), x.size() + 1, y.data(), y.size() + 1);
}

auto string::icompare(rstring x, rstring y) -> signed {
  return memory::icompare(x.data(), x.size() + 1, y.data(), y.size() + 1);
}

auto string::compare(rstring source) const -> signed {
  return memory::compare(data(), size() + 1, source.data(), source.size() + 1);
}

auto string::icompare(rstring source) const -> signed {
  return memory::icompare(data(), size() + 1, source.data(), source.size() + 1);
}

auto string::equals(rstring source) const -> bool {
  if(size() != source.size()) return false;
  return memory::compare(data(), source.data(), source.size()) == 0;
}

auto string::iequals(rstring source) const -> bool {
  if(size() != source.size()) return false;
  return memory::icompare(data(), source.data(), source.size()) == 0;
}

auto string::beginsWith(rstring source) const -> bool {
  if(source.size() > size()) return false;
  return memory::compare(data(), source.data(), source.size()) == 0;
}

auto string::ibeginsWith(rstring source) const -> bool {
  if(source.size() > size()) return false;
  return memory::icompare(data(), source.data(), source.size()) == 0;
}

auto string::endsWith(rstring source) const -> bool {
  if(source.size() > size()) return false;
  return memory::compare(data() + size() - source.size(), source.data(), source.size()) == 0;
}

auto string::iendsWith(rstring source) const -> bool {
  if(source.size() > size()) return false;
  return memory::icompare(data() + size() - source.size(), source.data(), source.size()) == 0;
}

}

#endif
