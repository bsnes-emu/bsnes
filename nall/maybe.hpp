#ifndef NALL_MAYBE_HPP
#define NALL_MAYBE_HPP

namespace nall {

struct nothing_t {};
static nothing_t nothing;

template<typename T, typename = void>
class maybe {
  T* value = nullptr;

public:
  maybe() {}
  maybe(nothing_t) {}
  maybe(const T& source) { operator=(source); }
  maybe(const maybe& source) { operator=(source); }
  maybe(maybe&& source) { operator=(move(source)); }
  ~maybe() { reset(); }

  auto operator=(nothing_t) -> maybe& { reset(); return *this; }
  auto operator=(const T& source) -> maybe& { reset(); value = new T(source); return *this; }
  auto operator=(const maybe& source) -> maybe& { reset(); if(source) value = new T(source()); return *this; }
  auto operator=(maybe&& source) -> maybe& { reset(); value = source.value; source.value = nullptr; return *this; }

  auto operator==(const maybe& source) const -> bool {
    if(value && source.value) return *value == *source.value;
    return !value && !source.value;
  }
  auto operator!=(const maybe& source) const -> bool { return !operator==(source); }

  explicit operator bool() const { return value; }
  auto operator->() -> T* { assert(value); return value; }
  auto operator->() const -> T* { assert(value); return value; }
  auto operator*() -> T& { assert(value); return *value; }
  auto operator*() const -> T& { assert(value); return *value; }
  auto operator()() -> T& { assert(value); return *value; }
  auto operator()() const -> T& { assert(value); return *value; }
  auto operator()(const T& invalid) const -> T& { if(value) return *value; return invalid; }

  auto empty() const -> bool { return value == nullptr; }
  auto reset() -> void { if(value) { delete value; value = nullptr; } }
  auto swap(maybe& source) -> void { std::swap(value, source.value); }
};

template<typename T>
class maybe<T&> {
  T* value = nullptr;

public:
  maybe() {}
  maybe(nothing_t) {}
  maybe(const T& source) { operator=(source); }
  maybe(const maybe& source) { operator=(source); }
  maybe(maybe&& source) { operator=(move(source)); }

  auto operator=(nothing_t) -> maybe& { value = nullptr; return *this; }
  auto operator=(const T& source) -> maybe& { value = (T*)&source; return *this; }
  auto operator=(const maybe& source) -> maybe& { value = source.value; return *this; }
  auto operator=(maybe&& source) -> maybe& { value = source.value; source.value = nullptr; return *this; }

  auto operator==(const maybe& source) const -> bool {
    if(value && source.value) return *value == *source.value;
    return !value && !source.value;
  }
  auto operator!=(const maybe& source) const -> bool { return !operator==(source); }

  explicit operator bool() const { return value; }
  auto operator->() -> T* { assert(value); return value; }
  auto operator->() const -> T* { assert(value); return *value; }
  auto operator*() -> T& { assert(value); return *value; }
  auto operator*() const -> T& { assert(value); return *value; }
  auto operator()() -> T& { assert(value); return *value; }
  auto operator()() const -> T& { assert(value); return *value; }
  auto operator()(const T& invalid) const -> T& { if(value) return *value; return invalid; }

  auto empty() const -> bool { return value == nullptr; }
  auto reset() -> void { value = nullptr; }
  auto swap(maybe& source) -> void { std::swap(value, source.value); }
};

template<typename T>
class maybe<T, enable_if<is_integral<T>>> {
  T value = 0;
  bool valid = false;

public:
  maybe() {}
  maybe(nothing_t) {}
  maybe(const T& source) { operator=(source); }
  maybe(const maybe& source) { operator=(source); }
  maybe(maybe&& source) { operator=(move(source)); }

  auto operator=(nothing_t) -> maybe& { valid = false; return *this; }
  auto operator=(const T& source) -> maybe& { valid = true; value = source; return *this; }
  auto operator=(const maybe& source) -> maybe& { valid = source.valid; value = source.value; return *this; }
  auto operator=(maybe&& source) -> maybe& { valid = source.valid; value = source.value; source.valid = false; return *this; }

  auto operator==(const maybe& source) const -> bool {
    if(valid && source.valid) return value == source.value;
    return !valid && !source.valid;
  }
  auto operator!=(const maybe& source) const -> bool { return !operator==(source); }

  explicit operator bool() const { return valid; }
  auto operator*() const -> T { assert(valid); return value; }
  auto operator()() const -> T { assert(valid); return value; }
  auto operator()(const T& invalid) const -> T { if(valid) return value; return invalid; }

  auto empty() const -> bool { return !valid; }
  auto reset() -> void { valid = false; }
  auto swap(maybe& source) -> void { std::swap(valid, source.valid); std::swap(value, source.value); }
};

}

#endif
