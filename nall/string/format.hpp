#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

//nall::format is a vector<string> of parameters that can be applied to a string
//each {#} token will be replaced with its appropriate format parameter

auto string::format(const nall::format& params) -> type& {
  auto size = (int)this->size();
  auto data = (char*)memory::allocate(size);
  memory::copy(data, this->data(), size);

  signed x = 0;
  while(x < size - 2) {  //2 = minimum tag length
    if(data[x] != '{') { x++; continue; }

    signed y = x + 1;
    while(y < size - 1) {  //-1 avoids going out of bounds on test after this loop
      if(data[y] != '}') { y++; continue; }
      break;
    }

    if(data[y++] != '}') { x++; continue; }

    static auto isNumeric = [](char* s, char* e) -> bool {
      if(s == e) return false;  //ignore empty tags: {}
      while(s < e) {
        if(*s >= '0' && *s <= '9') { s++; continue; }
        return false;
      }
      return true;
    };
    if(!isNumeric(&data[x + 1], &data[y - 1])) { x++; continue; }

    uint index = nall::natural(&data[x + 1]);
    if(index >= params.size()) { x++; continue; }

    uint sourceSize = y - x;
    uint targetSize = params[index].size();
    uint remaining = size - x;

    if(sourceSize > targetSize) {
      uint difference = sourceSize - targetSize;
      memory::move(&data[x], &data[x + difference], remaining);
      size -= difference;
    } else if(targetSize > sourceSize) {
      uint difference = targetSize - sourceSize;
      data = (char*)realloc(data, size + difference);
      size += difference;
      memory::move(&data[x + difference], &data[x], remaining);
    }
    memory::copy(&data[x], params[index].data(), targetSize);
    x += targetSize;
  }

  resize(size);
  memory::copy(get(), data, size);
  memory::free(data);
  return *this;
}

template<typename T, typename... P> auto format::append(const T& value, P&&... p) -> format& {
  vector<string>::append(value);
  return append(forward<P>(p)...);
}

auto format::append() -> format& {
  return *this;
}

template<typename... P> auto print(P&&... p) -> void {
  string s{forward<P>(p)...};
  fputs(s.data(), stdout);
}

auto integer(intmax value, long precision, char padchar) -> string {
  string buffer;
  buffer.resize(1 + sizeof(intmax) * 3);
  char* p = buffer.get();

  bool negative = value < 0;
  value = abs(value);
  uint size = 0;
  do {
    p[size++] = '0' + (value % 10);
    value /= 10;
  } while(value);
  if(negative) p[size++] = '-';
  buffer.resize(size);
  buffer.reverse();
  if(precision) buffer.size(precision, padchar);
  return buffer;
}

auto natural(uintmax value, long precision, char padchar) -> string {
  string buffer;
  buffer.resize(sizeof(uintmax) * 3);
  char* p = buffer.get();

  uint size = 0;
  do {
    p[size++] = '0' + (value % 10);
    value /= 10;
  } while(value);
  buffer.resize(size);
  buffer.reverse();
  if(precision) buffer.size(precision, padchar);
  return buffer;
}

auto hex(uintmax value, long precision, char padchar) -> string {
  string buffer;
  buffer.resize(sizeof(uintmax) * 2);
  char* p = buffer.get();

  uint size = 0;
  do {
    uint n = value & 15;
    p[size++] = n < 10 ? '0' + n : 'a' + n - 10;
    value >>= 4;
  } while(value);
  buffer.resize(size);
  buffer.reverse();
  if(precision) buffer.size(precision, padchar);
  return buffer;
}

auto octal(uintmax value, long precision, char padchar) -> string {
  string buffer;
  buffer.resize(sizeof(uintmax) * 3);
  char* p = buffer.get();

  uint size = 0;
  do {
    p[size++] = '0' + (value & 7);
    value >>= 3;
  } while(value);
  buffer.resize(size);
  buffer.reverse();
  if(precision) buffer.size(precision, padchar);
  return buffer;
}

auto binary(uintmax value, long precision, char padchar) -> string {
  string buffer;
  buffer.resize(sizeof(uintmax) * 8);
  char* p = buffer.get();

  uint size = 0;
  do {
    p[size++] = '0' + (value & 1);
    value >>= 1;
  } while(value);
  buffer.resize(size);
  buffer.reverse();
  if(precision) buffer.size(precision, padchar);
  return buffer;
}

template<typename T> auto pointer(const T* value, long precision) -> string {
  if(value == nullptr) return "(nullptr)";
  return {"0x", hex((uintptr)value, precision)};
}

auto pointer(uintptr value, long precision) -> string {
  if(value == 0) return "(nullptr)";
  return {"0x", hex(value, precision)};
}

auto real(long double value) -> string {
  string temp;
  temp.resize(real(nullptr, value));
  real(temp.get(), value);
  return temp;
}

}

#endif
