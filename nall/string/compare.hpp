#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive> inline auto _compare(const char* target, unsigned capacity, const char* source, unsigned size) -> signed {
  if(Insensitive) return memory::icompare(target, capacity, source, size);
  return memory::compare(target, capacity, source, size);
}

auto compare(const string& self, rstring source) -> signed {
  return memory::compare(self.data(), self.size(), source.data(), source.size());
}

auto icompare(const string& self, rstring source) -> signed {
  return memory::icompare(self.data(), self.size(), source.data(), source.size());
}

auto equals(const string& self, rstring source) -> bool {
  if(self.size() != source.size()) return false;
  return memory::compare(self.data(), source.data(), source.size()) == 0;
}

auto iequals(const string& self, rstring source) -> bool {
  if(self.size() != source.size()) return false;
  return memory::icompare(self.data(), source.data(), source.size()) == 0;
}

auto beginsWith(const string& self, rstring source) -> bool {
  if(source.size() > self.size()) return false;
  return memory::compare(self.data(), source.data(), source.size()) == 0;
}

auto ibeginsWith(const string& self, rstring source) -> bool {
  if(source.size() > self.size()) return false;
  return memory::icompare(self.data(), source.data(), source.size()) == 0;
}

auto endsWith(const string& self, rstring source) -> bool {
  if(source.size() > self.size()) return false;
  return memory::compare(self.data() + self.size() - source.size(), source.data(), source.size()) == 0;
}

auto iendsWith(const string& self, rstring source) -> bool {
  if(source.size() > self.size()) return false;
  return memory::icompare(self.data() + self.size() - source.size(), source.data(), source.size()) == 0;
}

}

#endif
