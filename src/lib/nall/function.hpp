#ifndef NALL_FUNCTION_HPP
#define NALL_FUNCTION_HPP

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

    function& operator=(const function &source) { memcpy(&data, &source.data, sizeof(data_t)); return *this; }
    function(const function &source) { operator=(source); }

    function() {
      data.callback = 0;
    }

    function(void *callback) {
      data.callback = callback ? &callback_global : 0;
      data.callback_global = (R (*)(P...))callback;
    }

    function(R (*callback)(P...)) {
      data.callback = &callback_global;
      data.callback_global = callback;
    }

    template<typename C>
    function(R (C::*callback)(P...), C *object) {
      static_assert(sizeof data.callback_member >= sizeof callback, "callback_member is too small");
      data.callback = &callback_member<C>;
      (R (C::*&)(P...))data.callback_member = callback;
      data.object = object;
    }

    template<typename C>
    function(R (C::*callback)(P...) const, C *object) {
      static_assert(sizeof data.callback_member >= sizeof callback, "callback_member is too small");
      data.callback = &callback_member<C>;
      (R (C::*&)(P...))data.callback_member = (R (C::*&)(P...))callback;
      data.object = object;
    }
  };

  template<typename R, typename... P>
  function<R (P...)> bind(R (*callback)(P...)) {
    return function<R (P...)>(callback);
  }

  template<typename C, typename R, typename... P>
  function<R (P...)> bind(R (C::*callback)(P...), C *object) {
    return function<R (P...)>(callback, object);
  }

  template<typename C, typename R, typename... P>
  function<R (P...)> bind(R (C::*callback)(P...) const, C *object) {
    return function<R (P...)>(callback, object);
  }
}

#endif
