#ifndef NALL_METHOD_HPP
#define NALL_METHOD_HPP

//provides extension-method and chaining-method support to classes
//extension: class(function, params...);
//chaining:  class[function](params...)[function](params...);

//usage:
//struct object : method<object> {
//  using method::operator[];  //if object::operator[] defined
//  using method::operator();  //if object::operator() defined
//};

//note: extension-methods would be obsolete if C++17 introduces unified function call syntax
//currently proposed as N4165 and N4174

namespace nall {

template<typename T> struct method {
  template<typename F> struct chain {
    chain(T& self, const F& f) : self(self), f(f) {}
    template<typename... P> auto operator()(P&&... p) -> T& {
      return f(self, std::forward<P>(p)...), self;
    }
  private:
    T& self;
    const F& f;
  };

  template<typename F> struct const_chain {
    const_chain(const T& self, const F& f) : self(self), f(f) {}
    template<typename... P> auto operator()(P&&... p) const -> const T& {
      return f(self, std::forward<P>(p)...), self;
    }
  private:
    const T& self;
    const F& f;
  };

  template<typename F, typename = enable_if<is_function<F>>>
  auto operator[](const F& f) -> chain<F> { return chain<F>((T&)*this, f); }

  template<typename F, typename = enable_if<is_function<F>>>
  auto operator[](const F& f) const -> const_chain<F> { return const_chain<F>((const T&)*this, f); }

  template<typename F, typename... P, typename = enable_if<is_function<F>>>
  auto operator()(const F& f, P&&... p) -> decltype(f((T&)*this, std::forward<P>(p)...)) {
    return f((T&)*this, std::forward<P>(p)...);
  }

  template<typename F, typename... P, typename = enable_if<is_function<F>>>
  auto operator()(const F& f, P&&... p) const -> decltype(f((const T&)*this, std::forward<P>(p)...)) {
    return f((const T&)*this, std::forward<P>(p)...);
  }
};

}

#endif
