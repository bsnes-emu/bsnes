#ifndef NALL_STRING_STRPOS_HPP
#define NALL_STRING_STRPOS_HPP

//usage example:
//if(auto pos = strpos(str, key)) print(pos(), "\n");
//prints position of key within str, only if it is found

namespace nall {

optional<unsigned> inline strpos(const char *str, const char *key) {
  unsigned ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl) return { false, 0 };

  for(unsigned i = 0; i <= ssl - ksl; i++) {
    if(!memcmp(str + i, key, ksl)) return { true, i };
  }

  return { false, 0 };
}

optional<unsigned> inline qstrpos(const char *str, const char *key) {
  unsigned ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl) return { false, 0 };

  for(unsigned i = 0; i <= ssl - ksl;) {
    uint8_t x = str[i];
    if(x == '\"' || x == '\'') {
      uint8_t z = i++;
      while(str[i] != x && i < ssl) i++;
      if(i >= ssl) i = z;
    }
    if(!memcmp(str + i, key, ksl)) return { true, i };
    i++;
  }

  return { false, 0 };
}

}

#endif
