#pragma once

#include <nall/counting-sort.hpp>
#include <nall/div-suf-sort.hpp>
#include <nall/range.hpp>

namespace nall {

/*

input:
  data = "acaaacatat"

suffix_array:
  suffixes = [2, 3, 0, 4, 8, 6, 1, 5, 9, 7]
  2 "aaaacatat"
  3 "aacatat"
  0 "acaaacatat"
  4 "acatat"
  8 "at"
  6 "atat"
  1 "caaacatat"
  5 "catat"
  9 "t"
  7 "tat"

suffix_array_inv:
  inverted = [2, 6, 0, 1, 3, 7, 5, 9, 4, 8]

suffix_array_lcp:
  prefixes = [-, 2, 1, 3, 1, 2, 0, 2, 0, 1]
  "aaaacatat"  -
  "aacatat"    2 "aa"
  "acaaacatat" 1 "a"
  "acatat"     3 "aca"
  "at"         1 "a"
  "atat"       2 "at"
  "caaacatat"  0
  "catat"      2 "ca"
  "t"          0
  "tat"        1 "t"

suffix_array_phi:
  phi = [3, 6, -, 2, 0, 1, 8, 9, 4, 5]

suffix_array_lps:
  lengths = [-, 0, 1, 2, 3, 2, 1, 0, 2, 1]
  offsets = [-, -, 0, 2, 0, 1, 4, -, 6, 7]
  "acaaacatat" (-,-)
   "caaacatat" (0,-)
    "aaacatat" (1,0) at 0, match "a"
     "aacatat" (2,2) at 2, match "aa"
      "acatat" (3,0) at 0, match "aca"
       "catat" (2,1) at 1, match "ca"
        "atat" (1,4) at 4, match "a" (not 0)
         "tat" (0,-)
          "at" (2,6) at 6, match "at"
           "t" (1,7) at 7, match "a" (not 0)

*/

// O(n log n)
inline auto suffix_array(int* suffixes, const uint8_t* data, int size) -> void {
  for(int n : range(size)) suffixes[n] = n;
  #if 1
  div_suf_sort(suffixes, data, size);
  #else
  sort(suffixes, size, [&](int lhs, int rhs) -> bool {
    return memory::compare(data + lhs, size - lhs, data + rhs, size - rhs) < 0;
  });
  #endif
}

// inverse
// O(n)
inline auto suffix_array_inv(int* inverted, const int* suffixes, int size) -> void {
  for(int i : range(size)) inverted[suffixes[i]] = i;
}

// longest common prefix
// O(n)
inline auto suffix_array_lcp(int* prefixes, const int* inverted, const int* suffixes, const uint8_t* data, int size) -> void {
  prefixes[0] = -1;
  for(int i = 0, l = 0; i < size; i++) {
    if(inverted[i] == size - 1) { l = 0; continue; }
    int j = suffixes[inverted[i] + 1];
    while(i + l < size && j + l < size && data[i + l] == data[j + l]) l++;
    prefixes[1 + inverted[i]] = l;
    if(l) l--;
  }
}

// O(n)
inline auto suffix_array_phi(int* phi, const int* suffixes, int size) -> void {
  phi[suffixes[0]] = -1;
  for(int i : range(1, size)) phi[suffixes[i]] = suffixes[i - 1];
}

// longest previous string (longest previous factor)
// O(n)
inline auto suffix_array_lps(int* lengths, int* offsets, const int* phi, const uint8_t* data, int size) -> void {
  function<void (int, int, int)> sop = [&](int i, int l, int j) -> void {
    if(lengths[i] < 0) {
      lengths[i] = l;
      offsets[i] = j;
    } else {
      if(lengths[i] < l) {
        if(offsets[i] > j) {
          sop(offsets[i], lengths[i], j);
        } else {
          sop(j, lengths[i], offsets[i]);
        }
        lengths[i] = l;
        offsets[i] = j;
      } else {
        if(offsets[i] > j) {
          sop(offsets[i], l, j);
        } else {
          sop(j, l, offsets[i]);
        }
      }
    }
  };

  int l = 0;
  for(int i : range(size)) lengths[i] = -1;
  for(int i : range(size)) {
    int j = phi[i];
    while(i + l < size && j + l < size && data[i + l] == data[j + l]) l++;
    if(i > j) {
      sop(i, l, j);
    } else {
      sop(j, l, i);
    }
    if(l) l--;
  }
  lengths[0] = -1;
}

//partial_suffix_array computes a suffix array in O(n) time by only sorting by SuffixBits into each prefix
//this is much faster than a proper suffix_array, but at the penalty of not being 100% sorted
//thus, least common prefixes cannot readily be used with this; deduplication is suggested for binary searching
//suffixes[] = (offsets) list of indexes into data[] in sorted order
//prefixes[] = (values) sorted list of data[]

template<uint SuffixBits, uint PrefixBits>
inline auto partial_suffix_array(uint* suffixes, uint* prefixes, const void* data, uint64_t size, uint offset = 0) -> void;

template<>
inline auto partial_suffix_array<32, 24>(uint* suffixes, uint* prefixes, const void* data, uint64_t size, uint offset) -> void {
  auto input = (const uint8_t*)data;
  if(size == 0 || !data || !suffixes || !prefixes) return;
  if(size == 1) return suffixes[0] = offset << 16, prefixes[0] = input[0], void();

  auto elements = new uint64_t[2 * size], lhs = &elements[0], rhs = &elements[size];
  for(uint index : range(size - 2)) {
    elements[index] = index | uint64_t(input[0] << 16 | input[1] << 8 | input[2] << 0) << 32, input++;
  }
  elements[size - 2] = size - 2 | uint64_t(input[0] << 16 | input[1] << 8) << 32, input++;
  elements[size - 1] = size - 1 | uint64_t(input[0] << 16) << 32;

  counting_sort<12, 32>(rhs, lhs, size);
  counting_sort<12, 44>(lhs, rhs, size);
  for(uint n : range(size)) {
    suffixes[n] = (uint32_t)lhs[n] + offset;
    prefixes[n] = lhs[n] >> 32;
  }

  delete[] elements;
}

template<>
inline auto partial_suffix_array<32, 32>(uint* suffixes, uint* prefixes, const void* data, uint64_t size, uint offset) -> void {
  auto input = (const uint8_t*)data;
  if(size == 0 || !data || !suffixes || !prefixes) return;
  if(size == 1) return suffixes[0] = offset, prefixes[0] = input[0], void();
  if(size == 2) {
    suffixes[0] = offset, suffixes[1] = 1 + offset;
    prefixes[0] = input[0] << 24 | input[1] << 16, prefixes[1] = input[1] << 24;
    if(input[0] >= input[1]) swap(suffixes[0], suffixes[1]), swap(prefixes[0], prefixes[1]);
    return;
  }

  auto elements = new uint64_t[2 * size], lhs = &elements[0], rhs = &elements[size];
  for(uint index : range(size - 3)) {
    elements[index] = index | uint64_t(input[0] << 24 | input[1] << 16 | input[2] << 8 |  input[3] << 0) << 32, input++;
  }
  elements[size - 3] = size - 3 | uint64_t(input[0] << 24 | input[1] << 16 | input[2] << 8) << 32, input++;
  elements[size - 2] = size - 2 | uint64_t(input[0] << 24 | input[1] << 16) << 32, input++;
  elements[size - 1] = size - 1 | uint64_t(input[0] << 24) << 32;

  counting_sort<16, 32>(rhs, lhs, size);
  counting_sort<16, 48>(lhs, rhs, size);
  for(uint n : range(size)) {
    suffixes[n] = (uint32_t)lhs[n] + offset;
    prefixes[n] = lhs[n] >> 32;
  }

  delete[] elements;
}

}
