/*
  libstring : version 0.18 ~byuu (2007-06-06)
*/

#ifndef LIBSTRING_H
#define LIBSTRING_H

#include "libbase.h"
#include "libvector.h"

class string;
typedef linear_vector<string> stringarray;

char chrlower(char c);
char chrupper(char c);

uint count(stringarray &str);

int find(stringarray &str, const char *key);
int find(stringarray &str, const string &key);

char *strptr(const string &str);
uint  strlen(const string &str);

int strcmp(const string &dest, const char *src);
int strcmp(const char *dest, const string &src);
int strcmp(const string &dest, const string &src);

//vc6/win32 and gcc/dos only support stricmp, whereas
//gcc/unix only supports strcasecmp. this is an attempt
//to avoid platform-specific defines...
#define stricmp __stricmp
int __stricmp(const char *dest, const char *src);
int stricmp(const string &dest, const char *src);
int stricmp(const char *dest, const string &src);
int stricmp(const string &dest, const string &src);

void strcpy(string &dest, const char *src);
void strcpy(string &dest, const string &src);
uint strlcpy(char *dest, const char *src, uint length);
uint strlcpy(string &dest, const char *src, uint length);
uint strlcpy(string &dest, const string &src, uint length);

void strcat(string &dest, const char *src);
void strcat(string &dest, const string &src);
uint strlcat(char *dest, const char *src, uint length);
uint strlcat(string &dest, const char *src, uint length);
uint strlcat(string &dest, const string &src, uint length);

string substr(string &dest, const char *src, uint start = 0, uint length = 0);
string substr(string &dest, const string &src, uint start = 0, uint length = 0);

void strinsert(string &dest, const char *src, uint pos);
void strinsert(string &dest, const string &src, uint pos);

void strremove(string &dest, uint start, uint length = 0);

char   *strlower(char *str);
string &strlower(string &str);

char   *strupper(char *str);
string &strupper(string &str);

int strpos(const char   *str, const char   *key);
int strpos(const string &str, const char   *key);
int strpos(const char   *str, const string &key);
int strpos(const string &str, const string &key);

int qstrpos(const char   *str, const char   *key);
int qstrpos(const string &str, const char   *key);
int qstrpos(const char   *str, const string &key);
int qstrpos(const string &str, const string &key);

void strtr(char *dest, const char *before, const char *after);
void strtr(string &dest, const char *before, const char *after);

bool strbegin(const char   *str, const char *key);
bool strbegin(const string &str, const char *key);

bool stribegin(const char   *str, const char *key);
bool stribegin(const string &str, const char *key);

bool strend(const char   *str, const char *key);
bool strend(const string &str, const char *key);

bool striend(const char   *str, const char *key);
bool striend(const string &str, const char *key);

void strltrim(char *str, const char *key);
void strltrim(string &str, const char *key);

void striltrim(char *str, const char *key);
void striltrim(string &str, const char *key);

void strrtrim(char *str, const char *key);
void strrtrim(string &str, const char *key);

void strirtrim(char *str, const char *key);
void strirtrim(string &str, const char *key);

void strtrim(char *str, const char *key);
void strtrim(string &str, const char *key);

void stritrim(char *str, const char *key);
void stritrim(string &str, const char *key);

void strquote(string &str);

bool strunquote(char *str);
bool strunquote(string &str);

uint strhex(const char   *str);
uint strhex(const string &str);

uint strdec(const char   *str);
uint strdec(const string &str);

uint strbin(const char   *str);
uint strbin(const string &str);

char   *utoa(char *str, uint num);
string &utoa(string &str, uint num);

char   *itoa(char *str, uint num);
string &itoa(string &str, uint num);

char   *htoa(char *str, uint num);
string &htoa(string &str, uint num);

char   *btoa(char *str, uint num);
string &btoa(string &str, uint num);

bool strfread(string &str, const char *filename);

string strfmt(const char *fmt, int num);

int strmath(const char   *str);
int strmath(const string &str);

string &replace(string &str, const char *key, const char   *token);
string &replace(string &str, const char *key, const string &token);

string &qreplace(string &str, const char *key, const char   *token);
string &qreplace(string &str, const char *key, const string &token);

void split(stringarray &dest, const char *key, const char   *src);
void split(stringarray &dest, const char *key, const string &src);

void qsplit(stringarray &dest, const char *key, const char   *src);
void qsplit(stringarray &dest, const char *key, const string &src);

uint vsprintf(string &str, const char *s, va_list args);
uint sprintf(string &str, const char *s, ...);

class string {
public:
char *s;
uint size;
  void reserve(uint reqsize) {
    if(reqsize > size) {
      size = reqsize;
      s = (char*)realloc(s, size + 1);
      s[size] = 0;
    }
  }

  void swap(string &str) {
    ::swap(s, str.s);
    ::swap(size, str.size);
  }

  string() {
    size = 16;
    s = (char*)malloc(size + 1);
    s[0] = 0;
  }

  string(const char *str) {
    size = strlen(str);
    s = (char*)malloc(size + 1);
    strcpy(s, str);
  }

  string(const string &str) {
    size = strlen(str);
    s = (char*)malloc(size + 1);
    strcpy(s, strptr(str));
  }

  ~string() { safe_free(s); }

  operator const char*()   const { return s; }
  const char* operator()() const { return s; }
  char& operator[](const uint);

  string& operator=(const int);
  string& operator=(const char*);
  string& operator=(const string&);

  string& operator<<(const int);
  string& operator<<(const char*);
  string& operator<<(const string&);

  bool operator==(const char*);
  bool operator==(const string&);
  bool operator!=(const char*);
  bool operator!=(const string&);
  bool operator< (const char*);
  bool operator< (const string&);
  bool operator<=(const char*);
  bool operator<=(const string&);
  bool operator> (const char*);
  bool operator> (const string&);
  bool operator>=(const char*);
  bool operator>=(const string&);
};

inline void swap(string &x, string &y) { x.swap(y); }

#endif //LIBSTRING_H
