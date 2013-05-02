#ifndef NALL_GROUP_HPP
#define NALL_GROUP_HPP

//group
//vector of unique references

#include <nall/vector.hpp>

namespace nall {

template<typename T> struct group : protected vector<T*> {
  group& operator=(const group& source) { vector<T*>::operator=(source); return *this; }
  group& operator=(group&& source) { vector<T*>::operator=(std::move(source)); return *this; }
  template<typename... Args> group(Args&&... args) { construct(std::forward<Args>(args)...); }

  bool empty() const { return vector<T*>::empty(); }
  unsigned size() const { return vector<T*>::size(); }
  void reset() { vector<T*>::reset(); }

  T& first() const { return *vector<T*>::operator[](0); }

  //return true if at least one item was appended
  template<typename... Args> bool append(T& value, Args&&... args) {
    bool result = append(value);
    return append(std::forward<Args>(args)...) | result;
  }

  bool append(T& value) {
    if(vector<T*>::find(&value)) return false;
    return vector<T*>::append(&value), true;
  }

  //return true if at least one item was removed
  template<typename... Args> bool remove(T& value, Args&&... args) {
    bool result = remove(value);
    return remove(std::forward<Args>(args)...) | result;
  }

  bool remove(T& value) {
    if(auto position = vector<T*>::find(&value)) return vector<T*>::remove(position()), true;
    return false;
  }

  struct iterator : protected vector<T*>::const_iterator {
    T& operator*() const { return *vector<T*>::const_iterator::operator*(); }
    bool operator!=(const iterator& source) const { return vector<T*>::const_iterator::operator!=(source); }
    iterator& operator++() { vector<T*>::const_iterator::operator++(); return *this; }
    iterator(const group& source, unsigned position) : vector<T*>::const_iterator(source, position) {}
  };

  const iterator begin() const { return iterator(*this, 0); }
  const iterator end() const { return iterator(*this, size()); }

private:
  void construct() {}
  void construct(const group& source) { vector<T*>::operator=(source); }
  void construct(group&& source) { vector<T*>::operator=(std::move(source)); }
  template<typename... Args> void construct(T& value, Args&&... args) {
    append(value);
    construct(std::forward<Args>(args)...);
  }
};

}

#endif
