#pragma once

#include <nall/nall.hpp>
#include <nall/dsp.hpp>
using namespace nall;

//todo: update code to support Natural<8,16,32>
//using int8 = Integer<8>;
//using int16 = Integer<16>;
//using int32 = Integer<32>;
//using int64 = Integer<64>;
//using uint32 = Natural<32>;
//using uint64 = Natural<64>;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using varuint = varuint_t<uint>;

namespace Emulator {
  static const string Name = "higan";
  static const string Version = "097.14";
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
