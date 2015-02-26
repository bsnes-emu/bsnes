#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

auto trim(string& self, rstring lhs, rstring rhs) -> bool {
  if(lhs.size() + rhs.size() > self.size()) return false;
  if(memory::compare(self.data(), lhs.data(), lhs.size()) != 0) return false;
  if(memory::compare(self.data() + self.size() - rhs.size(), rhs.data(), rhs.size()) != 0) return false;
  self.resize(self.size() - rhs.size());
  self.remove(0, lhs.size());
  return true;
}

auto ltrim(string& self, rstring lhs) -> bool {
  if(lhs.size() > self.size()) return false;
  if(memory::compare(self.data(), lhs.data(), lhs.size()) != 0) return false;
  self.remove(0, lhs.size());
  return true;
}

auto rtrim(string& self, rstring rhs) -> bool {
  if(rhs.size() > self.size()) return false;
  if(memory::compare(self.data() + self.size() - rhs.size(), rhs.data(), rhs.size()) != 0) return false;
  self.resize(self.size() - rhs.size());
  return true;
}

auto itrim(string& self, rstring lhs, rstring rhs) -> bool {
  if(lhs.size() + rhs.size() > self.size()) return false;
  if(memory::icompare(self.data(), lhs.data(), lhs.size()) != 0) return false;
  if(memory::icompare(self.data() + self.size() - rhs.size(), rhs.data(), rhs.size()) != 0) return false;
  self.resize(self.size() - rhs.size());
  self.remove(0, lhs.size());
  return true;
}

auto iltrim(string& self, rstring lhs) -> bool {
  if(lhs.size() > self.size()) return false;
  if(memory::icompare(self.data(), lhs.data(), lhs.size()) != 0) return false;
  self.remove(0, lhs.size());
  return true;
}

auto irtrim(string& self, rstring rhs) -> bool {
  if(rhs.size() > self.size()) return false;
  if(memory::icompare(self.data() + self.size() - rhs.size(), rhs.data(), rhs.size()) != 0) return false;
  self.resize(self.size() - rhs.size());
  return true;
}

auto strip(string& self) -> bool {
  return rstrip(self) | lstrip(self);
}

auto lstrip(string& self) -> bool {
  unsigned size = 0;
  while(size < self.size()) {
    char input = self[size];
    if(input != ' ' && input != '\t' && input != '\r' && input != '\n') break;
    size++;
  }
  if(size == 0) return false;
  self.remove(0, size);
  return true;
}

auto rstrip(string& self) -> bool {
  unsigned size = 0;
  while(size < self.size()) {
    bool matched = false;
    char input = self[self.size() - size - 1];
    if(input != ' ' && input != '\t' && input != '\r' && input != '\n') break;
    size++;
  }
  if(size == 0) return false;
  self.resize(self.size() - size);
  return true;
}

}

#endif
