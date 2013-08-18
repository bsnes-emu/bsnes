#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<bool Insensitive>
bool chrequal(char x, char y) {
  if(Insensitive) return chrlower(x) == chrlower(y);
  return x == y;
}

template<bool Quoted, typename T>
bool quoteskip(T*& p) {
  if(Quoted == false) return false;
  if(*p != '\'' && *p != '\"') return false;

  while(*p == '\'' || *p == '\"') {
    char x = *p++;
    while(*p && *p++ != x);
  }
  return true;
}

template<bool Quoted, typename T>
bool quotecopy(char*& t, T*& p) {
  if(Quoted == false) return false;
  if(*p != '\'' && *p != '\"') return false;

  while(*p == '\'' || *p == '\"') {
    char x = *p++;
    *t++ = x;
    while(*p && *p != x) *t++ = *p++;
    *t++ = *p++;
  }
  return true;
}

}

#endif
