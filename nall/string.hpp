#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <initializer_list>
#include <memory>

#include <nall/platform.hpp>
#include <nall/atoi.hpp>
#include <nall/function.hpp>
#include <nall/intrinsics.hpp>
#include <nall/memory.hpp>
#include <nall/primitives.hpp>
#include <nall/shared-pointer.hpp>
#include <nall/stdint.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>
#include <nall/view.hpp>

namespace nall {

struct string;
struct string_format;

template<> struct view<string> {
  using type = view<string>;

  //view.hpp
  inline view();
  inline view(const view& source);
  inline view(view&& source);
  inline view(const char* data);
  inline view(const char* data, uint size);
  inline view(const string& source);
  template<typename... P> inline view(P&&... p);
  inline ~view();

  inline auto operator=(const view& source) -> view&;
  inline auto operator=(view&& source) -> view&;

  inline operator const char*() const;
  inline auto data() const -> const char*;
  inline auto size() const -> uint;

  inline auto begin() const { return &_data[0]; }
  inline auto end() const { return &_data[size()]; }

protected:
  string* _string;
  const char* _data;
  mutable int _size;
};

#define NALL_STRING_ALLOCATOR_ADAPTIVE
//#define NALL_STRING_ALLOCATOR_COPY_ON_WRITE
//#define NALL_STRING_ALLOCATOR_SMALL_STRING_OPTIMIZATION
//#define NALL_STRING_ALLOCATOR_VECTOR

//cast.hpp
template<typename T> struct stringify;

//format.hpp
template<typename... P> inline auto print(P&&...) -> void;
template<typename... P> inline auto print(FILE*, P&&...) -> void;
template<typename T> inline auto pad(const T& value, long precision = 0, char padchar = ' ') -> string;
inline auto hex(uintmax value, long precision = 0, char padchar = '0') -> string;
inline auto octal(uintmax value, long precision = 0, char padchar = '0') -> string;
inline auto binary(uintmax value, long precision = 0, char padchar = '0') -> string;

//match.hpp
inline auto tokenize(const char* s, const char* p) -> bool;
inline auto tokenize(vector<string>& list, const char* s, const char* p) -> bool;

//utility.hpp
inline auto slice(view<string> self, int offset = 0, int length = -1) -> string;
template<typename T> inline auto fromInteger(char* result, T value) -> char*;
template<typename T> inline auto fromNatural(char* result, T value) -> char*;
template<typename T> inline auto fromReal(char* str, T value) -> uint;

struct string {
  using type = string;

protected:
  #if defined(NALL_STRING_ALLOCATOR_ADAPTIVE)
  enum : uint { SSO = 24 };
  union {
    struct {  //copy-on-write
      char* _data;
      uint* _refs;
    };
    struct {  //small-string-optimization
      char _text[SSO];
    };
  };
  inline auto _allocate() -> void;
  inline auto _copy() -> void;
  inline auto _resize() -> void;
  #endif

  #if defined(NALL_STRING_ALLOCATOR_COPY_ON_WRITE)
  char* _data;
  mutable uint* _refs;
  inline auto _allocate() -> char*;
  inline auto _copy() -> char*;
  #endif

  #if defined(NALL_STRING_ALLOCATOR_SMALL_STRING_OPTIMIZATION)
  enum : uint { SSO = 24 };
  union {
    char* _data;
    char _text[SSO];
  };
  #endif

  #if defined(NALL_STRING_ALLOCATOR_VECTOR)
  char* _data;
  #endif

  uint _capacity;
  uint _size;

public:
  inline string();
  template<typename T = char> inline auto get() -> T*;
  template<typename T = char> inline auto data() const -> const T*;
  inline auto reset() -> type&;
  inline auto reserve(uint) -> type&;
  inline auto resize(uint) -> type&;
  inline auto operator=(const string&) -> type&;
  inline auto operator=(string&&) -> type&;

  template<typename T, typename... P> string(T&& s, P&&... p) : string() {
    append(forward<T>(s), forward<P>(p)...);
  }
  ~string() { reset(); }

  explicit operator bool() const { return _size; }
  operator const char*() const { return (const char*)data(); }

  auto size() const -> uint { return _size; }
  auto capacity() const -> uint { return _capacity; }

