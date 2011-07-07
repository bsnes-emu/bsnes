#ifndef NALL_STACK_HPP
#define NALL_STACK_HPP

#include <nall/vector.hpp>

namespace nall {
  template<typename T> struct stack_fifo {
    T& operator()() {
      if(list.size() == 0) throw;
      return list[0];
    }

    void reset() {
      list.reset();
    }

    unsigned size() const {
      return list.size();
    }

    void push(const T &value) {
      list.append(value);
    }

    T pull() {
      if(list.size() == 0) throw;
      T value = list[0];
      list.remove(0);
      return value;
    }

  private:
    linear_vector<T> list;
  };

  template<typename T> struct stack_filo {
    T& operator()() {
      if(list.size() == 0) throw;
      return list[list.size() - 1];
    }

    void reset() {
      list.reset();
    }

    unsigned size() const {
      return list.size();
    }

    void push(const T &value) {
      list.append(value);
    }

    T pull() {
      if(list.size() == 0) throw;
      T value = list[list.size() - 1];
      list.remove(list.size() - 1);
      return value;
    }

  private:
    linear_vector<T> list;
  };
}

#endif
