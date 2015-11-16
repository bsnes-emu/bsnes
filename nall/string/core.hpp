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

auto string::operator[](int position) const -> const char& {
  if(position > size() + 1) throw exception_out_of_bounds{};
  return data()[position];
}

template<typename... P> auto string::assign(P&&... p) -> string& {
  resize(0);
  return append(forward<P>(p)...);
}

template<typename T, typename... P> auto string::append(const T& value, P&&... p) -> string& {
  _append(make_string(value));
  return append(forward<P>(p)...);
}

template<typename... P> auto string::append(const nall::format& value, P&&... p) -> string& {
  format(value);
  return append(forward<P>(p)...);
}

auto string::append() -> string& {
  return *this;
}

template<typename T> auto string::_append(const stringify<T>& source) -> string& {
  resize(size() + source.size());
  memory::copy(get() + size() - source.size(), source.data(), source.size());
  return *this;
}

auto string::empty() const -> bool {
  return size() == 0;
}

auto string::length() const -> uint {
  return strlen(data());
}

}

#endif
