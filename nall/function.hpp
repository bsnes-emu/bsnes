#ifndef NALL_FUNCTION_HPP
#define NALL_FUNCTION_HPP

#include <nall/traits.hpp>

namespace nall {

template<typename T> class function;

template<typename R, typename... P> class function<R (P...)> {
  struct container {
    virtual R operator()(P... p) const = 0;
    virtual container* copy() const = 0;
    virtual ~container() {}
  };

  container* callback = nullptr;

  struct global : container {
    R (*function)(P...);
    R operator()(P... p) const { return function(forward<P>(p)...); }
    container* copy() const { return new global(function); }
    global(R (*function)(P...)) : function(function) {}
  };

  template<typename C> struct member : container {
    R (C::*function)(P...);
    C* object;
    R operator()(P... p) const { return (object->*function)(forward<P>(p)...); }
    container* copy() const { return new member(function, object); }
    member(R (C::*function)(P...), C* object) : function(function), object(object) {}
  };

  template<typename L> struct lambda : container {
    mutable L object;
    R operator()(P... p) const { return object(forward<P>(p)...); }
    container* copy() const { return new lambda(object); }
    lambda(const L& object) : object(object) {}
  };

  //value = true if R L::operator()(P...) exists
  template<typename L> struct is_compatible {
    template<typename T> static const typename is_same<R, decltype(declval<T>().operator()(declval<P>()...))>::type exists(T*);
    template<typename T> static const false_type exists(...);
    static constexpr bool value = decltype(exists<L>(0))::value;
  };

public:
  explicit operator bool() const { return callback; }
  R operator()(P... p) const { return (*callback)(forward<P>(p)...); }
  void reset() { if(callback) { delete callback; callback = nullptr; } }

  function& operator=(const function& source) {
    if(this != &source) {
      if(callback) { delete callback; callback = nullptr; }
      if(source.callback) callback = source.callback->copy();
    }
    return *this;
  }

  function() = default;
  function(const function& source) { operator=(source); }
  function(void* function) { if(function) callback = new global((R (*)(P...))function); }
  function(R (*function)(P...)) { callback = new global(function); }
  template<typename C> function(R (C::*function)(P...), C* object) { callback = new member<C>(function, object); }
  template<typename C> function(R (C::*function)(P...) const, C* object) { callback = new member<C>((R (C::*)(P...))function, object); }
  template<typename L, typename = enable_if<is_compatible<L>>> function(const L& object) { callback = new lambda<L>(object); }
  ~function() { if(callback) delete callback; }
};

}

#endif
