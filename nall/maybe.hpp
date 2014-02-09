#ifndef NALL_MAYBE_HPP
#define NALL_MAYBE_HPP

namespace nall {

struct nothing_t {};
static nothing_t nothing;

template<typename T>
class maybe {
  T* value = nullptr;

public:
  maybe() {}
  maybe(nothing_t) {}
  maybe(const T& source) { operator=(source); }
  maybe(const maybe& source) { operator=(source); }
  maybe(maybe&& source) { operator=(std::move(source)); }
  ~maybe() { reset(); }

  maybe& operator=(nothing_t) { reset(); return *this; }
  maybe& operator=(const T& source) { reset(); value = new T(source); return *this; }
  maybe& operator=(const maybe& source) { reset(); if(source) value = new T(source()); return *this; }
  maybe& operator=(maybe&& source) { reset(); value = source.value; source.value = nullptr; return *this; }

  bool operator==(const maybe& source) const {
    if(value && source.value) return *value == *source.value;
    return !value && !source.value;
  }
  bool operator!=(const maybe& source) const { return !operator==(source); }

  explicit operator bool() const { return value; }
  T& operator()() { assert(value); return *value; }
  const T& operator()() const { assert(value); return *value; }
  const T& operator()(const T& invalid) const { if(value) return *value; return invalid; }

  bool empty() const { return value == nullptr; }
  void reset() { if(value) { delete value; value = nullptr; } }
  void swap(maybe& source) { std::swap(value, source.value); }
};

template<typename T>
class maybe<T&> {
  T* value = nullptr;

public:
  maybe() {}
  maybe(nothing_t) {}
  maybe(const T& source) { operator=(source); }
  maybe(const maybe& source) { operator=(source); }
  maybe(maybe&& source) { operator=(std::move(source)); }

  maybe& operator=(nothing_t) { value = nullptr; return *this; }
  maybe& operator=(const T& source) { value = (T*)&source; return *this; }
  maybe& operator=(const maybe& source) { value = source.value; return *this; }
  maybe& operator=(maybe&& source) { value = source.value; source.value = nullptr; return *this; }

  bool operator==(const maybe& source) const {
    if(value && source.value) return *value == *source.value;
    return !value && !source.value;
  }
  bool operator!=(const maybe& source) const { return !operator==(source); }

  explicit operator bool() const { return value; }
  T& operator()() { assert(value); return *value; }
  const T& operator()() const { assert(value); return *value; }
  const T& operator()(const T& invalid) const { if(value) return *value; return invalid; }

  bool empty() const { return value == nullptr; }
  void reset() { value = nullptr; }
  void swap(maybe& source) { std::swap(value, source.value); }
};

}

#endif
