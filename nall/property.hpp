#pragma once

namespace nall {

template<typename T> struct property {
  property() = default;
  property(const T& value) : value(value) {}

  operator T&() { return value; }             //direct assignment
  auto operator->() -> T* { return &value; }  //class-member access

  operator const T&() const { return value; }
  auto operator()() const -> const T& { return value; }
  auto get() const -> const T& { return value; }

  auto operator=(const T& value) -> T& { return this->value = value; }
  auto set(const T& value) -> T& { return this->value = value; }

  T value;
};

template<typename T, typename R = T> struct functional_property {
  functional_property(
    const function<R (const T&)>& getter = {},
    const function<R (T&, const T&)>& setter = {},
    const T& value = {}
  ) {
    getter ? this->getter = getter : this->getter = [](const T& self) -> R { return self; };
    setter ? this->setter = setter : this->setter = [](T& self, const T& value) -> R { return self = value; };
    this->setter(this->value, value);
  }

  operator R() const { return getter(value); }
  auto operator()() const -> R { return getter(value); }
  auto get() const -> R { return getter(value); }

  auto operator=(const T& value) -> R { return setter(this->value, value); }
  auto set(const T& value) -> R { return setter(this->value, value); }

  T value;
  function<R (const T&)> getter;
  function<R (T&, const T&)> setter;
};

template<typename T, typename R = T> struct virtual_property {
  virtual_property(
    const function<R ()>& getter = {},
    const function<R (const T&)>& setter = {},
    const T& value = {}
  ) {
    this->getter = getter;
    this->setter = setter;
    if(this->setter) this->setter(value);
  }

  operator R() const { return getter(); }
  auto operator()() const -> R { return getter(); }
  auto get() const -> R { return getter(); }

  auto operator=(const T& value) -> R { return setter(value); }
  auto set(const T& value) -> R { return setter(value); }

  function<R ()> getter;
  function<R (const T&)> setter;
};

}
