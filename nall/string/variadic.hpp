#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

void sprint(string& output) {
}

template<typename T, typename... Args>
void sprint(string& output, const T& value, Args&&... args) {
  output._append(make_string(value));
  sprint(output, std::forward<Args>(args)...);
}

template<typename... Args> void print(Args&&... args) {
  printf("%s", (const char*)string(std::forward<Args>(args)...));
}

}

#endif
