#pragma once

//divsufsort
//author: Yuta Mori
//license: MIT

//class:   suffix sort (variant)
//average: O(n)
//worst:   O(n log n)
//memory:  O(5n)
//stable?: yes (all values unique)

//omitted functionality:
//* OpenMP support
//* assertions
//* non 8-bit values
//* non 32-bit indexes

#define ALPHABET_SIZE (256)
#define BUCKET_A(_c0) bucket_A[(_c0)]
#define BUCKET_B(_c0, _c1) (bucket_B[((_c1) << 8) | (_c0)])
#define BUCKET_BSTAR(_c0, _c1) (bucket_B[((_c0) << 8) | (_c1)])
#define BUCKET_A_SIZE (ALPHABET_SIZE)
#define BUCKET_B_SIZE (ALPHABET_SIZE * ALPHABET_SIZE)
#define SS_INSERTIONSORT_THRESHOLD (8)
#define SS_BLOCKSIZE (1024)  //0..32767
#if SS_BLOCKSIZE == 0
  #define SS_MISORT_STACKSIZE (64)
#elif SS_BLOCKSIZE <= 4096
  #define SS_MISORT_STACKSIZE (16)
#else
  #define SS_MISORT_STACKSIZE (24)
#endif
#define SS_SMERGE_STACKSIZE (32)
#define TR_INSERTIONSORT_THRESHOLD (8)
#define TR_STACKSIZE (64)
#define STACK_PUSH(_a, _b, _c, _d) \
  do { \
    stack[ssize].a = (_a), stack[ssize].b = (_b), \
    stack[ssize].c = (_c), stack[ssize++].d = (_d); \
  } while(0)
#define STACK_PUSH5(_a, _b, _c, _d, _e) \
  do { \
    stack[ssize].a = (_a), stack[ssize].b = (_b), \
    stack[ssize].c = (_c), stack[ssize].d = (_d), stack[ssize++].e = (_e); \
  } while(0)
#define STACK_POP(_a, _b, _c, _d) \
  do { \
    if(ssize == 0) return; \
    (_a) = stack[--ssize].a, (_b) = stack[ssize].b, \
    (_c) = stack[ssize].c, (_d) = stack[ssize].d; \
  } while(0)
#define STACK_POP5(_a, _b, _c, _d, _e) \
  do { \
    if(ssize == 0) return; \
    (_a) = stack[--ssize].a, (_b) = stack[ssize].b, \
    (_c) = stack[ssize].c, (_d) = stack[ssize].d, (_e) = stack[ssize].e; \
  } while(0)

