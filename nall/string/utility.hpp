#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

auto string::read(const string& filename) -> string {
  #if !defined(_WIN32)
  FILE* fp = fopen(filename, "rb");
  #else
  FILE* fp = _wfopen(utf16_t(filename), L"rb");
  #endif

  string result;
  if(!fp) return result;

  fseek(fp, 0, SEEK_END);
  signed filesize = ftell(fp);
  if(filesize < 0) return fclose(fp), result;

  rewind(fp);
  result.resize(filesize);
  fread(result.pointer(), 1, filesize, fp);
  return fclose(fp), result;
}

auto string::repeat(const string& pattern, unsigned times) -> string {
  string result;
  while(times--) result.append(pattern);
  return result;
}

auto fill(string& self, char fill) -> string& {
  memory::fill(self.pointer(), self.size(), fill);
  return self;
}

auto hash(const string& self) -> unsigned {
  const char* p = self.data();
  unsigned size = self.size();
  unsigned result = 5381;
  while(size--) result = (result << 5) + result + *p++;
  return result;
}

auto remove(string& self, unsigned offset, unsigned length) -> string& {
  char* p = self.pointer();
  length = min(length, self.size());
  memory::move(p + offset, p + offset + length, self.size() - length);
  return self.resize(self.size() - length);
}

auto reverse(string& self) -> string& {
  char* p = self.pointer();
  unsigned size = self.size();
  unsigned pivot = size >> 1;
  for(signed x = 0, y = size - 1; x < pivot && y >= 0; x++, y--) std::swap(p[x], p[y]);
  return self;
}

//+length => insert/delete from start (right justify)
//-length => insert/delete from end (left justify)
auto size(string& self, signed length, char fill) -> string& {
  unsigned size = self.size();
  if(size == length) return self;

  bool right = length >= 0;
  length = abs(length);

  if(size < length) {  //expand
    self.resize(length);
    char* p = self.pointer();
    unsigned displacement = length - size;
    if(right) memory::move(p + displacement, p, size);
    else p += size;
    while(displacement--) *p++ = fill;
  } else {  //shrink
    char* p = self.pointer();
    unsigned displacement = size - length;
    if(right) memory::move(p, p + displacement, length);
    self.resize(length);
  }

  return self;
}

auto slice(const string& self, signed offset, signed length) -> string {
  string result;
  if(offset < self.size()) {
    if(length < 0) length = self.size() - offset;
    result.resize(length);
    memory::copy(result.pointer(), self.data() + offset, length);
  }
  return result;
}

//legacy function: required for some library functions, do not use in newly written code
auto substr(rstring source, signed offset, signed length) -> string {
  string result;
  if(length < 0) length = source.size() - offset;
  result.resize(length);
  memory::copy(result.pointer(), source.data() + offset, length);
  return result;
}

auto integer(char* result, intmax_t value) -> char* {
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

  for(signed x = size - 1, y = 0; x >= 0 && y < size; x--, y++) result[x] = buffer[y];
  result[size] = 0;
  return result;
}

auto decimal(char* result, uintmax_t value) -> char* {
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
auto real(char* result, long double value) -> unsigned {
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
  if(result) strcpy(result, buffer);
  return length + 1;
}

}

#endif
