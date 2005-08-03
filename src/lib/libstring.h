/*
  libstring : version 0.05 ~byuu
*/

#ifndef __LIBSTRING
#define __LIBSTRING

#include "libvector.h"

class string;
class _string;

uint32 count(string &str);
void   strresize(_string &str, uint32 size);
char*  strptr(_string &str);
void   strcpy(_string &dest, char *src);
void   strset(_string &dest, uint32 pos, uint8 c);
void   strcat(_string &dest, char *src);
void   strinsert(_string &dest, char *src, uint32 pos);
void   strremove(_string &dest, uint32 start, uint32 length = 0);
bool   stricmp(char *dest, char *src);
void   strlower(char *str);
void   strupper(char *str);
uint32 strpos(char *str, char *key);
uint32 strqpos(char *str, char *key);
void   strtr(char *dest, char *before, char *after);
uint32 strbegin(char *str, char *key);
uint32 stribegin(char *str, char *key);
uint32 strend(char *str, char *key);
uint32 striend(char *str, char *key);
void   strltrim(char *str, char *key);
void   striltrim(char *str, char *key);
void   strrtrim(char *str, char *key);
void   strirtrim(char *str, char *key);
void   strquote(_string &str);
bool   strunquote(char *str);
uint32 strhex(char *str);
int    strdec(char *str);
uint32 strbin(char *str);
uint32 strmath(char *in_str);
uint32 strmathentity(char *str);
void   replace (_string &str, char *key, char *token);
void   qreplace(_string &str, char *key, char *token);
void   split (string &dest, char *key, char *src);
void   qsplit(string &dest, char *key, char *src);
void   sprintf(_string &str, char *s, ...);

class _string {
public:
char *s;
uint32 size;
  inline char* operator*() { return s; }

#ifdef __LIBSTRING_OVERLOADS
  inline _string& operator=(char *cpy);
  inline _string& operator=(_string &cpy);
  inline _string& operator=(string &cpy);

  inline _string& operator+=(char *cat);
  inline _string& operator+=(_string &cat);
  inline _string& operator+=(string &cat);

  inline _string& operator-=(char *cut);
  inline _string& operator-=(_string &cut);
  inline _string& operator-=(string &cut);

  inline bool operator==(char *cmp);
  inline bool operator==(_string &cmp);
  inline bool operator==(string &cmp);

  inline bool operator!=(char *cmp);
  inline bool operator!=(_string &cmp);
  inline bool operator!=(string &cmp);
#endif

  inline operator char*() { return s; }

  _string();
  ~_string();
};

class string {
public:
vector<_string*> list;
uint32 count, listcount;
  void addto(uint32 num);   //creates all needed strings to make list[num] valid
  _string &str(uint32 num); //gets a _string reference, creating it + new strings if needed

  inline char* operator*() { return strptr(str(0)); }

#ifdef __LIBSTRING_OVERLOADS
  inline string& operator=(char *cpy);
  inline string& operator=(_string &cpy);
  inline string& operator=(string &cpy);

  inline string& operator+=(char *cat);
  inline string& operator+=(_string &cat);
  inline string& operator+=(string &cat);

  inline string& operator-=(char *cut);
  inline string& operator-=(_string &cut);
  inline string& operator-=(string &cut);

  inline bool operator==(char *cmp);
  inline bool operator==(_string &cmp);
  inline bool operator==(string &cmp);

  inline bool operator!=(char *cmp);
  inline bool operator!=(_string &cmp);
  inline bool operator!=(string &cmp);
#endif

  inline operator char*()              { return str(0).s; }
  inline operator _string&()           { return str(0); }
  inline _string& operator[](uint32 i) { return str(i); }
  inline _string& operator[](int i)    { return str(i); }

  string();
  ~string();
};

#ifdef __LIBSTRING_OVERLOADS
inline _string& _string::operator=(char *cpy) {
  strcpy(*this, cpy);
  return *this;
}
inline _string& _string::operator=(_string &cpy) {
  strcpy(*this, cpy);
  return *this;
}
inline _string& _string::operator=(string &cpy) {
  strcpy(*this, cpy.str(0));
  return *this;
}

inline string& string::operator=(char *cpy) {
  strcpy(str(0), cpy);
  return *this;
}
inline string& string::operator=(_string &cpy) {
  strcpy(str(0), cpy);
  return *this;
}
inline string& string::operator=(string &cpy) {
  strcpy(str(0), cpy.str(0));
  return *this;
}

inline _string& _string::operator+=(char *cat) {
  strcat(*this, cat);
  return *this;
}
inline _string& _string::operator+=(_string &cat) {
  strcat(*this, cat);
  return *this;
}
inline _string& _string::operator+=(string &cat) {
  strcat(*this, cat.str(0));
  return *this;
}

inline string& string::operator+=(char *cat) {
  strcat(str(0), cat);
  return *this;
}
inline string& string::operator+=(_string &cat) {
  strcat(str(0), cat);
  return *this;
}
inline string& string::operator+=(string &cat) {
  strcat(str(0), cat.str(0));
  return *this;
}

inline _string& _string::operator-=(char *cut) {
  strrtrim(*this, cut);
  return *this;
}
inline _string& _string::operator-=(_string &cut) {
  strrtrim(*this, cut);
  return *this;
}
inline _string& _string::operator-=(string &cut) {
  strrtrim(*this, cut.str(0));
  return *this;
}

inline string& string::operator-=(char *cut) {
  strrtrim(str(0), cut);
  return *this;
}
inline string& string::operator-=(_string &cut) {
  strrtrim(str(0), cut);
  return *this;
}
inline string& string::operator-=(string &cut) {
  strrtrim(str(0), cut.str(0));
  return *this;
}

inline bool _string::operator==(char *cmp) {
  if(!strcmp(*this, cmp))return true;
  return false;
}
inline bool _string::operator==(_string &cmp) {
  if(!strcmp(*this, cmp))return true;
  return false;
}
inline bool _string::operator==(string &cmp) {
  if(!strcmp(*this, cmp.str(0)))return true;
  return false;
}

inline bool string::operator==(char *cmp) {
  if(!strcmp(str(0), cmp))return true;
  return false;
}
inline bool string::operator==(_string &cmp) {
  if(!strcmp(str(0), cmp))return true;
  return false;
}
inline bool string::operator==(string &cmp) {
  if(!strcmp(str(0), cmp.str(0)))return true;
  return false;
}

inline bool _string::operator!=(char *cmp) {
  if(!strcmp(*this, cmp))return false;
  return true;
}
inline bool _string::operator!=(_string &cmp) {
  if(!strcmp(*this, cmp))return false;
  return true;
}
inline bool _string::operator!=(string &cmp) {
  if(!strcmp(*this, cmp.str(0)))return false;
  return true;
}

inline bool string::operator!=(char *cmp) {
  if(!strcmp(str(0), cmp))return false;
  return true;
}
inline bool string::operator!=(_string &cmp) {
  if(!strcmp(str(0), cmp))return false;
  return true;
}
inline bool string::operator!=(string &cmp) {
  if(!strcmp(str(0), cmp.str(0)))return false;
  return true;
}
#endif //__LIBSTRING_OVERLOADS

#endif //__LIBSTRING
