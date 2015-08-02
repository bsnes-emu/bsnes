#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

auto downcase(string& self) -> string& {
  char* p = self.get();
  for(unsigned n = 0; n < self.size(); n++) {
    if(p[n] >= 'A' && p[n] <= 'Z') p[n] += 0x20;
  }
  return self;
}

auto qdowncase(string& self) -> string& {
  char* p = self.get();
  for(unsigned n = 0, quoted = 0; n < self.size(); n++) {
    if(p[n] == '\"') quoted ^= 1;
    if(!quoted && p[n] >= 'A' && p[n] <= 'Z') p[n] += 0x20;
  }
  return self;
}

auto upcase(string& self) -> string& {
  char* p = self.get();
  for(unsigned n = 0; n < self.size(); n++) {
    if(p[n] >= 'a' && p[n] <= 'z') p[n] -= 0x20;
  }
  return self;
}

auto qupcase(string& self) -> string& {
  char* p = self.get();
  for(unsigned n = 0, quoted = 0; n < self.size(); n++) {
    if(p[n] == '\"') quoted ^= 1;
    if(!quoted && p[n] >= 'a' && p[n] <= 'z') p[n] -= 0x20;
  }
  return self;
}

auto transform(string& self, rstring from, rstring to) -> string& {
  if(from.size() != to.size() || from.size() == 0) return self;  //patterns must be the same length
  char* p = self.get();
  for(unsigned n = 0; n < self.size(); n++) {
    for(unsigned s = 0; s < from.size(); s++) {
      if(p[n] == from[s]) {
        p[n] = to[s];
        break;
      }
    }
  }
  return self;
}

}

#endif
