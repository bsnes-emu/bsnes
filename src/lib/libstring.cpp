#include "libbase.h"
#include "libstring.h"

substring::substring() {
  size = 16;
  s = (char*)malloc(size + 1);
  *s = 0;
}

substring::~substring() {
  SafeFree(s);
}

void string::addto(uint num) {
  while(listcount < (num + 1)) {
    list[listcount++] = new substring();
  }
}

substring &string::str(uint num) {
  if(listcount < (num + 1)) { addto(num); }
  return *list[num];
}

string::string() {
  count = 0;
  listcount = 0;
  addto(0);
}

string::~string() {
  for(int i = listcount - 1; i >= 0; i--) {
    delete((substring*)list[i]);
  }
}

char chrlower(char c) {
  if(c >= 'A' && c <= 'Z')return c + ('a' - 'A');
  return c;
}

char chrupper(char c) {
  if(c >= 'a' && c <= 'z')return c - ('a' - 'A');
  return c;
}

uint count(string &str) { return str.count; }

void strresize(substring &str, uint size) {
  str.s = (char*)realloc(str.s, size + 1);
  str.s[size] = 0;
  str.size = size;
}

char *strptr(substring &str) { return str.s; }

uint strlen(substring &str) { return strlen(strptr(str)); }

int strcmp(substring &dest, const char *src) { return strcmp(strptr(dest), src); }
int strcmp(const char *dest, substring &src) { return strcmp(dest, strptr(src)); }
int strcmp(substring &dest, substring &src) { return strcmp(strptr(dest), strptr(src)); }

int __stricmp(const char *dest, const char *src) {
  while(*dest) {
    if(chrlower(*dest) != chrlower(*src))break;
    dest++;
    src++;
  }
  return (int)chrlower(*dest) - (int)chrlower(*src);
}
int stricmp(substring &dest, const char *src) { return __stricmp(strptr(dest), src); }
int stricmp(const char *dest, substring &src) { return __stricmp(dest, strptr(src)); }
int stricmp(substring &dest, substring &src) { return __stricmp(strptr(dest), strptr(src)); }

bool strmatch(const char *dest, const char *src) { return !strcmp(dest, src); }
bool strmatch(substring &dest, const char *src) { return strmatch(strptr(dest), src); }
bool strmatch(const char *dest, substring &src) { return strmatch(dest, strptr(src)); }
bool strmatch(substring &dest, substring &src) { return strmatch(strptr(dest), strptr(src)); }

bool strimatch(const char *dest, const char *src) { return !stricmp(dest, src); }
bool strimatch(substring &dest, const char *src) { return strimatch(strptr(dest), src); }
bool strimatch(const char *dest, substring &src) { return strimatch(dest, strptr(src)); }
bool strimatch(substring &dest, substring &src) { return strimatch(strptr(dest), strptr(src)); }

void strcpy(substring &dest, const char src) {
  if(1 > dest.size) { strresize(dest, 1); }
  dest.s[0] = src;
  dest.s[1] = 0;
}

void strcpy(substring &dest, const char *src) {
int srclen = strlen(src);
  if(srclen > dest.size) { strresize(dest, srclen); }
  strcpy(dest.s, src);
}
void strcpy(substring &dest, substring &src) { strcpy(dest, strptr(src)); }

//this differs from libc's strncpy in that it always
//appends a null terminator to the end of a copied string
void strncpy(substring &dest, const char *src, uint32 length) {
int srclen = strlen(src);
//never copy more text than is in the string
  if(srclen > length)srclen = length;
  if(srclen > dest.size) { strresize(dest, srclen); }
  strncpy(dest.s, src, srclen);
  dest.s[srclen] = 0;
}
void strncpy(substring &dest, substring &src, uint32 length) { strncpy(dest, strptr(src), length); }

void strset(substring &dest, uint pos, uint8 c) {
  if(pos > dest.size) { strresize(dest, pos); }
  dest.s[pos] = c;
}

void strcat(substring &dest, const char src) {
int length = strlen(dest);
  if(length + 1 > dest.size) { strresize(dest, length + 1); }
  dest.s[length]     = src;
  dest.s[length + 1] = 0;
}

void strcat(substring &dest, const char *src) {
int srclen  = strlen(src);
int destlen = strlen(dest);
  if(srclen + destlen > dest.size) { strresize(dest, srclen + destlen); }
  strcat(dest.s, src);
}
void strcat(substring &dest, substring &src) { strcat(dest, strptr(src)); }

void strinsert(substring &dest, const char *src, uint pos) {
static substring s;
  strcpy(s, strptr(dest) + pos);
  strset(dest, pos, 0);
  strcat(dest, src);
  strcat(dest, s);
}
void strinsert(substring &dest, substring &src, uint pos) { strinsert(dest, strptr(src), pos); }

