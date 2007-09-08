#ifdef LIBFUNCTOR_H

template<TL>
class functor<R (PL)> {
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

  functor() { data.fn_call = 0; }

  functor(R (*fn)(PL)) {
    data.fn_call = &fn_call_global;
    data.fn_global = fn;
  }

  template<typename C>
  functor(R (C::*fn)(PL), C *obj) {
    data.fn_call = &fn_call_member<C>;
    (R (C::*&)(PL))data.fn_member = fn;
    assert(sizeof data.fn_member >= sizeof fn);
    data.object = obj;
  }

  template<typename C>
  functor(R (C::*fn)(PL) const, C *obj) {
    data.fn_call = &fn_call_member<C>;
    (R (C::*&)(PL))data.fn_member = (R (C::*&)(PL))fn;
    assert(sizeof data.fn_member >= sizeof fn);
    data.object = obj;
  }

  functor &operator=(const functor &source) { memcpy(&data, &source.data, sizeof(data_t)); return *this; }
  functor(const functor &source) { memcpy(&data, &source.data, sizeof(data_t)); }
};

template<TL>
functor<R (PL)> bind(R (*fn)(PL)) {
  return functor<R (PL)>(fn);
}

template<typename C, TL>
functor<R (PL)> bind(R (C::*fn)(PL), C *obj) {
  return functor<R (PL)>(fn, obj);
}

template<typename C, TL>
functor<R (PL)> bind(R (C::*fn)(PL) const, C *obj) {
  return functor<R (PL)>(fn, obj);
}

#undef cat
#undef TL
#undef PL
#undef CL

#endif
