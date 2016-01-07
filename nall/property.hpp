#pragma once

namespace nall {

template<typename C> struct property {
  template<typename T> struct readonly {
    auto operator->() const -> const T* { return &value; }
    auto operator()() const -> const T& { return value; }
    operator const T&() const { return value; }
  private:
    auto operator->() -> T* { return &value; }
    operator T&() { return value; }
    auto operator=(const T& value_) -> const T& { return value = value_; }
    T value;
    friend C;
  };

  template<typename T> struct writeonly {
    auto operator=(const T& value_) -> void { value = value_; }
  private:
    auto operator->() const -> const T* { return &value; }
    auto operator()() const -> const T& { return value; }
    operator const T&() const { return value; }
    auto operator->() -> T* { return &value; }
    operator T&() { return value; }
    T value;
    friend C;
  };

  template<typename T> struct readwrite {
    auto operator->() const -> const T* { return &value; }
    auto operator()() const -> const T& { return value; }
    operator const T&() const { return value; }
    auto operator->() -> T* { return &value; }
    operator T&() { return value; }
    auto operator=(const T& value_) -> const T& { return value = value_; }
    T value;
  };
};

}
