#ifndef NALL_STRING_VARIADIC_HPP
#define NALL_STRING_VARIADIC_HPP

namespace nall {
  static void sprint(string &output, unsigned &offset, const char *&s) {
    while(*s) output[offset++] = *s++;
  }

  template<typename T, typename... Args>
  static void sprint(string &output, unsigned &offset, const char *&s, T value, Args... args) {
    while(*s) {
      if(*s == '$') {
        string data = to_string<T>(value);
        unsigned i = 0;
        while(data[i]) output[offset++] = data[i++];
        sprint(output, offset, ++s, args...);
        return;
      } else {
        output[offset++] = *s++;
      }
    }
  }

  template<typename... Args> inline string sprint(const char *s, Args... args) {
    string output;
    unsigned offset = 0;
    sprint(output, offset, s, args...);
    output[offset] = 0;
    return output;
  }

  template<typename... Args> inline void print(const char *s, Args... args) {
    printf("%s", (const char*)sprint(s, args...));
  }
}

#endif
