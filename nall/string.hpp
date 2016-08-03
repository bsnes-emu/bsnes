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

namespace nall {

struct string;
struct string_view;
struct string_vector;
struct string_format;

struct string_view {
  inline string_view();
  inline string_view(const string_view& source);
  inline string_view(string_view&& source);
  inline string_view(const char* data);
  inline string_view(const char* data, uint size);
  inline string_view(const string& source);
  template<typename... P> inline string_view(P&&... p);
  inline ~string_view();

  inline auto operator=(const string_view& source) -> string_view&;
  inline auto operator=(string_view&& source) -> string_view&;

  inline operator const char*() const;
  inline auto data() const -> const char*;
  inline auto size() const -> uint;

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
inline auto hex(uintmax_t value, long precision = 0, char padchar = '0') -> string;
inline auto octal(uintmax_t value, long precision = 0, char padchar = '0') -> string;
inline auto binary(uintmax_t value, long precision = 0, char padchar = '0') -> string;
template<typename T> inline auto pointer(const T* value, long precision = 0) -> string;
inline auto pointer(uintptr_t value, long precision = 0) -> string;

//match.hpp
inline auto tokenize(const char* s, const char* p) -> bool;
inline auto tokenize(string_vector& list, const char* s, const char* p) -> bool;

//utility.hpp
inline auto slice(string_view self, int offset = 0, int length = -1) -> string;
inline auto fromInteger(char* result, intmax_t value) -> char*;
inline auto fromNatural(char* result, uintmax_t value) -> char*;
inline auto fromReal(char* str, long double value) -> uint;

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

  auto operator==(string_view source) const -> bool { return compare(source) == 0; }
  auto operator!=(string_view source) const -> bool { return compare(source) != 0; }
  auto operator< (string_view source) const -> bool { return compare(source) <  0; }
  auto operator<=(string_view source) const -> bool { return compare(source) <= 0; }
  auto operator> (string_view source) const -> bool { return compare(source) >  0; }
  auto operator>=(string_view source) const -> bool { return compare(source) >= 0; }

  string(const string& source) : string() { operator=(source); }
  string(string&& source) : string() { operator=(move(source)); }

  auto begin() -> char* { return &get()[0]; }
  auto end() -> char* { return &get()[size()]; }
  auto begin() const -> const char* { return &data()[0]; }
  auto end() const -> const char* { return &data()[size()]; }

  //atoi.hpp
  inline auto integer() const -> intmax_t;
  inline auto natural() const -> uintmax_t;
  inline auto hex() const -> uintmax_t;
  inline auto real() const -> double;

  //core.hpp
  inline auto operator[](int) const -> const char&;
  inline auto operator()(int, char) const -> char;
  template<typename... P> inline auto assign(P&&...) -> type&;
  template<typename T, typename... P> inline auto append(const T&, P&&...) -> type&;
  template<typename... P> inline auto append(const nall::string_format&, P&&...) -> type&;
  inline auto append() -> type&;
  template<typename T> inline auto _append(const stringify<T>&) -> string&;
  inline auto length() const -> uint;

  //find.hpp
  template<bool, bool> inline auto _find(int, string_view) const -> maybe<uint>;

  inline auto find(string_view source) const -> maybe<uint>;
  inline auto ifind(string_view source) const -> maybe<uint>;
  inline auto qfind(string_view source) const -> maybe<uint>;
  inline auto iqfind(string_view source) const -> maybe<uint>;

  inline auto findFrom(int offset, string_view source) const -> maybe<uint>;
  inline auto ifindFrom(int offset, string_view source) const -> maybe<uint>;

  //format.hpp
  inline auto format(const nall::string_format& params) -> type&;

  //compare.hpp
  template<bool> inline static auto _compare(const char*, uint, const char*, uint) -> int;

  inline static auto compare(string_view, string_view) -> int;
  inline static auto icompare(string_view, string_view) -> int;

  inline auto compare(string_view source) const -> int;
  inline auto icompare(string_view source) const -> int;

  inline auto equals(string_view source) const -> bool;
  inline auto iequals(string_view source) const -> bool;

  inline auto beginsWith(string_view source) const -> bool;
  inline auto ibeginsWith(string_view source) const -> bool;

  inline auto endsWith(string_view source) const -> bool;
  inline auto iendsWith(string_view source) const -> bool;

  //convert.hpp
  inline auto downcase() -> type&;
  inline auto upcase() -> type&;

