#ifndef NALL_FUNCTION_HPP
#define NALL_FUNCTION_HPP

#include <stdlib.h>
#include <functional>
#include <type_traits>

namespace nall {
  template<typename T> class function;

  template<typename R, typename... P>
  class function<R (P...)> {
  private:
    struct base1 { virtual void func1(P...) {} };
    struct base2 { virtual void func2(P...) {} };
    struct derived : base1, virtual base2 {};

    struct data_t {
      R (*callback)(const data_t&, P...);
      R (*callback_global)(P...);
      struct {
        R (derived::*callback_member)(P...);
        void *callback_object;
      };
      std::function<R (P...)> callback_lambda;
    } data;

    static R callback_global(const data_t &data, P... p) {
      return data.callback_global(std::forward<P>(p)...);
    }

    template<typename C>
    static R callback_member(const data_t &data, P... p) {
      return (((C*)data.callback_object)->*((R (C::*&)(P...))data.callback_member))(std::forward<P>(p)...);
    }

    static R callback_lambda(const data_t &data, P... p) {
      return data.callback_lambda(std::forward<P>(p)...);
    }

  public:
    R operator()(P... p) const { return data.callback(data, std::forward<P>(p)...); }
    operator bool() const { return data.callback; }
    void reset() { data.callback = 0; }

    function& operator=(const function &source) {
      data.callback = source.data.callback;
      data.callback_global = source.data.callback_global;
      data.callback_member = source.data.callback_member;
      data.callback_object = source.data.callback_object;
      data.callback_lambda = source.data.callback_lambda;
      return *this;
    }

    function(const function &source) {
      operator=(source);
    }

    //no pointer
    function() {
      data.callback = 0;
    }

    //symbolic link pointer (nall/dl.hpp::sym, etc)
    function(void *callback) {
      data.callback = callback ? &callback_global : 0;
      data.callback_global = (R (*)(P...))callback;
    }

    //global function pointer
    function(R (*callback)(P...)) {
      data.callback = &callback_global;
      data.callback_global = callback;
    }

    //member function pointer
    template<typename C>
    function(R (C::*callback)(P...), C *object) {
      static_assert(sizeof data.callback_member >= sizeof callback, "callback_member is too small");
      data.callback = &callback_member<C>;
      (R (C::*&)(P...))data.callback_member = callback;
      data.callback_object = object;
    }

    //const member function pointer
    template<typename C>
    function(R (C::*callback)(P...) const, C *object) {
      static_assert(sizeof data.callback_member >= sizeof callback, "callback_member is too small");
      data.callback = &callback_member<C>;
      (R (C::*&)(P...))data.callback_member = (R (C::*&)(P...))callback;
      data.callback_object = object;
    }

    //lambda function pointer
    template<typename T>
    function(const T &callback) {
      static_assert(std::is_same<R, typename std::result_of<T(P...)>::type>::value, "lambda mismatch");
      data.callback = &callback_lambda;
      data.callback_lambda = callback;
    }
  };
}

#endif
