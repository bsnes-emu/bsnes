/*
  libfunctor : version 0.01 ~byuu (2007-01-04)
*/

#ifndef __LIBFUNCTOR
#define __LIBFUNCTOR

template<typename T> struct functor;
template<typename T> struct functor_t;

/*****
 * macros
 *****/

#define base_functor \
functor *f; \
  virtual operator bool() const { return f && (bool)(*f); } \
  virtual functor *copy() const { return f->copy(); } \
  functor &operator=(const functor *source) { if(f) { delete f; } f = (source->f) ? source->copy() : 0; return *this; } \
  functor &operator=(const functor &source) { if(f) { delete f; } f = (source.f)  ? source.copy()  : 0; return *this; } \
  functor(const functor &source) { /* ........................ */ f = (source.f)  ? source.copy()  : 0; } \
  functor() : f(0) {} \
  ~functor() { if(f && f != this) { delete f; } }

#define base_global_functor \
  operator bool() const { return proc; } \
  functor_t *copy() const { return new functor_t(*this); } \
  functor_t(const functor_t &source) { proc = source.proc; }

#define base_member_functor \
  operator bool() const { return obj && proc; } \
  functor_t *copy() const { return new functor_t(*this); } \
  functor_t(const functor_t &source) { obj = source.obj; proc = source.proc; } \
C *obj;

/*****
 * parameters = 0
 *****/

template<typename R>
struct functor<R (*)()> { base_functor
  virtual R operator()() const { return (*f)(); }
};

template<typename R>
struct functor_t<R (*)()> : public functor<R (*)()> { base_global_functor
R (*proc)();
  R operator()() const { return (*proc)(); }
  functor_t(R (*_proc)()) : proc(_proc) { this->f = this; }
};