  inline auto qdowncase() -> type&;
  inline auto qupcase() -> type&;

  inline auto transform(string_view from, string_view to) -> type&;

  //match.hpp
  inline auto match(string_view source) const -> bool;
  inline auto imatch(string_view source) const -> bool;

  //replace.hpp
  template<bool, bool> inline auto _replace(string_view, string_view, long) -> type&;
  inline auto replace(string_view from, string_view to, long limit = LONG_MAX) -> type&;
  inline auto ireplace(string_view from, string_view to, long limit = LONG_MAX) -> type&;
  inline auto qreplace(string_view from, string_view to, long limit = LONG_MAX) -> type&;
  inline auto iqreplace(string_view from, string_view to, long limit = LONG_MAX) -> type&;

  //split.hpp
  inline auto split(string_view key, long limit = LONG_MAX) const -> string_vector;
  inline auto isplit(string_view key, long limit = LONG_MAX) const -> string_vector;
  inline auto qsplit(string_view key, long limit = LONG_MAX) const -> string_vector;
  inline auto iqsplit(string_view key, long limit = LONG_MAX) const -> string_vector;

  //trim.hpp
  inline auto trim(string_view lhs, string_view rhs, long limit = LONG_MAX) -> type&;
  inline auto trimLeft(string_view lhs, long limit = LONG_MAX) -> type&;
  inline auto trimRight(string_view rhs, long limit = LONG_MAX) -> type&;

  inline auto itrim(string_view lhs, string_view rhs, long limit = LONG_MAX) -> type&;
  inline auto itrimLeft(string_view lhs, long limit = LONG_MAX) -> type&;
  inline auto itrimRight(string_view rhs, long limit = LONG_MAX) -> type&;

  inline auto strip() -> type&;
  inline auto stripLeft() -> type&;
  inline auto stripRight() -> type&;

  //utility.hpp
  inline static auto read(string_view filename) -> string;
  inline static auto repeat(string_view pattern, uint times) -> string;
  inline auto fill(char fill = ' ') -> type&;
  inline auto hash() const -> uint;
  inline auto remove(uint offset, uint length) -> type&;
  inline auto reverse() -> type&;
  inline auto size(int length, char fill = ' ') -> type&;
};

struct string_vector : vector<string> {
  using type = string_vector;

  string_vector(const string_vector& source) { vector::operator=(source); }
  string_vector(string_vector& source) { vector::operator=(source); }
  string_vector(string_vector&& source) { vector::operator=(move(source)); }
  template<typename... P> string_vector(P&&... p) { append(forward<P>(p)...); }

  //list.hpp
  inline auto operator==(const string_vector&) const -> bool;
  inline auto operator!=(const string_vector&) const -> bool;

  inline auto operator=(const string_vector& source) -> type& { return vector::operator=(source), *this; }
  inline auto operator=(string_vector& source) -> type& { return vector::operator=(source), *this; }
  inline auto operator=(string_vector&& source) -> type& { return vector::operator=(move(source)), *this; }

  inline auto isort() -> type&;

  template<typename... P> inline auto append(const string&, P&&...) -> type&;
  inline auto append() -> type&;

  inline auto find(string_view source) const -> maybe<uint>;
  inline auto ifind(string_view source) const -> maybe<uint>;
  inline auto match(string_view pattern) const -> string_vector;
  inline auto merge(string_view separator) const -> string;
  inline auto strip() -> type&;

  //split.hpp
  template<bool, bool> inline auto _split(string_view, string_view, long) -> type&;
};

struct string_format : vector<string> {
  using type = string_format;

  template<typename... P> string_format(P&&... p) { reserve(sizeof...(p)); append(forward<P>(p)...); }
  template<typename T, typename... P> inline auto append(const T&, P&&... p) -> type&;
  inline auto append() -> type&;
};

}

#include <nall/string/view.hpp>
#include <nall/string/atoi.hpp>
#include <nall/string/cast.hpp>
#include <nall/string/compare.hpp>
#include <nall/string/convert.hpp>
#include <nall/string/core.hpp>
#include <nall/string/find.hpp>
#include <nall/string/format.hpp>
#include <nall/string/list.hpp>
#include <nall/string/match.hpp>
#include <nall/string/replace.hpp>
#include <nall/string/split.hpp>
#include <nall/string/trim.hpp>
#include <nall/string/utility.hpp>
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
