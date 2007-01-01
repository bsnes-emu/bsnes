#include "libbase.h"
#include "libstring.h"
#include "libstring_oo.cpp"

uint count(stringarray &str) { return str.size(); }

char chrlower(char c) {
  if(c >= 'A' && c <= 'Z')return c + ('a' - 'A');
  return c;
}

char chrupper(char c) {
  if(c >= 'a' && c <= 'z')return c - ('a' - 'A');
  return c;
}

char *strptr(string &str) { return str.s; }

uint strlen(string &str) { return strlen(strptr(str)); }

int strcmp(string &dest, const char *src) { return strcmp(strptr(dest), src); }
int strcmp(const char *dest, string &src) { return strcmp(dest, strptr(src)); }
int strcmp(string &dest, string &src) { return strcmp(strptr(dest), strptr(src)); }

int __stricmp(const char *dest, const char *src) {
  while(*dest) {
    if(chrlower(*dest) != chrlower(*src))break;
    dest++;
    src++;
  }
  return (int)chrlower(*dest) - (int)chrlower(*src);
}
int stricmp(string &dest, const char *src) { return __stricmp(strptr(dest), src); }
int stricmp(const char *dest, string &src) { return __stricmp(dest, strptr(src)); }
int stricmp(string &dest, string &src) { return __stricmp(strptr(dest), strptr(src)); }

void strcpy(string &dest, const char src) {
  dest.reserve(2);
  dest.s[0] = src;
  dest.s[1] = 0;
}

void strcpy(string &dest, const char *src) {
int srclen = strlen(src);
  dest.reserve(srclen);
  strcpy(dest.s, src);
}
void strcpy(string &dest, string &src) { strcpy(dest, strptr(src)); }

uint strlcpy(char *dest, const char *src, uint length) {
uint srclen = strlen(src);
  length--;
  if(length > srclen)length = srclen;
  memcpy(dest, src, length);
  dest[length] = 0;
  return srclen;
}

uint strlcpy(string &dest, const char *src, uint length) {
  dest.reserve(length);
  return strlcpy(strptr(dest), src, length);
}

uint strlcpy(string &dest, string &src, uint length) {
  dest.reserve(length);
  return strlcpy(strptr(dest), strptr(src), length);
}

void strcat(string &dest, const char src) {
int length = strlen(dest);
  dest.reserve(length + 1);
  dest.s[length]     = src;
  dest.s[length + 1] = 0;
}

void strcat(string &dest, const char *src) {
int srclen  = strlen(src);
int destlen = strlen(dest);
  dest.reserve(srclen + destlen);
  strcat(dest.s, src);
}
void strcat(string &dest, string &src) { strcat(dest, strptr(src)); }

uint strlcat(char *dest, const char *src, uint length) {
uint destlen = strlen(dest), srclen = strlen(src);
  length--;
  if(length > destlen + srclen)length = destlen + srclen;
  memcpy(dest + destlen, src, length - destlen);
  dest[length] = 0;
  return destlen + srclen;
}

uint strlcat(string &dest, const char *src, uint length) {
  dest.reserve(length);
  return strlcat(strptr(dest), src, length);
}

uint strlcat(string &dest, string &src, uint length) {
  dest.reserve(length);
  return strlcat(strptr(dest), strptr(src), length);
}

string substr(string &dest, const char *src, uint start, uint length) {
string temp;
  if(length == 0) {
  //copy entire string
    strcpy(temp, src + start);
  } else {
  //copy partial string
    strlcpy(temp, src + start, length + 1);
  }
  return temp;
}

string substr(string &dest, string &src, uint start, uint length) { return substr(dest, strptr(src), start, length); }

void strinsert(string &dest, const char *src, uint pos) {
string temp;
  strcpy(temp, strptr(dest) + pos);
  dest[pos] = 0;
  strcat(dest, src);
  strcat(dest, temp);
}
void strinsert(string &dest, string &src, uint pos) { strinsert(dest, strptr(src), pos); }

void strremove(string &dest, uint start, uint length) {
int i, destlen = strlen(dest);
  dest.reserve(start);
  if(!length) {
    dest[start] = 0;
    return;
  }
  for(i = start; i < destlen; i++) { dest.s[i] = dest.s[i + length]; }
  dest.s[i] = 0;
}

char *strlower(char *str) {
uint i = 0;
  while(str[i]) {
    str[i] = chrlower(str[i]);
    i++;
  }
  return str;
}
string &strlower(string &str) { strlower(strptr(str)); return str; }

char *strupper(char *str) {
uint i = 0;
  while(str[i]) {
    str[i] = chrupper(str[i]);
    i++;
  }
  return str;
}
string &strupper(string &str) { strupper(strptr(str)); return str; }

bool strpos(const char *str, const char *key, uint &pos) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  for(int i = 0; i <= ssl - ksl; i++) {
    if(!memcmp(str + i, key, ksl)) {
      pos = i;
      return true;
    }
  }
  return false;
}
bool strpos(string &str, const char *key, uint &pos) { return strpos(strptr(str), key, pos); }
bool strpos(const char *str, string &key, uint &pos) { return strpos(str, strptr(key), pos); }
bool strpos(string &str, string &key, uint &pos) { return strpos(strptr(str), strptr(key), pos); }

