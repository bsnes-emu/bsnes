#ifndef NALL_STRING_STRPOS_HPP
#define NALL_STRING_STRPOS_HPP

//usage example:
//if(auto pos = strpos(str, key)) print(pos(), "\n");
//prints position of key within str, only if it is found

namespace nall {

class strpos {
  bool found;
  unsigned position;
public:
  inline operator bool() const { return found; }
  inline unsigned operator()() const { return position; }
  inline strpos(const char *str, const char *key) : found(false), position(0) {
    unsigned ssl = strlen(str), ksl = strlen(key);
    if(ksl > ssl) return;

    for(unsigned i = 0; i <= ssl - ksl; i++) {
      if(!memcmp(str + i, key, ksl)) {
        found = true;
        position = i;
        return;
      }
    }
  }
};

class qstrpos {
  bool found;
  unsigned position;
public:
  inline operator bool() const { return found; }
  inline unsigned operator()() const { return position; }
  inline qstrpos(const char *str, const char *key) : found(false), position(0) {
    unsigned ssl = strlen(str), ksl = strlen(key);
    if(ksl > ssl) return;

    for(unsigned i = 0; i <= ssl - ksl;) {
      uint8_t x = str[i];
      if(x == '\"' || x == '\'') {
        uint8_t z = i++;
        while(str[i] != x && i < ssl) i++;
        if(i >= ssl) i = z;
      }
      if(!memcmp(str + i, key, ksl)) {
        found = true;
        position = i;
        return;
      } else {
        i++;
      }
    }
  }
};

}

#endif
