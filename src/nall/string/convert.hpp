#ifndef NALL_STRING_CONVERT_HPP
#define NALL_STRING_CONVERT_HPP

namespace nall {

char* strlower(char *str) {
  if(!str) return 0;
  int i = 0;
  while(str[i]) {
    str[i] = chrlower(str[i]);
    i++;
  }
  return str;
}

char* strupper(char *str) {
  if(!str) return 0;
  int i = 0;
  while(str[i]) {
    str[i] = chrupper(str[i]);
    i++;
  }
  return str;
}

char* strtr(char *dest, const char *before, const char *after) {
  if(!dest || !before || !after) return dest;
  int sl = strlen(dest), bsl = strlen(before), asl = strlen(after);

  if(bsl != asl || bsl == 0) return dest;  //patterns must be the same length for 1:1 replace
  for(unsigned i = 0; i < sl; i++) {
    for(unsigned l = 0; l < bsl; l++) {
      if(dest[i] == before[l]) {
        dest[i] = after[l];
        break;
      }
    }
  }

  return dest;
}

string& string::lower() { nall::strlower(data); return *this; }
string& string::upper() { nall::strupper(data); return *this; }
string& string::transform(const char *before, const char *after) { nall::strtr(data, before, after); return *this; }

uintmax_t strhex(const char *str) {
  if(!str) return 0;
  uintmax_t result = 0;

  //skip hex identifiers 0x and $, if present
  if(*str == '0' && (*(str + 1) == 'X' || *(str + 1) == 'x')) str += 2;
  else if(*str == '$') str++;

  while(*str) {
    uint8_t x = *str++;
    if(x >= '0' && x <= '9') x -= '0';
    else if(x >= 'A' && x <= 'F') x -= 'A' - 10;
    else if(x >= 'a' && x <= 'f') x -= 'a' - 10;
    else break;  //stop at first invalid character
    result = result * 16 + x;
  }

  return result;
}

intmax_t strsigned(const char *str) {
  if(!str) return 0;
  intmax_t result = 0;
  bool negate = false;

  //check for negation
  if(*str == '-') {
    negate = true;
    str++;
  }

  while(*str) {
    uint8_t x = *str++;
    if(x >= '0' && x <= '9') x -= '0';
    else break;  //stop at first invalid character
    result = result * 10 + x;
  }

  return !negate ? result : -result;
}

uintmax_t strunsigned(const char *str) {
  if(!str) return 0;
  uintmax_t result = 0;

  while(*str) {
    uint8_t x = *str++;
    if(x >= '0' && x <= '9') x -= '0';
    else break;  //stop at first invalid character
    result = result * 10 + x;
  }

  return result;
}

uintmax_t strbin(const char *str) {
  if(!str) return 0;
  uintmax_t result = 0;

  //skip bin identifiers 0b and %, if present
  if(*str == '0' && (*(str + 1) == 'B' || *(str + 1) == 'b')) str += 2;
  else if(*str == '%') str++;

  while(*str) {
    uint8_t x = *str++;
    if(x == '0' || x == '1') x -= '0';
    else break;  //stop at first invalid character
    result = result * 2 + x;
  }

  return result;
}

double strdouble(const char *str) {
  if(!str) return 0.0;
  bool negate = false;

  //check for negation
  if(*str == '-') {
    negate = true;
    str++;
  }

  intmax_t result_integral = 0;
  while(*str) {
    uint8_t x = *str++;
    if(x >= '0' && x <= '9') x -= '0';
    else if(x == '.' || x == ',') break;  //break loop and read fractional part
    else return (double)result_integral;  //invalid value, assume no fractional part
    result_integral = result_integral * 10 + x;
  }

  intmax_t result_fractional = 0;
  while(*str) {
    uint8_t x = *str++;
    if(x >= '0' && x <= '9') x -= '0';
    else break;  //stop at first invalid character
    result_fractional = result_fractional * 10 + x;
  }

  //calculate fractional portion
  double result = (double)result_fractional;
  while((uintmax_t)result > 0) result /= 10.0;
  result += (double)result_integral;

  return !negate ? result : -result;
}

}

#endif
