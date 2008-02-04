#ifndef NALL_STRING_SPLIT_CPP
#define NALL_STRING_SPLIT_CPP

void split(nall::lstring &dest, const char *key, const char *src, size_t limit) {
  dest.reset();

  int ssl = strlen(src), ksl = strlen(key);
  int lp = 0, split_count = 0;

  for(int i = 0; i <= ssl - ksl;) {
    if(!memcmp(src + i, key, ksl)) {
      strlcpy(dest[split_count++], src + lp, i - lp + 1);
      i += ksl;
      lp = i;
      if(!--limit) break;
    } else i++;
  }

  strcpy(dest[split_count++], src + lp);
}

void qsplit(nall::lstring &dest, const char *key, const char *src, size_t limit) {
  dest.reset();

  int ssl = strlen(src), ksl = strlen(key);
  int lp = 0, split_count = 0;

  for(int i = 0; i <= ssl - ksl;) {
    uint8_t x = src[i];

    if(x == '\"' || x == '\'') {
      int z = i++; //skip opening quote
      while(i < ssl && src[i] != x) i++;
      if(i >= ssl) i = z; //failed match, rewind i
      else {
        i++; //skip closing quote
        continue; //restart in case next char is also a quote
      }
    }

    if(!memcmp(src + i, key, ksl)) {
      strlcpy(dest[split_count++], src + lp, i - lp + 1);
      i += ksl;
      lp = i;
      if(!--limit) break;
    } else i++;
  }

  strcpy(dest[split_count++], src + lp);
}

#endif //ifndef NALL_STRING_SPLIT_CPP
