#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

bool strmatch(const char* s, const char* p) {
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

bool istrmatch(const char* s, const char* p) {
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

bool tokenize(const char* s, const char* p) {
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

}

#endif
