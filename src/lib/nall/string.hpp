#ifndef NALL_STRING_HPP
#define NALL_STRING_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nall/stdint.hpp>
#include <nall/vector.hpp>

//compare
char chrlower(char c);
char chrupper(char c);
int stricmp(const char *dest, const char *src);
int strpos(const char *str, const char *key);
int qstrpos(const char *str, const char *key);
bool strbegin(const char *str, const char *key);
bool stribegin(const char *str, const char *key);
bool strend(const char *str, const char *key);
bool striend(const char *str, const char *key);

//convert
char* strlower(char *str);
char* strupper(char *str);
char* strtr(char *dest, const char *before, const char *after);
uintmax_t strhex(const char *str);
intmax_t strdec(const char *str);
uintmax_t strbin(const char *str);
double strdouble(const char *str);
size_t strhex(char *str, uintmax_t value, size_t length = 0);
size_t strdec(char *str, intmax_t value, size_t length = 0);
size_t strbin(char *str, uintmax_t value, size_t length = 0);
size_t strdouble(char *str, double value, size_t length = 0);

//match
bool match(const char *pattern, const char *str);

//math
bool strint (const char *str, int &result);
bool strmath(const char *str, int &result);

//strl
size_t strlcpy(char *dest, const char *src, size_t length);
size_t strlcat(char *dest, const char *src, size_t length);

//trim
char* ltrim(char *str, const char *key = " ");
char* rtrim(char *str, const char *key = " ");
char* trim (char *str, const char *key = " ");
char* ltrim_once(char *str, const char *key = " ");
char* rtrim_once(char *str, const char *key = " ");
char* trim_once (char *str, const char *key = " ");

namespace nall {
  class string {
  public:
    char *data;
    size_t size;

    void reserve(size_t size_);

    operator int() const;
    operator const char*() const;
    char* operator()();
    char& operator[](int);

    string& operator=(int num);
    string& operator=(double num);
    string& operator=(const char *str);
    string& operator=(const string &str);
    string& operator<<(int num);
    string& operator<<(double num);
    string& operator<<(const char *str);
    string& operator<<(const string& str);

    bool operator==(const char *str) const;
    bool operator!=(const char *str) const;
    bool operator< (const char *str) const;
    bool operator<=(const char *str) const;
    bool operator> (const char *str) const;
    bool operator>=(const char *str) const;

    string();
    string(int num);
    string(double num);
    string(const char *source);
    string(const string &source);
    ~string();
  };

  typedef vector<string> lstring;
}

size_t count(nall::lstring&);
int find(nall::lstring &str, const char *key);
size_t strlcpy(nall::string &dest, const char *src, size_t length);
size_t strlcat(nall::string &dest, const char *src, size_t length);
void strcpy(nall::string &dest, const char *src);
void strcat(nall::string &dest, const char *src);
nall::string substr(const char *src, size_t start = 0, size_t length = 0);
nall::string& strlower(nall::string &str);
nall::string& strupper(nall::string &str);
nall::string& strtr(nall::string &dest, const char *before, const char *after);
nall::string strhex(uintmax_t value);
nall::string strdec(intmax_t value);
nall::string strbin(uintmax_t value);
nall::string strdouble(double value);
bool fread(nall::string &str, const char *filename);
nall::string& replace (nall::string &str, const char *key, const char *token);
nall::string& qreplace(nall::string &str, const char *key, const char *token);
void split (nall::lstring &dest, const char *key, const char *src, size_t limit = 0);
void qsplit(nall::lstring &dest, const char *key, const char *src, size_t limit = 0);
nall::string& ltrim(nall::string &str, const char *key = " ");
nall::string& rtrim(nall::string &str, const char *key = " ");
nall::string& trim (nall::string &str, const char *key = " ");
nall::string& ltrim_once(nall::string &str, const char *key = " ");
nall::string& rtrim_once(nall::string &str, const char *key = " ");
nall::string& trim_once (nall::string &str, const char *key = " ");

#endif
