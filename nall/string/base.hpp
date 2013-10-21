#ifdef NALL_STRING_INTERNAL_HPP

namespace nall {

struct string;
struct stringref;
struct lstring;
typedef const stringref& rstring;

//#define NALL_STRING_ALLOCATOR_COPY_ON_WRITE
#define NALL_STRING_ALLOCATOR_SMALL_STRING_OPTIMIZATION
//#define NALL_STRING_ALLOCATOR_VECTOR

struct string {
protected:
  #if defined(NALL_STRING_ALLOCATOR_COPY_ON_WRITE)
  inline void _copy();
  std::shared_ptr<char> _data;
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
  //core.hpp
  inline char* data();
  inline const char* data() const;
  inline unsigned length() const;
  inline unsigned size() const;
  inline unsigned capacity() const;
  inline bool empty() const;

  inline void reset();
  inline void reserve(unsigned);
  inline void resize(unsigned);
  inline void clear(char);

  inline unsigned hash() const;

  template<typename... Args> inline string& assign(Args&&... args);
  template<typename... Args> inline string& append(Args&&... args);

  //file.hpp
  inline static string read(const string& filename);

  //datetime.hpp
  inline static string date();
  inline static string time();
  inline static string datetime();

  //replace.hpp
  template<unsigned Limit = 0> inline string& replace(rstring, rstring);
  template<unsigned Limit = 0> inline string& ireplace(rstring, rstring);
  template<unsigned Limit = 0> inline string& qreplace(rstring, rstring);
  template<unsigned Limit = 0> inline string& iqreplace(rstring, rstring);

  //wrapper.hpp
  template<unsigned Limit = 0> inline lstring split(rstring) const;
  template<unsigned Limit = 0> inline lstring isplit(rstring) const;
  template<unsigned Limit = 0> inline lstring qsplit(rstring) const;
  template<unsigned Limit = 0> inline lstring iqsplit(rstring) const;

  inline signed compare(rstring) const;
  inline signed icompare(rstring) const;

  inline bool equals(rstring) const;
  inline bool iequals(rstring) const;

  inline bool match(rstring) const;
  inline bool imatch(rstring) const;

  inline bool beginswith(rstring) const;
  inline bool ibeginswith(rstring) const;
  inline bool endswith(rstring) const;
  inline bool iendswith(rstring) const;

  inline string slice(unsigned offset, unsigned length = ~0u) const;

  inline string& lower();
  inline string& upper();
  inline string& qlower();
  inline string& qupper();
  inline string& transform(rstring before, rstring after);
  inline string& reverse();

  template<unsigned limit = 0> inline string& ltrim() { return ltrim<limit>(" "); }
  template<unsigned limit = 0> inline string& ltrim(rstring key);

  template<unsigned limit = 0> inline string& rtrim() { return rtrim<limit>(" "); }
  template<unsigned limit = 0> inline string& rtrim(rstring key);

  template<unsigned limit = 0> inline string& trim() { return trim<limit>(" "); }
  template<unsigned limit = 0> inline string& trim(rstring key);
  template<unsigned limit = 0> inline string& trim(rstring key, rstring rkey);

  inline string& strip();

  inline optional<unsigned> find(rstring key) const;
  inline optional<unsigned> ifind(rstring key) const;
  inline optional<unsigned> qfind(rstring key) const;
  inline optional<unsigned> iqfind(rstring key) const;

  //core.hpp
  inline explicit operator bool() const;
  inline operator const char*() const;
  inline char& operator[](signed);
  inline const char& operator[](signed) const;

  inline bool operator==(const char*) const;
  inline bool operator!=(const char*) const;
  inline bool operator< (const char*) const;
  inline bool operator<=(const char*) const;
  inline bool operator> (const char*) const;
  inline bool operator>=(const char*) const;

  inline string& operator=(const string&);
  inline string& operator=(string&&);

  template<typename T, typename... Args> inline string(T&& source, Args&&... args);
  inline string();
  inline string(const string&);
  inline string(string&&);
  inline ~string();

  inline char* begin() { return &data()[0]; }
  inline char* end() { return &data()[size()]; }
  inline const char* begin() const { return &data()[0]; }
  inline const char* end() const { return &data()[size()]; }

//protected:
  struct exception_out_of_bounds{};
  template<unsigned Limit, bool Insensitive, bool Quoted> inline string& ureplace(rstring, rstring);
  inline string& _append(const char*);

#if defined(QSTRING_H)
public:
  inline operator QString() const;
#endif
};

//list.hpp
struct lstring : vector<string> {
  inline optional<unsigned> find(rstring) const;
  inline string merge(const string&) const;
  inline string concatenate(const string&) const;  //deprecated
  inline lstring& isort();
  inline lstring& strip();
  inline void append() {}
  template<typename... Args> inline void append(const string&, Args&&...);

  inline bool operator==(const lstring&) const;
  inline bool operator!=(const lstring&) const;

  inline lstring& operator=(const lstring&);
  inline lstring& operator=(lstring&);
  inline lstring& operator=(lstring&&);

  template<typename... Args> inline lstring(Args&&... args);
  inline lstring(const lstring&);
  inline lstring(lstring&);
  inline lstring(lstring&&);

  //split.hpp
  template<unsigned Limit = 0> inline lstring& split(rstring, rstring);
  template<unsigned Limit = 0> inline lstring& isplit(rstring, rstring);
  template<unsigned Limit = 0> inline lstring& qsplit(rstring, rstring);
  template<unsigned Limit = 0> inline lstring& iqsplit(rstring, rstring);

protected:
  template<unsigned Limit, bool Insensitive, bool Quoted> inline lstring& usplit(rstring, rstring);
};

//filename.hpp
inline string dir(string name);
inline string notdir(string name);
inline string parentdir(string name);
inline string basename(string name);
inline string extension(string name);

//format.hpp
template<signed precision = 0, char padchar = ' '> inline string format(const string& value);
template<signed precision = 0, char padchar = '0'> inline string hex(uintmax_t value);
template<signed precision = 0, char padchar = '0'> inline string octal(uintmax_t value);
template<signed precision = 0, char padchar = '0'> inline string binary(uintmax_t value);

//platform.hpp
inline string activepath();
inline string realpath(const string& name);
inline string userpath();
inline string configpath();
inline string sharedpath();
inline string temppath();

//utility.hpp
inline string substr(rstring source, unsigned offset = 0, unsigned length = ~0u);
inline string sha256(const uint8_t* data, unsigned size);
inline bool tokenize(lstring& list, const char* s, const char* p);

inline char* integer(char* result, intmax_t value);
inline char* decimal(char* result, uintmax_t value);

inline unsigned real(char* str, long double value);
inline string real(long double value);

//variadic.hpp
inline void sprint(string& output);
template<typename T, typename... Args> inline void sprint(string& output, const T& value, Args&&... args);
template<typename... Args> inline void print(Args&&... args);

}

#endif
