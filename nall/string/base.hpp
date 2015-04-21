#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

struct string;
struct format;
struct stringref;
struct lstring;
using cstring = const string&;
using rstring = const stringref&;

#define NALL_STRING_ALLOCATOR_ADAPTIVE
//#define NALL_STRING_ALLOCATOR_COPY_ON_WRITE
//#define NALL_STRING_ALLOCATOR_SMALL_STRING_OPTIMIZATION
//#define NALL_STRING_ALLOCATOR_VECTOR

//cast.hpp
template<typename T> struct stringify;

//compare.hpp
inline auto compare(const string& self, rstring source) -> signed;
inline auto icompare(const string& self, rstring source) -> signed;

inline auto equals(const string& self, rstring source) -> bool;
inline auto iequals(const string& self, rstring source) -> bool;

inline auto beginsWith(const string& self, rstring source) -> bool;
inline auto ibeginsWith(const string& self, rstring source) -> bool;

inline auto endsWith(const string& self, rstring source) -> bool;
inline auto iendsWith(const string& self, rstring source) -> bool;

//convert.hpp
inline auto downcase(string& self) -> string&;
inline auto qdowncase(string& self) -> string&;

inline auto upcase(string& self) -> string&;
inline auto qupcase(string& self) -> string&;

inline auto transform(string& self, rstring from, rstring to) -> string&;

//core.hpp
template<typename... P> inline auto assign(string& self, P&&... p) -> string&;
template<typename T, typename... P> inline auto append(string& self, const T& value, P&&... p) -> string&;
template<typename... P> inline auto append(string& self, const format& value, P&&... p) -> string&;
inline auto append(string& self) -> string&;
template<typename T> inline auto _append(string& self, const stringify<T>& source) -> string&;
inline auto empty(const string& self) -> bool;
inline auto length(const string& self) -> unsigned;

//find.hpp
template<bool I, bool Q> inline auto _find(const string& self, signed offset, rstring source) -> maybe<unsigned>;
inline auto find(const string& self, rstring source) -> maybe<unsigned>;
inline auto ifind(const string& self, rstring source) -> maybe<unsigned>;
inline auto qfind(const string& self, rstring source) -> maybe<unsigned>;
inline auto iqfind(const string& self, rstring source) -> maybe<unsigned>;
inline auto findFrom(const string& self, signed offset, rstring source) -> maybe<unsigned>;
inline auto ifindFrom(const string& self, signed offset, rstring source) -> maybe<unsigned>;

//format.hpp
template<typename... P> inline auto print(P&&...) -> void;
template<signed precision = 0, char padchar = '0'> inline auto integer(intmax_t value) -> string;
template<signed precision = 0, char padchar = '0'> inline auto decimal(uintmax_t value) -> string;
template<signed precision = 0, char padchar = '0'> inline auto hex(uintmax_t value) -> string;
template<signed precision = 0, char padchar = '0'> inline auto octal(uintmax_t value) -> string;
template<signed precision = 0, char padchar = '0'> inline auto binary(uintmax_t value) -> string;
template<signed precision = 0, typename T> inline auto pointer(const T* value) -> string;
template<signed precision = 0> inline auto pointer(uintptr_t value) -> string;
inline auto real(long double value) -> string;

//hash.hpp
inline auto crc16(const string& self) -> string;
inline auto crc32(const string& self) -> string;
inline auto sha256(const string& self) -> string;

//match.hpp
inline auto match(const string& self, rstring source) -> bool;
inline auto imatch(const string& self, rstring source) -> bool;
inline auto tokenize(lstring& list, const char* s, const char* p) -> bool;

//path.hpp
inline auto pathname(const string& self) -> string;
inline auto filename(const string& self) -> string;

inline auto dirname(const string& self) -> string;
inline auto basename(const string& self) -> string;
inline auto prefixname(const string& self) -> string;
inline auto suffixname(const string& self) -> string;

//platform.hpp
inline auto activepath() -> string;
inline auto realpath(rstring name) -> string;
inline auto programpath() -> string;
inline auto userpath() -> string;
inline auto configpath() -> string;
inline auto localpath() -> string;
inline auto sharedpath() -> string;
inline auto temppath() -> string;

//replace.hpp
template<unsigned L, bool I, bool Q> inline auto _replace(string& self, rstring from, rstring to) -> string&;
template<unsigned L = ~0u> inline auto replace(string& self, rstring from, rstring to) -> string&;
template<unsigned L = ~0u> inline auto ireplace(string& self, rstring from, rstring to) -> string&;
template<unsigned L = ~0u> inline auto qreplace(string& self, rstring from, rstring to) -> string&;
template<unsigned L = ~0u> inline auto iqreplace(string& self, rstring from, rstring to) -> string&;

