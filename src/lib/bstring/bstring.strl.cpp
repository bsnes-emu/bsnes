#ifdef BSTRING_H

uint strlcpy(char *dest, const char *src, uint length) {
uint srclen = strlen(src);
  length--;
  if(length > srclen) length = srclen;
  memcpy(dest, src, length);
  dest[length] = 0;
  return srclen;
}

uint strlcat(char *dest, const char *src, uint length) {
uint destlen = strlen(dest), srclen = strlen(src);
  length--;
  if(length > destlen + srclen) length = destlen + srclen;
  memcpy(dest + destlen, src, length - destlen);
  dest[length] = 0;
  return destlen + srclen;
}

uint strlcpy(string &dest, const char *src, uint length) {
  dest.reserve(length);
  return strlcpy(dest(), src, length);
}

uint strlcat(string &dest, const char *src, uint length) {
  dest.reserve(length);
  return strlcat(dest(), src, length);
}

#endif //ifdef BSTRING_H
