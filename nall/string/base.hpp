#pragma once

namespace nall {

struct string_view;
struct string;
struct format;
struct lstring;

using rstring = const string_view&;
using cstring = const string&;

#define NALL_STRING_ALLOCATOR_ADAPTIVE
//#define NALL_STRING_ALLOCATOR_COPY_ON_WRITE
//#define NALL_STRING_ALLOCATOR_SMALL_STRING_OPTIMIZATION
//#define NALL_STRING_ALLOCATOR_VECTOR

//cast.hpp
template<typename T> struct stringify;

//format.hpp
template<typename... P> inline auto print(P&&...) -> void;
inline auto integer(intmax value, long precision = 0, char padchar = '0') -> string;
inline auto natural(uintmax value, long precision = 0, char padchar = '0') -> string;
inline auto hex(uintmax value, long precision = 0, char padchar = '0') -> string;
inline auto octal(uintmax value, long precision = 0, char padchar = '0') -> string;
inline auto binary(uintmax value, long precision = 0, char padchar = '0') -> string;
template<typename T> inline auto pointer(const T* value, long precision = 0) -> string;
inline auto pointer(uintptr value, long precision = 0) -> string;
inline auto real(long double value) -> string;

//hash.hpp
inline auto crc16(rstring self) -> string;
inline auto crc32(rstring self) -> string;
inline auto crc64(rstring self) -> string;
inline auto sha256(rstring self) -> string;

//match.hpp
inline auto tokenize(const char* s, const char* p) -> bool;
inline auto tokenize(lstring& list, const char* s, const char* p) -> bool;

//path.hpp
inline auto pathname(rstring self) -> string;
inline auto filename(rstring self) -> string;

inline auto dirname(rstring self) -> string;
inline auto basename(rstring self) -> string;
inline auto prefixname(rstring self) -> string;
inline auto suffixname(rstring self) -> string;

//platform.hpp
inline auto activepath() -> string;
inline auto realpath(rstring name) -> string;
inline auto programpath() -> string;
inline auto rootpath() -> string;
inline auto userpath() -> string;
inline auto configpath() -> string;
inline auto localpath() -> string;
inline auto sharedpath() -> string;
inline auto temppath() -> string;

//utility.hpp
inline auto slice(rstring self, int offset = 0, int length = -1) -> string;

inline auto integer(char* result, intmax value) -> char*;
inline auto natural(char* result, uintmax value) -> char*;
inline auto real(char* str, long double value) -> uint;

struct string {
  using type = string;
  struct exception_out_of_bounds{};

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
  inline auto get() -> char*;
  inline auto data() const -> const char*;
  inline auto reset() -> type&;
  inline auto reserve(uint) -> type&;
  inline auto resize(uint) -> type&;
  inline auto operator=(const string&) -> type&;
  inline auto operator=(string&&) -> type&;

  template<typename T, typename... P> string(T&& s, P&&... p) : string() { append(forward<T>(s), forward<P>(p)...); }
  ~string() { reset(); }

  explicit operator bool() const { return _size; }
  operator const uint8_t*() const { return (const uint8_t*)data(); }
  operator const char*() const { return (const char*)data(); }

  auto binary() const -> const uint8_t* { return (const uint8_t*)data(); }
  auto size() const -> uint { return _size; }
  auto capacity() const -> uint { return _capacity; }

  auto operator==(const string& source) const -> bool { return size() == source.size() && memory::compare(data(), source.data(), size()) == 0; }
  auto operator!=(const string& source) const -> bool { return size() != source.size() || memory::compare(data(), source.data(), size()) != 0; }

  auto operator==(const char* source) const -> bool { return strcmp(data(), source) == 0; }
  auto operator!=(const char* source) const -> bool { return strcmp(data(), source) != 0; }

  auto operator==(rstring source) const -> bool { return compare(source) == 0; }
  auto operator!=(rstring source) const -> bool { return compare(source) != 0; }
  auto operator< (rstring source) const -> bool { return compare(source) <  0; }
  auto operator<=(rstring source) const -> bool { return compare(source) <= 0; }
  auto operator> (rstring source) const -> bool { return compare(source) >  0; }
  auto operator>=(rstring source) const -> bool { return compare(source) >= 0; }

  string(const string& source) : string() { operator=(source); }
  string(string&& source) : string() { operator=(move(source)); }

  auto begin() -> char* { return &get()[0]; }
  auto end() -> char* { return &get()[size()]; }
  auto begin() const -> const char* { return &data()[0]; }
  auto end() const -> const char* { return &data()[size()]; }

  //atoi.hpp
  inline auto integer() const -> intmax;
  inline auto natural() const -> uintmax;
  inline auto real() const -> double;

  //core.hpp
  inline auto operator[](int) const -> const char&;
  template<typename... P> inline auto assign(P&&...) -> type&;
  template<typename T, typename... P> inline auto append(const T&, P&&...) -> type&;
  template<typename... P> inline auto append(const nall::format&, P&&...) -> type&;
  inline auto append() -> type&;
  template<typename T> inline auto _append(const stringify<T>&) -> string&;
  inline auto empty() const -> bool;
  inline auto length() const -> uint;

