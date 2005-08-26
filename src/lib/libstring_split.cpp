void split(string &dest, const char *key, char *src) {
int   i, ssl = strlen(src), ksl = strlen(key);
uint  lp = 0, split_count = 0;
uint8 x;
  for(i=0;i<=ssl-ksl;) {
    if(!memcmp(src + i, key, ksl)) {
      x = src[i];
      src[i] = 0;
      strcpy(dest[split_count++], src + lp);
      src[i] = x;
      i += ksl;
      lp = i;
    } else i++;
  }
  strcpy(dest[split_count++], src + lp);
  dest.count = split_count;
}
void split(string &dest, const char *key, substring &src) { split(dest, key, strptr(src)); }

void qsplit(string &dest, const char *key, char *src) {
int   i, z, ssl = strlen(src), ksl = strlen(key);
uint  lp = 0, split_count = 0;
uint8 x;
  for(i=0;i<=ssl-ksl;) {
    x = src[i];
    if(x=='\"' || x=='\'') {
      z = i++;
      while(src[i] != x && i < ssl)i++;
      if(i >= ssl)i = z;
    }
    if(!memcmp(src + i, key, ksl)) {
      x = src[i];
      src[i] = 0;
      strcpy(dest[split_count++], src + lp);
      src[i] = x;
      i += ksl;
      lp = i;
    } else i++;
  }
  strcpy(dest[split_count++], src + lp);
  dest.count = split_count;
}
void qsplit(string &dest, const char *key, substring &src) { qsplit(dest, key, strptr(src)); }
