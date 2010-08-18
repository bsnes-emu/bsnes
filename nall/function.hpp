#ifndef NALL_FUNCTION_HPP
#define NALL_FUNCTION_HPP

#include <malloc.h>
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
      union {
        R (*callback_global)(P...);
        struct {
          R (derived::*callback_member)(P...);
          void *object;
        };
      };
    } data;

    static R callback_global(const data_t &data, P... p) {
      return data.callback_global(p...);
    }

    template<typename C>
    static R callback_member(const data_t &data, P... p) {
      return (((C*)data.object)->*((R (C::*&)(P...))data.callback_member))(p...);
    }

  public:
    R operator()(P... p) const { return data.callback(data, p...); }
    operator bool() const { return data.callback; }
    void reset() { data.callback = 0; }

    function& operator=(const function &source) { memcpy(&data, &source.data, sizeof(data_t)); return *this; }
    function(const function &source) { operator=(source); }

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
      data.object = object;
    }

    //const member function pointer
    template<typename C>
    function(R (C::*callback)(P...) const, C *object) {
      static_assert(sizeof data.callback_member >= sizeof callback, "callback_member is too small");
      data.callback = &callback_member<C>;
      (R (C::*&)(P...))data.callback_member = (R (C::*&)(P...))callback;
      data.object = object;
    }

    //lambda function pointer
    template<typename T>
    function(T callback) {
      static_assert(std::is_same<R, typename std::result_of<T(P...)>::type>::value, "lambda mismatch");
      data.callback = &callback_global;
      data.callback_global = (R (*)(P...))callback;
    }
  };
}

#endif
