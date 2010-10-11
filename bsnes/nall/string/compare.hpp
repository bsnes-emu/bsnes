#ifndef NALL_STRING_COMPARE_HPP
#define NALL_STRING_COMPARE_HPP

namespace nall {

char chrlower(char c) {
  return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

char chrupper(char c) {
  return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
}

int stricmp(const char *dest, const char *src) {
  while(*dest) {
    if(chrlower(*dest) != chrlower(*src)) break;
    dest++;
    src++;
  }

  return (int)chrlower(*dest) - (int)chrlower(*src);
}

int strwcmp(const char *str, const char *pattern, unsigned length) {
  while(length && *str) {
    if(*pattern != '?' && *str != *pattern) break;
    pattern++, str++, length--;
  }

  if(length == 0 || *pattern == '?') return 0;
  return (int)chrlower(*str) - (int)chrlower(*pattern);
}

int strwicmp(const char *str, const char *pattern, unsigned length) {
  while(length && *str) {
    if(*pattern != '?' && chrlower(*str) != chrlower(*pattern)) break;
    pattern++, str++, length--;
  }

  if(length == 0 || *pattern == '?') return 0;
  return (int)chrlower(*str) - (int)chrlower(*pattern);
}

bool wildcard(const char *str, const char *pattern) {
  while(*pattern) {
    char n = *pattern++;
    if(n == '*') {
      unsigned length = 0;
      while(true) {
        n = pattern[length];
        if(n == 0 || n == '*') break;
        length++;
      }
      if(length) while(true) {
        if(*str == 0) return false;
        if(!strwcmp(str, pattern, length)) break;
        str++;
      }
    } else if(n == '?') {
      str++;
    } else {
      if(*str++ != n) return false;
    }
  }
  return true;
}

bool iwildcard(const char *str, const char *pattern) {
  while(*pattern) {
    char n = *pattern++;
    if(n == '*') {
      unsigned length = 0;
      while(true) {
        n = pattern[length];
        if(n == 0 || n == '*') break;
        length++;
      }
      if(length) while(true) {
        if(*str == 0) return false;
        if(!strwicmp(str, pattern, length)) break;
        str++;
      }
    } else if(n == '?') {
      str++;
    } else {
      if(chrlower(*str++) != chrlower(n)) return false;
    }
  }
  return true;
}

bool strbegin(const char *str, const char *key) {
  int i, ssl = strlen(str), ksl = strlen(key);

  if(ksl > ssl) return false;
  return (!memcmp(str, key, ksl));
}

bool stribegin(const char *str, const char *key) {
  int ssl = strlen(str), ksl = strlen(key);

  if(ksl > ssl) return false;
  for(int i = 0; i < ksl; i++) {
    if(str[i] >= 'A' && str[i] <= 'Z') {
      if(str[i] != key[i] && str[i]+0x20 != key[i])return false;
    } else if(str[i] >= 'a' && str[i] <= 'z') {
      if(str[i] != key[i] && str[i]-0x20 != key[i])return false;
    } else {
      if(str[i] != key[i])return false;
    }
  }
  return true;
}

bool strend(const char *str, const char *key) {
  int ssl = strlen(str), ksl = strlen(key);

  if(ksl > ssl) return false;
  return (!memcmp(str + ssl - ksl, key, ksl));
}

bool striend(const char *str, const char *key) {
  int ssl = strlen(str), ksl = strlen(key);

  if(ksl > ssl) return false;
  for(int i = ssl - ksl, z = 0; i < ssl; i++, z++) {
    if(str[i] >= 'A' && str[i] <= 'Z') {
      if(str[i] != key[z] && str[i]+0x20 != key[z])return false;
    } else if(str[i] >= 'a' && str[i] <= 'z') {
      if(str[i] != key[z] && str[i]-0x20 != key[z])return false;
    } else {
      if(str[i] != key[z])return false;
    }
  }
  return true;
}

}

#endif
