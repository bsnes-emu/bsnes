#pragma once

#include <nall/nall.hpp>
using namespace nall;

#include <audio/audio.hpp>

namespace Emulator {
  static const string Name = "higan";
  static const string Version = "098.03";
  static const string Author = "byuu";
  static const string License = "GPLv3";
  static const string Website = "http://byuu.org/";
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
