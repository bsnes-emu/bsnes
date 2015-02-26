#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

//todo: these functions are not binary-safe

auto match(const string& self, rstring source) -> bool {
  const char* s = self.data();
  const char* p = source.data();

  const char* cp = nullptr;
  const char* mp = nullptr;
  while(*s && *p != '*') {
    if(*p != '?' && *s != *p) return false;
    p++, s++;
  }
  while(*s) {
    if(*p == '*') {
      if(!*++p) return true;
      mp = p, cp = s + 1;
    } else if(*p == '?' || *p == *s) {
      p++, s++;
    } else {
      p = mp, s = cp++;
    }
  }
  while(*p == '*') p++;
  return !*p;
}

auto imatch(const string& self, rstring source) -> bool {
  static auto chrlower = [](char c) -> char {
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
  };

  const char* s = self.data();
  const char* p = source.data();

  const char* cp = nullptr;
  const char* mp = nullptr;
  while(*s && *p != '*') {
    if(*p != '?' && chrlower(*s) != chrlower(*p)) return false;
    p++, s++;
  }
  while(*s) {
    if(*p == '*') {
      if(!*++p) return true;
      mp = p, cp = s + 1;
    } else if(*p == '?' || chrlower(*p) == chrlower(*s)) {
      p++, s++;
    } else {
      p = mp, s = cp++;
    }
  }
  while(*p == '*') p++;
  return !*p;
}

inline bool tokenize(const char* s, const char* p) {
  while(*s) {
    if(*p == '*') {
      while(*s) if(tokenize(s++, p + 1)) return true;
      return !*++p;
    }
    if(*s++ != *p++) return false;
  }
  while(*p == '*') p++;
  return !*p;
}

auto tokenize(lstring& list, const char* s, const char* p) -> bool {
  while(*s) {
    if(*p == '*') {
      const char* b = s;
      while(*s) {
        if(tokenize(list, s++, p + 1)) {
          list.prepend(substr(b, 0, --s - b));
          return true;
        }
      }
      list.prepend(b);
      return !*++p;
    }
    if(*s++ != *p++) return false;
  }
  while(*p == '*') { list.prepend(s); p++; }
  return !*p;
}

}

#endif
