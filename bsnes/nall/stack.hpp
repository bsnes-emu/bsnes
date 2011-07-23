#ifndef NALL_STACK_HPP
#define NALL_STACK_HPP

#include <nall/concept.hpp>
#include <nall/vector.hpp>

namespace nall {
  template<typename T> struct stack : public linear_vector<T> {
    void push(const T &value) {
      linear_vector<T>::append(value);
    }

    T pull() {
      if(linear_vector<T>::size() == 0) throw;
      T value = linear_vector<T>::operator[](linear_vector<T>::size() - 1);
      linear_vector<T>::remove(linear_vector<T>::size() - 1);
      return value;
    }

    T& operator()() {
      if(linear_vector<T>::size() == 0) throw;
      return linear_vector<T>::operator[](linear_vector<T>::size() - 1);
    }
  };

  template<typename T> struct has_size<stack<T>> { enum { value = true }; };
}

#endif
