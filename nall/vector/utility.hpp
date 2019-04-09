#pragma once

namespace nall {

template<typename T> auto vector<T>::fill(const T& value) -> void {
  for(uint64_t n : range(size())) _pool[n] = value;
}

template<typename T> auto vector<T>::sort(const function<bool (const T& lhs, const T& rhs)>& comparator) -> void {
  nall::sort(_pool, _size, comparator);
}

template<typename T> auto vector<T>::reverse() -> void {
  vector<T> reversed;
  for(uint64_t n : range(size())) reversed.prepend(_pool[n]);
  operator=(move(reversed));
}

template<typename T> auto vector<T>::find(const function<bool (const T& lhs)>& comparator) -> maybe<uint64_t> {
  for(uint64_t n : range(size())) if(comparator(_pool[n])) return n;
  return nothing;
}

template<typename T> auto vector<T>::find(const T& value) const -> maybe<uint64_t> {
  for(uint64_t n : range(size())) if(_pool[n] == value) return n;
  return nothing;
}

template<typename T> auto vector<T>::findSorted(const T& value) const -> maybe<uint64_t> {
  int64_t l = 0, r = size() - 1;
  while(l <= r) {
    int64_t m = l + (r - l >> 1);
    if(value == _pool[m]) return m;
    value < _pool[m] ? r = m - 1 : l = m + 1;
  }
  return nothing;
}

template<typename T> auto vector<T>::FindWhere::operator==(const T& value) -> vector<iterator<T>> { return move(find<std::equal_to<T>>(value)); }
template<typename T> auto vector<T>::FindWhere::operator!=(const T& value) -> vector<iterator<T>> { return move(find<std::not_equal_to<T>>(value)); }
template<typename T> auto vector<T>::FindWhere::operator< (const T& value) -> vector<iterator<T>> { return move(find<std::less<T>>(value)); }
template<typename T> auto vector<T>::FindWhere::operator<=(const T& value) -> vector<iterator<T>> { return move(find<std::less_equal<T>>(value)); }
template<typename T> auto vector<T>::FindWhere::operator> (const T& value) -> vector<iterator<T>> { return move(find<std::greater<T>>(value)); }
template<typename T> auto vector<T>::FindWhere::operator>=(const T& value) -> vector<iterator<T>> { return move(find<std::greater_equal<T>>(value)); }

template<typename T> template<typename Compare> auto vector<T>::FindWhere::find(const T& value) -> vector<iterator<T>> {
  vector<iterator<T>> found;
  for(auto iterator = self.begin(); iterator != self.end(); ++iterator) {
    if(Compare()(*iterator, value)) found.append(iterator);
  }
  return move(found);
}

template<typename T> auto vector<T>::FindWhereConst::operator==(const T& value) const -> vector<iterator_const<T>> { return move(find<std::equal_to<T>>(value)); }
template<typename T> auto vector<T>::FindWhereConst::operator!=(const T& value) const -> vector<iterator_const<T>> { return move(find<std::not_equal_to<T>>(value)); }
template<typename T> auto vector<T>::FindWhereConst::operator< (const T& value) const -> vector<iterator_const<T>> { return move(find<std::less<T>>(value)); }
template<typename T> auto vector<T>::FindWhereConst::operator<=(const T& value) const -> vector<iterator_const<T>> { return move(find<std::less_equal<T>>(value)); }
template<typename T> auto vector<T>::FindWhereConst::operator> (const T& value) const -> vector<iterator_const<T>> { return move(find<std::greater<T>>(value)); }
template<typename T> auto vector<T>::FindWhereConst::operator>=(const T& value) const -> vector<iterator_const<T>> { return move(find<std::greater_equal<T>>(value)); }

template<typename T> template<typename Compare> auto vector<T>::FindWhereConst::find(const T& value) const -> vector<iterator_const<T>> {
  vector<iterator_const<T>> found;
  for(auto iterator = self.begin(); iterator != self.end(); ++iterator) {
    if(Compare()(*iterator, value)) found.append(iterator);
  }
  return move(found);
}

template<typename T> auto vector<T>::foreach(const function<void (const T&)>& callback) -> void {
  for(uint64_t n : range(size())) callback(_pool[n]);
}

template<typename T> auto vector<T>::foreach(const function<void (uint, const T&)>& callback) -> void {
  for(uint64_t n : range(size())) callback(n, _pool[n]);
}

}
