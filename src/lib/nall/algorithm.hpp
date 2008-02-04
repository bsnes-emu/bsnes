#ifndef NALL_ALGORITHM_HPP
#define NALL_ALGORITHM_HPP

namespace nall {

#undef min
#undef max

template<typename T, typename U>
T min(const T& t, const U& u) {
  return t < u ? t : u;
}

template<typename T, typename U>
T max(const T& t, const U& u) {
  return t > u ? t : u;
}

} //namespace nall

#endif //ifndef NALL_ALGORITHM_HPP
