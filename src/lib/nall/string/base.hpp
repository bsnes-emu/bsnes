#ifndef NALL_STRING_BASE_HPP
#define NALL_STRING_BASE_HPP

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nall/concept.hpp>
#include <nall/stdint.hpp>
#include <nall/utf8.hpp>
#include <nall/vector.hpp>

namespace nall {
  class string;
  template<typename T> inline string to_string(T);

  class string {
  public:
    inline void reserve(unsigned);
    inline unsigned length() const;

    inline string& assign(const char*);
    inline string& append(const char*);
    template<typename T> inline string& operator= (T value);
    template<typename T> inline string& operator<<(T value);

    inline operator const char*() const;
    inline char* operator()();
    inline char& operator[](int);

    inline bool operator==(const char*) const;
    inline bool operator!=(const char*) const;
    inline bool operator< (const char*) const;
    inline bool operator<=(const char*) const;
    inline bool operator> (const char*) const;
    inline bool operator>=(const char*) const;

    inline string();
    inline string(const char*);
    inline string(const string&);
    inline string(string&&);
    inline string& operator=(const string&);
    inline string& operator=(string&&);
    inline ~string();

    inline bool readfile(const char*);
    inline string& replace (const char*, const char*);
    inline string& qreplace(const char*, const char*);

  protected:
    char *data;
    unsigned size;

  #if defined(QT_CORE_LIB)
  public:
    inline operator QString() const;
  #endif
  };

  class lstring : public linear_vector<string> {
  public:
    template<typename T> inline lstring& operator<<(T value);

    inline int find(const char*);
    inline void split (const char*, const char*, unsigned = 0);
    inline void qsplit(const char*, const char*, unsigned = 0);

    lstring();
    lstring(std::initializer_list<string>);
  };

  //compare.hpp
  inline char chrlower(char c);
  inline char chrupper(char c);
  inline int stricmp(const char *dest, const char *src);
  inline int strpos (const char *str, const char *key);
  inline int qstrpos(const char *str, const char *key);
  inline bool strbegin (const char *str, const char *key);
  inline bool stribegin(const char *str, const char *key);
  inline bool strend (const char *str, const char *key);
  inline bool striend(const char *str, const char *key);

  //convert.hpp
  inline char* strlower(char *str);
  inline char* strupper(char *str);
  inline char* strtr(char *dest, const char *before, const char *after);
  inline uintmax_t strhex     (const char *str);
  inline intmax_t  strsigned  (const char *str);
  inline uintmax_t strunsigned(const char *str);
  inline uintmax_t strbin     (const char *str);
  inline double    strdouble  (const char *str);

  //match.hpp
  inline bool match(const char *pattern, const char *str);

  //math.hpp
  inline bool strint (const char *str, int &result);
  inline bool strmath(const char *str, int &result);

  //strl.hpp
  inline unsigned strlcpy(char *dest, const char *src, unsigned length);
  inline unsigned strlcat(char *dest, const char *src, unsigned length);

  //trim.hpp
  inline char* ltrim(char *str, const char *key = " ");
  inline char* rtrim(char *str, const char *key = " ");
  inline char* trim (char *str, const char *key = " ");
  inline char* ltrim_once(char *str, const char *key = " ");
  inline char* rtrim_once(char *str, const char *key = " ");
  inline char* trim_once (char *str, const char *key = " ");

  //utility.hpp
  inline unsigned strlcpy(string &dest, const char *src, unsigned length);
  inline unsigned strlcat(string &dest, const char *src, unsigned length);
  inline string substr(const char *src, unsigned start = 0, unsigned length = 0);
  inline string& strlower(string &str);
  inline string& strupper(string &str);
  inline string& strtr(string &dest, const char *before, const char *after);
  inline string& ltrim(string &str, const char *key = " ");
  inline string& rtrim(string &str, const char *key = " ");
  inline string& trim (string &str, const char *key = " ");
  inline string& ltrim_once(string &str, const char *key = " ");
  inline string& rtrim_once(string &str, const char *key = " ");
  inline string& trim_once (string &str, const char *key = " ");
  template<unsigned length = 0, char padding = '0'> inline string strhex(uintmax_t value);
  template<unsigned length = 0, char padding = '0'> inline string strsigned(intmax_t value);
  template<unsigned length = 0, char padding = '0'> inline string strunsigned(uintmax_t value);
  template<unsigned length = 0, char padding = '0'> inline string strbin(uintmax_t value);
  inline unsigned strdouble(char *str, double value);
  inline string strdouble(double value);

  //variadic.hpp
  template<typename... Args> inline string sprint(Args... args);
  template<typename... Args> inline void print(Args... args);
};

#endif
