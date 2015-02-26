#ifndef NALL_ANY_HPP
#define NALL_ANY_HPP

#include <typeinfo>
#include <nall/traits.hpp>

namespace nall {

struct any {
  bool empty() const { return container; }
  void reset() { if(container) { delete container; container = nullptr; } }

  const std::type_info& type() const {
    return container ? container->type() : typeid(void);
  }

  template<typename T> any& operator=(const T& value) {
    using auto_t = type_if<is_array<T>, typename std::remove_extent<typename std::add_const<T>::type>::type*, T>;

    if(type() == typeid(auto_t)) {
      static_cast<holder<auto_t>*>(container)->value = (auto_t)value;
    } else {
      if(container) delete container;
      container = new holder<auto_t>((auto_t)value);
    }

    return *this;
  }

  any& operator=(const any& source) {
    if(container) { delete container; container = nullptr; }
    if(source.container) container = source.container->copy();
    return *this;
  }

  any& operator=(any&& source) {
    if(container) delete container;
    container = source.container;
    source.container = nullptr;
    return *this;
  }

  any() = default;
  any(const any& source) { operator=(source); }
  any(any&& source) { operator=(move(source)); }
  template<typename T> any(const T& value) { operator=(value); }
  ~any() { reset(); }

private:
  struct placeholder {
    virtual const std::type_info& type() const = 0;
    virtual placeholder* copy() const = 0;
    virtual ~placeholder() {}
  };
  placeholder* container = nullptr;

  template<typename T> struct holder : placeholder {
    T value;
    const std::type_info& type() const { return typeid(T); }
    placeholder* copy() const { return new holder(value); }
    holder(const T& value) : value(value) {}
  };

  template<typename T> friend T any_cast(any&);
  template<typename T> friend T any_cast(const any&);
  template<typename T> friend T* any_cast(any*);
  template<typename T> friend const T* any_cast(const any*);
};

template<typename T> T any_cast(any& value) {
  typedef typename std::remove_reference<T>::type nonref;
  if(value.type() != typeid(nonref)) throw;
  return static_cast<any::holder<nonref>*>(value.container)->value;
}

template<typename T> T any_cast(const any& value) {
  typedef const typename std::remove_reference<T>::type nonref;
  if(value.type() != typeid(nonref)) throw;
  return static_cast<any::holder<nonref>*>(value.container)->value;
}

template<typename T> T* any_cast(any* value) {
  if(!value || value->type() != typeid(T)) return nullptr;
  return &static_cast<any::holder<T>*>(value->container)->value;
}

template<typename T> const T* any_cast(const any* value) {
  if(!value || value->type() != typeid(T)) return nullptr;
  return &static_cast<any::holder<T>*>(value->container)->value;
}

}

#endif
