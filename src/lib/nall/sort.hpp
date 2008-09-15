#ifndef NALL_SORT_HPP
#define NALL_SORT_HPP

namespace nall {

template<typename T> inline void swap(T &x, T &y) {
  T temp = x;
  x = y;
  y = temp;
}

template<typename T>
void sort(T list[], unsigned length) {
  for(unsigned d = 0; d < length; d++) {
    unsigned min = d;
    for(unsigned s = d + 1; s < length; s++) {
      if(list[s] < list[min]) { min = s; }
    }
    if(min != d) {
      swap(list[d], list[min]);
    }
  }
}

template<typename T, typename Comparator>
void sort(T list[], unsigned length, Comparator comparator) {
  for(unsigned d = 0; d < length; d++) {
    unsigned min = d;
    for(unsigned s = d + 1; s < length; s++) {
      if(comparator(list[s], list[min]) == true) { min = s; }
    }
    if(min != d) {
      swap(list[d], list[min]);
    }
  }
}

} //namespace nall

#endif //ifndef NALL_SORT_HPP
