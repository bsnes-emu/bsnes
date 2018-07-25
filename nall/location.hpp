#pragma once

namespace nall { namespace Location {

// (/parent/child.type/)
// (/parent/child.type/)name.type
inline auto path(view<string> self) -> string {
  const char* p = self.data() + self.size() - 1;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/') return slice(self, 0, offset + 1);
  }
  return "";  //no path found
}

// /parent/child.type/()
// /parent/child.type/(name.type)
inline auto file(view<string> self) -> string {
  const char* p = self.data() + self.size() - 1;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/') return slice(self, offset + 1);
  }
  return self;  //no path found
}

// (/parent/)child.type/
// (/parent/child.type/)name.type
inline auto dir(view<string> self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') return slice(self, 0, offset + 1);
  }
  return "";  //no path found
}

// /parent/(child.type/)
// /parent/child.type/(name.type)
inline auto base(view<string> self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') return slice(self, offset + 1);
  }
  return self;  //no path found
}

// /parent/(child).type/
// /parent/child.type/(name).type
inline auto prefix(view<string> self) -> string {
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
inline auto suffix(view<string> self) -> string {
  const char* p = self.data() + self.size() - 1, *last = p;
  for(int offset = self.size() - 1; offset >= 0; offset--, p--) {
    if(*p == '/' && p == last) continue;
    if(*p == '/') break;
    if(*p == '.') return slice(self, offset).trimRight("/");
  }
  return "";  //no suffix found
}

inline auto notsuffix(view<string> self) -> string {
  return {path(self), prefix(self)};
}

}}
