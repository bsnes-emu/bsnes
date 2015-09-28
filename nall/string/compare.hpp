#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive>
auto string::_compare(const char* target, unsigned capacity, const char* source, unsigned size) -> signed {
  if(Insensitive) return memory::icompare(target, capacity, source, size);
  return memory::compare(target, capacity, source, size);
}

auto string::compare(rstring source) const -> signed {
  return memory::compare(data(), size(), source.data(), source.size());
}

auto string::icompare(rstring source) const -> signed {
  return memory::icompare(data(), size(), source.data(), source.size());
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
