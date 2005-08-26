#include "libbase.h"
#include "libstring.h"

substring::substring() {
  size = 16;
  s = (char*)malloc(size + 1);
  *s = 0;
}

substring::~substring() {
  if(s) {
    free(s);
    s = 0;
  }
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
int i;
  for(i=listcount-1;i>=0;i--) {
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

void strcpy(substring &dest, const char *src) {
int srclen = strlen(src);
  if(srclen > dest.size) { strresize(dest, srclen); }
  strcpy(dest.s, src);
}
void strcpy(substring &dest, substring &src) { strcpy(dest, strptr(src)); }

void strset(substring &dest, uint pos, uint8 c) {
char *s;
  if(pos > dest.size) { strresize(dest, pos); }
  dest.s[pos] = c;
}

void strcat(substring &dest, const char *src) {
int srclen, destlen;
  srclen = strlen(src);
  destlen = strlen(dest.s);
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

int __stricmp(const char *dest, const char *src) {
  while(*dest && *src) {
    if(chrlower(*dest) != chrlower(*src))break;
    dest++;
    src++;
  }
  return (int)chrlower(*dest) - (int)chrlower(*src);
}
int stricmp(substring &dest, const char *src) { return __stricmp(strptr(dest), src); }
int stricmp(const char *dest, substring &src) { return __stricmp(dest, strptr(src)); }
int stricmp(substring &dest, substring &src) { return __stricmp(strptr(dest), strptr(src)); }

void strlower(char *str) {
  while(*str) {
    *str = chrlower(*str);
    str++;
  }
}
void strlower(substring &str) { strlower(strptr(str)); }

void strupper(char *str) {
  while(*str) {
    *str = chrupper(*str);
    str++;
  }
}
void strupper(substring &str) { strupper(strptr(str)); }

uint strpos(const char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return null;
  for(i=0;i<=ssl-ksl;i++) {
    if(!memcmp(str+i, key, ksl))return i;
  }
  return null;
}
uint strpos(substring &str, const char *key) { return strpos(strptr(str), key); }
uint strpos(const char *str, substring &key) { return strpos(str, strptr(key)); }
uint strpos(substring &str, substring &key) { return strpos(strptr(str), strptr(key)); }

uint qstrpos(const char *str, const char *key) {
int i, z, ssl = strlen(str), ksl = strlen(key);
uint8 x;
  if(ksl > ssl)return null;
  for(i=0;i<=ssl-ksl;) {
    x = str[i];
    if(x == '\"' || x == '\'') {
      z = i++;
      while(str[i] != x && i < ssl)i++;
      if(i >= ssl)i = z;
    }
    if(!memcmp(str+i, key, ksl)) {
      return i;
    } else {
      i++;
    }
  }
  return null;
}
uint qstrpos(substring &str, const char *key) { return qstrpos(strptr(str), key); }
uint qstrpos(const char *str, substring &key) { return qstrpos(str, strptr(key)); }
uint qstrpos(substring &str, substring &key) { return qstrpos(strptr(str), strptr(key)); }

void strtr(char *dest, const char *before, const char *after) {
int i, l, sl = strlen(dest), bsl = strlen(before), asl = strlen(after);
  if((bsl != asl) || bsl == 0)return;
  for(i=0;i<sl;i++) {
    for(l=0;l<bsl;l++) {
      if(dest[i] == before[l])dest[i] = after[l];
    }
  }
}
void strtr(substring &dest, const char *before, const char *after) { strtr(strptr(dest), before, after); }

uint strbegin(const char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return 1;
  if(!memcmp(str, key, ksl))return 0;
  return 1;
}
uint strbegin(substring &str, const char *key) { return strbegin(strptr(str), key); }

uint stribegin(const char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return 1;
  for(i=0;i<ksl;i++) {
    if(str[i] >= 'A' && str[i] <= 'Z') {
      if(str[i] != key[i] && str[i]+0x20 != key[i])return 1;
    } else if(str[i] >= 'a' && str[i] <= 'z') {
      if(str[i] != key[i] && str[i]-0x20 != key[i])return 1;
    } else {
      if(str[i] != key[i])return 1;
    }
  }
  return 0;
}
uint stribegin(substring &str, const char *key) { return stribegin(strptr(str), key); }

uint strend(const char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return 1;
  if(!memcmp(str + ssl - ksl, key, ksl))return 0;
  return 1;
}
uint strend(substring &str, const char *key) { return strend(strptr(str), key); }

uint striend(const char *str, const char *key) {
int i, z, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return 1;
  for(i=ssl-ksl, z=0;i<ssl;i++, z++) {
    if(str[i] >= 'A' && str[i] <= 'Z') {
      if(str[i] != key[z] && str[i]+0x20 != key[z])return 1;
    } else if(str[i] >= 'a' && str[i] <= 'z') {
      if(str[i] != key[z] && str[i]-0x20 != key[z])return 1;
    } else {
      if(str[i] != key[z])return 1;
    }
  }
  return 0;
}
uint striend(substring &str, const char *key) { return striend(strptr(str), key); }

void strltrim(char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!strbegin(str, key)) {
    for(i=0;i<ssl-ksl;i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}
void strltrim(substring &str, const char *key) { strltrim(strptr(str), key); }

void striltrim(char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!stribegin(str, key)) {
    for(i=0;i<ssl-ksl;i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}
void striltrim(substring &str, const char *key) { striltrim(strptr(str), key); }

void strrtrim(char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!strend(str, key)) {
    str[ssl - ksl] = 0;
  }
}
void strrtrim(substring &str, const char *key) { strrtrim(strptr(str), key); }

void strirtrim(char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!striend(str, key)) {
    str[ssl - ksl] = 0;
  }
}
void strirtrim(substring &str, const char *key) { strirtrim(strptr(str), key); }

//does not work on type char* because function increases string length
void strquote(substring &str) {
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
  for(i=0;i<ssl;i++) {
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
  for(i=0;i<ssl;i++) {
    if(str[i] >= '0' && str[i] <= '9');
    else if(str[i] >= 'A' && str[i] <= 'F');
    else if(str[i] >= 'a' && str[i] <= 'f');
    else break;
  }
  for(--i;i>=0;i--, m+=4) {
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
  for(i=0;i<ssl;i++) {
    if(str[i] >= '0' && str[i] <= '9');
    else break;
  }
  for(--i;i>=0;i--, m*=10) {
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
  for(i=0;i<ssl;i++) {
    if(str[i] == '0' || str[i] == '1');
    else break;
  }
  for(--i;i>=0;i--, m++) {
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

#include "libstring_math.cpp"
#include "libstring_split.cpp"
#include "libstring_replace.cpp"
#include "libstring_sprintf.cpp"
