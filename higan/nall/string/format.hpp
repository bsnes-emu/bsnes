#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

template<signed precision, char padchar> string format(const string& value) {
  if(precision == 0) return value;

  bool padright = precision >= 0;
  unsigned padding = abs(precision);

  unsigned length = value.length();
  if(padding <= length) {
    if(padright) return substr(value, length - padding);
    else return substr(value, 0, padding);
  }

  string buffer;
  buffer.resize(padding);
  buffer.clear(padchar);

  memcpy(buffer() + (padright ? padding - length : 0), value, length);
  return buffer;
}

template<signed precision, char padchar> string hex(uintmax_t value) {
  string buffer;
  buffer.resize(sizeof(uintmax_t) * 2);

  unsigned size = 0;
  do {
    unsigned n = value & 15;
    buffer[size++] = n < 10 ? '0' + n : 'a' + n - 10;
    value >>= 4;
  } while(value);
  buffer[size] = 0;
  buffer.reverse();

  return format<precision, padchar>(buffer);
}

template<signed precision, char padchar> string octal(uintmax_t value) {
  string buffer;
  buffer.resize(sizeof(uintmax_t) * 3);

  unsigned size = 0;
  do {
    buffer[size++] = '0' + (value & 7);
    value >>= 3;
  } while(value);
  buffer[size] = 0;
  buffer.reverse();

  return format<precision, padchar>(buffer);
}

template<signed precision, char padchar> string binary(uintmax_t value) {
  string buffer;
  buffer.resize(sizeof(uintmax_t) * 8);

  unsigned size = 0;
  do {
    buffer[size++] = '0' + (value & 1);
    value >>= 1;
  } while(value);
  buffer[size] = 0;
  buffer.reverse();

  return format<precision, padchar>(buffer);
}

}

#endif
