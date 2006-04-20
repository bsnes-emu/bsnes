void split(string &dest, const char *key, const char *src) {
int  ssl = strlen(src), ksl = strlen(key);
uint lp = 0, split_count = 0;
  for(int32 i = 0; i <= ssl - ksl;) {
    if(!memcmp(src + i, key, ksl)) {
      strncpy(dest[split_count++], src + lp, i - lp);
      i += ksl;
      lp = i;
    } else i++;
  }
  strcpy(dest[split_count++], src + lp);
  dest.count = split_count;
}
void split(string &dest, const char *key, substring &src) { split(dest, key, strptr(src)); }

void qsplit(string &dest, const char *key, const char *src) {
int  z, ssl = strlen(src), ksl = strlen(key);
uint lp = 0, split_count = 0;
  for(int32 i = 0; i <= ssl - ksl;) {
  uint8 x = src[i];
    if(x == '\"' || x == '\'') {
      z = i++;
      while(src[i] != x && i < ssl)i++;
      if(i >= ssl)i = z;
    }
    if(!memcmp(src + i, key, ksl)) {
      strncpy(dest[split_count++], src + lp, i - lp);
      i += ksl;
      lp = i;
    } else i++;
  }
  strcpy(dest[split_count++], src + lp);
  dest.count = split_count;
}
void qsplit(string &dest, const char *key, substring &src) { qsplit(dest, key, strptr(src)); }
