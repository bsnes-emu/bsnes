#ifndef NALL_STRING_HPP
#define NALL_STRING_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nall/stdint.hpp>
#include <nall/vector.hpp>

//===============
//libc extensions
//===============

//compare.cpp
char chrlower(char c);
char chrupper(char c);

int stricmp(const char *dest, const char *src);

int strpos (const char *str, const char *key);
int qstrpos(const char *str, const char *key);

bool strbegin (const char *str, const char *key);
bool stribegin(const char *str, const char *key);

bool strend (const char *str, const char *key);
bool striend(const char *str, const char *key);

//convert.cpp
char* strlower(char *str);
char* strupper(char *str);

char* strtr(char *dest, const char *before, const char *after);

uintmax_t strhex     (const char *str);
intmax_t  strsigned  (const char *str);
uintmax_t strunsigned(const char *str);
uintmax_t strbin     (const char *str);
double    strdouble  (const char *str);

size_t strhex     (char *str, uintmax_t value, size_t length = 0);
size_t strsigned  (char *str, intmax_t  value, size_t length = 0);
size_t strunsigned(char *str, uintmax_t value, size_t length = 0);
size_t strbin     (char *str, uintmax_t value, size_t length = 0);
size_t strdouble  (char *str, double    value, size_t length = 0);

//match.cpp
bool match(const char *pattern, const char *str);

//math.cpp
bool strint (const char *str, int &result);
bool strmath(const char *str, int &result);

//strl.cpp
size_t strlcpy(char *dest, const char *src, size_t length);
size_t strlcat(char *dest, const char *src, size_t length);

//trim.cpp
char* ltrim(char *str, const char *key = " ");
char* rtrim(char *str, const char *key = " ");
char* trim (char *str, const char *key = " ");

char* ltrim_once(char *str, const char *key = " ");
char* rtrim_once(char *str, const char *key = " ");
char* trim_once (char *str, const char *key = " ");

//================
//string + lstring
//================

namespace nall {
  class string;
  template<typename T> inline string to_string(T);

  class string {
  public:
    void reserve(size_t);
    unsigned length() const;

    string& assign(const char*);
    string& append(const char*);
    template<typename T> string& operator= (T value) { return assign(to_string<T>(value)); }
    template<typename T> string& operator<<(T value) { return append(to_string<T>(value)); }

    operator const char*() const;
    char* operator()();
    char& operator[](int);

    bool operator==(const char*) const;
    bool operator!=(const char*) const;
    bool operator< (const char*) const;
    bool operator<=(const char*) const;
    bool operator> (const char*) const;
    bool operator>=(const char*) const;

    string();
    string(const char*);
    string(const string&);
    string& operator=(const string&);
    ~string();

    //core.cpp
    bool readfile(const char*);

    //replace.cpp
    string& replace (const char*, const char*);
    string& qreplace(const char*, const char*);

  protected:
    char *data;
    size_t size;
  };

  class lstring : public vector<string> {
  public:
    template<typename T> lstring& operator<<(T value) {
      operator[](size()).assign(to_string<T>(value));
      return *this;
    }

    //core.cpp
    int find(const char*);

    //split.cpp
    void split (const char*, const char*, unsigned = 0);
    void qsplit(const char*, const char*, unsigned = 0);
  };
}

//=====================
//string<>libc wrappers
//=====================

size_t strlcpy(nall::string &dest, const char *src, size_t length);
size_t strlcat(nall::string &dest, const char *src, size_t length);

nall::string& strlower(nall::string &str);
nall::string& strupper(nall::string &str);

nall::string& strtr(nall::string &dest, const char *before, const char *after);

nall::string& ltrim(nall::string &str, const char *key = " ");
nall::string& rtrim(nall::string &str, const char *key = " ");
nall::string& trim (nall::string &str, const char *key = " ");

nall::string& ltrim_once(nall::string &str, const char *key = " ");
nall::string& rtrim_once(nall::string &str, const char *key = " ");
nall::string& trim_once (nall::string &str, const char *key = " ");

//==============
//misc functions
//==============

nall::string substr(const char *src, size_t start = 0, size_t length = 0);

nall::string strhex     (uintmax_t value);
nall::string strsigned  (intmax_t  value);
nall::string strunsigned(uintmax_t value);
nall::string strbin     (uintmax_t value);
nall::string strdouble  (double    value);

namespace nall {
  //this is needed, as C++98 does not support explicit template specialization inside classes;
  //redundant memory allocation should hopefully be avoided via compiler optimizations.
  template<> inline string to_string<bool>         (bool v)          { return v ? "true" : "false"; }
  template<> inline string to_string<signed int>   (signed int v)    { return strsigned(v); }
  template<> inline string to_string<unsigned int> (unsigned int v)  { return strunsigned(v); }
  template<> inline string to_string<double>       (double v)        { return strdouble(v); }
  template<> inline string to_string<char*>        (char *v)         { return v; }
  template<> inline string to_string<const char*>  (const char *v)   { return v; }
  template<> inline string to_string<string>       (string v)        { return v; }
  template<> inline string to_string<const string&>(const string &v) { return v; }
}

#endif