namespace nall { namespace libdivsufsort {

inline auto ilg16(int n) -> int {
  static int lg_table[256] = {-1};
  if(!lg_table[255]) for(uint n : range(1, 256)) lg_table[n] = floor(log2(n));
  return (n & 0xff00) ?
           8 + lg_table[(n >> 8) & 0xff]:
           0 + lg_table[(n >> 0) & 0xff];
}

inline auto ilg32(int n) -> int {
  static int lg_table[256] = {-1};
  if(!lg_table[255]) for(uint n : range(1, 256)) lg_table[n] = floor(log2(n));
  return (n & 0xffff0000) ?
           ((n & 0xff000000) ?
             24 + lg_table[(n >> 24) & 0xff] :
             16 + lg_table[(n >> 16) & 0xff]):
           ((n & 0x0000ff00) ?
              8 + lg_table[(n >>  8) & 0xff] :
              0 + lg_table[(n >>  0) & 0xff]);
}

// [[sssort.c]]

#if (SS_BLOCKSIZE == 0) || (SS_INSERTIONSORT_THRESHOLD < SS_BLOCKSIZE)

inline auto ss_ilg(int n) -> int {
  #if SS_BLOCKSIZE == 0
  return ilg32(n);
  #elif SS_BLOCKSIZE < 256
  static int lg_table[256] = {-1};
  if(!lg_table[255]) for(uint n : range(1, 256)) lg_table[n] = floor(log2(n));
  return lg_table[n];
  #else
  return ilg16(n);
  #endif
}

#endif

#if SS_BLOCKSIZE != 0

inline auto ss_isqrt(int x) -> int {
  static int sqq_table[256] = {};
  if(!sqq_table[255]) for(uint n : range(256)) sqq_table[n] = floor(sqrt(n) * 16.0);

  int y, e;

  if(x >= (SS_BLOCKSIZE * SS_BLOCKSIZE)) { return SS_BLOCKSIZE; }
  e = ilg32(x);

  if(e >= 16) {
    y = sqq_table[x >> ((e - 6) - (e & 1))] << ((e >> 1) - 7);
    if(e >= 24) { y = (y + 1 + x / y) >> 1; }
    y = (y + 1 + x / y) >> 1;
  } else if(e >= 8) {
    y = (sqq_table[x >> ((e - 6) - (e & 1))] >> (7 - (e >> 1))) + 1;
  } else {
    return sqq_table[x] >> 4;
  }

  return (x < (y * y)) ? y - 1 : y;
}

#endif

//compare two suffixes
inline auto ss_compare(const uint8_t* T, const int* p1, const int* p2, int depth) -> int {
  const uint8_t *U1, *U2, *U1n, *U2n;
  for(U1 = T + depth + *p1, U2 = T + depth + *p2, U1n = T + *(p1 + 1) + 2, U2n = T + *(p2 + 1) + 2;
     (U1 < U1n) && (U2 < U2n) && (*U1 == *U2);
     ++U1, ++U2
  );
  return U1 < U1n ? (U2 < U2n ? *U1 - *U2 : 1) : (U2 < U2n ? -1 : 0);
}

#if (SS_BLOCKSIZE != 1) && (SS_INSERTIONSORT_THRESHOLD != 1)

inline auto ss_insertionsort(const uint8_t* T, const int* PA, int* first, int* last, int depth) -> void {
  int *i, *j, t, r;

  for(i = last - 2; first <= i; --i) {
    for(t = *i, j = i + 1; 0 < (r = ss_compare(T, PA + t, PA + *j, depth));) {
      do { *(j - 1) = *j; } while((++j < last) && (*j < 0));
      if(last <= j) break;
    }
    if(r == 0) *j = ~*j;
    *(j - 1) = t;
  }
}

#endif

#if (SS_BLOCKSIZE == 0) || (SS_INSERTIONSORT_THRESHOLD < SS_BLOCKSIZE)

inline auto ss_fixdown(const uint8_t* Td, const int* PA, int* SA, int i, int size) -> void {
  int j, k, v, c, d, e;

  for(v = SA[i], c = Td[PA[v]]; (j = 2 * i + 1) < size; SA[i] = SA[k], i = k) {
    d = Td[PA[SA[k = j++]]];
    if(d < (e = Td[PA[SA[j]]])) { k = j; d = e; }
    if(d <= c) break;
  }
  SA[i] = v;
}

//simple top-down heapsort
inline auto ss_heapsort(const uint8_t* Td, const int* PA, int* SA, int size) -> void {
  int i, m, t;

  m = size;
  if((size % 2) == 0) {
    m--;
    if(Td[PA[SA[m / 2]]] < Td[PA[SA[m]]]) swap(SA[m], SA[m / 2]);
  }

  for(i = m / 2 - 1; 0 <= i; --i) ss_fixdown(Td, PA, SA, i, m);
  if((size % 2) == 0) { swap(SA[0], SA[m]); ss_fixdown(Td, PA, SA, 0, m); }
  for(i = m - 1; 0 < i; --i) {
    t = SA[0], SA[0] = SA[i];
    ss_fixdown(Td, PA, SA, 0, i);
    SA[i] = t;
  }
}

//returns the median of three elements
inline auto ss_median3(const uint8_t* Td, const int* PA, int* v1, int* v2, int* v3) -> int* {
  if(Td[PA[*v1]] > Td[PA[*v2]]) swap(v1, v2);
  if(Td[PA[*v2]] > Td[PA[*v3]]) return Td[PA[*v1]] > Td[PA[*v3]] ? v1 : v3;
  return v2;
}

//returns the median of five elements
inline auto ss_median5(const uint8_t* Td, const int* PA, int* v1, int* v2, int* v3, int* v4, int* v5) -> int* {
  if(Td[PA[*v2]] > Td[PA[*v3]]) swap(v2, v3);
  if(Td[PA[*v4]] > Td[PA[*v5]]) swap(v4, v5);
  if(Td[PA[*v2]] > Td[PA[*v4]]) swap(v2, v4), swap(v3, v5);
  if(Td[PA[*v1]] > Td[PA[*v3]]) swap(v1, v3);
  if(Td[PA[*v1]] > Td[PA[*v4]]) swap(v1, v4), swap(v3, v5);
  if(Td[PA[*v3]] > Td[PA[*v4]]) return v4;
  return v3;
}

//returns the pivot element
inline auto ss_pivot(const uint8_t* Td, const int* PA, int* first, int* last) -> int* {
  int *middle, t;

  t = last - first;
  middle = first + t / 2;

  if(t <= 512) {
    if(t <= 32) {
      return ss_median3(Td, PA, first, middle, last - 1);
    } else {
      t >>= 2;
      return ss_median5(Td, PA, first, first + t, middle, last - 1 - t, last - 1);
    }
  }
  t >>= 3;
  first  = ss_median3(Td, PA, first, first + t, first + (t << 1));
  middle = ss_median3(Td, PA, middle - t, middle, middle + t);
  last   = ss_median3(Td, PA, last - 1 - (t << 1), last - 1 - t, last - 1);
  return   ss_median3(Td, PA, first, middle, last);
}

//binary partition for substrings
inline auto ss_partition(const int* PA, int* first, int* last, int depth) -> int* {
  int *a, *b, t;
  for(a = first - 1, b = last;;) {
    for(; (++a < b) && ((PA[*a] + depth) >= (PA[*a + 1] + 1));) *a = ~*a;
    for(; (a < --b) && ((PA[*b] + depth) <  (PA[*b + 1] + 1)););
    if(b <= a) break;
    t = ~*b;
    *b = *a;
    *a = t;
  }
  if(first < a) *first = ~*first;
  return a;
}

//multikey introsort for medium size groups
inline auto ss_mintrosort(const uint8_t* T, const int* PA, int* first, int* last, int depth) -> void {
  #define STACK_SIZE SS_MISORT_STACKSIZE
  struct { int *a, *b, c, d; } stack[STACK_SIZE];
  const uint8_t* Td;
  int *a, *b, *c, *d, *e, *f;
  int s, t, ssize, limit, v, x = 0;

  for(ssize = 0, limit = ss_ilg(last - first);;) {
    if((last - first) <= SS_INSERTIONSORT_THRESHOLD) {
      #if 1 < SS_INSERTIONSORT_THRESHOLD
      if(1 < (last - first)) ss_insertionsort(T, PA, first, last, depth);
      #endif
      STACK_POP(first, last, depth, limit);
      continue;
    }

    Td = T + depth;
    if(limit-- == 0) ss_heapsort(Td, PA, first, last - first);
    if(limit < 0) {
      for(a = first + 1, v = Td[PA[*first]]; a < last; ++a) {
        if((x = Td[PA[*a]]) != v) {
          if(1 < (a - first)) break;
          v = x;
          first = a;
        }
      }
      if(Td[PA[*first] - 1] < v) {
        first = ss_partition(PA, first, a, depth);
      }
      if((a - first) <= (last - a)) {
        if(1 < (a - first)) {
          STACK_PUSH(a, last, depth, -1);
          last = a, depth += 1, limit = ss_ilg(a - first);
        } else {
          first = a, limit = -1;
        }
      } else {
        if(1 < (last - a)) {
          STACK_PUSH(first, a, depth + 1, ss_ilg(a - first));
          first = a, limit = -1;
        } else {
          last = a, depth += 1, limit = ss_ilg(a - first);
        }
      }
      continue;
    }

    //choose pivot
    a = ss_pivot(Td, PA, first, last);
    v = Td[PA[*a]];
    swap(*first, *a);

    //partition
    for(b = first; (++b < last) && ((x = Td[PA[*b]]) == v););
    if(((a = b) < last) && (x < v)) {
      for(; (++b < last) && ((x = Td[PA[*b]]) <= v);) {
        if(x == v) { swap(*b, *a); ++a; }
      }
    }
    for(c = last; (b < --c) && ((x = Td[PA[*c]]) == v););
    if((b < (d = c)) && (x > v)) {
      for(; (b < --c) && ((x = Td[PA[*c]]) >= v);) {
        if(x == v) { swap(*c, *d); --d; }
      }
    }
    for(; b < c;) {
      swap(*b, *c);
      for(; (++b < c) && ((x = Td[PA[*b]]) <= v);) {
        if(x == v) { swap(*b, *a); ++a; }
      }
      for(; (b < --c) && ((x = Td[PA[*c]]) >= v);) {
        if(x == v) { swap(*c, *d); --d; }
      }
    }

    if(a <= d) {
      c = b - 1;

      if((s = a - first) > (t = b - a)) s = t;
      for(e = first, f = b - s; 0 < s; --s, ++e, ++f) swap(*e, *f);
      if((s = d - c) > (t = last - d - 1)) s = t;
      for(e = b, f = last - s; 0 < s; --s, ++e, ++f) swap(*e, *f);

      a = first + (b - a), c = last - (d - c);
      b = (v <= Td[PA[*a] - 1]) ? a : ss_partition(PA, a, c, depth);

      if((a - first) <= (last - c)) {
        if((last - c) <= (c - b)) {
          STACK_PUSH(b, c, depth + 1, ss_ilg(c - b));
          STACK_PUSH(c, last, depth, limit);
          last = a;
        } else if((a - first) <= (c - b)) {
          STACK_PUSH(c, last, depth, limit);
          STACK_PUSH(b, c, depth + 1, ss_ilg(c - b));
          last = a;
        } else {
          STACK_PUSH(c, last, depth, limit);
          STACK_PUSH(first, a, depth, limit);
          first = b, last = c, depth += 1, limit = ss_ilg(c - b);
        }
      } else {
        if((a - first) <= (c - b)) {
          STACK_PUSH(b, c, depth + 1, ss_ilg(c - b));
          STACK_PUSH(first, a, depth, limit);
          first = c;
        } else if((last - c) <= (c - b)) {
          STACK_PUSH(first, a, depth, limit);
          STACK_PUSH(b, c, depth + 1, ss_ilg(c - b));
          first = c;
        } else {
          STACK_PUSH(first, a, depth, limit);
          STACK_PUSH(c, last, depth, limit);
          first = b, last = c, depth += 1, limit = ss_ilg(c - b);
        }
      }
    } else {
      limit += 1;
      if(Td[PA[*first] - 1] < v) {
        first = ss_partition(PA, first, last, depth);
        limit = ss_ilg(last - first);
      }
      depth += 1;
    }
  }
  #undef STACK_SIZE
}

#endif

#if SS_BLOCKSIZE != 0

inline auto ss_blockswap(int* a, int* b, int n) -> void {
  int t;
  for(; 0 < n; --n, ++a, ++b) {
    t = *a, *a = *b, *b = t;
  }
}

inline auto ss_rotate(int* first, int* middle, int* last) -> void {
  int *a, *b, t, l, r;
  l = middle - first, r = last - middle;
  for(; (0 < l) && (0 < r);) {
    if(l == r) { ss_blockswap(first, middle, l); break; }
    if(l < r) {
      a = last - 1, b = middle - 1;
      t = *a;
      do {
        *a-- = *b, *b-- = *a;
        if(b < first) {
          *a = t;
          last = a;
          if((r -= l + 1) <= l) break;
          a -= 1, b = middle - 1;
          t = *a;
        }
      } while(1);
    } else {
      a = first, b = middle;
      t = *a;
      do {
        *a++ = *b, *b++ = *a;
        if(last <= b) {
          *a = t;
          first = a + 1;
          if((l -= r + 1) <= r) break;
          a += 1, b = middle;
          t = *a;
        }
      } while(1);
    }
  }
}

inline auto ss_inplacemerge(
  const uint8_t* T, const int* PA, int* first, int* middle, int* last, int depth
) -> void {
  const int* p;
  int *a, *b, len, half, q, r, x;

  for(;;) {
    if(*(last - 1) < 0) { x = 1; p = PA + ~*(last - 1); }
    else                { x = 0; p = PA +  *(last - 1); }
    for(a = first, len = middle - first, half = len >> 1, r = -1; 0 < len; len = half, half >>= 1) {
      b = a + half;
      q = ss_compare(T, PA + ((0 <= *b) ? *b : ~*b), p, depth);
      if(q < 0) {
        a = b + 1;
        half -= (len & 1) ^ 1;
      } else {
        r = q;
      }
    }
    if(a < middle) {
      if(r == 0) *a = ~*a;
      ss_rotate(a, middle, last);
      last -= middle - a;
      middle = a;
      if(first == middle) break;
    }
    --last;
    if(x != 0) { while(*--last < 0); }
    if(middle == last) break;
  }
}

//merge-forward with internal buffer
inline auto ss_mergeforward(
  const uint8_t* T, const int* PA, int* first, int* middle, int* last, int* buf, int depth
) -> void {
  int *a, *b, *c, *bufend, t, r;

  bufend = buf + (middle - first) - 1;
  ss_blockswap(buf, first, middle - first);

  for(t = *(a = first), b = buf, c = middle;;) {
    r = ss_compare(T, PA + *b, PA + *c, depth);
    if(r < 0) {
      do {
        *a++ = *b;
        if(bufend <= b) { *bufend = t; return; }
        *b++ = *a;
      } while(*b < 0);
    } else if(r > 0) {
      do {
        *a++ = *c, *c++ = *a;
        if(last <= c) {
          while(b < bufend) { *a++ = *b, *b++ = *a; }
          *a = *b, *b = t;
          return;
        }
      } while(*c < 0);
    } else {
      *c = ~*c;
      do {
        *a++ = *b;
        if(bufend <= b) { *bufend = t; return; }
        *b++ = *a;
      } while(*b < 0);

      do {
        *a++ = *c, *c++ = *a;
        if(last <= c) {
          while(b < bufend) *a++ = *b, *b++ = *a;
          *a = *b, *b = t;
          return;
        }
      } while(*c < 0);
    }
  }
}

//merge-backward with internal buffer
inline auto ss_mergebackward(
  const uint8_t* T, const int* PA, int* first, int* middle, int* last, int* buf, int depth
) -> void {
  const int *p1, *p2;
  int *a, *b, *c, *bufend, t, r, x;

  bufend = buf + (last - middle - 1);
  ss_blockswap(buf, middle, last - middle);

  x = 0;
  if(*bufend < 0)       { p1 = PA + ~*bufend; x |= 1; }
  else                  { p1 = PA +  *bufend; }
  if(*(middle - 1) < 0) { p2 = PA + ~*(middle - 1); x |= 2; }
  else                  { p2 = PA +  *(middle - 1); }
  for(t = *(a = last - 1), b = bufend, c = middle - 1;;) {
    r = ss_compare(T, p1, p2, depth);
    if(0 < r) {
      if(x & 1) { do { *a-- = *b, *b-- = *a; } while(*b < 0); x ^= 1; }
      *a-- = *b;
      if(b <= buf) { *buf = t; break; }
      *b-- = *a;
      if(*b < 0) { p1 = PA + ~*b; x |= 1; }
      else       { p1 = PA +  *b; }
    } else if(r < 0) {
      if(x & 2) { do { *a-- = *c, *c-- = *a; } while(*c < 0); x ^= 2; }
      *a-- = *c, *c-- = *a;
      if(c < first) {
        while(buf < b) *a-- = *b, *b-- = *a;
        *a = *b, *b = t;
        break;
      }
      if(*c < 0) { p2 = PA + ~*c; x |= 2; }
      else       { p2 = PA +  *c; }
    } else {
      if(x & 1) { do { *a-- = *b, *b-- = *a; } while(*b < 0); x ^= 1; }
      *a-- = ~*b;
      if(b <= buf) { *buf = t; break; }
      *b-- = *a;
      if(x & 2) { do { *a-- = *c, *c-- = *a; } while(*c < 0); x ^= 2; }
      *a-- = *c, *c-- = *a;
      if(c < first) {
        while(buf < b) *a-- = *b, *b-- = *a;
        *a = *b, *b = t;
        break;
      }
      if(*b < 0) { p1 = PA + ~*b; x |= 1; }
      else       { p1 = PA +  *b; }
      if(*c < 0) { p2 = PA + ~*c; x |= 2; }
      else       { p2 = PA +  *c; }
    }
  }
}

//D&C based merge
inline auto ss_swapmerge(
  const uint8_t* T, const int* PA, int* first, int* middle, int* last, int* buf, int bufsize, int depth
) -> void {
  #define STACK_SIZE SS_SMERGE_STACKSIZE
  #define GETIDX(a) ((0 <= (a)) ? (a) : (~(a)))
  #define MERGE_CHECK(a, b, c) \
    do { \
      if(((c) & 1) || (((c) & 2) && (ss_compare(T, PA + GETIDX(*((a) - 1)), PA + *(a), depth) == 0))) { \
        *(a) = ~*(a); \
      } \
      if(((c) & 4) && ((ss_compare(T, PA + GETIDX(*((b) - 1)), PA + *(b), depth) == 0))) { \
        *(b) = ~*(b); \
      } \
    } while(0)
  struct { int *a, *b, *c, d; } stack[STACK_SIZE];
  int *l, *r, *lm, *rm;
  int m, len, half, ssize, check, next;

  for(check = 0, ssize = 0;;) {
    if((last - middle) <= bufsize) {
      if((first < middle) && (middle < last)) {
        ss_mergebackward(T, PA, first, middle, last, buf, depth);
      }
      MERGE_CHECK(first, last, check);
      STACK_POP(first, middle, last, check);
      continue;
    }

    if((middle - first) <= bufsize) {
      if(first < middle) {
        ss_mergeforward(T, PA, first, middle, last, buf, depth);
      }
      MERGE_CHECK(first, last, check);
      STACK_POP(first, middle, last, check);
      continue;
    }

    for(m = 0, len = min(middle - first, last - middle), half = len >> 1; 0 < len; len = half, half >>= 1) {
      if(ss_compare(T, PA + GETIDX(*(middle + m + half)),
                       PA + GETIDX(*(middle - m - half - 1)), depth) < 0
      ) {
        m += half + 1;
        half -= (len & 1) ^ 1;
      }
    }

    if(0 < m) {
      lm = middle - m, rm = middle + m;
      ss_blockswap(lm, middle, m);
      l = r = middle, next = 0;
      if(rm < last) {
        if(*rm < 0) {
          *rm = ~*rm;
          if(first < lm) { for(; *--l < 0;); next |= 4; }
          next |= 1;
        } else if(first < lm) {
          for(; *r < 0; ++r);
          next |= 2;
        }
      }

      if((l - first) <= (last - r)) {
        STACK_PUSH(r, rm, last, (next & 3) | (check & 4));
        middle = lm, last = l, check = (check & 3) | (next & 4);
      } else {
        if((next & 2) && (r == middle)) next ^= 6;
        STACK_PUSH(first, lm, l, (check & 3) | (next & 4));
        first = r, middle = rm, check = (next & 3) | (check & 4);
      }
    } else {
      if(ss_compare(T, PA + GETIDX(*(middle - 1)), PA + *middle, depth) == 0) {
        *middle = ~*middle;
      }
      MERGE_CHECK(first, last, check);
      STACK_POP(first, middle, last, check);
    }
  }

  #undef STACK_SIZE
  #undef GETIDX
  #undef MERGE_CHECK
}

#endif

//substring sort
inline auto sssort(
  const uint8_t* T, const int* PA, int* first, int* last, int* buf, int bufsize, int depth, int n, int lastsuffix
) -> void {
  int* a;
  #if SS_BLOCKSIZE != 0
  int *b, *middle, *curbuf;
  int j, k, curbufsize, limit;
  #endif
  int i;

  if(lastsuffix != 0) ++first;

  #if SS_BLOCKSIZE == 0
  ss_mintrosort(T, PA, first, last, depth);
  #else
  if((bufsize < SS_BLOCKSIZE) &&
     (bufsize < (last - first)) &&
     (bufsize < (limit = ss_isqrt(last - first)))
  ) {
    if(SS_BLOCKSIZE < limit) limit = SS_BLOCKSIZE;
    buf = middle = last - limit, bufsize = limit;
  } else {
    middle = last, limit = 0;
  }
  for(a = first, i = 0; SS_BLOCKSIZE < (middle - a); a += SS_BLOCKSIZE, ++i) {
    #if SS_INSERTIONSORT_THRESHOLD < SS_BLOCKSIZE
    ss_mintrosort(T, PA, a, a + SS_BLOCKSIZE, depth);
    #elif 1 < SS_BLOCKSIZE
    ss_insertionsort(T, PA, a, a + SS_BLOCKSIZE, depth);
    #endif
    curbufsize = last - (a + SS_BLOCKSIZE);
    curbuf = a + SS_BLOCKSIZE;
    if(curbufsize <= bufsize) curbufsize = bufsize, curbuf = buf;
    for(b = a, k = SS_BLOCKSIZE, j = i; j & 1; b -= k, k <<= 1, j >>= 1) {
      ss_swapmerge(T, PA, b - k, b, b + k, curbuf, curbufsize, depth);
    }
  }
  #if SS_INSERTIONSORT_THRESHOLD < SS_BLOCKSIZE
  ss_mintrosort(T, PA, a, middle, depth);
  #elif 1 < SS_BLOCKSIZE
  ss_insertionsort(T, PA, a, middle, depth);
  #endif
  for(k = SS_BLOCKSIZE; i != 0; k <<= 1, i >>= 1) {
    if(i & 1) {
      ss_swapmerge(T, PA, a - k, a, middle, buf, bufsize, depth);
      a -= k;
    }
  }
  if(limit != 0) {
    #if SS_INSERTIONSORT_THRESHOLD < SS_BLOCKSIZE
    ss_mintrosort(T, PA, middle, last, depth);
    #elif 1 < SS_BLOCKSIZE
    ss_insertionsort(T, PA, middle, last, depth);
    #endif
    ss_inplacemerge(T, PA, first, middle, last, depth);
  }
  #endif

  if(lastsuffix != 0) {
    //insert last type B* suffix
    int PAi[2];
    PAi[0] = PA[*(first - 1)];
    PAi[1] = n - 2;
    for(a = first, i = *(first - 1); (a < last) && ((*a < 0) || (0 < ss_compare(T, &(PAi[0]), PA + *a, depth))); ++a) {
      *(a - 1) = *a;
    }
    *(a - 1) = i;
  }
}

// [[trsort.c]]

inline auto tr_ilg(int n) -> int {
  return ilg32(n);
}

//simple insertionsort for small size groups
inline auto tr_insertionsort(const int* ISAd, int* first, int* last) -> void {
  int *a, *b;
  int t, r;

  for(a = first + 1; a < last; ++a) {
    for(t = *a, b = a - 1; 0 > (r = ISAd[t] - ISAd[*b]);) {
      do { *(b + 1) = *b; } while((first <= --b) && (*b < 0));
      if(b < first) { break; }
    }
    if(r == 0) { *b = ~*b; }
    *(b + 1) = t;
  }
}

inline auto tr_fixdown(const int* ISAd, int* SA, int i, int size) -> void {
  int j, k, v, c, d, e;

  for(v = SA[i], c = ISAd[v]; (j = 2 * i + 1) < size; SA[i] = SA[k], i = k) {
    d = ISAd[SA[k = j++]];
    if(d < (e = ISAd[SA[j]])) { k = j; d = e; }
    if(d <= c) { break; }
  }
  SA[i] = v;
}

//simple top-down heapsort
inline auto tr_heapsort(const int* ISAd, int* SA, int size) -> void {
  int i, m, t;

  m = size;
  if((size % 2) == 0) {
    m--;
    if(ISAd[SA[m / 2]] < ISAd[SA[m]]) { swap(SA[m], SA[m / 2]); }
  }

  for(i = m / 2 - 1; 0 <= i; --i) { tr_fixdown(ISAd, SA, i, m); }
  if((size % 2) == 0) { swap(SA[0], SA[m]); tr_fixdown(ISAd, SA, 0, m); }
  for(i = m - 1; 0 < i; --i) {
    t = SA[0], SA[0] = SA[i];
    tr_fixdown(ISAd, SA, 0, i);
    SA[i] = t;
  }
}

//returns the median of three elements
inline auto tr_median3(const int* ISAd, int* v1, int* v2, int* v3) -> int* {
  if(ISAd[*v1] > ISAd[*v2]) swap(v1, v2);
  if(ISAd[*v2] > ISAd[*v3]) return ISAd[*v1] > ISAd[*v3] ? v1 : v3;
  return v2;
}

//returns the median of five elements
inline auto tr_median5(const int* ISAd, int* v1, int* v2, int* v3, int* v4, int* v5) -> int* {
  if(ISAd[*v2] > ISAd[*v3]) swap(v2, v3);
  if(ISAd[*v4] > ISAd[*v5]) swap(v4, v5);
  if(ISAd[*v2] > ISAd[*v4]) swap(v2, v4), swap(v3, v5);
  if(ISAd[*v1] > ISAd[*v3]) swap(v1, v3);
  if(ISAd[*v1] > ISAd[*v4]) swap(v1, v4), swap(v3, v5);
  if(ISAd[*v3] > ISAd[*v4]) return v4;
  return v3;
}

//returns the pivot element
inline auto tr_pivot(const int* ISAd, int* first, int* last) -> int* {
  int* middle;
  int t;

  t = last - first;
  middle = first + t / 2;

  if(t <= 512) {
    if(t <= 32) {
      return tr_median3(ISAd, first, middle, last - 1);
    } else {
      t >>= 2;
      return tr_median5(ISAd, first, first + t, middle, last - 1 - t, last - 1);
    }
  }
  t >>= 3;
  first  = tr_median3(ISAd, first, first + t, first + (t << 1));
  middle = tr_median3(ISAd, middle - t, middle, middle + t);
  last   = tr_median3(ISAd, last - 1 - (t << 1), last - 1 - t, last - 1);
  return   tr_median3(ISAd, first, middle, last);
}

struct trbudget_t {
  int chance;
  int remain;
  int incval;
  int count;
};

inline auto trbudget_init(trbudget_t* budget, int chance, int incval) -> void {
  budget->chance = chance;
  budget->remain = budget->incval = incval;
}

inline auto trbudget_check(trbudget_t* budget, int size) -> int {
  if(size <= budget->remain) { budget->remain -= size; return 1; }
  if(budget->chance == 0) { budget->count += size; return 0; }
  budget->remain += budget->incval - size;
  budget->chance -= 1;
  return 1;
}

inline auto tr_partition(const int* ISAd, int* first, int* middle, int* last, int** pa, int** pb, int v) -> void {
  int *a, *b, *c, *d, *e, *f;
  int t, s;
  int x = 0;

  for(b = middle - 1; (++b < last) && ((x = ISAd[*b]) == v););
  if(((a = b) < last) && (x < v)) {
    for(; (++b < last) && ((x = ISAd[*b]) <= v);) {
      if(x == v) { swap(*b, *a); ++a; }
    }
  }
  for(c = last; (b < --c) && ((x = ISAd[*c]) == v););
  if((b < (d = c)) && (x > v)) {
    for(; (b < --c) && ((x = ISAd[*c]) >= v);) {
      if(x == v) { swap(*c, *d); --d; }
    }
  }
  for(; b < c;) {
    swap(*b, *c);
    for(; (++b < c) && ((x = ISAd[*b]) <= v);) {
      if(x == v) { swap(*b, *a); ++a; }
    }
    for(; (b < --c) && ((x = ISAd[*c]) >= v);) {
      if(x == v) { swap(*c, *d); --d; }
    }
  }

  if(a <= d) {
    c = b - 1;
    if((s = a - first) > (t = b - a)) { s = t; }
    for(e = first, f = b - s; 0 < s; --s, ++e, ++f) { swap(*e, *f); }
    if((s = d - c) > (t = last - d - 1)) { s = t; }
    for(e = b, f = last - s; 0 < s; --s, ++e, ++f) { swap(*e, *f); }
    first += (b - a), last -= (d - c);
  }
  *pa = first, *pb = last;
}

//sort suffixes of middle partition by using sorted order of suffixes of left and right partition
inline auto tr_copy(int* ISA, int* SA, int* first, int* a, int* b, int* last, int depth) -> void {
  int *c, *d, *e;
  int s, v;

  v = b - SA - 1;
  for(c = first, d = a - 1; c <= d; ++c) {
    if((0 <= (s = *c - depth)) && (ISA[s] == v)) {
      *++d = s;
      ISA[s] = d - SA;
    }
  }
  for(c = last - 1, e = d + 1, d = b; e < d; --c) {
    if((0 <= (s = *c - depth)) && (ISA[s] == v)) {
      *--d = s;
      ISA[s] = d - SA;
    }
  }
}

inline auto tr_partialcopy(int* ISA, int* SA, int* first, int* a, int* b, int* last, int depth) -> void {
  int *c, *d, *e;
  int s, v;
  int rank, lastrank, newrank = -1;

  v = b - SA - 1;
  lastrank = -1;
  for(c = first, d = a - 1; c <= d; ++c) {
    if((0 <= (s = *c - depth)) && (ISA[s] == v)) {
      *++d = s;
      rank = ISA[s + depth];
      if(lastrank != rank) { lastrank = rank; newrank = d - SA; }
      ISA[s] = newrank;
    }
  }

  lastrank = -1;
  for(e = d; first <= e; --e) {
    rank = ISA[*e];
    if(lastrank != rank) { lastrank = rank; newrank = e - SA; }
    if(newrank != rank) { ISA[*e] = newrank; }
  }

  lastrank = -1;
  for(c = last - 1, e = d + 1, d = b; e < d; --c) {
    if((0 <= (s = *c - depth)) && (ISA[s] == v)) {
      *--d = s;
      rank = ISA[s + depth];
      if(lastrank != rank) { lastrank = rank; newrank = d - SA; }
      ISA[s] = newrank;
    }
  }
}

inline auto tr_introsort(int* ISA, const int* ISAd, int* SA, int* first, int* last, trbudget_t* budget) -> void {
  struct { const int *a; int *b, *c; int d, e; } stack[TR_STACKSIZE];
  int *a, *b, *c;
  int t;
  int v, x = 0;
  int incr = ISAd - ISA;
  int limit, next;
  int ssize, trlink = -1;

  for(ssize = 0, limit = tr_ilg(last - first);;) {
    if(limit < 0) {
      if(limit == -1) {
        //tandem repeat partition
        tr_partition(ISAd - incr, first, first, last, &a, &b, last - SA - 1);

        //update ranks
        if(a < last) {
          for(c = first, v = a - SA - 1; c < a; ++c) { ISA[*c] = v; }
        }
        if(b < last) {
          for(c = a, v = b - SA - 1; c < b; ++c) { ISA[*c] = v; }
        }

        //push
        if(1 < (b - a)) {
          STACK_PUSH5(nullptr, a, b, 0, 0);
          STACK_PUSH5(ISAd - incr, first, last, -2, trlink);
          trlink = ssize - 2;
        }
        if((a - first) <= (last - b)) {
          if(1 < (a - first)) {
            STACK_PUSH5(ISAd, b, last, tr_ilg(last - b), trlink);
            last = a, limit = tr_ilg(a - first);
          } else if(1 < (last - b)) {
            first = b, limit = tr_ilg(last - b);
          } else {
            STACK_POP5(ISAd, first, last, limit, trlink);
          }
        } else {
          if(1 < (last - b)) {
            STACK_PUSH5(ISAd, first, a, tr_ilg(a - first), trlink);
            first = b, limit = tr_ilg(last - b);
          } else if(1 < (a - first)) {
            last = a, limit = tr_ilg(a - first);
          } else {
            STACK_POP5(ISAd, first, last, limit, trlink);
          }
        }
      } else if(limit == -2) {
        //tandem repeat copy
        a = stack[--ssize].b, b = stack[ssize].c;
        if(stack[ssize].d == 0) {
          tr_copy(ISA, SA, first, a, b, last, ISAd - ISA);
        } else {
          if(0 <= trlink) { stack[trlink].d = -1; }
          tr_partialcopy(ISA, SA, first, a, b, last, ISAd - ISA);
        }
        STACK_POP5(ISAd, first, last, limit, trlink);
      } else {
        //sorted partition
        if(0 <= *first) {
          a = first;
          do { ISA[*a] = a - SA; } while((++a < last) && (0 <= *a));
          first = a;
        }
        if(first < last) {
          a = first; do { *a = ~*a; } while(*++a < 0);
          next = (ISA[*a] != ISAd[*a]) ? tr_ilg(a - first + 1) : -1;
          if(++a < last) { for(b = first, v = a - SA - 1; b < a; ++b) { ISA[*b] = v; } }

          //push
          if(trbudget_check(budget, a - first)) {
            if((a - first) <= (last - a)) {
              STACK_PUSH5(ISAd, a, last, -3, trlink);
              ISAd += incr, last = a, limit = next;
            } else {
              if(1 < (last - a)) {
                STACK_PUSH5(ISAd + incr, first, a, next, trlink);
                first = a, limit = -3;
              } else {
                ISAd += incr, last = a, limit = next;
              }
            }
          } else {
            if(0 <= trlink) { stack[trlink].d = -1; }
            if(1 < (last - a)) {
              first = a, limit = -3;
            } else {
              STACK_POP5(ISAd, first, last, limit, trlink);
            }
          }
        } else {
          STACK_POP5(ISAd, first, last, limit, trlink);
        }
      }
      continue;
    }

    if((last - first) <= TR_INSERTIONSORT_THRESHOLD) {
      tr_insertionsort(ISAd, first, last);
      limit = -3;
      continue;
    }

    if(limit-- == 0) {
      tr_heapsort(ISAd, first, last - first);
      for(a = last - 1; first < a; a = b) {
        for(x = ISAd[*a], b = a - 1; (first <= b) && (ISAd[*b] == x); --b) { *b = ~*b; }
      }
      limit = -3;
      continue;
    }

    //choose pivot
    a = tr_pivot(ISAd, first, last);
    swap(*first, *a);
    v = ISAd[*first];

    //partition
    tr_partition(ISAd, first, first + 1, last, &a, &b, v);
    if((last - first) != (b - a)) {
      next = (ISA[*a] != v) ? tr_ilg(b - a) : -1;

      //update ranks
      for(c = first, v = a - SA - 1; c < a; ++c) { ISA[*c] = v; }
      if(b < last) { for(c = a, v = b - SA - 1; c < b; ++c) { ISA[*c] = v; } }

      //push
      if((1 < (b - a)) && (trbudget_check(budget, b - a))) {
        if((a - first) <= (last - b)) {
          if((last - b) <= (b - a)) {
            if(1 < (a - first)) {
              STACK_PUSH5(ISAd + incr, a, b, next, trlink);
              STACK_PUSH5(ISAd, b, last, limit, trlink);
              last = a;
            } else if(1 < (last - b)) {
              STACK_PUSH5(ISAd + incr, a, b, next, trlink);
              first = b;
            } else {
              ISAd += incr, first = a, last = b, limit = next;
            }
          } else if((a - first) <= (b - a)) {
            if(1 < (a - first)) {
              STACK_PUSH5(ISAd, b, last, limit, trlink);
              STACK_PUSH5(ISAd + incr, a, b, next, trlink);
              last = a;
            } else {
              STACK_PUSH5(ISAd, b, last, limit, trlink);
              ISAd += incr, first = a, last = b, limit = next;
            }
          } else {
            STACK_PUSH5(ISAd, b, last, limit, trlink);
            STACK_PUSH5(ISAd, first, a, limit, trlink);
            ISAd += incr, first = a, last = b, limit = next;
          }
        } else {
          if((a - first) <= (b - a)) {
            if(1 < (last - b)) {
              STACK_PUSH5(ISAd + incr, a, b, next, trlink);
              STACK_PUSH5(ISAd, first, a, limit, trlink);
              first = b;
            } else if(1 < (a - first)) {
              STACK_PUSH5(ISAd + incr, a, b, next, trlink);
              last = a;
            } else {
              ISAd += incr, first = a, last = b, limit = next;
            }
          } else if((last - b) <= (b - a)) {
            if(1 < (last - b)) {
              STACK_PUSH5(ISAd, first, a, limit, trlink);
              STACK_PUSH5(ISAd + incr, a, b, next, trlink);
              first = b;
            } else {
              STACK_PUSH5(ISAd, first, a, limit, trlink);
              ISAd += incr, first = a, last = b, limit = next;
            }
          } else {
            STACK_PUSH5(ISAd, first, a, limit, trlink);
            STACK_PUSH5(ISAd, b, last, limit, trlink);
            ISAd += incr, first = a, last = b, limit = next;
          }
        }
      } else {
        if((1 < (b - a)) && (0 <= trlink)) { stack[trlink].d = -1; }
        if((a - first) <= (last - b)) {
          if(1 < (a - first)) {
            STACK_PUSH5(ISAd, b, last, limit, trlink);
            last = a;
          } else if(1 < (last - b)) {
            first = b;
          } else {
            STACK_POP5(ISAd, first, last, limit, trlink);
          }
        } else {
          if(1 < (last - b)) {
            STACK_PUSH5(ISAd, first, a, limit, trlink);
            first = b;
          } else if(1 < (a - first)) {
            last = a;
          } else {
            STACK_POP5(ISAd, first, last, limit, trlink);
          }
        }
      }
    } else {
      if(trbudget_check(budget, last - first)) {
        limit = tr_ilg(last - first), ISAd += incr;
      } else {
        if(0 <= trlink) { stack[trlink].d = -1; }
        STACK_POP5(ISAd, first, last, limit, trlink);
      }
    }
  }
}

//tandem repeat sort
inline auto trsort(int* ISA, int* SA, int n, int depth) -> void {
  int *ISAd;
  int *first, *last;
  trbudget_t budget;
  int t, skip, unsorted;

  trbudget_init(&budget, tr_ilg(n) * 2 / 3, n);
  for(ISAd = ISA + depth; -n < *SA; ISAd += ISAd - ISA) {
    first = SA;
    skip = 0;
    unsorted = 0;
    do {
      if((t = *first) < 0) { first -= t; skip += t; }
      else {
        if(skip != 0) { *(first + skip) = skip; skip = 0; }
        last = SA + ISA[t] + 1;
        if(1 < (last - first)) {
          budget.count = 0;
          tr_introsort(ISA, ISAd, SA, first, last, &budget);
          if(budget.count != 0) { unsorted += budget.count; }
          else { skip = first - last; }
        } else if((last - first) == 1) {
          skip = -1;
        }
        first = last;
      }
    } while(first < (SA + n));
    if(skip != 0) { *(first + skip) = skip; }
    if(unsorted == 0) break;
  }
}

// [[divsufsort.c]]

inline auto sort_typeBstar(const uint8_t* T, int* SA, int* bucket_A, int* bucket_B, int n) -> int {
  int *PAb, *ISAb, *buf;
  int i, j, k, t, m, bufsize;
  int c0, c1;

  //initialize bucket arrays
  for(i = 0; i < BUCKET_A_SIZE; ++i) { bucket_A[i] = 0; }
  for(i = 0; i < BUCKET_B_SIZE; ++i) { bucket_B[i] = 0; }

  //count the number of occurrences of the first one or two characters of each type A, B, and B* suffix
  //moreover, store the beginning position of all type b* suffixes into the array SA
  for(i = n - 1, m = n, c0 = T[n - 1]; 0 <= i;) {
    //type A suffix
    do { ++BUCKET_A(c1 = c0); } while((0 <= --i) && ((c0 = T[i]) >= c1));
    if(0 <= i) {
      //type B* suffix
      ++BUCKET_BSTAR(c0, c1);
      SA[--m] = i;
      //type B suffix
      for(--i, c1 = c0; (0 <= i) && ((c0 = T[i]) <= c1); --i, c1 = c0) {
        ++BUCKET_B(c0, c1);
      }
    }
  }
  m = n - m;

  //note: a type B* suffix is lexicographically smaller than a type B suffix that begins with the same first two characters

  //calculate the index of start/end point of each bucket
  for(c0 = 0, i = 0, j = 0; c0 < ALPHABET_SIZE; ++c0) {
    t = i + BUCKET_A(c0);
    BUCKET_A(c0) = i + j;  //start point
    i = t + BUCKET_B(c0, c0);
    for(c1 = c0 + 1; c1 < ALPHABET_SIZE; ++c1) {
      j += BUCKET_BSTAR(c0, c1);
      BUCKET_BSTAR(c0, c1) = j;  //end point
      i += BUCKET_B(c0, c1);
    }
  }

  if(0 < m) {
    //sort the type B* suffixes by their first two characters
    PAb = SA + n - m; ISAb = SA + m;
    for(int i = m - 2; 0 <= i; --i) {
      t = PAb[i], c0 = T[t], c1 = T[t + 1];
      SA[--BUCKET_BSTAR(c0, c1)] = i;
    }
    t = PAb[m - 1], c0 = T[t], c1 = T[t + 1];
    SA[--BUCKET_BSTAR(c0, c1)] = m - 1;

    //sort the type B* substrings using sssort
    buf = SA + m, bufsize = n - (2 * m);
    for(c0 = ALPHABET_SIZE - 2, j = m; 0 < j; --c0) {
      for(c1 = ALPHABET_SIZE - 1; c0 < c1; j = i, --c1) {
        i = BUCKET_BSTAR(c0, c1);
        if(1 < (j - i)) {
          sssort(T, PAb, SA + i, SA + j, buf, bufsize, 2, n, *(SA + i) == (m - 1));
        }
      }
    }

    //compare ranks of type B* substrings
    for(i = m - 1; 0 <= i; --i) {
      if(0 <= SA[i]) {
        j = i;
        do { ISAb[SA[i]] = i; } while((0 <= --i) && (0 <= SA[i]));
        SA[i + 1] = i - j;
        if(i <= 0) break;
      }
      j = i;
      do { ISAb[SA[i] = ~SA[i]] = j; } while(SA[--i] < 0);
      ISAb[SA[i]] = j;
    }

    //construct the inverse suffix array of type B* suffixes using trsort
    trsort(ISAb, SA, m, 1);

    //set the sorted order of type B* suffixes
    for(i = n - 1, j = m, c0 = T[n - 1]; 0 <= i;) {
      for(--i, c1 = c0; (0 <= i) && ((c0 = T[i]) >= c1); --i, c1 = c0);
      if(0 <= i) {
        t = i;
        for(--i, c1 = c0; (0 <= i) && ((c0 = T[i]) <= c1); --i, c1 = c0);
        SA[ISAb[--j]] = ((t == 0) || (1 < (t - i))) ? t : ~t;
      }
    }

    //calculate the index of start/end point of each bucket
    BUCKET_B(ALPHABET_SIZE - 1, ALPHABET_SIZE - 1) = n;  //end point
    for(c0 = ALPHABET_SIZE - 2, k = m - 1; 0 <= c0; --c0) {
      i = BUCKET_A(c0 + 1) - 1;
      for(c1 = ALPHABET_SIZE - 1; c0 < c1; --c1) {
        t = i - BUCKET_B(c0, c1);
        BUCKET_B(c0, c1) = i;  //end point

        //move all type B* suffixes to the correct position
        for(i = t, j = BUCKET_BSTAR(c0, c1); j <= k; --i, --k) SA[i] = SA[k];
      }
      BUCKET_BSTAR(c0, c0 + 1) = i - BUCKET_B(c0, c0) + 1;  //start point
      BUCKET_B(c0, c0) = i;  //end point
    }
  }

  return m;
}

inline auto construct_SA(const uint8_t* T, int* SA, int* bucket_A, int* bucket_B, int n, int m) -> void {
  int *i, *j, *k;
  int s;
  int c0, c1, c2;

  if(0 < m) {
    //construct the sorted order of type B suffixes by using the sorted order of type B* suffixes
    for(c1 = ALPHABET_SIZE - 2; 0 <= c1; --c1) {
      //scan the suffix array from right to left
      for(i = SA + BUCKET_BSTAR(c1, c1 + 1),
          j = SA + BUCKET_A(c1 + 1) - 1, k = nullptr, c2 = -1;
          i <= j;
          --j) {
        if(0 < (s = *j)) {
          *j = ~s;
          c0 = T[--s];
          if((0 < s) && (T[s - 1] > c0)) { s = ~s; }
          if(c0 != c2) {
            if(0 <= c2) { BUCKET_B(c2, c1) = k - SA; }
            k = SA + BUCKET_B(c2 = c0, c1);
          }
          *k-- = s;
        } else {
          *j = ~s;
        }
      }
    }
  }

  //construct the suffix array by using the sorted order of type B suffixes
  k = SA + BUCKET_A(c2 = T[n - 1]);
  *k++ = (T[n - 2] < c2) ? ~(n - 1) : (n - 1);
  //scan the suffix array from left to right
  for(i = SA, j = SA + n; i < j; ++i) {
    if(0 < (s = *i)) {
      c0 = T[--s];
      if((s == 0) || (T[s - 1] < c0)) { s = ~s; }
      if(c0 != c2) {
        BUCKET_A(c2) = k - SA;
        k = SA + BUCKET_A(c2 = c0);
      }
      *k++ = s;
    } else {
      *i = ~s;
    }
  }
}

//constructs the burrows-wheeler transformed string directly by using the sorted order of type B* suffixes
inline auto construct_BWT(const uint8_t* T, int* SA, int* bucket_A, int* bucket_B, int n, int m) -> int {
  int *i, *j, *k, *orig, s, c0, c1, c2;

  if(0 < m) {
    //construct the sorted order of type B suffixes by using the sorted order of type B* suffixes
    for(c1 = ALPHABET_SIZE - 2; 0 <= c1; --c1) {
      //scan the suffix array from right to left
      for(i = SA + BUCKET_BSTAR(c1, c1 + 1), j = SA + BUCKET_A(c1 + 1) - 1, k = nullptr, c2 = -1; i <= j; --j) {
        if(0 < (s = *j)) {
          c0 = T[--s];
          *j = ~((int)c0);
          if((0 < s) && (T[s - 1] > c0)) s = ~s;
          if(c0 != c2) {
            if(0 <= c2) BUCKET_B(c2, c1) = k - SA;
            k = SA + BUCKET_B(c2 = c0, c1);
          }
          *k-- = s;
        } else if(s != 0) {
          *j = ~s;
        }
      }
    }
  }

  //construct the BWTed string by using the sorted order of type B suffixes
  k = SA + BUCKET_A(c2 = T[n - 1]);
  *k++ = (T[n - 2] < c2) ? ~((int)T[n - 2]) : (n - 1);
  //scan the suffix array from left to right
  for(i = SA, j = SA + n, orig = SA; i < j; ++i) {
    if(0 < (s = *i)) {
      c0 = T[--s];
      *i = c0;
      if((0 < s) && (T[s - 1] < c0)) s = ~((int)T[s - 1]);
      if(c0 != c2) {
        BUCKET_A(c2) = k - SA;
        k = SA + BUCKET_A(c2 = c0);
      }
      *k++ = s;
    } else if(s != 0) {
      *i = ~s;
    } else {
      orig = i;
    }
  }

  return orig - SA;
}

}}