template<typename C, typename R>
struct functor_t<R (C::*)()> : public functor<R (*)()> { base_member_functor
R (C::*proc)();
  R operator()() const { return (obj->*proc)(); }
  functor_t(C *_obj, R (C::*_proc)()) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R>
functor_t<R (*)()> make_functor(R (*proc)()) {
  return functor_t<R (*)()>(proc);
}

template<typename C, typename R>
functor_t<R (C::*)()> make_functor(C *obj, R (C::*proc)()) {
  return functor_t<R (C::*)()>(obj, proc);
}

/*****
 * parameters = 1
 *****/

template<typename R, typename P1>
struct functor<R (*)(P1)> { base_functor
  virtual R operator()(P1 p1) const { return (*f)(p1); }
};

template<typename R, typename P1>
struct functor_t<R (*)(P1)> : public functor<R (*)(P1)> { base_global_functor
R (*proc)(P1);
  R operator()(P1 p1) const { return (*proc)(p1); }
  functor_t(R (*_proc)(P1)) : proc(_proc) { this->f = this; }
};

template<typename C, typename R, typename P1>
struct functor_t<R (C::*)(P1)> : public functor<R (*)(P1)> { base_member_functor
R (C::*proc)(P1);
  R operator()(P1 p1) const { return (obj->*proc)(p1); }
  functor_t(C *_obj, R (C::*_proc)(P1)) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R, typename P1>
functor_t<R (*)(P1)> make_functor(R (*proc)(P1)) {
  return functor_t<R (*)(P1)>(proc);
}

template<typename C, typename R, typename P1>
functor_t<R (C::*)(P1)> make_functor(C *obj, R (C::*proc)(P1)) {
  return functor_t<R (C::*)(P1)>(obj, proc);
}

/*****
 * parameters = 2
 *****/

template<typename R, typename P1, typename P2>
struct functor<R (*)(P1, P2)> { base_functor
  virtual R operator()(P1 p1, P2 p2) const { return (*f)(p1, p2); }
};

template<typename R, typename P1, typename P2>
struct functor_t<R (*)(P1, P2)> : public functor<R (*)(P1, P2)> { base_global_functor
R (*proc)(P1, P2);
  R operator()(P1 p1, P2 p2) const { return (*proc)(p1, p2); }
  functor_t(R (*_proc)(P1, P2)) : proc(_proc) { this->f = this; }
};

template<typename C, typename R, typename P1, typename P2>
struct functor_t<R (C::*)(P1, P2)> : public functor<R (*)(P1, P2)> { base_member_functor
R (C::*proc)(P1, P2);
  R operator()(P1 p1, P2 p2) const { return (obj->*proc)(p1, p2); }
  functor_t(C *_obj, R (C::*_proc)(P1, P2)) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R, typename P1, typename P2>
functor_t<R (*)(P1, P2)> make_functor(R (*proc)(P1, P2)) {
  return functor_t<R (*)(P1, P2)>(proc);
}

template<typename C, typename R, typename P1, typename P2>
functor_t<R (C::*)(P1, P2)> make_functor(C *obj, R (C::*proc)(P1, P2)) {
  return functor_t<R (C::*)(P1, P2)>(obj, proc);
}

/*****
 * parameters = 3
 *****/

template<typename R, typename P1, typename P2, typename P3>
struct functor<R (*)(P1, P2, P3)> { base_functor
  virtual R operator()(P1 p1, P2 p2, P3 p3) const { return (*f)(p1, p2, p3); }
};

template<typename R, typename P1, typename P2, typename P3>
struct functor_t<R (*)(P1, P2, P3)> : public functor<R (*)(P1, P2, P3)> { base_global_functor
R (*proc)(P1, P2, P3);
  R operator()(P1 p1, P2 p2, P3 p3) const { return (*proc)(p1, p2, p3); }
  functor_t(R (*_proc)(P1, P2, P3)) : proc(_proc) { this->f = this; }
};

template<typename C, typename R, typename P1, typename P2, typename P3>
struct functor_t<R (C::*)(P1, P2, P3)> : public functor<R (*)(P1, P2, P3)> { base_member_functor
R (C::*proc)(P1, P2, P3);
  R operator()(P1 p1, P2 p2, P3 p3) const { return (obj->*proc)(p1, p2, p3); }
  functor_t(C *_obj, R (C::*_proc)(P1, P2, P3)) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R, typename P1, typename P2, typename P3>
functor_t<R (*)(P1, P2, P3)> make_functor(R (*proc)(P1, P2, P3)) {
  return functor_t<R (*)(P1, P2, P3)>(proc);
}

template<typename C, typename R, typename P1, typename P2, typename P3>
functor_t<R (C::*)(P1, P2, P3)> make_functor(C *obj, R (C::*proc)(P1, P2, P3)) {
  return functor_t<R (C::*)(P1, P2, P3)>(obj, proc);
}

/*****
 * parameters = 4
 *****/

template<typename R, typename P1, typename P2, typename P3, typename P4>
struct functor<R (*)(P1, P2, P3, P4)> { base_functor
  virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4) const { return (*f)(p1, p2, p3, p4); }
};

template<typename R, typename P1, typename P2, typename P3, typename P4>
struct functor_t<R (*)(P1, P2, P3, P4)> : public functor<R (*)(P1, P2, P3, P4)> { base_global_functor
R (*proc)(P1, P2, P3, P4);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4) const { return (*proc)(p1, p2, p3, p4); }
  functor_t(R (*_proc)(P1, P2, P3, P4)) : proc(_proc) { this->f = this; }
};

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4>
struct functor_t<R (C::*)(P1, P2, P3, P4)> : public functor<R (*)(P1, P2, P3, P4)> { base_member_functor
R (C::*proc)(P1, P2, P3, P4);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4) const { return (obj->*proc)(p1, p2, p3, p4); }
  functor_t(C *_obj, R (C::*_proc)(P1, P2, P3, P4)) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R, typename P1, typename P2, typename P3, typename P4>
functor_t<R (*)(P1, P2, P3, P4)> make_functor(R (*proc)(P1, P2, P3, P4)) {
  return functor_t<R (*)(P1, P2, P3, P4)>(proc);
}

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4>
functor_t<R (C::*)(P1, P2, P3, P4)> make_functor(C *obj, R (C::*proc)(P1, P2, P3, P4)) {
  return functor_t<R (C::*)(P1, P2, P3, P4)>(obj, proc);
}

/*****
 * parameters = 5
 *****/

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
struct functor<R (*)(P1, P2, P3, P4, P5)> { base_functor
  virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const { return (*f)(p1, p2, p3, p4, p5); }
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
struct functor_t<R (*)(P1, P2, P3, P4, P5)> : public functor<R (*)(P1, P2, P3, P4, P5)> { base_global_functor
R (*proc)(P1, P2, P3, P4, P5);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const { return (*proc)(p1, p2, p3, p4, p5); }
  functor_t(R (*_proc)(P1, P2, P3, P4, P5)) : proc(_proc) { this->f = this; }
};

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
struct functor_t<R (C::*)(P1, P2, P3, P4, P5)> : public functor<R (*)(P1, P2, P3, P4, P5)> { base_member_functor
R (C::*proc)(P1, P2, P3, P4, P5);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const { return (obj->*proc)(p1, p2, p3, p4, p5); }
  functor_t(C *_obj, R (C::*_proc)(P1, P2, P3, P4, P5)) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
functor_t<R (*)(P1, P2, P3, P4, P5)> make_functor(R (*proc)(P1, P2, P3, P4, P5)) {
  return functor_t<R (*)(P1, P2, P3, P4, P5)>(proc);
}

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4, typename P5>
functor_t<R (C::*)(P1, P2, P3, P4, P5)> make_functor(C *obj, R (C::*proc)(P1, P2, P3, P4, P5)) {
  return functor_t<R (C::*)(P1, P2, P3, P4, P5)>(obj, proc);
}

/*****
 * parameters = 6
 *****/

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct functor<R (*)(P1, P2, P3, P4, P5, P6)> { base_functor
  virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const { return (*f)(p1, p2, p3, p4, p5, p6); }
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct functor_t<R (*)(P1, P2, P3, P4, P5, P6)> : public functor<R (*)(P1, P2, P3, P4, P5, P6)> { base_global_functor
R (*proc)(P1, P2, P3, P4, P5, P6);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const { return (*proc)(p1, p2, p3, p4, p5, p6); }
  functor_t(R (*_proc)(P1, P2, P3, P4, P5, P6)) : proc(_proc) { this->f = this; }
};

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct functor_t<R (C::*)(P1, P2, P3, P4, P5, P6)> : public functor<R (*)(P1, P2, P3, P4, P5, P6)> { base_member_functor
R (C::*proc)(P1, P2, P3, P4, P5, P6);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const { return (obj->*proc)(p1, p2, p3, p4, p5, p6); }
  functor_t(C *_obj, R (C::*_proc)(P1, P2, P3, P4, P5, P6)) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
functor_t<R (*)(P1, P2, P3, P4, P5, P6)> make_functor(R (*proc)(P1, P2, P3, P4, P5, P6)) {
  return functor_t<R (*)(P1, P2, P3, P4, P5, P6)>(proc);
}

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
functor_t<R (C::*)(P1, P2, P3, P4, P5, P6)> make_functor(C *obj, R (C::*proc)(P1, P2, P3, P4, P5, P6)) {
  return functor_t<R (C::*)(P1, P2, P3, P4, P5, P6)>(obj, proc);
}

/*****
 * parameters = 7
 *****/

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
struct functor<R (*)(P1, P2, P3, P4, P5, P6, P7)> { base_functor
  virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) const { return (*f)(p1, p2, p3, p4, p5, p6, p7); }
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
struct functor_t<R (*)(P1, P2, P3, P4, P5, P6, P7)> : public functor<R (*)(P1, P2, P3, P4, P5, P6, P7)> { base_global_functor
R (*proc)(P1, P2, P3, P4, P5, P6, P7);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) const { return (*proc)(p1, p2, p3, p4, p5, p6, p7); }
  functor_t(R (*_proc)(P1, P2, P3, P4, P5, P6, P7)) : proc(_proc) { this->f = this; }
};

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
struct functor_t<R (C::*)(P1, P2, P3, P4, P5, P6, P7)> : public functor<R (*)(P1, P2, P3, P4, P5, P6, P7)> { base_member_functor
R (C::*proc)(P1, P2, P3, P4, P5, P6, P7);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) const { return (obj->*proc)(p1, p2, p3, p4, p5, p6, p7); }
  functor_t(C *_obj, R (C::*_proc)(P1, P2, P3, P4, P5, P6, P7)) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
functor_t<R (*)(P1, P2, P3, P4, P5, P6, P7)> make_functor(R (*proc)(P1, P2, P3, P4, P5, P6, P7)) {
  return functor_t<R (*)(P1, P2, P3, P4, P5, P6, P7)>(proc);
}

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
functor_t<R (C::*)(P1, P2, P3, P4, P5, P6, P7)> make_functor(C *obj, R (C::*proc)(P1, P2, P3, P4, P5, P6, P7)) {
  return functor_t<R (C::*)(P1, P2, P3, P4, P5, P6, P7)>(obj, proc);
}

/*****
 * parameters = 8
 *****/

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
struct functor<R (*)(P1, P2, P3, P4, P5, P6, P7, P8)> { base_functor
  virtual R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) const { return (*f)(p1, p2, p3, p4, p5, p6, p7, p8); }
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
struct functor_t<R (*)(P1, P2, P3, P4, P5, P6, P7, P8)> : public functor<R (*)(P1, P2, P3, P4, P5, P6, P7, P8)> { base_global_functor
R (*proc)(P1, P2, P3, P4, P5, P6, P7, P8);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) const { return (*proc)(p1, p2, p3, p4, p5, p6, p7, p8); }
  functor_t(R (*_proc)(P1, P2, P3, P4, P5, P6, P7, P8)) : proc(_proc) { this->f = this; }
};

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
struct functor_t<R (C::*)(P1, P2, P3, P4, P5, P6, P7, P8)> : public functor<R (*)(P1, P2, P3, P4, P5, P6, P7, P8)> { base_member_functor
R (C::*proc)(P1, P2, P3, P4, P5, P6, P7, P8);
  R operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) const { return (obj->*proc)(p1, p2, p3, p4, p5, p6, p7, p8); }
  functor_t(C *_obj, R (C::*_proc)(P1, P2, P3, P4, P5, P6, P7, P8)) : obj(_obj), proc(_proc) { this->f = this; }
};

template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
functor_t<R (*)(P1, P2, P3, P4, P5, P6, P7, P8)> make_functor(R (*proc)(P1, P2, P3, P4, P5, P6, P7, P8)) {
  return functor_t<R (*)(P1, P2, P3, P4, P5, P6, P7, P8)>(proc);
}

template<typename C, typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
functor_t<R (C::*)(P1, P2, P3, P4, P5, P6, P7, P8)> make_functor(C *obj, R (C::*proc)(P1, P2, P3, P4, P5, P6, P7, P8)) {
  return functor_t<R (C::*)(P1, P2, P3, P4, P5, P6, P7, P8)>(obj, proc);
}

/*****
 * epilogue
 *****/

#undef base_functor
#undef base_global_functor
#undef base_member_functor

#endif
