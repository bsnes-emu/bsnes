/*
  libsort : version 0.01 ~byuu (2006-11-15)
*/

#ifndef LIBSORT_H
#define LIBSORT_H

template<typename T>
void sort(T list[], uint length) {
  for(uint d = 0; d < length; d++) {
  uint min = d;
    for(uint s = d + 1; s < length; s++) {
      if(list[s] < list[min]) { min = s; }
    }
    if(min != d) {
      swap(list[d], list[min]);
    }
  }
}

template<typename T, typename Tcmp>
void sort(T list[], uint length, Tcmp comparator) {
  for(uint d = 0; d < length; d++) {
  uint min = d;
    for(uint s = d + 1; s < length; s++) {
      if(comparator(list[s], list[min]) == true) { min = s; }
    }
    if(min != d) {
      swap(list[d], list[min]);
    }
  }
}

#endif
