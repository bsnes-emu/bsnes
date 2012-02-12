#ifndef BASE_HPP
#define BASE_HPP

const char Version[] = "085.07";

#include <nall/platform.hpp>
#include <nall/algorithm.hpp>
#include <nall/any.hpp>
#include <nall/array.hpp>
#include <nall/dl.hpp>
#include <nall/dsp.hpp>
#include <nall/endian.hpp>
#include <nall/file.hpp>
#include <nall/function.hpp>
#include <nall/moduloarray.hpp>
#include <nall/priorityqueue.hpp>
#include <nall/property.hpp>
#include <nall/random.hpp>
#include <nall/serializer.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>
using namespace nall;

//debugging function hook:
//no overhead (and no debugger invocation) if not compiled with -DDEBUGGER
//wraps testing of function to allow invocation without a defined callback
template<typename T> struct hook;
template<typename R, typename... P> struct hook<R (P...)> {
  function<R (P...)> callback;

  R operator()(P... p) const {
    #if defined(DEBUGGER)
    if(callback) return callback(std::forward<P>(p)...);
    #endif
    return R();
  }

  hook() {}
  hook(const hook &hook) { callback = hook.callback; }
  hook(void *function) { callback = function; }
  hook(R (*function)(P...)) { callback = function; }
  template<typename C> hook(R (C::*function)(P...), C *object) { callback = { function, object }; }
  template<typename C> hook(R (C::*function)(P...) const, C *object) { callback = { function, object }; }
  template<typename L> hook(const L& function) { callback = function; }

  hook& operator=(const hook& hook) { callback = hook.callback; return *this; }
};

#if defined(DEBUGGER)
  #define privileged public
#else
  #define privileged private
#endif

typedef int_t< 1>  int1;
typedef int_t< 2>  int2;
typedef int_t< 3>  int3;
typedef int_t< 4>  int4;
typedef int_t< 5>  int5;
typedef int_t< 6>  int6;
typedef int_t< 7>  int7;
typedef int8_t     int8;
typedef int_t< 9>  int9;
typedef int_t<10> int10;
typedef int_t<11> int11;
typedef int_t<12> int12;
typedef int_t<13> int13;
typedef int_t<14> int14;
typedef int_t<15> int15;
typedef int16_t   int16;
typedef int_t<17> int17;
typedef int_t<18> int18;
typedef int_t<19> int19;
typedef int_t<20> int20;
typedef int_t<21> int21;
typedef int_t<22> int22;
typedef int_t<23> int23;
typedef int_t<24> int24;
typedef int_t<25> int25;
typedef int_t<26> int26;
typedef int_t<27> int27;
typedef int_t<28> int28;
typedef int_t<29> int29;
typedef int_t<30> int30;
typedef int_t<31> int31;
typedef int32_t   int32;
typedef int64_t   int64;

typedef uint_t< 1>  uint1;
typedef uint_t< 2>  uint2;
typedef uint_t< 3>  uint3;
typedef uint_t< 4>  uint4;
typedef uint_t< 5>  uint5;
typedef uint_t< 6>  uint6;
typedef uint_t< 7>  uint7;
typedef uint8_t     uint8;
typedef uint_t< 9>  uint9;
typedef uint_t<10> uint10;
typedef uint_t<11> uint11;
typedef uint_t<12> uint12;
typedef uint_t<13> uint13;
typedef uint_t<14> uint14;
typedef uint_t<15> uint15;
typedef uint16_t   uint16;
typedef uint_t<17> uint17;
typedef uint_t<18> uint18;
typedef uint_t<19> uint19;
typedef uint_t<20> uint20;
typedef uint_t<21> uint21;
typedef uint_t<22> uint22;
typedef uint_t<23> uint23;
typedef uint_t<24> uint24;
typedef uint_t<25> uint25;
typedef uint_t<26> uint26;
typedef uint_t<27> uint27;
typedef uint_t<28> uint28;
typedef uint_t<29> uint29;
typedef uint_t<30> uint30;
typedef uint_t<31> uint31;
typedef uint32_t   uint32;
typedef uint64_t   uint64;

typedef varuint_t varuint;

#endif
