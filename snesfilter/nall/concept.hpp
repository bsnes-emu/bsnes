#ifndef NALL_CONCEPT_HPP
#define NALL_CONCEPT_HPP

namespace nall {
  //unsigned count() const;
  template<typename T> struct has_count { enum { value = false }; };

  //unsigned length() const;
  template<typename T> struct has_length { enum { value = false }; };

  //unsigned size() const;
  template<typename T> struct has_size { enum { value = false }; };
}

#endif
