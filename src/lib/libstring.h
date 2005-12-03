/*
  libstring : version 0.07 ~byuu (11/30/05)
*/

#ifndef __LIBSTRING
#define __LIBSTRING

#include "libvector.h"

class string;
class substring;

char  chrlower(char c);
char  chrupper(char c);

uint  count(string &str);

void  strresize(substring &str, uint size);

char* strptr(substring &str);

uint  strlen(substring &str);

int   strcmp(substring &dest, const char *src);
int   strcmp(const char *dest, substring &src);
int   strcmp(substring &dest, substring &src);

//vc6/win32 and gcc/dos only support stricmp, whereas
//gcc/unix only supports strcasecmp. this is an attempt
//to avoid platform-specific defines...
#define stricmp __stricmp
int   __stricmp(const char *dest, const char *src);
int   stricmp(substring &dest, const char *src);
int   stricmp(const char *dest, substring &src);
int   stricmp(substring &dest, substring &src);

bool  strmatch(const char *dest, const char *src);
bool  strmatch(substring &dest, const char *src);
bool  strmatch(const char *dest, substring &src);
bool  strmatch(substring &dest, substring &src);

bool  strimatch(const char *dest, const char *src);
bool  strimatch(substring &dest, const char *src);
bool  strimatch(const char *dest, substring &src);
bool  strimatch(substring &dest, substring &src);

void  strcpy(substring &dest, const char *src);
void  strcpy(substring &dest, substring &src);

void  strset(substring &dest, uint pos, uint8 c);

void  strcat(substring &dest, const char *src);
void  strcat(substring &dest, substring &src);

void  strinsert(substring &dest, const char *src, uint pos);
void  strinsert(substring &dest, substring &src, uint pos);

void  strremove(substring &dest, uint start, uint length = 0);

void  strlower(char *str);
void  strlower(substring &str);

void  strupper(char *str);
void  strupper(substring &str);

bool  strpos(const char *str, const char *key, uint &pos);
bool  strpos(substring &str, const char *key, uint &pos);
bool  strpos(const char *str, substring &key, uint &pos);
bool  strpos(substring &str, substring &key, uint &pos);

bool  qstrpos(const char *str, const char *key, uint &pos);
bool  qstrpos(substring &str, const char *key, uint &pos);
bool  qstrpos(const char *str, substring &key, uint &pos);
bool  qstrpos(substring &str, substring &key, uint &pos);

void  strtr(char *dest, const char *before, const char *after);
void  strtr(substring &dest, const char *before, const char *after);

bool  strbegin(const char *str, const char *key);
bool  strbegin(substring &str, const char *key);

bool  stribegin(const char *str, const char *key);
bool  stribegin(substring &str, const char *key);

bool  strend(const char *str, const char *key);
bool  strend(substring &str, const char *key);

bool  striend(const char *str, const char *key);
bool  striend(substring &str, const char *key);

void  strltrim(char *str, const char *key);
void  strltrim(substring &str, const char *key);

void  striltrim(char *str, const char *key);
void  striltrim(substring &str, const char *key);

void  strrtrim(char *str, const char *key);
void  strrtrim(substring &str, const char *key);

void  strirtrim(char *str, const char *key);
void  strirtrim(substring &str, const char *key);

void  strquote(substring &str);

bool  strunquote(char *str);
bool  strunquote(substring &str);

uint  strhex(const char *str);
uint  strhex(substring &str);

int   sstrhex(const char *str);
int   sstrhex(substring &str);

uint  strdec(const char *str);
uint  strdec(substring &str);

int   sstrdec(const char *str);
int   sstrdec(substring &str);

uint  strbin(const char *str);
uint  strbin(substring &str);

int   sstrbin(const char *str);
int   sstrbin(substring &str);

uint  strmath(const char *in_str);
uint  strmath(substring &in_str);

bool  strmathentity(const char *str);
bool  strmathentity(substring &str);

void  replace(substring &str, const char *key, const char *token);
void  replace(substring &str, const char *key, substring &token);

void  qreplace(substring &str, const char *key, const char *token);
void  qreplace(substring &str, const char *key, substring &token);

void  split(string &dest, const char *key, char *src);
void  split(string &dest, const char *key, substring &src);

void  qsplit(string &dest, const char *key, char *src);
void  qsplit(string &dest, const char *key, substring &src);

void  sprintf(substring &str, const char *s, ...);

class substring {
public:
char *s;
uint size;
//inline char* operator*() { return s; }
//inline operator char*()  { return s; }

  substring();
  ~substring();
};

/* listcount is the actual size of list[], used for allocation
 * of substrings.
 * count is used by split() and count() to return the number of
 * active/valid substrings. example: if string T contains 16
 * substrings, and split is called, which sets three substrings,
 * then count() needs to reflect that only three substrings are
 * now used, but listcount still needs to reflect the true size
 * of list[].
 */
class string {
public:
vector<substring*> list;
uint listcount, count;
  void addto(uint num);     //creates all needed strings to make list[num] valid
  substring &str(uint num); //gets a substring reference, creating it + new strings if needed

//inline char* operator*()               { return strptr(str(0)); }
//inline operator char*()                { return str(0).s; }
  inline operator substring&()           { return str(0); }
  inline substring& operator[](uint   i) { return str(i); }
  inline substring& operator[](uint8  i) { return str(i); }
  inline substring& operator[](uint16 i) { return str(i); }
  inline substring& operator[](uint32 i) { return str(i); }
  inline substring& operator[](int    i) { return str(i); }
  inline substring& operator[](int8   i) { return str(i); }
  inline substring& operator[](int16  i) { return str(i); }
  inline substring& operator[](int32  i) { return str(i); }

  string();
  ~string();
};

#endif //__LIBSTRING
