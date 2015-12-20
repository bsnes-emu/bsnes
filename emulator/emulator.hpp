#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <nall/nall.hpp>
#include <nall/dsp.hpp>
using namespace nall;

namespace Emulator {
  static const string Name = "higan";
  static const string Version = "095.17";
  static const string Author = "byuu";
  static const string License = "GPLv3";
  static const string Website = "http://byuu.org/";

  #if defined(PROFILE_ACCURACY)
  static const string Profile = "Accuracy";
  #elif defined(PROFILE_BALANCED)
  static const string Profile = "Balanced";
  #elif defined(PROFILE_PERFORMANCE)
  static const string Profile = "Performance";
  #endif
}

#include "interface.hpp"

//debugging function hook:
//no overhead (and no debugger invocation) if not compiled with -DDEBUGGER
//wraps testing of function to allow invocation without a defined callback
template<typename T> struct hook;
template<typename R, typename... P> struct hook<auto (P...) -> R> {
  function<auto (P...) -> R> callback;

  auto operator()(P... p) const -> R {
    #if defined(DEBUGGER)
    if(callback) return callback(forward<P>(p)...);
    #endif
    return R();
  }

  hook() {}
  hook(const hook& hook) { callback = hook.callback; }
  hook(void* function) { callback = function; }
  hook(auto (*function)(P...) -> R) { callback = function; }
  template<typename C> hook(auto (C::*function)(P...) -> R, C* object) { callback = {function, object}; }
  template<typename C> hook(auto (C::*function)(P...) const -> R, C* object) { callback = {function, object}; }
  template<typename L> hook(const L& function) { callback = function; }

  auto operator=(const hook& source) -> hook& { callback = source.callback; return *this; }
};

#if defined(DEBUGGER)
  #define privileged public
#else
  #define privileged private
#endif

using varuint = varuint_t<uint>;

#endif
