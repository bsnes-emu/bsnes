#ifndef NALL_FOREACH_HPP
#define NALL_FOREACH_HPP

#undef foreach
#define foreach(iter, object) \
  for(unsigned foreach_counter = 0, foreach_limit = foreach_size(object), foreach_once = 0, foreach_broken = 0; foreach_counter < foreach_limit && foreach_broken == 0; foreach_counter++, foreach_once = 0) \
    for(auto &iter = object[foreach_counter]; foreach_once == 0 && (foreach_broken = 1); foreach_once++, foreach_broken = 0)

#include <type_traits>
#include <nall/concept.hpp>
#include <nall/static.hpp>

namespace nall {
  template<typename T> unsigned foreach_size(const T& object, typename mp_enable_if<has_count<T>>::type = 0) {
    return object.count();
  }

  template<typename T> unsigned foreach_size(const T& object, typename mp_enable_if<has_length<T>>::type = 0) {
    return object.length();
  }

  template<typename T> unsigned foreach_size(const T& object, typename mp_enable_if<has_size<T>>::type = 0) {
    return object.size();
  }

  template<typename T> unsigned foreach_size(const T& object, typename mp_enable_if<std::is_array<T>>::type = 0) {
    return sizeof(T) / sizeof(typename std::remove_extent<T>::type);
  }
}

#endif