//split.hpp
template<unsigned L, bool I, bool Q> inline auto _split(lstring& self, rstring source, rstring find) -> lstring&;
template<unsigned L = ~0u> inline auto split(string& self, rstring key) -> lstring;
template<unsigned L = ~0u> inline auto isplit(string& self, rstring key) -> lstring;
template<unsigned L = ~0u> inline auto qsplit(string& self, rstring key) -> lstring;
template<unsigned L = ~0u> inline auto iqsplit(string& self, rstring key) -> lstring;

//trim.hpp
inline auto trim(string& self, rstring lhs, rstring rhs) -> bool;
inline auto ltrim(string& self, rstring lhs) -> bool;
inline auto rtrim(string& self, rstring rhs) -> bool;

inline auto itrim(string& self, rstring lhs, rstring rhs) -> bool;
inline auto iltrim(string& self, rstring lhs) -> bool;
inline auto irtrim(string& self, rstring rhs) -> bool;

inline auto strip(string& self) -> bool;
inline auto lstrip(string& self) -> bool;
inline auto rstrip(string& self) -> bool;

//utility.hpp
inline auto fill(string& self, char fill = ' ') -> string&;
inline auto hash(const string& self) -> unsigned;
inline auto remove(string& self, unsigned offset, unsigned length) -> string&;
inline auto reverse(string& self) -> string&;
inline auto size(string& self, signed length, char fill = ' ') -> string&;
inline auto slice(const string& self, signed offset = 0, signed length = -1) -> string;
inline auto substr(rstring source, signed offset = 0, signed length = -1) -> string;

inline auto integer(char* result, intmax_t value) -> char*;
inline auto decimal(char* result, uintmax_t value) -> char*;
inline auto real(char* str, long double value) -> unsigned;

struct string {
  using type = string;
  struct exception_out_of_bounds{};

protected:
  #if defined(NALL_STRING_ALLOCATOR_ADAPTIVE)
  enum : unsigned { SSO = 24 };
  union {
    struct {  //copy-on-write
      char* _data;
      unsigned* _refs;
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
  mutable unsigned* _refs;
  inline auto _allocate() -> char*;
  inline auto _copy() -> char*;
  #endif

  #if defined(NALL_STRING_ALLOCATOR_SMALL_STRING_OPTIMIZATION)
  enum : unsigned { SSO = 24 };
  union {
    char* _data;
    char _text[SSO];
  };
  #endif

  #if defined(NALL_STRING_ALLOCATOR_VECTOR)
  char* _data;
  #endif

  unsigned _capacity;
  unsigned _size;

public:
  inline string();
  inline auto pointer() -> char*;
  inline auto data() const -> const char*;
  inline auto reset() -> type&;
  inline auto reserve(unsigned) -> type&;
  inline auto resize(unsigned) -> type&;
  inline auto operator=(const string&) -> type&;
  inline auto operator=(string&&) -> type&;

  template<typename T, typename... P> string(T&& s, P&&... p) : string() { append(std::forward<T>(s), std::forward<P>(p)...); }
  ~string() { reset(); }

  explicit operator bool() const { return _size; }
  operator const uint8_t*() const { return (const uint8_t*)data(); }
  operator const char*() const { return (const char*)data(); }

  auto binary() const -> const uint8_t* { return (const uint8_t*)data(); }
  auto size() const -> unsigned { return _size; }
  auto capacity() const -> unsigned { return _capacity; }

  auto operator==(const string& s) const -> bool { return size() == s.size() && memory::compare(data(), s.data(), s.size()) == 0; }
  auto operator!=(const string& s) const -> bool { return size() != s.size() || memory::compare(data(), s.data(), s.size()) != 0; }

  auto operator==(const char* s) const -> bool { return strcmp(data(), s) == 0; }
  auto operator!=(const char* s) const -> bool { return strcmp(data(), s) != 0; }
  auto operator< (const char* s) const -> bool { return strcmp(data(), s) <  0; }
  auto operator<=(const char* s) const -> bool { return strcmp(data(), s) <= 0; }
  auto operator> (const char* s) const -> bool { return strcmp(data(), s) >  0; }
  auto operator>=(const char* s) const -> bool { return strcmp(data(), s) >= 0; }

  string(const string& source) : string() { operator=(source); }
  string(string&& source) : string() { operator=(std::move(source)); }

