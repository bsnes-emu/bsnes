#ifndef NALL_STRING_VARIADIC_HPP
#define NALL_STRING_VARIADIC_HPP

namespace nall {

static void isprint(string &output) {
}

template<typename T, typename... Args>
static void isprint(string &output, T value, Args... args) {
  output << to_string<T>(value);
  isprint(output, args...);
}

template<typename... Args> inline string sprint(Args... args) {
  string output;
  isprint(output, args...);
  return output;
}

template<typename... Args> inline void print(Args... args) {
  printf("%s", (const char*)sprint(args...));
}

}

#endif
