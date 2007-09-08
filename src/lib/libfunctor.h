/*
  libfunctor : version 0.05 ~byuu (2007-09-08)
  license: public domain
*/

#ifndef LIBFUNCTOR_H
#define LIBFUNCTOR_H

//prologue

#define TN typename

template<typename T> class functor;

//parameters = 0

#define cat(n)  n
#define TL typename R
#define PL
#define CL

#include "libfunctor_impl.h"

//parameters = 1

#define cat(n)  , n
#define TL TN R, TN P1
#define PL P1 p1
#define CL p1

#include "libfunctor_impl.h"

//parameters = 2

#define cat(n)  , n
#define TL TN R, TN P1, TN P2
#define PL P1 p1, P2 p2
#define CL p1, p2

#include "libfunctor_impl.h"

//parameters = 3

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3
#define PL P1 p1, P2 p2, P3 p3
#define CL p1, p2, p3

#include "libfunctor_impl.h"

//parameters = 4

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4
#define PL P1 p1, P2 p2, P3 p3, P4 p4
#define CL p1, p2, p3, p4

#include "libfunctor_impl.h"

//parameters = 5

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4, TN P5
#define PL P1 p1, P2 p2, P3 p3, P4 p4, P5 p5
#define CL p1, p2, p3, p4, p5

#include "libfunctor_impl.h"

//parameters = 6

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4, TN P5, TN P6
#define PL P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6
#define CL p1, p2, p3, p4, p5, p6

#include "libfunctor_impl.h"

//parameters = 7

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4, TN P5, TN P6, TN P7
#define PL P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7
#define CL p1, p2, p3, p4, p5, p6, p7

#include "libfunctor_impl.h"

//parameters = 8

#define cat(n)  , n
#define TL TN R, TN P1, TN P2, TN P3, TN P4, TN P5, TN P6, TN P7, TN P8
#define PL P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8
#define CL p1, p2, p3, p4, p5, p6, p7, p8

#include "libfunctor_impl.h"

//epilogue

#undef TN

#endif