  auto begin() -> char* { return &pointer()[0]; }
  auto end() -> char* { return &pointer()[size()]; }
  auto begin() const -> const char* { return &data()[0]; }
  auto end() const -> const char* { return &data()[size()]; }

  //nall/atoi.hpp
  inline auto integer() const -> intmax_t { return nall::integer(*this); }
  inline auto decimal() const -> uintmax_t { return nall::decimal(*this); }
  inline auto hex() const -> uintmax_t { return nall::hex(*this); }

  //core.hpp
  inline auto operator[](signed) const -> const char&;

  //datetime.hpp
  inline static auto date(time_t = 0) -> string;
  inline static auto time(time_t = 0) -> string;
  inline static auto datetime(time_t = 0) -> string;

  //format.hpp
  inline auto format(const nall::format& params) -> type&;

  //utility.hpp
  inline static auto read(const string& filename) -> string;
  template<unsigned L> inline static auto repeat(const string& pattern) -> string;

  //extension methods
  //=================

  //compare.hpp
  auto compare(rstring source) const -> signed { return nall::compare(*this, source); }
  auto icompare(rstring source) const -> signed { return nall::compare(*this, source); }

  auto equals(rstring source) const -> bool { return nall::equals(*this, source); }
  auto iequals(rstring source) const -> bool { return nall::iequals(*this, source); }

  auto beginsWith(rstring source) const -> bool { return nall::beginsWith(*this, source); }
  auto ibeginsWith(rstring source) const -> bool { return nall::ibeginsWith(*this, source); }

  auto endsWith(rstring source) const -> bool { return nall::endsWith(*this, source); }
  auto iendsWith(rstring source) const -> bool { return nall::iendsWith(*this, source); }

  //convert.hpp
  auto downcase() -> type& { return nall::downcase(*this); }
  auto upcase() -> type& { return nall::upcase(*this); }

  auto qdowncase() -> type& { return nall::qdowncase(*this); }
  auto qupcase() -> type& { return nall::qupcase(*this); }

  auto transform(rstring from, rstring to) -> type& { return nall::transform(*this, from, to); }

  //core.hpp
  template<typename... P> auto assign(P&&... p) -> type& { return nall::assign(*this, std::forward<P>(p)...); }
  template<typename... P> auto append(P&&... p) -> type& { return nall::append(*this, std::forward<P>(p)...); }
  auto empty() const -> bool { return nall::empty(*this); }
  auto length() const -> unsigned { return nall::length(*this); }

  //find.hpp
  auto find(rstring source) const -> maybe<unsigned> { return nall::find(*this, source); }
  auto ifind(rstring source) const -> maybe<unsigned> { return nall::ifind(*this, source); }
  auto qfind(rstring source) const -> maybe<unsigned> { return nall::qfind(*this, source); }
  auto iqfind(rstring source) const -> maybe<unsigned> { return nall::iqfind(*this, source); }

  auto findFrom(signed offset, rstring source) const -> maybe<unsigned> { return nall::findFrom(*this, offset, source); }
  auto ifindFrom(signed offset, rstring source) const -> maybe<unsigned> { return nall::ifindFrom(*this, offset, source); }

  //hash.hpp
  auto crc16() const -> string { return nall::crc16(*this); }
  auto crc32() const -> string { return nall::crc32(*this); }
  auto sha256() const -> string { return nall::sha256(*this); }

  //match.hpp
  auto match(rstring source) const -> bool { return nall::match(*this, source); }
  auto imatch(rstring source) const -> bool { return nall::imatch(*this, source); }

  //path.hpp
  auto pathname() const -> string { return nall::pathname(*this); }
  auto filename() const -> string { return nall::filename(*this); }

  auto dirname() const -> string { return nall::dirname(*this); }
  auto basename() const -> string { return nall::basename(*this); }
  auto prefixname() const -> string { return nall::prefixname(*this); }
  auto suffixname() const -> string { return nall::suffixname(*this); }

  //replace.hpp
  template<unsigned L = ~0u> auto replace(rstring from, rstring to) -> type& { return nall::_replace<L, 0, 0>(*this, from, to); }
  template<unsigned L = ~0u> auto ireplace(rstring from, rstring to) -> type& { return nall::_replace<L, 1, 0>(*this, from, to); }
  template<unsigned L = ~0u> auto qreplace(rstring from, rstring to) -> type& { return nall::_replace<L, 0, 1>(*this, from, to); }
  template<unsigned L = ~0u> auto iqreplace(rstring from, rstring to) -> type& { return nall::_replace<L, 1, 1>(*this, from, to); }