namespace nall {

inline auto div_suf_sort(int* SA, const uint8_t* T, int n) -> int {
  int m, err = 0;

  if(T == nullptr || SA == nullptr || n < 0) return -1;
  if(n == 0) return 0;
  if(n == 1) { SA[0] = 0; return 0; }
  if(n == 2) { m = T[0] < T[1]; SA[m ^ 1] = 0; SA[m] = 1; return 0; }

  auto bucket_A = memory::allocate<int>(BUCKET_A_SIZE);
  auto bucket_B = memory::allocate<int>(BUCKET_B_SIZE);

  if(bucket_A && bucket_B) {
    m = libdivsufsort::sort_typeBstar(T, SA, bucket_A, bucket_B, n);
    libdivsufsort::construct_SA(T, SA, bucket_A, bucket_B, n, m);
  } else {
    err = -2;
  }

  memory::free(bucket_A);
  memory::free(bucket_B);

  return err;
}

//byuu: note that this function is broken, and not just in my port of it to nall
//even with the original library, it is incapable of producing a correct BWT result in *any* case

inline auto div_suf_sort_bwt(const uint8_t* T, uint8_t* U, int* A, int n) -> int {
  int *B, *bucket_A, *bucket_B, m, pidx, i;

  if(T == nullptr || U == nullptr || n < 0) return -1;
  if(n == 0) return 0;
  if(n == 1) return U[0] = T[0], 1;

  if((B = A) == nullptr) B = memory::allocate<int>(n + 1);
  bucket_A = memory::allocate<int>(BUCKET_A_SIZE);
  bucket_B = memory::allocate<int>(BUCKET_B_SIZE);

  //burrows-wheeler transform
  if((B != nullptr) && (bucket_A != nullptr) && (bucket_B != nullptr)) {
    m = libdivsufsort::sort_typeBstar(T, B, bucket_A, bucket_B, n);
    pidx = libdivsufsort::construct_BWT(T, B, bucket_A, bucket_B, n, m);

    //copy to output string
    U[0] = T[n - 1];
    for(i = 0; i < pidx; ++i) U[i + 1] = (uint8_t)B[i];
    for(i += 1; i < n; ++i) U[i] = (uint8_t)B[i];
    pidx += 1;
  } else {
    pidx -= 2;
  }

  memory::free(bucket_A);
  memory::free(bucket_B);
  if(A == nullptr) memory::free(B);

  return pidx;
}

#undef ALPHABET_SIZE
#undef BUCKET_A
#undef BUCKET_B
#undef BUCKET_BSTAR
#undef BUCKET_A_SIZE
#undef BUCKET_B_SIZE
#undef SS_INSERTIONSORT_THRESHOLD
#undef SS_BLOCKSIZE
#undef SS_MISORT_STACKSIZE
#undef SS_SMERGE_STACKSIZE
#undef TR_INSERTIONSORT_THRESHOLD
#undef TR_STACKSIZE
#undef STACK_PUSH
#undef STACK_PUSH5
#undef STACK_POP
#undef STACK_POP5

}