  //datetime.hpp
  inline static auto date(time_t = 0) -> string;
  inline static auto time(time_t = 0) -> string;
  inline static auto datetime(time_t = 0) -> string;

  //find.hpp
  template<bool, bool> inline auto _find(int, rstring) const -> maybe<uint>;

  inline auto find(rstring source) const -> maybe<unsigned>;
  inline auto ifind(rstring source) const -> maybe<unsigned>;
  inline auto qfind(rstring source) const -> maybe<unsigned>;
  inline auto iqfind(rstring source) const -> maybe<unsigned>;

  inline auto findFrom(int offset, rstring source) const -> maybe<uint>;
  inline auto ifindFrom(int offset, rstring source) const -> maybe<uint>;

  //format.hpp
  inline auto format(const nall::format& params) -> type&;

  //compare.hpp
  template<bool> inline static auto _compare(const char*, uint, const char*, uint) -> signed;

  inline static auto compare(rstring, rstring) -> signed;
  inline static auto icompare(rstring, rstring) -> signed;

  inline auto compare(rstring source) const -> signed;
  inline auto icompare(rstring source) const -> signed;

  inline auto equals(rstring source) const -> bool;
  inline auto iequals(rstring source) const -> bool;

  inline auto beginsWith(rstring source) const -> bool;
  inline auto ibeginsWith(rstring source) const -> bool;

  inline auto endsWith(rstring source) const -> bool;
  inline auto iendsWith(rstring source) const -> bool;

  //convert.hpp
  inline auto downcase() -> type&;
  inline auto upcase() -> type&;

  inline auto qdowncase() -> type&;
  inline auto qupcase() -> type&;

  inline auto transform(rstring from, rstring to) -> type&;

  //match.hpp
  inline auto match(rstring source) const -> bool;
  inline auto imatch(rstring source) const -> bool;

  //replace.hpp
  template<bool, bool> inline auto _replace(rstring, rstring, long) -> type&;
  inline auto replace(rstring from, rstring to, long limit = LONG_MAX) -> type&;
  inline auto ireplace(rstring from, rstring to, long limit = LONG_MAX) -> type&;
  inline auto qreplace(rstring from, rstring to, long limit = LONG_MAX) -> type&;
  inline auto iqreplace(rstring from, rstring to, long limit = LONG_MAX) -> type&;

  //split.hpp
  inline auto split(rstring key, long limit = LONG_MAX) const -> lstring;
  inline auto isplit(rstring key, long limit = LONG_MAX) const -> lstring;
  inline auto qsplit(rstring key, long limit = LONG_MAX) const -> lstring;
  inline auto iqsplit(rstring key, long limit = LONG_MAX) const -> lstring;

  //trim.hpp
  inline auto trim(rstring lhs, rstring rhs, long limit = LONG_MAX) -> type&;
  inline auto ltrim(rstring lhs, long limit = LONG_MAX) -> type&;
  inline auto rtrim(rstring rhs, long limit = LONG_MAX) -> type&;

  inline auto itrim(rstring lhs, rstring rhs, long limit = LONG_MAX) -> type&;
  inline auto iltrim(rstring lhs, long limit = LONG_MAX) -> type&;
  inline auto irtrim(rstring rhs, long limit = LONG_MAX) -> type&;

  inline auto strip() -> type&;
  inline auto lstrip() -> type&;
  inline auto rstrip() -> type&;

  //utility.hpp
  inline static auto read(rstring filename) -> string;
  inline static auto repeat(rstring pattern, uint times) -> string;
  inline auto fill(char fill = ' ') -> type&;
  inline auto hash() const -> uint;
  inline auto remove(uint offset, uint length) -> type&;
  inline auto reverse() -> type&;
  inline auto size(int length, char fill = ' ') -> type&;
};

struct lstring : vector<string> {
  using type = lstring;

  lstring(const lstring& source) { vector::operator=(source); }
  lstring(lstring& source) { vector::operator=(source); }
  lstring(lstring&& source) { vector::operator=(move(source)); }
  template<typename... P> lstring(P&&... p) { append(forward<P>(p)...); }

  //list.hpp
  inline auto operator==(const lstring&) const -> bool;
  inline auto operator!=(const lstring&) const -> bool;

  inline auto operator=(const lstring& source) -> type& { return vector::operator=(source), *this; }
  inline auto operator=(lstring& source) -> type& { return vector::operator=(source), *this; }
  inline auto operator=(lstring&& source) -> type& { return vector::operator=(move(source)), *this; }

  inline auto isort() -> type&;

  template<typename... P> inline auto append(const string&, P&&...) -> type&;
  inline auto append() -> type&;

  inline auto find(rstring source) const -> maybe<uint>;
  inline auto ifind(rstring source) const -> maybe<uint>;
  inline auto match(rstring pattern) const -> lstring;
  inline auto merge(rstring separator) const -> string;
  inline auto strip() -> type&;

  //split.hpp
  template<bool, bool> inline auto _split(rstring, rstring, long) -> lstring&;
};

struct format : vector<string> {
  using type = format;

  template<typename... P> format(P&&... p) { reserve(sizeof...(p)); append(forward<P>(p)...); }
  template<typename T, typename... P> inline auto append(const T&, P&&... p) -> type&;
  inline auto append() -> type&;
};

}