bool qstrpos(const char *str, const char *key, uint &pos) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  for(int i = 0; i <= ssl - ksl;) {
  uint8 x = str[i];
    if(x == '\"' || x == '\'') {
    uint8 z = i++;
      while(str[i] != x && i < ssl)i++;
      if(i >= ssl)i = z;
    }
    if(!memcmp(str + i, key, ksl)) {
      pos = i;
      return true;
    } else {
      i++;
    }
  }
  return false;
}
bool qstrpos(string &str, const char *key, uint &pos) { return qstrpos(strptr(str), key, pos); }
bool qstrpos(const char *str, string &key, uint &pos) { return qstrpos(str, strptr(key), pos); }
bool qstrpos(string &str, string &key, uint &pos) { return qstrpos(strptr(str), strptr(key), pos); }

void strtr(char *dest, const char *before, const char *after) {
int sl = strlen(dest), bsl = strlen(before), asl = strlen(after);
  if(bsl != asl || bsl == 0)return;
  for(int i = 0; i < sl; i++) {
    for(int l = 0; l < bsl; l++) {
      if(dest[i] == before[l]) {
        dest[i] = after[l];
        break;
      }
    }
  }
}
void strtr(string &dest, const char *before, const char *after) { strtr(strptr(dest), before, after); }

bool strbegin(const char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  return (!memcmp(str, key, ksl));
}
bool strbegin(string &str, const char *key) { return strbegin(strptr(str), key); }

bool stribegin(const char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  for(int i = 0; i < ksl; i++) {
    if(str[i] >= 'A' && str[i] <= 'Z') {
      if(str[i] != key[i] && str[i]+0x20 != key[i])return false;
    } else if(str[i] >= 'a' && str[i] <= 'z') {
      if(str[i] != key[i] && str[i]-0x20 != key[i])return false;
    } else {
      if(str[i] != key[i])return false;
    }
  }
  return true;
}
bool stribegin(string &str, const char *key) { return stribegin(strptr(str), key); }

bool strend(const char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  return (!memcmp(str + ssl - ksl, key, ksl));
}
bool strend(string &str, const char *key) { return strend(strptr(str), key); }

bool striend(const char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  for(int i = ssl - ksl, z = 0; i < ssl; i++, z++) {
    if(str[i] >= 'A' && str[i] <= 'Z') {
      if(str[i] != key[z] && str[i]+0x20 != key[z])return false;
    } else if(str[i] >= 'a' && str[i] <= 'z') {
      if(str[i] != key[z] && str[i]-0x20 != key[z])return false;
    } else {
      if(str[i] != key[z])return false;
    }
  }
  return true;
}
bool striend(string &str, const char *key) { return striend(strptr(str), key); }

