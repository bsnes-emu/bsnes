#pragma once

namespace nall {

template<typename T> auto vector<T>::sort(const function<bool (const T& lhs, const T& rhs)>& comparator) -> void {
  if(!comparator) return nall::sort(_pool, _size, [](const T& lhs, const T& rhs) { return lhs < rhs; });
  nall::sort(_pool, _size, comparator);
}

template<typename T> auto vector<T>::find(const T& value) const -> maybe<uint> {
  for(uint n : range(size())) if(_pool[n] == value) return n;
  return nothing;
}

}
