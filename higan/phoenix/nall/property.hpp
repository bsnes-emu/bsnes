#ifndef NALL_PROPERTY_HPP
#define NALL_PROPERTY_HPP

//nall::property implements ownership semantics into container classes
//example: property<owner>::readonly<type> implies that only owner has full
//access to type; and all other code has readonly access.
//
//property can be used either of two ways:
//struct foo {
//  property<foo>::readonly<bool> x;
//  property<foo>::readwrite<int> y;
//};
//-or-
//struct foo : property<foo> {
//  readonly<bool> x;
//  readwrite<int> y;
//};

//return types are const T& (byref) instead of T (byval) to avoid major speed
//penalties for objects with expensive copy constructors

//operator-> provides access to underlying object type:
//readonly<Object> foo;
//foo->bar();
//... will call Object::bar();

//operator='s reference is constant so as to avoid leaking a reference handle
//that could bypass access restrictions

//both constant and non-constant operators are provided, though it may be
//necessary to cast first, for instance:
//struct foo : property<foo> { readonly<int> bar; } object;
//int main() { int value = const_cast<const foo&>(object); }

//writeonly is useful for objects that have non-const reads, but const writes.
//however, to avoid leaking handles, the interface is very restricted. the only
//way to write is via operator=, which requires conversion via eg copy
//constructor. example:
//struct foo {
//  foo(bool value) { ... }
//};
//writeonly<foo> bar;
//bar = true;

namespace nall {
  template<typename C> struct property {
    template<typename T> struct readonly {
      const T* operator->() const { return &value; }
      const T& operator()() const { return value; }
      operator const T&() const { return value; }
    private:
      T* operator->() { return &value; }
      operator T&() { return value; }
      const T& operator=(const T& value_) { return value = value_; }
      T value;
      friend C;
    };

    template<typename T> struct writeonly {
      void operator=(const T& value_) { value = value_; }
    private:
      const T* operator->() const { return &value; }
      const T& operator()() const { return value; }
      operator const T&() const { return value; }
      T* operator->() { return &value; }
      operator T&() { return value; }
      T value;
      friend C;
    };

    template<typename T> struct readwrite {
      const T* operator->() const { return &value; }
      const T& operator()() const { return value; }
      operator const T&() const { return value; }
      T* operator->() { return &value; }
      operator T&() { return value; }
      const T& operator=(const T& value_) { return value = value_; }
      T value;
    };
  };
}

#endif
