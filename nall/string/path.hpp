#pragma once

namespace nall {

// (/parent/child.type/)
// (/parent/child.type/)name.type
auto pathname(string_view self) -> string {
  const char* p = self.data() + self.size() - 1;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/') return slice(self, 0, offset + 1);
  }
  return "";  //no path found
}

// /parent/child.type/()
// /parent/child.type/(name.type)
auto filename(string_view self) -> string {
  const char* p = self.data() + self.size() - 1;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/') return slice(self, offset + 1);
  }
  return self;  //no path found
}

// (/parent/)child.type/
// (/parent/child.type/)name.type
auto dirname(string_view self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') return slice(self, 0, offset + 1);
  }
  return "";  //no path found
}

// /parent/(child.type/)
// /parent/child.type/(name.type)
auto basename(string_view self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') return slice(self, offset + 1);
  }
  return self;  //no path found
}

// /parent/(child).type/
// /parent/child.type/(name).type
auto prefixname(string_view self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(int offset = self.size() - 1, suffix = -1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') return slice(self, offset + 1, suffix >= 0 ? suffix - offset - 1 : 0).trimRight("/");
    if(*p == '.' && suffix == -1) { suffix = offset; continue; }
    if(offset == 0) return slice(self, offset, suffix).trimRight("/");
  }
  return "";  //no prefix found
}

// /parent/child(.type)/
// /parent/child.type/name(.type)
auto suffixname(string_view self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') break;
    if(*p == '.') return slice(self, offset).trimRight("/");
  }
  return "";  //no suffix found
}

}
