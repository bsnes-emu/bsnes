#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

string substr(rstring source, unsigned offset, unsigned length) {
  string result;
  if(length == ~0u) length = source.size() - offset;
  result.resize(length);
  memcpy(result.data(), source.data() + offset, length);
  return result;
}

string sha256(const uint8_t* data, unsigned size) {
  sha256_ctx sha;
  uint8_t hash[32];
  sha256_init(&sha);
  sha256_chunk(&sha, data, size);
  sha256_final(&sha);
  sha256_hash(&sha, hash);
  string result;
  for(auto& byte : hash) result.append(hex<2>(byte));
  return result;
}

bool tokenize(lstring& list, const char* s, const char* p) {
  while(*s) {
    if(*p == '*') {
      const char* b = s;
      while(*s) {
        if(tokenize(list, s++, p + 1)) {
          list.prepend(substr(b, 0, --s - b));
          return true;
        }
      }
      list.prepend(b);
      return !*++p;
    }
    if(*s++ != *p++) return false;
  }
  while(*p == '*') { list.prepend(s); p++; }
  return !*p;
}

char* integer(char* result, intmax_t value) {
  bool negative = value < 0;
  if(negative) value = -value;

  char buffer[64];
  unsigned size = 0;

  do {
    unsigned n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);
  if(negative) buffer[size++] = '-';
//buffer[size++] = negative ? '-' : '+';

  for(signed x = size - 1, y = 0; x >= 0 && y < size; x--, y++) result[x] = buffer[y];
  result[size] = 0;
  return result;
}

char* decimal(char* result, uintmax_t value) {
  char buffer[64];
  unsigned size = 0;

  do {
    unsigned n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);

  for(signed x = size - 1, y = 0; x >= 0 && y < size; x--, y++) result[x] = buffer[y];
  result[size] = 0;
  return result;
}

//using sprintf is certainly not the most ideal method to convert
//a double to a string ... but attempting to parse a double by
//hand, digit-by-digit, results in subtle rounding errors.
unsigned real(char* str, long double value) {
  char buffer[256];
  #ifdef _WIN32
  //Windows C-runtime does not support long double via sprintf()
  sprintf(buffer, "%f", (double)value);
  #else
  sprintf(buffer, "%Lf", value);
  #endif

  //remove excess 0's in fraction (2.500000 -> 2.5)
  for(char* p = buffer; *p; p++) {
    if(*p == '.') {
      char* p = buffer + strlen(buffer) - 1;
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

string real(long double value) {
  string temp;
  temp.resize(real(nullptr, value));
  real(temp.data(), value);
  return temp;
}

}

#endif
