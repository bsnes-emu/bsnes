#ifndef NALL_STRING_UTILITY_HPP
#define NALL_STRING_UTILITY_HPP

size_t strlcpy(nall::string &dest, const char *src, size_t length) {
  dest.reserve(length);
  return strlcpy(dest(), src, length);
}

size_t strlcat(nall::string &dest, const char *src, size_t length) {
  dest.reserve(length);
  return strlcat(dest(), src, length);
}

nall::string substr(const char *src, size_t start, size_t length) {
  nall::string dest;
  if(length == 0) {
    //copy entire string
    dest = src + start;
  } else {
    //copy partial string
    strlcpy(dest, src + start, length + 1);
  }
  return dest;
}

/* very simplistic wrappers to return nall::string& instead of char* type */

nall::string& strlower(nall::string &str) { strlower(str()); return str; }
nall::string& strupper(nall::string &str) { strupper(str()); return str; }
nall::string& strtr(nall::string &dest, const char *before, const char *after) { strtr(dest(), before, after); return dest; }
nall::string& ltrim(nall::string &str, const char *key) { ltrim(str(), key); return str; }
nall::string& rtrim(nall::string &str, const char *key) { rtrim(str(), key); return str; }
nall::string& trim (nall::string &str, const char *key) { trim (str(), key); return str; }
nall::string& ltrim_once(nall::string &str, const char *key) { ltrim_once(str(), key); return str; }
nall::string& rtrim_once(nall::string &str, const char *key) { rtrim_once(str(), key); return str; }
nall::string& trim_once (nall::string &str, const char *key) { trim_once (str(), key); return str; }

/* arithmetic <> string */

template<unsigned length, char padding> nall::string strhex(uintmax_t value) {
  nall::string output;
  unsigned offset = 0;

  //render string backwards, as we do not know its length yet
  do {
    unsigned n = value & 15;
    output[offset++] = n < 10 ? '0' + n : 'a' + n - 10;
    value >>= 4;
  } while(value);

  while(offset < length) output[offset++] = padding;
  output[offset--] = 0;

  //reverse the string in-place
  for(unsigned i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return output;
}

template<unsigned length, char padding> nall::string strsigned(intmax_t value) {
  nall::string output;
  unsigned offset = 0;

  bool negative = value < 0;
  if(negative) value = abs(value);

  do {
    unsigned n = value % 10;
    output[offset++] = '0' + n;
    value /= 10;
  } while(value);

  while(offset < length) output[offset++] = padding;
  if(negative) output[offset++] = '-';
  output[offset--] = 0;

  for(unsigned i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return output;
}

template<unsigned length, char padding> nall::string strunsigned(uintmax_t value) {
  nall::string output;
  unsigned offset = 0;

  do {
    unsigned n = value % 10;
    output[offset++] = '0' + n;
    value /= 10;
  } while(value);

  while(offset < length) output[offset++] = padding;
  output[offset--] = 0;

  for(unsigned i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return output;
}

template<unsigned length, char padding> nall::string strbin(uintmax_t value) {
  nall::string output;
  unsigned offset = 0;

  do {
    unsigned n = value & 1;
    output[offset++] = '0' + n;
    value >>= 1;
  } while(value);

  while(offset < length) output[offset++] = padding;
  output[offset--] = 0;

  for(unsigned i = 0; i < (offset + 1) >> 1; i++) {
    char temp = output[i];
    output[i] = output[offset - i];
    output[offset - i] = temp;
  }

  return output;
}

//using sprintf is certainly not the most ideal method to convert
//a double to a string ... but attempting to parse a double by
//hand, digit-by-digit, results in subtle rounding errors.
size_t strdouble(char *str, double value) {
  char buffer[256];
  sprintf(buffer, "%f", value);

  //remove excess 0's in fraction (2.500000 -> 2.5)
  for(char *p = buffer; *p; p++) {
    if(*p == '.') {
      char *p = buffer + strlen(buffer) - 1;
      while(*p == '0') {
        if(*(p - 1) != '.') *p = 0;  //... but not for eg 1.0 -> 1.
        p--;
      }
      break;
    }
  }

  unsigned length = strlen(buffer);
  if(str) strcpy(str, buffer);
  return length + 1;
}

nall::string strdouble(double value) {
  nall::string temp;
  temp.reserve(strdouble(0, value));
  strdouble(temp(), value);
  return temp;
}

#endif
