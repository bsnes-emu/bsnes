#ifdef NALL_STRING_INTERNAL_HPP

//only allocators may access _data or modify _size and _capacity
//all other functions must use data(), size(), capacity()

#if defined(NALL_STRING_ALLOCATOR_ADAPTIVE)
  #include <nall/string/allocator/adaptive.hpp>
#elif defined(NALL_STRING_ALLOCATOR_COPY_ON_WRITE)
  #include <nall/string/allocator/copy-on-write.hpp>
#elif defined(NALL_STRING_ALLOCATOR_SMALL_STRING_OPTIMIZATION)
  #include <nall/string/allocator/small-string-optimization.hpp>
#elif defined(NALL_STRING_ALLOCATOR_VECTOR)
  #include <nall/string/allocator/vector.hpp>
#endif

namespace nall {

auto string::operator[](signed position) const -> const char& {
  if(position > size() + 1) throw exception_out_of_bounds{};
  return data()[position];
}

template<typename... P> auto assign(string& self, P&&... p) -> string& {
  self.resize(0);
  return self.append(std::forward<P>(p)...);
}

template<typename T, typename... P> auto append(string& self, const T& value, P&&... p) -> string& {
  _append(self, make_string(value));
  return self.append(std::forward<P>(p)...);
}

template<typename... P> auto append(string& self, const format& value, P&&... p) -> string& {
  self.format(value);
  return self.append(std::forward<P>(p)...);
}

auto append(string& self) -> string& {
  return self;
}

template<typename T> auto _append(string& self, const stringify<T>& source) -> string& {
  unsigned size = self.size();
  unsigned length = source.size();
  self.resize(size + length);
  memory::copy(self.pointer() + size, source.data(), length);
  return self;
}

auto empty(const string& self) -> bool {
  return self.size() == 0;
}

auto length(const string& self) -> unsigned {
  return strlen(self.data());
}

}

#endif
