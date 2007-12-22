#ifdef BSTRING_H

uint count(lstring &str) {
  return str.size();
}

int find(lstring &str, const char *key) {
  for(uint i = 0; i < count(str); i++) {
    if(str[i] == key) return i;
  }
  return -1;
}

#endif //ifdef BSTRING_H
