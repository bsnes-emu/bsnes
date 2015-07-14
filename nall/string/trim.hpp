#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

auto trim(string& self, rstring lhs, rstring rhs, long limit) -> string& {
  rtrim(self, rhs, limit);
  ltrim(self, lhs, limit);
  return self;
}

auto ltrim(string& self, rstring lhs, long limit) -> string& {
  if(lhs.size() == 0) return self;
  long matches = 0;
  while(matches < limit) {
    signed offset = lhs.size() * matches;
    signed size = (signed)self.size() - offset;
    if(size < (signed)lhs.size()) break;
    if(memory::compare(self.data() + offset, lhs.data(), lhs.size()) != 0) break;
    matches++;
  }
  if(matches) self.remove(0, lhs.size() * matches);
  return self;
}

auto rtrim(string& self, rstring rhs, long limit) -> string& {
  if(rhs.size() == 0) return self;
  long matches = 0;
  while(matches < limit) {
    signed offset = (signed)self.size() - rhs.size() * (matches + 1);
    signed size = (signed)self.size() - offset;
    if(offset < 0 || size < (signed)rhs.size()) break;
    if(memory::compare(self.data() + offset, rhs.data(), rhs.size()) != 0) break;
    matches++;
  }
  if(matches) self.resize(self.size() - rhs.size() * matches);
  return self;
}

auto itrim(string& self, rstring lhs, rstring rhs, long limit) -> string& {
  irtrim(self, rhs, limit);
  iltrim(self, lhs, limit);
  return self;
}

auto iltrim(string& self, rstring lhs, long limit) -> string& {
  if(lhs.size() == 0) return self;
  long matches = 0;
  while(matches < limit) {
    signed offset = lhs.size() * matches;
    signed size = (signed)self.size() - offset;
    if(size < (signed)lhs.size()) break;
    if(memory::icompare(self.data() + offset, lhs.data(), lhs.size()) != 0) break;
    matches++;
  }
  if(matches) self.remove(0, lhs.size() * matches);
  return self;
}

auto irtrim(string& self, rstring rhs, long limit) -> string& {
  if(rhs.size() == 0) return self;
  long matches = 0;
  while(matches < limit) {
    signed offset = (signed)self.size() - rhs.size() * (matches + 1);
    signed size = (signed)self.size() - offset;
    if(offset < 0 || size < (signed)rhs.size()) break;
    if(memory::icompare(self.data() + offset, rhs.data(), rhs.size()) != 0) break;
    matches++;
  }
  if(matches) self.resize(self.size() - rhs.size() * matches);
  return self;
}

auto strip(string& self) -> string& {
  rstrip(self);
  lstrip(self);
  return self;
}

auto lstrip(string& self) -> string& {
  unsigned size = 0;
  while(size < self.size()) {
    char input = self[size];
    if(input != ' ' && input != '\t' && input != '\r' && input != '\n') break;
    size++;
  }
  if(size) self.remove(0, size);
  return self;
}

auto rstrip(string& self) -> string& {
  unsigned size = 0;
  while(size < self.size()) {
    bool matched = false;
    char input = self[self.size() - size - 1];
    if(input != ' ' && input != '\t' && input != '\r' && input != '\n') break;
    size++;
  }
  if(size) self.resize(self.size() - size);
  return self;
}

}

#endif
