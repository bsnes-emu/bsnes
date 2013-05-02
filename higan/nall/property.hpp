#ifndef NALL_PROPERTY_HPP
#define NALL_PROPERTY_HPP

namespace nall {

template<typename C> struct property {
  template<typename T> struct readonly {
    const T* operator->() const { return &value; }
    const T& operator()() const { return value; }
    operator const T&() const { return value; }
  private:
    T* operator->() { return &value; }
    operator T&() { return value; }
    const T& operator=(const T& value_) { return value = value_; }
    T value;
    friend C;
  };

  template<typename T> struct writeonly {
    void operator=(const T& value_) { value = value_; }
  private:
    const T* operator->() const { return &value; }
    const T& operator()() const { return value; }
    operator const T&() const { return value; }
    T* operator->() { return &value; }
    operator T&() { return value; }
    T value;
    friend C;
  };

  template<typename T> struct readwrite {
    const T* operator->() const { return &value; }
    const T& operator()() const { return value; }
    operator const T&() const { return value; }
    T* operator->() { return &value; }
    operator T&() { return value; }
    const T& operator=(const T& value_) { return value = value_; }
    T value;
  };
};

}

#endif
