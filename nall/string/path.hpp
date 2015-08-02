#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

// (/parent/child.type/)
// (/parent/child.type/)name.type
auto pathname(const string& self) -> string {
  const char* p = self.data() + self.size() - 1;
  for(signed offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/') return slice(self, 0, offset + 1);
  }
  return "";
}

// /parent/child.type/()
// /parent/child.type/(name.type)
auto filename(const string& self) -> string {
  const char* p = self.data() + self.size() - 1;
  for(signed offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/') return slice(self, offset + 1);
  }
  return "";
}

// (/parent/)child.type/
// (/parent/child.type/)name.type
auto dirname(const string& self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(signed offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') return slice(self, 0, offset + 1);
  }
  return self;  //this is the root directory
}

// /parent/(child.type/)
// /parent/child.type/(name.type)
auto basename(const string& self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(signed offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') return slice(self, offset + 1);
  }
  return "";
}

// /parent/(child).type/
// /parent/child.type/(name).type
auto prefixname(const string& self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(signed offset = self.size() - 1, suffix = -1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') return slice(self, offset + 1, suffix >= 0 ? suffix - offset - 1 : 0).rtrim("/");
    if(*p == '.' && suffix == -1) { suffix = offset; continue; }
    if(offset == 0) return slice(self, offset, suffix).rtrim("/");
  }
  return "";
}

// /parent/child(.type)/
// /parent/child.type/name(.type)
auto suffixname(const string& self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(signed offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') break;
    if(*p == '.') return slice(self, offset).rtrim("/");
  }
  return "";
}

}

#endif
