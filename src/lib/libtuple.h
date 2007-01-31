/*
  libtuple : version 0.01 ~byuu (2007-01-04)

  dependencies:
  - libbase
*/

#ifndef __LIBTUPLE
#define __LIBTUPLE

/*****
 * tuple definitions
 *****/

template<
  typename = null_t, typename = null_t, typename = null_t, typename = null_t,
  typename = null_t, typename = null_t, typename = null_t, typename = null_t
> struct tuple;
template<int, typename> struct tuple_traits;

/*****
 * tuple_traits
 *****/

#define make_tuple_traits(n) \
template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7> \
struct tuple_traits< n, tuple<T0, T1, T2, T3, T4, T5, T6, T7> > { \
  typedef T##n type; \
  static inline T##n &get(tuple<T0, T1, T2, T3, T4, T5, T6, T7> &t) { return t.t##n; } \
};

make_tuple_traits(0)
make_tuple_traits(1)
make_tuple_traits(2)
make_tuple_traits(3)
make_tuple_traits(4)
make_tuple_traits(5)
make_tuple_traits(6)
make_tuple_traits(7)

#undef make_tuple_traits

/*****
 * tuple
 *****/

template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
struct tuple {
T0 t0;
T1 t1;
T2 t2;
T3 t3;
T4 t4;
T5 t5;
T6 t6;
T7 t7;
  template<int index>
  typename tuple_traits<index, tuple>::type &get() {
    return tuple_traits<index, tuple>::get(*this);
  }
  bool operator==(const tuple &source) {
    return (t0 == source.t0 && t1 == source.t1 && t2 == source.t2 && t3 == source.t3 &&
            t4 == source.t4 && t5 == source.t5 && t6 == source.t6 && t7 == source.t7);
  }
  bool operator!=(const tuple &source) {
    return (t0 != source.t0 && t1 != source.t1 && t2 != source.t2 && t3 != source.t3 &&
            t4 != source.t4 && t5 != source.t5 && t6 != source.t6 && t7 != source.t7);
  }
  tuple(const tuple &t) { operator=(t); }
  tuple(const T0 &_t0 = T0(), const T1 &_t1 = T1(), const T2 &_t2 = T2(), const T3 &_t3 = T3(),
        const T4 &_t4 = T4(), const T5 &_t5 = T5(), const T6 &_t6 = T6(), const T7 &_t7 = T7()) :
        t0(_t0), t1(_t1), t2(_t2), t3(_t3), t4(_t4), t5(_t5), t6(_t6), t7(_t7) {}
};

template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
tuple<T0, T1, T2, T3, T4, T5, T6, T7> make_tuple(
  const T0 &t0 = T0(), const T1 &t1 = T1(), const T2 &t2 = T2(), const T3 &t3 = T3(),
  const T4 &t4 = T4(), const T5 &t5 = T5(), const T6 &t6 = T6(), const T7 &t7 = T7()) {
  return tuple<T0, T1, T2, T3, T4, T5, T6, T7>(t0, t1, t2, t3, t4, t5, t6, t7);
}

#endif
