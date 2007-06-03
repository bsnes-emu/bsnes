uint count(stringarray &str) {
  return str.size();
}

int find(stringarray &str, const char *key) {
  for(uint i = 0; i < count(str); i++) {
    if(str[i] == key) { return i; }
  }
  return -1;
}
int find(stringarray &str, const string &key) { return find(str, strptr(key)); }
