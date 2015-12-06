#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

auto string::read(rstring filename) -> string {
  #if !defined(_WIN32)
  FILE* fp = fopen(filename, "rb");
  #else
  FILE* fp = _wfopen(utf16_t(filename), L"rb");
  #endif

  string result;
  if(!fp) return result;

  fseek(fp, 0, SEEK_END);
  int filesize = ftell(fp);
  if(filesize < 0) return fclose(fp), result;

  rewind(fp);
  result.resize(filesize);
  auto unused = fread(result.get(), 1, filesize, fp);
  return fclose(fp), result;
}

auto string::repeat(rstring pattern, uint times) -> string {
  string result;
  while(times--) result.append(pattern.data());
  return result;
}

auto string::fill(char fill) -> string& {
  memory::fill(get(), size(), fill);
  return *this;
}

auto string::hash() const -> unsigned {
  const char* p = data();
  uint length = size();
  uint result = 5381;
  while(length--) result = (result << 5) + result + *p++;
  return result;
}

auto string::remove(uint offset, uint length) -> string& {
  char* p = get();
  length = min(length, size());
  memory::move(p + offset, p + offset + length, size() - length);
  return resize(size() - length);
}

auto string::reverse() -> string& {
  char* p = get();
  uint length = size();
  uint pivot = length >> 1;
  for(int x = 0, y = length - 1; x < pivot && y >= 0; x++, y--) std::swap(p[x], p[y]);
  return *this;
}

//+length => insert/delete from start (right justify)
//-length => insert/delete from end (left justify)
auto string::size(int length, char fill) -> string& {
  uint size = this->size();
  if(size == length) return *this;

  bool right = length >= 0;
  length = abs(length);

  if(size < length) {  //expand
    resize(length);
    char* p = get();
    uint displacement = length - size;
    if(right) memory::move(p + displacement, p, size);
    else p += size;
    while(displacement--) *p++ = fill;
  } else {  //shrink
    char* p = get();
    uint displacement = size - length;
    if(right) memory::move(p, p + displacement, length);
    resize(length);
  }

  return *this;
}

auto slice(rstring self, int offset, int length) -> string {
  string result;
  if(offset < self.size()) {
    if(length < 0) length = self.size() - offset;
    result.resize(length);
    memory::copy(result.get(), self.data() + offset, length);
  }
  return result;
}

auto integer(char* result, intmax value) -> char* {
  bool negative = value < 0;
  if(negative) value = -value;

  char buffer[64];
  uint size = 0;

  do {
    uint n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);
  if(negative) buffer[size++] = '-';

  for(int x = size - 1, y = 0; x >= 0 && y < size; x--, y++) result[x] = buffer[y];
  result[size] = 0;
  return result;
}

auto natural(char* result, uintmax value) -> char* {
  char buffer[64];
  uint size = 0;

  do {
    uint n = value % 10;
    buffer[size++] = '0' + n;
    value /= 10;
  } while(value);

  for(int x = size - 1, y = 0; x >= 0 && y < size; x--, y++) result[x] = buffer[y];
  result[size] = 0;
  return result;
}

//using sprintf is certainly not the most ideal method to convert
//a double to a string ... but attempting to parse a double by
//hand, digit-by-digit, results in subtle rounding errors.
auto real(char* result, long double value) -> uint {
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

  uint length = strlen(buffer);
  if(result) strcpy(result, buffer);
  return length + 1;
}

}

#endif