void strltrim(char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(strbegin(str, key)) {
    for(i = 0; i < (ssl - ksl); i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}
void strltrim(string &str, const char *key) { strltrim(strptr(str), key); }

void striltrim(char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(stribegin(str, key)) {
    for(i = 0; i < (ssl-ksl); i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}
void striltrim(string &str, const char *key) { striltrim(strptr(str), key); }

void strrtrim(char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(strend(str, key)) {
    str[ssl - ksl] = 0;
  }
}
void strrtrim(string &str, const char *key) { strrtrim(strptr(str), key); }

void strirtrim(char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(striend(str, key)) {
    str[ssl - ksl] = 0;
  }
}
void strirtrim(string &str, const char *key) { strirtrim(strptr(str), key); }

//does not work on type char* because function increases string length
void strquote(string &str) {
static string t;
  strcpy(t, "\"");
  strcat(t, str);
  strcat(t, "\"");
  strcpy(str, t);
}

bool strunquote(char *str) {
int i, ssl = strlen(str);
//make sure string is long enough to have quotes
  if(ssl < 2)return false;

//make sure string actually has quotes
  if(str[0] == '\"' && str[ssl - 1] == '\"');
  else if(str[0] == '\'' && str[ssl - 1] == '\'');
  else return false;

//now remove them
  for(i = 0; i < ssl; i++) {
    str[i] = str[i + 1];
  }
  str[i - 2] = 0;

  return true;
}
bool strunquote(string &str) { return strunquote(strptr(str)); }

uint strhex(const char *str) {
uint r = 0, m = 0;
int i, ssl = strlen(str);
uint8 x;
  for(i = 0; i < ssl; i++) {
    if(str[i] >= '0' && str[i] <= '9');
    else if(str[i] >= 'A' && str[i] <= 'F');
    else if(str[i] >= 'a' && str[i] <= 'f');
    else break;
  }
  for(--i; i >= 0; i--, m += 4) {
    x = str[i];
    if(x >= '0' && x <= '9')x -= '0';
    else if(x >= 'A' && x <= 'F')x -= 'A' - 0x0a;
    else if(x >= 'a' && x <= 'f')x -= 'a' - 0x0a;
    else return r;
    r |= x << m;
  }
  return r;
}
uint strhex(string &str) { return strhex(strptr(str)); }

int sstrhex(const char *str) {
  if(str[0] == '-') {
    return -strhex(str + 1);
  }
  return strhex(str);
}
int sstrhex(string &str) { return sstrhex(strptr(str)); }

uint strdec(const char *str) {
uint m = 1;
int i, r = 0, ssl = strlen(str);
uint8 x;
  for(i = 0; i < ssl; i++) {
    if(str[i] >= '0' && str[i] <= '9');
    else break;
  }
  for(--i; i >= 0; i--, m *= 10) {
    x = str[i];
    if(x >= '0' && x <= '9')x -= '0';
    else return r;
    r += x * m;
  }
  return r;
}
uint strdec(string &str) { return strdec(strptr(str)); }

int sstrdec(const char *str) {
  if(str[0] == '-') {
    return -strdec(str + 1);
  }
  return strdec(str);
}
int sstrdec(string &str) { return sstrdec(strptr(str)); }

uint strbin(const char *str) {
uint r = 0, m = 0;
int i, ssl = strlen(str);
uint8 x;
  for(i = 0; i < ssl; i++) {
    if(str[i] == '0' || str[i] == '1');
    else break;
  }
  for(--i; i >= 0; i--, m++) {
    x = str[i];
    if(str[i] == '0' || str[i] == '1')x -= '0';
    else return r;
    r |= x << m;
  }
  return r;
}
uint strbin(string &str) { return strbin(strptr(str)); }

int sstrbin(const char *str) {
  if(str[0] == '-') {
    return -strbin(str + 1);
  }
  return strbin(str);
}
int sstrbin(string &str) { return sstrbin(strptr(str)); }

char *utoa(char *str, uint num) {
char *pstr = str;
uint mask = 1000000000;
  while(mask > num)mask /= 10;
  while(mask > 1) {
    str[0] = '0';
    while(num >= mask) { str[0]++; num -= mask; }
    str++;
    mask /= 10;
  }
  str[0] = '0' + num;
  str++;
  str[0] = 0;
  return pstr;
}

string &utoa(string &str, uint num) {
  str.reserve(16);
  utoa(strptr(str), num);
  return str;
}

char *itoa(char *str, uint num) {
char *pstr = str;
  if(num < 0) {
    str[0] = '-';
    str++;
    num = abs(int(num));
  }
  utoa(str, num);
  return pstr;
}

string &itoa(string &str, uint num) {
  str.reserve(16);
  itoa(strptr(str), num);
  return str;
}

char *htoa(char *str, uint num) {
char *pstr = str;
uint mask = 28, r;
  while(mask && ((num >> mask) & 15) == 0)mask -= 4;
  while(mask) {
    r = (num >> mask) & 15;
    str[0] = (r < 10) ? ('0' + r) : ('a' + r - 10);
    str++;
    mask -= 4;
  }
  r = num & 15;
  str[0] = (r < 10) ? ('0' + r) : ('a' + r - 10);
  str++;
  str[0] = 0;
  return pstr;
}

string &htoa(string &str, uint num) {
  str.reserve(16);
  htoa(strptr(str), num);
  return str;
}

char *uhtoa(char *str, uint num) {
char *pstr = str;
uint mask = 28, r;
  while(mask && ((num >> mask) & 15) == 0)mask -= 4;
  while(mask) {
    r = (num >> mask) & 15;
    str[0] = (r < 10) ? ('0' + r) : ('A' + r - 10);
    str++;
    mask -= 4;
  }
  r = num & 15;
  str[0] = (r < 10) ? ('0' + r) : ('A' + r - 10);
  str++;
  str[0] = 0;
  return pstr;
}

string &uhtoa(string &str, uint num) {
  str.reserve(16);
  uhtoa(strptr(str), num);
  return str;
}

char *btoa(char *str, uint num) {
char *pstr = str;
uint mask = 0x80000000;
  while(mask && (num & mask) == 0)mask >>= 1;
  while(mask > 1) {
    str[0] = (num & mask) ? '1' : '0';
    str++;
    mask >>= 1;
  }
  str[0] = (num & mask) ? '1' : '0';
  str++;
  str[0] = 0;
  return pstr;
}

string &btoa(string &str, uint num) {
  str.reserve(48);
  btoa(strptr(str), num);
  return str;
}

bool strfread(string &str, const char *filename) {
  strcpy(str, "");

FILE *fp = fopen(filename, "rb");
  if(!fp)return false;

uint size = fsize(fp);
char *fdata = (char*)malloc(size + 1);
  fread(fdata, 1, size, fp);
  fclose(fp);
  fdata[size] = 0;
  strcpy(str, fdata);
  free(fdata);

  return true;
}

#include "libstring_int.cpp"
#include "libstring_math.cpp"
#include "libstring_split.cpp"
#include "libstring_replace.cpp"
#include "libstring_sprintf.cpp"
