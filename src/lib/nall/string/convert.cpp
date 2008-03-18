#ifdef NALL_STRING_CPP

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

  if(bsl != asl || bsl == 0) return dest; //patterns must be the same length for 1:1 replace
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

//note: ISO C++ only guarantees that 0-9 is contigious,
//it does not guarantee that either A-F or a-f are also contigious
//however, A-F and a-f are contigious on virtually every platform in existence
//the optimizations and simplifications made possible by this are therefore unignorable
//however, just to be safe, this is tested at compile-time with the below assertion ...
//if false, a compiler error will be thrown

static nall::static_assert<
  ('A' == 'B' - 1) && ('B' == 'C' - 1) && ('C' == 'D' - 1) && ('D' == 'E' - 1) && ('E' == 'F' - 1) &&
  ('a' == 'b' - 1) && ('b' == 'c' - 1) && ('c' == 'd' - 1) && ('d' == 'e' - 1) && ('e' == 'f' - 1)
> hex_contigious_assertion_;

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
    else break; //stop at first invalid character
    result = result * 16 + x;
  }

  return result;
}

intmax_t strdec(const char *str) {
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
    else break; //stop at first invalid character
    result = result * 10 + x;
  }

  return !negate ? result : -result;
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
    else break; //stop at first invalid character
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
    else if(x == '.') break; //break loop and read fractional part
    else return (double)result_integral; //invalid value, assume no fractional part
    result_integral = result_integral * 10 + x;
  }

  intmax_t result_fractional = 0;
  while(*str) {
    uint8_t x = *str++;
    if(x >= '0' && x <= '9') x -= '0';
    else break; //stop at first invalid character
    result_fractional = result_fractional * 10 + x;
  }

  //calculate fractional portion
  double result = (double)result_fractional;
  while((uintmax_t)result > 0) result /= 10.0;
  result += (double)result_integral;

  return !negate ? result : -result;
}

size_t strhex(char *str, uintmax_t value, size_t length /* = 0 */) {
  if(!length) length -= 1U; //"infinite" length
  size_t initial_length = length;

  //count number of digits in value
  int digits_integral = 1;
  uintmax_t digits_integral_ = value;
  while(digits_integral_ /= 16) digits_integral++;

  int digits = digits_integral;
  if(!str) return digits + 1; //only computing required length?

  length = nall::min(digits, length - 1);
  str += length; //seek to end of target string
  *str = 0; //set null terminator

  while(length--) {
    uint8_t x = value % 16;
    value /= 16;
    *--str = x < 10 ? (x + '0') : (x + 'a' - 10); //iterate backwards to write string
  }

  return nall::min(initial_length, digits + 1);
}

size_t strdec(char *str, intmax_t value_, size_t length /* = 0 */) {
  if(!length) length = -1U; //"infinite" length
  size_t initial_length = length;

  bool negate = value_ < 0;
  uintmax_t value = value_ >= 0 ? value_ : -value_;

  //count number of digits in value
  int digits_integral = 1;
  uintmax_t digits_integral_ = value;
  while(digits_integral_ /= 10) digits_integral++;

  int digits = (negate ? 1 : 0) + digits_integral;
  if(!str) return digits + 1; //only computing required length?

  length = nall::min(digits, length - 1);
  str += length; //seek to end of target string
  *str = 0; //set null terminator
  while(length && digits_integral--) {
    uint8_t x = '0' + (value % 10);
    value /= 10;
    *--str = x; //iterate backwards to write string
    length--;
  }

  if(length && negate) {
    *--str = '-';
  }

  return nall::min(initial_length, digits + 1);
}

size_t strbin(char *str, uintmax_t value, size_t length /* = 0 */) {
  if(!length) length = -1U; //"infinite" length
  size_t initial_length = length;

  //count number of digits in value
  int digits_integral = 1;
  uintmax_t digits_integral_ = value;
  while(digits_integral_ /= 2) digits_integral++;

  int digits = digits_integral;
  if(!str) return digits + 1; //only computing required length?

  length = nall::min(digits, length - 1);
  str += length; //seek to end of target string
  *str = 0; //set null terminator

  while(length--) {
    uint8_t x = '0' + (value % 2);
    value /= 2;
    *--str = x; //iterate backwards to write string
  }

  return nall::min(initial_length, digits + 1);
}

size_t strdouble(char *str, double value, size_t length /* = 0 */) {
  if(!length) length = -1U; //"infinite" length
  size_t initial_length = length;

  double fractional, integral;
  fractional = modf(value, &integral);
  uintmax_t value_integral = (uintmax_t)integral;
  uintmax_t value_fractional = 0;

  //convert fractional portion to integral number (eg 0.275 -> 275)
  //six nibbles of precision, one nibble for rounding
  for(int i = 0; i < 7; i++) {
    fractional *= 10.0;
    value_fractional = value_fractional * 10 + ((uintmax_t)fractional % 10);
  }

  //use seventh nibble to round fraction
  value_fractional = (uintmax_t)((double)value_fractional / 10.0 + 0.5);

  //cull fractional zero nibbles (eg 275000 -> 275)
  while(value_fractional && !(value_fractional % 10)) value_fractional /= 10;

  //count number of digits in integral value
  int digits_integral = 1;
  uintmax_t digits_integral_ = value_integral;
  while(digits_integral_ /= 10) digits_integral++;

  //count number of digits in fractional value
  int digits_fractional = 1;
  uintmax_t digits_fractional_ = value_fractional;
  while(digits_fractional_ /= 10) digits_fractional++;

  int digits = digits_integral + 1 + digits_fractional; //integral '.' fractional
  if(!str) return digits + 1; //only computing required length?

  length = nall::min(digits, length - 1);
  str += length; //seek to end of target string
  *str = 0; //set null terminator

  while(length && digits_fractional--) {
    uint8_t x = '0' + (value_fractional % 10);
    value_fractional /= 10;
    *--str = x; //iterate backwards to write string
    length--;
  }

  if(length) {
    *--str = '.';
    length--;
  }

  while(length-- && digits_integral--) {
    uint8_t x = '0' + (value_integral % 10);
    value_integral /= 10;
    *--str = x; //interate backwards to write string
  }

  return nall::min(initial_length, digits + 1);
}

#endif //ifdef NALL_STRING_CPP
