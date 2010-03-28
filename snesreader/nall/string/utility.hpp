#ifndef NALL_STRING_UTILITY_HPP
#define NALL_STRING_UTILITY_HPP

namespace nall {

unsigned strlcpy(string &dest, const char *src, unsigned length) {
  dest.reserve(length);
  return strlcpy(dest(), src, length);
}

unsigned strlcat(string &dest, const char *src, unsigned length) {
  dest.reserve(length);
  return strlcat(dest(), src, length);
}

string substr(const char *src, unsigned start, unsigned length) {
  string dest;
  if(length == 0) {
    //copy entire string
    dest = src + start;
  } else {
    //copy partial string
    strlcpy(dest, src + start, length + 1);
  }
  return dest;
}

/* very simplistic wrappers to return string& instead of char* type */

string& strlower(string &str) { strlower(str()); return str; }
string& strupper(string &str) { strupper(str()); return str; }
string& strtr(string &dest, const char *before, const char *after) { strtr(dest(), before, after); return dest; }
string& ltrim(string &str, const char *key) { ltrim(str(), key); return str; }
string& rtrim(string &str, const char *key) { rtrim(str(), key); return str; }
string& trim (string &str, const char *key) { trim (str(), key); return str; }
string& ltrim_once(string &str, const char *key) { ltrim_once(str(), key); return str; }
string& rtrim_once(string &str, const char *key) { rtrim_once(str(), key); return str; }
string& trim_once (string &str, const char *key) { trim_once (str(), key); return str; }

/* arithmetic <> string */

template<unsigned length, char padding> string strhex(uintmax_t value) {
  string output;
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

template<unsigned length, char padding> string strsigned(intmax_t value) {
  string output;
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

template<unsigned length, char padding> string strunsigned(uintmax_t value) {
  string output;
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

template<unsigned length, char padding> string strbin(uintmax_t value) {
  string output;
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
unsigned strdouble(char *str, double value) {
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

string strdouble(double value) {
  string temp;
  temp.reserve(strdouble(0, value));
  strdouble(temp(), value);
  return temp;
}

}

#endif
