#ifndef NALL_UTILITY_HPP
#define NALL_UTILITY_HPP

#include <type_traits>
#include <utility>
#include <nall/any.hpp>

namespace nall {

template<typename T> struct base_from_member {
  T value;
  base_from_member(T value) : value(value) {}
};

template<typename T> struct ref {
  T& operator*() {
    if(type == Type::Reference) return *any_cast<T*>(value);
    return any_cast<T&>(value);
  }

  operator T&() { return operator*(); }

  ref(T& value) : type(Type::Reference), value(&value) {}
  ref(T&& value) : type(Type::Temporary), value(value) {}

protected:
  enum class Type : unsigned { Reference, Temporary } type;
  any value;
};

template<typename TT> struct optional {
  typedef typename std::remove_reference<TT>::type T;
  static const bool isConst = std::is_const<TT>::value;
  static const bool isReference = std::is_reference<TT>::value;
  struct optional_value_not_valid{};

  bool valid = false;
  T* value = nullptr;

  operator bool() const { return valid; }

  void reset() {
    valid = false;
    if(value) {
      if(!isReference) delete value;
      value = nullptr;
    }
  }

  template<typename = typename std::enable_if<!isConst>::type>
  T& operator*() {
    if(!valid) throw optional_value_not_valid{};
    return *value;
  }

  template<typename = typename std::enable_if<!isConst>::type>
  T& operator()() {
    if(!valid) throw optional_value_not_valid{};
    return *value;
  }

  const T& operator*() const {
    if(!valid) throw optional_value_not_valid{};
    return *value;
  }

  const T& operator()() const {
    if(!valid) throw optional_value_not_valid{};
    return *value;
  }

  const T& operator()(const T& alternate) const {
    if(!valid) return alternate;
    return *value;
  }

  const bool operator==(const optional& source) const {
    if(valid && source.valid) return *value == *source.value;
    if(!valid && !source.valid) return true;
    return false;
  }

  const bool operator!=(const optional& source) const {
    return !operator==(source);
  }

  optional& operator=(const T& source) {
    reset();
    valid = true;
    if(isReference) value = (T*)&source;
    else value = new T(source);
    return *this;
  }

  optional& operator=(T&& source) {
    reset();
    valid = true;
    if(isReference) value = &source;
    else value = new T(std::move(source));
    return *this;
  }

  optional& operator=(const optional& source) {
    reset();
    valid = source.valid;
    if(valid) operator=(source);
    return *this;
  }

  optional& operator=(optional&& source) {
    reset();
    valid = source.valid;
    value = source.value;
    source.valid = false;
    source.value = nullptr;
    return *this;
  }

  optional() = default;
  optional(bool valid) : valid(valid) {}
  optional(const T& value) { operator=(value); }
  optional(T&& value) { operator=(std::move(value)); }
  optional(bool valid, const T& value) : valid(valid) { if(valid) operator=(value); }
  optional(bool valid, T&& value) : valid(valid) { if(valid) operator=(std::move(value)); }
  optional(const optional& source) { operator=(source); }
  optional(optional&& source) { operator=(std::move(source)); }
  ~optional() { reset(); }
};

template<typename T> inline T* allocate(unsigned size, const T& value) {
  T* array = new T[size];
  for(unsigned i = 0; i < size; i++) array[i] = value;
  return array;
}

}

#endif
