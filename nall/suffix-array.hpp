#pragma once

#include <nall/array.hpp>
#include <nall/counting-sort.hpp>
#include <nall/induced-sort.hpp>
#include <nall/range.hpp>
#include <nall/view.hpp>

namespace nall {

/*

input:
  data = "acaacatat"
  0 "acaacatat"
  1 "caacatat"
  2 "aacatat"
  3 "acatat"
  4 "catat"
  5 "atat"
  6 "tat"
  7 "at"
  8 "t"
  9 ""

suffix_array:
  suffixes = [9,2,0,3,7,5,1,4,8,6] => input + suffixes:
  9 ""
  2 "aacatat"
  0 "acaacatat"
  3 "acatat"
  7 "at"
  5 "atat"
  1 "caacatat"
  4 "catat"
  8 "t"
  6 "tat"

[auxiliary data structures to represent information lost from suffix trees]

suffix_array_invert:
  inverted = [2,6,1,3,7,5,9,4,8,0] => input + suffixes[inverted]:
  2 "acaacatat"
  6 "caacatat"
  1 "aacatat"
  3 "acatat"
  7 "catat"
  5 "atat"
  9 "tat"
  4 "at"
  8 "t"
  0 ""

suffix_array_lcp:
  prefixes = [0,1,3,1,2,0,2,0,1] => lcp[n] == lcp(n, n-1)
  ""          -
  "aacatat"   0
  "acaacatat" 1 "a"
  "acatat"    3 "aca"
  "at"        1 "a"
  "atat"      2 "at"
  "caacatat"  0
  "catat"     2 "ca"
  "t"         0
  "tat"       1 "t"

suffix_array_lrcp:
  llcp = [0,0,0,3,1,0,0,0,0,1] => llcp[m] == lcp(l, m)
  rlcp = [0,1,1,1,2,0,2,0,0,0] => rlcp[m] == lcp(m, r)

suffix_array_phi:
  phi = [2,5,9,0,1,7,8,3,4]

suffix_array_lpf:
  lengths = [0,0,1,3,2,1,0,2,1,0]
  offsets = [0,0,0,0,1,3,4,5,6,2]
  "acaacatat" (0,-)
   "caacatat" (0,-)
    "aacatat" (1,0) at 0, match "a"
     "acatat" (3,0) at 0, match "aca"
      "catat" (2,1) at 1, match "ca"
       "atat" (1,3) at 3, match "a"
        "tat" (0,-)
         "at" (2,5) at 5, match "at"
          "t" (1,6) at 6, match "t"
           "" (0,-)

*/

// via induced sorting
// O(n)
inline auto suffix_array(array_view<uint8_t> input) -> vector<int> {
  return induced_sort(input.data(), input.size());
}

// inverse
// O(n)
inline auto suffix_array_invert(array_view<int> suffixes) -> vector<int> {
  vector<int> inverted;
  inverted.reset(), inverted.reallocate(suffixes.size());
  for(int n : range(suffixes.size())) inverted[suffixes[n]] = n;
  return inverted;
}

// longest common prefix: lcp[n] == lcp(n, n-1)
// algorithm: kasai
// O(n)
inline auto suffix_array_lcp(array_view<int> suffixes, array_view<int> inverted, array_view<uint8_t> input) -> vector<int> {
  int size = input.size();
  vector<int> prefixes;
  prefixes.reset(), prefixes.reallocate(size);
  for(int i = 0, l = 0; i < size; i++) {
    if(inverted[i] == size) { l = 0; continue; }  //the next substring is empty; ignore it
    int j = suffixes[inverted[i] + 1];
    while(i + l < size && j + l < size && input[i + l] == input[j + l]) l++;
    prefixes[inverted[i]] = l;
    if(l) l--;
  }
  return prefixes;
}

// longest common prefixes - left + right
// llcp[m] == lcp(l, m)
// rlcp[m] == lcp(m, r)
// O(n)
inline auto suffix_array_lrcp(vector<int>& llcp, vector<int>& rlcp, array_view<int> lcp, array_view<int> suffixes, array_view<uint8_t> input) -> void {
  llcp.reset(), llcp.reallocate(lcp.size() + 1);
  rlcp.reset(), rlcp.reallocate(lcp.size() + 1);

  function<int (int, int)> recurse = [&](int l, int r) -> int {
    if(l == lcp.size()) return 0;
    if(l == r - 1) return lcp[l];
    int m = l + r >> 1;
    llcp[m] = recurse(l, m);
    rlcp[m] = recurse(m, r);
    return min(llcp[m], rlcp[m]);
  };
  recurse(0, lcp.size() + 1);
  llcp[0] = 0;
  rlcp[0] = 0;
}

// auxiliary data for suffix_array_lpf
// O(n)
inline auto suffix_array_phi(array_view<int> suffixes) -> vector<int> {
  vector<int> phi;
  phi.reset(), phi.reallocate(suffixes.size() - 1);
  for(int i : range(1, suffixes.size())) {
    phi[suffixes[i]] = suffixes[i - 1];
  }
  return phi;
}

// longest previous factor
// O(n)
inline auto suffix_array_lpf(vector<int>& lengths, vector<int>& offsets, array_view<int> phi, array_view<uint8_t> input) -> void {
  int l = 0, size = input.size();
  lengths.reset(), lengths.resize(size + 1, -1);
  offsets.reset(), offsets.resize(size + 1, -1);

  function<void (int, int, int)> recurse = [&](int i, int l, int j) -> void {
    if(lengths[i] < 0) {
      lengths[i] = l;
      offsets[i] = j;
    } else {
      if(lengths[i] < l) {
        if(offsets[i] > j) {
          recurse(offsets[i], lengths[i], j);
        } else {
          recurse(j, lengths[i], offsets[i]);
        }
        lengths[i] = l;
        offsets[i] = j;
      } else {
        if(offsets[i] > j) {
          recurse(offsets[i], l, j);
        } else {
          recurse(j, l, offsets[i]);
        }
      }
    }
  };

  for(int i : range(size)) {
    int j = phi[i];
    while(i + l < size && j + l < size && input[i + l] == input[j + l]) l++;
    if(i > j) {
      recurse(i, l, j);
    } else {
      recurse(j, l, i);
    }
    if(l) l--;
  }

  //there can be no previous factor for the start of input; clear these values from -1 to 0
  lengths[0] = 0;
  offsets[0] = 0;
}

// longest common prefix: lcp(l, r)
// O(n)
inline auto suffix_array_lcp(int l, int r, array_view<int> suffixes, array_view<uint8_t> input) -> int {
  int k = 0, size = input.size();
  l = suffixes[l], r = suffixes[r];
  while(l + k < size && r + k < size) {
    if(input[l + k] != input[r + k]) break;
    k++;
  }
  return k;
}

// O(n log m)
inline auto suffix_array_find(int& length, int& offset, array_view<int> suffixes, array_view<uint8_t> input, array_view<uint8_t> match) -> bool {
  length = 0, offset = 0;
  int l = 0, r = input.size();

  while(l < r - 1) {
    int m = l + r >> 1;
    int s = suffixes[m];

    int k = 0;
    while(k < match.size() && s + k < input.size()) {
      if(match[k] != input[s + k]) break;
      k++;
    }

    if(k > length) {
      length = k;
      offset = s;
      if(k == match.size()) return true;
    }

    if(match[k] < input[s + k]) {
      r = m;
    } else {
      l = m;
    }
  }

  return false;
}

// O(n + log m)
inline auto suffix_array_find(int& length, int& offset, array_view<int> llcp, array_view<int> rlcp, array_view<int> suffixes, array_view<uint8_t> input, array_view<uint8_t> match) -> bool {
  length = 0, offset = 0;
  int l = 0, r = input.size(), k = 0;

  while(l < r - 1) {
    int m = l + r >> 1;
    int s = suffixes[m];

    while(k < match.size() && s + k < input.size()) {
      if(match[k] != input[s + k]) break;
      k++;
    }

    if(k > length) {
      length = k;
      offset = s;
      if(k == match.size()) return true;
    }

    if(match[k] < input[s + k]) {
      r = m;
      k = min(k, llcp[m]);
    } else {
      l = m;
      k = min(k, rlcp[m]);
    }
  }

  return false;
}

//

struct SuffixArray {
  using type = SuffixArray;