  auto operator==(const string& source) const -> bool {
    return size() == source.size() && memory::compare(data(), source.data(), size()) == 0;
  }
  auto operator!=(const string& source) const -> bool {
    return size() != source.size() || memory::compare(data(), source.data(), size()) != 0;
  }

  auto operator==(const char* source) const -> bool { return strcmp(data(), source) == 0; }
  auto operator!=(const char* source) const -> bool { return strcmp(data(), source) != 0; }

  auto operator==(view<string> source) const -> bool { return compare(source) == 0; }
  auto operator!=(view<string> source) const -> bool { return compare(source) != 0; }
  auto operator< (view<string> source) const -> bool { return compare(source) <  0; }
  auto operator<=(view<string> source) const -> bool { return compare(source) <= 0; }
  auto operator> (view<string> source) const -> bool { return compare(source) >  0; }
  auto operator>=(view<string> source) const -> bool { return compare(source) >= 0; }

  string(const string& source) : string() { operator=(source); }
  string(string&& source) : string() { operator=(move(source)); }

  auto begin() -> char* { return &get()[0]; }
  auto end() -> char* { return &get()[size()]; }
  auto begin() const -> const char* { return &data()[0]; }
  auto end() const -> const char* { return &data()[size()]; }

  //atoi.hpp
  inline auto integer() const -> intmax;
  inline auto natural() const -> uintmax;
  inline auto hex() const -> uintmax;
  inline auto real() const -> double;

  //core.hpp
  inline auto operator[](uint) const -> const char&;
  inline auto operator()(uint, char = 0) const -> char;
  template<typename... P> inline auto assign(P&&...) -> type&;
  template<typename T, typename... P> inline auto prepend(const T&, P&&...) -> type&;
  template<typename... P> inline auto prepend(const nall::string_format&, P&&...) -> type&;
  inline auto prepend() -> type&;
  template<typename T> inline auto _prepend(const stringify<T>&) -> string&;
  template<typename T, typename... P> inline auto append(const T&, P&&...) -> type&;
  template<typename... P> inline auto append(const nall::string_format&, P&&...) -> type&;
  inline auto append() -> type&;
  template<typename T> inline auto _append(const stringify<T>&) -> string&;
  inline auto length() const -> uint;

  //find.hpp
  inline auto contains(view<string> characters) const -> maybe<uint>;

  template<bool, bool> inline auto _find(int, view<string>) const -> maybe<uint>;

  inline auto find(view<string> source) const -> maybe<uint>;
  inline auto ifind(view<string> source) const -> maybe<uint>;
  inline auto qfind(view<string> source) const -> maybe<uint>;
  inline auto iqfind(view<string> source) const -> maybe<uint>;

  inline auto findFrom(int offset, view<string> source) const -> maybe<uint>;
  inline auto ifindFrom(int offset, view<string> source) const -> maybe<uint>;

  //format.hpp
  inline auto format(const nall::string_format& params) -> type&;

  //compare.hpp
  template<bool> inline static auto _compare(const char*, uint, const char*, uint) -> int;

  inline static auto compare(view<string>, view<string>) -> int;
  inline static auto icompare(view<string>, view<string>) -> int;

  inline auto compare(view<string> source) const -> int;
  inline auto icompare(view<string> source) const -> int;

  inline auto equals(view<string> source) const -> bool;
  inline auto iequals(view<string> source) const -> bool;

  inline auto beginsWith(view<string> source) const -> bool;
  inline auto ibeginsWith(view<string> source) const -> bool;

  inline auto endsWith(view<string> source) const -> bool;
  inline auto iendsWith(view<string> source) const -> bool;

  //convert.hpp
  inline auto downcase() -> type&;
  inline auto upcase() -> type&;

  inline auto qdowncase() -> type&;
  inline auto qupcase() -> type&;

  inline auto transform(view<string> from, view<string>to) -> type&;

  //match.hpp
  inline auto match(view<string> source) const -> bool;
  inline auto imatch(view<string> source) const -> bool;

  //replace.hpp
  template<bool, bool> inline auto _replace(view<string>, view<string>, long) -> type&;
  inline auto replace(view<string> from, view<string> to, long limit = LONG_MAX) -> type&;
  inline auto ireplace(view<string> from, view<string> to, long limit = LONG_MAX) -> type&;
  inline auto qreplace(view<string> from, view<string> to, long limit = LONG_MAX) -> type&;
  inline auto iqreplace(view<string> from, view<string> to, long limit = LONG_MAX) -> type&;

