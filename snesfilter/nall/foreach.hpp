#ifndef NALL_FOREACH_HPP
#define NALL_FOREACH_HPP

#include <type_traits>
#include <nall/concept.hpp>

#undef foreach

#define foreach2(iter, object) foreach3(iter, object, foreach_counter)
#define foreach3(iter, object, foreach_counter) \
  for(unsigned foreach_counter = 0, foreach_limit = container_size(object), foreach_once = 0, foreach_broken = 0; foreach_counter < foreach_limit && foreach_broken == 0; foreach_counter++, foreach_once = 0) \
    for(auto &iter = object[foreach_counter]; foreach_once == 0 && (foreach_broken = 1); foreach_once++, foreach_broken = 0)

#define foreach_impl(...) foreach_decl(__VA_ARGS__, foreach3(__VA_ARGS__), foreach2(__VA_ARGS__), foreach_too_few_arguments)
#define foreach_decl(_1, _2, _3, N, ...) N
#define foreach(...) foreach_impl(__VA_ARGS__)

#endif
