/*
  libfunction : version 0.06 ~byuu (2007-10-14)
  license: public domain
*/

#ifndef LIBFUNCTION_H
#define LIBFUNCTION_H

//prologue

#define TN typename

template<typename T> class function;

//parameters = 0

#define cat(n)  n
#define TL typename R
#define PL
#define CL

#include "libfunction.h"

//parameters = 1

#define cat(n)  , n
#define TL TN R, TN P1
#define PL P1 p1
#define CL p1

#include "libfunction.h"

//parameters = 2

#define cat(n)  , n
#define TL TN R, TN P1, TN P2
#define PL P1 p1, P2 p2
#define CL p1, p2

#include "libfunction.h"

//parameters = 3

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3
#define PL P1 p1, P2 p2, P3 p3
#define CL p1, p2, p3

#include "libfunction.h"

//parameters = 4

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4
#define PL P1 p1, P2 p2, P3 p3, P4 p4
#define CL p1, p2, p3, p4

#include "libfunction.h"

//parameters = 5

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4, TN P5
#define PL P1 p1, P2 p2, P3 p3, P4 p4, P5 p5
#define CL p1, p2, p3, p4, p5

#include "libfunction.h"

//parameters = 6

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4, TN P5, TN P6
#define PL P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6
#define CL p1, p2, p3, p4, p5, p6

#include "libfunction.h"

//parameters = 7

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4, TN P5, TN P6, TN P7
#define PL P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7
#define CL p1, p2, p3, p4, p5, p6, p7

#include "libfunction.h"

//parameters = 8

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4, TN P5, TN P6, TN P7, TN P8
#define PL P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8
#define CL p1, p2, p3, p4, p5, p6, p7, p8

#include "libfunction.h"

//epilogue

#undef TN

#else //defined(LIBFUNCTION_H)

//function implementation template class

template<TL>
class function<R (PL)> {
private:
  struct base1 { virtual void func1(PL) {} };
  struct base2 { virtual void func2(PL) {} };
  struct derived : base1, virtual base2 {};

  struct data_t {
    R (*fn_call)(const data_t& cat(PL));
    union {
      R (*fn_global)(PL);
      struct {
        R (derived::*fn_member)(PL);
        void *object;
      };
    };
  } data;

  static R fn_call_global(const data_t &d cat(PL)) {
    return d.fn_global(CL);
  }

  template<typename C>
  static R fn_call_member(const data_t &d cat(PL)) {
    return (((C*)d.object)->*((R (C::*&)(PL))d.fn_member))(CL);
  }

public:
  R operator()(PL) const { return data.fn_call(data cat(CL)); }
  operator bool() const { return data.fn_call; }

  function() { data.fn_call = 0; }

  function(R (*fn)(PL)) {
    data.fn_call = &fn_call_global;
    data.fn_global = fn;
  }

  template<typename C>
  function(R (C::*fn)(PL), C *obj) {
    data.fn_call = &fn_call_member<C>;
    (R (C::*&)(PL))data.fn_member = fn;
    assert(sizeof data.fn_member >= sizeof fn);
    data.object = obj;
  }

  template<typename C>
  function(R (C::*fn)(PL) const, C *obj) {
    data.fn_call = &fn_call_member<C>;
    (R (C::*&)(PL))data.fn_member = (R (C::*&)(PL))fn;
    assert(sizeof data.fn_member >= sizeof fn);
    data.object = obj;
  }

  function &operator=(const function &source) { memcpy(&data, &source.data, sizeof(data_t)); return *this; }
  function(const function &source) { memcpy(&data, &source.data, sizeof(data_t)); }
};

template<TL>
function<R (PL)> bind(R (*fn)(PL)) {
  return function<R (PL)>(fn);
}

template<typename C, TL>
function<R (PL)> bind(R (C::*fn)(PL), C *obj) {
  return function<R (PL)>(fn, obj);
}

template<typename C, TL>
function<R (PL)> bind(R (C::*fn)(PL) const, C *obj) {
  return function<R (PL)>(fn, obj);
}

#undef cat
#undef TL
#undef PL
#undef CL

#endif
