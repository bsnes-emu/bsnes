#ifndef NALL_PUBLIC_CAST_HPP
#define NALL_PUBLIC_CAST_HPP

//this is a proof-of-concept-*only* C++ access-privilege elevation exploit.
//it allows full access to both protected and private member variables, objects and functions
//from the global scope, regardless of inheritance and/or friendship privileges.
//the code itself is 100% legal C++, per C++98 section 14.7.2 paragraph 8:
//"Access checking rules do not apply to names in explicit instantiations."

//C++0x variadic templates are used simply to allow public_cast name to be used
//on second derived class name for simplification, however the basic idea also
//works on C++98.

//usage example:
//struct N { typedef void (Class::*)(); };
//template class public_cast<N, &Class::Function>;
//(class.*public_cast<N>::value)();

namespace nall {
  template<typename T, typename T::type... P> struct public_cast;

  template<typename T> struct public_cast<T> {
    static typename T::type value;
  };

  template<typename T> typename T::type public_cast<T>::value;

  template<typename T, typename T::type P> struct public_cast<T, P> : public_cast<T> {
    struct ref_t {
      ref_t() { public_cast<T>::value = P; }
    };
    static ref_t ref;
  };

  template<typename T, typename T::type P> typename public_cast<T, P>::ref_t public_cast<T, P>::ref;
}

#endif