void strremove(substring &dest, uint start, uint length) {
int destlen;
char *s;
int i, sl = strlen(dest.s);
  if(start > dest.size) { strresize(dest, start); }
  if(!length) {
    strset(dest, start, 0);
    return;
  }
  s = dest.s;
  for(i=start;i<sl;i++) { s[i] = s[i+length]; }
  s[i] = 0;
}

char *strlower(char *str) {
uint i = 0;
  while(str[i]) {
    str[i] = chrlower(str[i]);
    i++;
  }
  return str;
}
substring &strlower(substring &str) { strlower(strptr(str)); return str; }

char *strupper(char *str) {
uint i = 0;
  while(str[i]) {
    str[i] = chrupper(str[i]);
    i++;
  }
  return str;
}
substring &strupper(substring &str) { strupper(strptr(str)); return str; }

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
bool strpos(substring &str, const char *key, uint &pos) { return strpos(strptr(str), key, pos); }
bool strpos(const char *str, substring &key, uint &pos) { return strpos(str, strptr(key), pos); }
bool strpos(substring &str, substring &key, uint &pos) { return strpos(strptr(str), strptr(key), pos); }

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
bool qstrpos(substring &str, const char *key, uint &pos) { return qstrpos(strptr(str), key, pos); }
bool qstrpos(const char *str, substring &key, uint &pos) { return qstrpos(str, strptr(key), pos); }
bool qstrpos(substring &str, substring &key, uint &pos) { return qstrpos(strptr(str), strptr(key), pos); }

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
void strtr(substring &dest, const char *before, const char *after) { strtr(strptr(dest), before, after); }

bool strbegin(const char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  return (!memcmp(str, key, ksl));
}
bool strbegin(substring &str, const char *key) { return strbegin(strptr(str), key); }

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
bool stribegin(substring &str, const char *key) { return stribegin(strptr(str), key); }

bool strend(const char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  return (!memcmp(str + ssl - ksl, key, ksl));
}
bool strend(substring &str, const char *key) { return strend(strptr(str), key); }

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
bool striend(substring &str, const char *key) { return striend(strptr(str), key); }

void strltrim(char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(strbegin(str, key)) {
    for(i = 0; i < (ssl - ksl); i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}
void strltrim(substring &str, const char *key) { strltrim(strptr(str), key); }

void striltrim(char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(stribegin(str, key)) {
    for(i = 0; i < (ssl-ksl); i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}
void striltrim(substring &str, const char *key) { striltrim(strptr(str), key); }

void strrtrim(char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(strend(str, key)) {
    str[ssl - ksl] = 0;
  }
}
void strrtrim(substring &str, const char *key) { strrtrim(strptr(str), key); }

void strirtrim(char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(striend(str, key)) {
    str[ssl - ksl] = 0;
  }
}
void strirtrim(substring &str, const char *key) { strirtrim(strptr(str), key); }

//does not work on type char* because function increases string length
void strquote(substring &str) {
static substring t;
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
bool strunquote(substring &str) { return strunquote(strptr(str)); }

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
uint strhex(substring &str) { return strhex(strptr(str)); }

int sstrhex(const char *str) {
  if(str[0] == '-') {
    return -strhex(str + 1);
  }
  return strhex(str);
}
int sstrhex(substring &str) { return sstrhex(strptr(str)); }

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
uint strdec(substring &str) { return strdec(strptr(str)); }

int sstrdec(const char *str) {
  if(str[0] == '-') {
    return -strdec(str + 1);
  }
  return strdec(str);
}
int sstrdec(substring &str) { return sstrdec(strptr(str)); }

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
uint strbin(substring &str) { return strbin(strptr(str)); }

int sstrbin(const char *str) {
  if(str[0] == '-') {
    return -strbin(str + 1);
  }
  return strbin(str);
}
int sstrbin(substring &str) { return sstrbin(strptr(str)); }

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

substring &utoa(substring &str, uint num) {
  if(str.size < 16) { strresize(str, 16); }
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

substring &itoa(substring &str, uint num) {
  if(str.size < 16) { strresize(str, 16); }
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

substring &htoa(substring &str, uint num) {
  if(str.size < 16) { strresize(str, 16); }
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

substring &uhtoa(substring &str, uint num) {
  if(str.size < 16) { strresize(str, 16); }
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

substring &btoa(substring &str, uint num) {
  if(str.size < 48) { strresize(str, 48); }
  btoa(strptr(str), num);
  return str;
}

#include "libstring_math.cpp"
#include "libstring_split.cpp"
#include "libstring_replace.cpp"
#include "libstring_sprintf.cpp"