  //split.hpp
  inline auto split(view<string> key, long limit = LONG_MAX) const -> vector<string>;
  inline auto isplit(view<string> key, long limit = LONG_MAX) const -> vector<string>;
  inline auto qsplit(view<string> key, long limit = LONG_MAX) const -> vector<string>;
  inline auto iqsplit(view<string> key, long limit = LONG_MAX) const -> vector<string>;

  //trim.hpp
  inline auto trim(view<string> lhs, view<string> rhs, long limit = LONG_MAX) -> type&;
  inline auto trimLeft(view<string> lhs, long limit = LONG_MAX) -> type&;
  inline auto trimRight(view<string> rhs, long limit = LONG_MAX) -> type&;

  inline auto itrim(view<string> lhs, view<string> rhs, long limit = LONG_MAX) -> type&;
  inline auto itrimLeft(view<string> lhs, long limit = LONG_MAX) -> type&;
  inline auto itrimRight(view<string> rhs, long limit = LONG_MAX) -> type&;

  inline auto strip() -> type&;
  inline auto stripLeft() -> type&;
  inline auto stripRight() -> type&;

  //utility.hpp
  inline static auto read(view<string> filename) -> string;
  inline static auto repeat(view<string> pattern, uint times) -> string;
  inline auto fill(char fill = ' ') -> type&;
  inline auto hash() const -> uint;
  inline auto remove(uint offset, uint length) -> type&;
  inline auto reverse() -> type&;
  inline auto size(int length, char fill = ' ') -> type&;
};

template<> struct vector<string> : vector_base<string> {
  using type = vector<string>;
  using vector_base<string>::vector_base;

  vector(const vector& source) { vector_base::operator=(source); }
  vector(vector& source) { vector_base::operator=(source); }
  vector(vector&& source) { vector_base::operator=(move(source)); }
  template<typename... P> vector(P&&... p) { append(forward<P>(p)...); }

  inline auto operator=(const vector& source) -> type& { return vector_base::operator=(source), *this; }
  inline auto operator=(vector& source) -> type& { return vector_base::operator=(source), *this; }
  inline auto operator=(vector&& source) -> type& { return vector_base::operator=(move(source)), *this; }

  //list.hpp
  template<typename... P> inline auto append(const string&, P&&...) -> type&;
  inline auto append() -> type&;

  inline auto isort() -> type&;
  inline auto find(view<string> source) const -> maybe<uint>;
  inline auto ifind(view<string> source) const -> maybe<uint>;
  inline auto match(view<string> pattern) const -> vector<string>;
  inline auto merge(view<string> separator) const -> string;
  inline auto strip() -> type&;

  //split.hpp
  template<bool, bool> inline auto _split(view<string>, view<string>, long) -> type&;
};

struct string_format : vector<string> {
  using type = string_format;

  template<typename... P> string_format(P&&... p) { reserve(sizeof...(p)); append(forward<P>(p)...); }
  template<typename T, typename... P> inline auto append(const T&, P&&... p) -> type&;
  inline auto append() -> type&;
};

}

#include <nall/string/view.hpp>
#include <nall/string/pascal.hpp>

#include <nall/string/atoi.hpp>
#include <nall/string/cast.hpp>
#include <nall/string/compare.hpp>
#include <nall/string/convert.hpp>
#include <nall/string/core.hpp>
#include <nall/string/find.hpp>
#include <nall/string/format.hpp>
#include <nall/string/match.hpp>
#include <nall/string/replace.hpp>
#include <nall/string/split.hpp>
#include <nall/string/trim.hpp>
#include <nall/string/utility.hpp>
#include <nall/string/vector.hpp>

#include <nall/string/eval/node.hpp>
#include <nall/string/eval/literal.hpp>
#include <nall/string/eval/parser.hpp>
#include <nall/string/eval/evaluator.hpp>

#include <nall/string/markup/node.hpp>
#include <nall/string/markup/find.hpp>
#include <nall/string/markup/bml.hpp>
#include <nall/string/markup/xml.hpp>

#include <nall/string/transform/cml.hpp>
#include <nall/string/transform/dml.hpp>
