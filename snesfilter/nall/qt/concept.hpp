#ifndef NALL_QT_CONCEPT_HPP
#define NALL_QT_CONCEPT_HPP

#include <nall/concept.hpp>

namespace nall {
  template<typename T> struct has_count<QList<T>> { enum { value = true }; };
}

#endif
