#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

//limit defaults to zero, which will underflow on first compare; equivalent to no limit
template<unsigned Limit> char* ltrim(char* str, const char* key) {
  if(!str || !key || !*key) return str;
  unsigned limit = Limit;
  while(strbegin(str, key)) {
    char* dest = str;
    char* src = str + strlen(key);
    while(true) {
      *dest = *src++;
      if(!*dest) break;
      dest++;
    }
    if(--limit == 0) break;
  }
  return str;
}

template<unsigned Limit> char* rtrim(char* str, const char* key) {
  if(!str || !key || !*key) return str;
  unsigned limit = Limit;
  while(strend(str, key)) {
    str[strlen(str) - strlen(key)] = 0;
    if(--limit == 0) break;
  }
  return str;
}

template<unsigned limit> char* trim(char* str, const char* key, const char* rkey) {
  if(rkey) return ltrim<limit>(rtrim<limit>(str, rkey), key);
  return ltrim<limit>(rtrim<limit>(str, key), key);
}

//remove whitespace characters from both left and right sides of string
char* strip(char* s) {
  if(!s) return nullptr;

  signed n = 0, p = 0;
  while(s[n]) {
    if(s[n] != ' ' && s[n] != '\t' && s[n] != '\r' && s[n] != '\n') break;
    n++;
  }
  while(s[n]) s[p++] = s[n++];
  s[p--] = 0;
  while(p >= 0) {
    if(s[p] != ' ' && s[p] != '\t' && s[p] != '\r' && s[p] != '\n') break;
    p--;
  }
  s[++p] = 0;

  return s;
}

}

#endif