  //split.hpp
  template<unsigned L = ~0u> inline auto split(rstring key) const -> lstring;
  template<unsigned L = ~0u> inline auto isplit(rstring key) const -> lstring;
  template<unsigned L = ~0u> inline auto qsplit(rstring key) const -> lstring;
  template<unsigned L = ~0u> inline auto iqsplit(rstring key) const -> lstring;

  //trim.hpp
  auto trim(rstring lhs, rstring rhs) -> type& { return nall::trim(*this, lhs, rhs), *this; }
  auto ltrim(rstring lhs) -> type& { return nall::ltrim(*this, lhs), *this; }
  auto rtrim(rstring rhs) -> type& { return nall::rtrim(*this, rhs), *this; }

  auto itrim(rstring lhs, rstring rhs) -> type& { return nall::itrim(*this, lhs, rhs), *this; }
  auto iltrim(rstring lhs) -> type& { return nall::iltrim(*this, lhs), *this; }
  auto irtrim(rstring rhs) -> type& { return nall::irtrim(*this, rhs), *this; }

  auto strip() -> type& { return nall::strip(*this), *this; }
  auto lstrip() -> type& { return nall::lstrip(*this), *this; }
  auto rstrip() -> type& { return nall::rstrip(*this), *this; }

  //utility.hpp
  auto fill(char fill = ' ') -> type& { return nall::fill(*this, fill); }
  auto hash() const -> const type& { return nall::hash(*this), *this; }
  auto remove(unsigned offset, unsigned length) -> type& { return nall::remove(*this, offset, length); }
  auto reverse() -> type& { return nall::reverse(*this); }
  auto size(signed length, char fill = ' ') -> type& { return nall::size(*this, length, fill); }
  auto slice(signed offset = 0, signed length = -1) const -> string { return nall::slice(*this, offset, length); }

  #if defined(QSTRING_H)
  inline operator QString() const;
  #endif
};

//list.hpp
template<typename... P> auto append(lstring& self, const string& value, P&&... p) -> lstring&;
inline auto append(lstring& self) -> lstring&;
inline auto find(const lstring& self, const string& source) -> maybe<unsigned>;
inline auto ifind(const lstring& self, const string& source) -> maybe<unsigned>;
inline auto merge(const lstring& self, const string& separator) -> string;
inline auto strip(lstring& self) -> lstring&;

struct lstring : vector<string> {
  using type = lstring;

  lstring(const lstring& source) { vector::operator=(source); }
  lstring(lstring& source) { vector::operator=(source); }
  lstring(lstring&& source) { vector::operator=(std::move(source)); }
  template<typename... P> lstring(P&&... p) { append(std::forward<P>(p)...); }

  //list.hpp
  inline auto operator==(const lstring&) const -> bool;
  inline auto operator!=(const lstring&) const -> bool;

  inline auto operator=(const lstring& source) -> type& { return vector::operator=(source), *this; }
  inline auto operator=(lstring& source) -> type& { return vector::operator=(source), *this; }
  inline auto operator=(lstring&& source) -> type& { return vector::operator=(std::move(source)), *this; }

  inline auto isort() -> type&;

  //extension methods
  //=================

  //list.hpp
  template<typename... P> auto append(P&&... p) -> type& { return nall::append(*this, std::forward<P>(p)...); }
  auto find(const string& source) const -> maybe<unsigned> { return nall::find(*this, source); }
  auto ifind(const string& source) const -> maybe<unsigned> { return nall::ifind(*this, source); }
  auto merge(const string& separator) const -> string { return nall::merge(*this, separator); }
  auto strip() -> type& { return nall::strip(*this); }

  //split.hpp
  template<unsigned L = ~0u> auto split(rstring source, rstring on) -> type& { return nall::_split<L, 0, 0>(*this, source, on); }
  template<unsigned L = ~0u> auto isplit(rstring source, rstring on) -> type& { return nall::_split<L, 1, 0>(*this, source, on); }
  template<unsigned L = ~0u> auto qsplit(rstring source, rstring on) -> type& { return nall::_split<L, 0, 1>(*this, source, on); }
  template<unsigned L = ~0u> auto iqsplit(rstring source, rstring on) -> type& { return nall::_split<L, 1, 1>(*this, source, on); }
};

//format.hpp
template<typename T, typename... P> inline auto append(format& self, const T& value, P&&... p) -> format&;
inline auto append(format& self) -> format&;

struct format : vector<string> {
  using type = format;

  template<typename... P> format(P&&... p) { reserve(sizeof...(p)); append(std::forward<P>(p)...); }
  template<typename... P> auto append(P&&... p) -> type& { return nall::append(*this, std::forward<P>(p)...); }
};

}

#endif
