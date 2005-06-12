void split(string &dest, char *key, char *src) {
int i, ssl = strlen(src), ksl = strlen(key);
uint8 x;
uint32 lp = 0, split_count = 0;
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

void qsplit(string &dest, char *key, char *src) {
int i, z, ssl = strlen(src), ksl = strlen(key);
uint8 x;
uint32 lp = 0, split_count = 0;
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
