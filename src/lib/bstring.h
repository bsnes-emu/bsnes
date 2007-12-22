/*
  bstring : version 0.01a ~byuu (2007-12-12)
  license: public domain
*/

#ifndef BSTRING_H
#define BSTRING_H

#include "bbase.h"
#include "bvector.h"

class string;
typedef linear_vector<string> lstring;

uint count(lstring&);
int find(lstring &str, const char *key);

uint strlcpy(char *dest, const char *src, uint length);
uint strlcat(char *dest, const char *src, uint length);
uint strlcpy(string &dest, const char *src, uint length);
uint strlcat(string &dest, const char *src, uint length);

int strmath(const char *str);

char chrlower(char c);
char chrupper(char c);
int stricmp(const char *dest, const char *src);
void strcpy(string &dest, const char *src);
void strcat(string &dest, const char *src);
string substr(string &dest, const char *src, uint start = 0, uint length = 0);
char* strlower(char *str);
char* strupper(char *str);
string& strlower(string &str);
string& strupper(string &str);
int strpos(const char *str, const char *key);
int qstrpos(const char *str, const char *key);
char* strtr(char *dest, const char *before, const char *after);
string& strtr(string &dest, const char *before, const char *after);
bool strbegin(const char *str, const char *key);
bool stribegin(const char *str, const char *key);
bool strend(const char *str, const char *key);
bool striend(const char *str, const char *key);

uint strhex(const char *str);
uint strdec(const char *str);
uint strbin(const char *str);

string& utoa(string &str, uint num);
string& itoa(string &str, uint num);
string& htoa(string &str, uint num);
string& btoa(string &str, uint num);

bool fread(string &str, const char *filename);

string& replace(string &str, const char *key, const char *token);
string& qreplace(string &str, const char *key, const char *token);

void split(lstring &dest, const char *key, const char *src);
void qsplit(lstring &dest, const char *key, const char *src);

char* ltrim(char *str, const char *key = " ");
char* rtrim(char *str, const char *key = " ");
char* trim(char *str, const char *key = " ");
string& ltrim(string &str, const char *key = " ");
string& rtrim(string &str, const char *key = " ");
string& trim(string &str, const char *key = " ");

class string {
public:
  char *data;
  uint size;

  void reserve(uint size_);

  operator const char*() const;
  char* operator()();
  char& operator[](const uint);

  string& operator=(const int num);
  string& operator=(const char *str);
  string& operator=(const string &str);
  string& operator<<(const int num);
  string& operator<<(const char *str);

  bool operator==(const char *str);
  bool operator!=(const char *str);
  bool operator< (const char *str);
  bool operator<=(const char *str);
  bool operator> (const char *str);
  bool operator>=(const char *str);

  string();
  string(const int num);
  string(const char *source);
  string(const string &source);
  ~string();
};

#endif //ifndef BSTRING_H
