void split(stringlist *dest, char *key, char *src) {
int i, ssl = strlen(src), ksl = strlen(key);
byte x;
ulong lp = 0, split_count = 0;
  for(i=0;i<=ssl-ksl;) {
    if(!memcmp(src + i, key, ksl)) {
      x = src[i];
      src[i] = 0;
      strcpy(dest, split_count++, src + lp);
      src[i] = x;
      i += ksl;
      lp = i;
    } else i++;
  }
  strcpy(dest, split_count++, src + lp);
}
void split(stringlist *dest, char *key, string *src) { split(dest, key, src->s); }

void split_q(stringlist *dest, char *key, char *src) {
int i, z, ssl = strlen(src), ksl = strlen(key);
byte x;
ulong lp = 0, split_count = 0;
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
      strcpy(dest, split_count++, src + lp);
      src[i] = x;
      i += ksl;
      lp = i;
    } else i++;
  }
  strcpy(dest, split_count++, src + lp);
}
void split_q(stringlist *dest, char *key, string *src) { split_q(dest, key, src->s); }
