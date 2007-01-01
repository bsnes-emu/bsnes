void split(stringarray &dest, const char *key, const char *src) {
  dest.reset();

int ssl = strlen(src), ksl = strlen(key);
int lp = 0, split_count = 0;
  for(int i = 0; i <= ssl - ksl;) {
    if(!memcmp(src + i, key, ksl)) {
      strlcpy(dest[split_count++], src + lp, i - lp + 1);
      i += ksl;
      lp = i;
    } else i++;
  }
  strcpy(dest[split_count++], src + lp);
}
void split(stringarray &dest, const char *key, string &src) { split(dest, key, strptr(src)); }

void qsplit(stringarray &dest, const char *key, const char *src) {
  dest.reset();

int z, ssl = strlen(src), ksl = strlen(key);
int lp = 0, split_count = 0;
  for(int i = 0; i <= ssl - ksl;) {
  uint8 x = src[i];
    if(x == '\"' || x == '\'') {
      z = i++;
      while(src[i] != x && i < ssl)i++;
      if(i >= ssl)i = z;
    }
    if(!memcmp(src + i, key, ksl)) {
      strlcpy(dest[split_count++], src + lp, i - lp + 1);
      i += ksl;
      lp = i;
    } else i++;
  }
  strcpy(dest[split_count++], src + lp);
}
void qsplit(stringarray &dest, const char *key, string &src) { qsplit(dest, key, strptr(src)); }