  //O(n)
  inline SuffixArray(array_view<uint8_t> input) : input(input) {
    suffixes = suffix_array(input);
  }

  //O(n)
  inline auto lcp() -> type& {
    inverted = suffix_array_invert(suffixes);
    prefixes = suffix_array_lcp(suffixes, inverted, input);
    return *this;
  }

  //O(n)
  inline auto lrcp() -> type& {
    lcp();
    suffix_array_lrcp(prefixesL, prefixesR, prefixes, suffixes, input);
    return *this;
  }

  //O(n)
  inline auto lpf() -> type& {
    auto phi = suffix_array_phi(suffixes);
    suffix_array_lpf(lengths, offsets, phi, input);
    return *this;
  }

  inline auto operator[](int offset) const -> int {
    return suffixes[offset];
  }

  //O(n log m)
  //inline auto find(int& length, int& offset, array_view<uint8_t> match) -> bool {
  //  return suffix_array_find(length, offset, suffixes, input, match);
  //}

  //requires: lrcp()
  //O(n + log m)
  inline auto find(int& length, int& offset, array_view<uint8_t> match) -> bool {
    return suffix_array_find(length, offset, prefixesL, prefixesR, suffixes, input, match);
  }

  //requires: lpf()
  //O(n)
  inline auto previous(int& length, int& offset, int address) -> void {
    length = lengths[address];
    offset = offsets[address];
  }

  //non-owning reference: SuffixArray is invalidated if memory is freed
  array_view<uint8_t> input;

  //suffix array and auxiliary data structures
  vector<int> suffixes;   //suffix array
  vector<int> inverted;   //inverted suffix array
  vector<int> prefixes;   //longest common prefixes - lcp(n, n-1)
  vector<int> prefixesL;  //longest common prefixes - lcp(l, m)
  vector<int> prefixesR;  //longest common prefixes - lcp(m, r)
  vector<int> lengths;    //longest previous factors
  vector<int> offsets;    //longest previous factors
};

}
