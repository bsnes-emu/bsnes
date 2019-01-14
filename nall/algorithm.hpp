#pragma once

#include <nall/traits.hpp>

#undef min
#undef max

namespace nall { namespace {

template<typename T, typename U> auto min(const T& t, const U& u) -> T {
  return t < u ? t : (T)u;
}

template<typename T, typename U, typename... P> auto min(const T& t, const U& u, P&&... p) -> T {
  return t < u ? min(t, forward<P>(p)...) : min(u, forward<P>(p)...);
}

template<typename T, typename U> auto max(const T& t, const U& u) -> T {
  return t > u ? t : (T)u;
}

template<typename T, typename U, typename... P> auto max(const T& t, const U& u, P&&... p) -> T {
  return t > u ? max(t, forward<P>(p)...) : max(u, forward<P>(p)...);
}

//template<typename T, typename U> auto ternary(bool test, const T& lhs, const U& rhs) -> T {
//  return test ? lhs : (T)rhs;
//}

}}
