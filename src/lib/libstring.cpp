#include "libbase.h"
#include "libstring.h"

_string::_string() {
  size = 16;
  s = (char*)malloc(size + 1);
  *s = 0;
}

_string::~_string() {
  free(s);
}

void string::addto(uint32 num) {
  while(listcount < (num + 1)) {
    list[listcount++] = new _string();
  }
}

_string &string::str(uint32 num) {
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
    delete((_string*)list[i]);
  }
}

uint32 count(string &str) {
  return str.count;
}

void strresize(_string &str, uint32 size) {
char *t;
int sl;
  if(str.size == size)return;
  sl = strlen(str.s);
  t = (char*)malloc(size + 1);
  strcpy(t, str.s);
  free(str.s);
  str.s = t;
  str.size = size;
}

char *strptr(_string &str) {
  return str.s;
}

void strcpy(_string &dest, char *src) {
int srclen = strlen(src);
  if(srclen > dest.size) { strresize(dest, srclen); }
  strcpy(dest.s, src);
}

void strset(_string &dest, uint32 pos, uint8 c) {
char *s;
  if(pos > dest.size) { strresize(dest, pos); }
  dest.s[pos] = c;
}

void strcat(_string &dest, char *src) {
int srclen, destlen;
  srclen = strlen(src);
  destlen = strlen(dest.s);
  if(srclen + destlen > dest.size) { strresize(dest, srclen + destlen); }
  strcat(dest.s, src);
}

void strinsert(_string &dest, char *src, uint32 pos) {
_string *s = new _string();
  strcpy(*s, strptr(dest) + pos);
  strset(dest, pos, 0);
  strcat(dest, src);
  strcat(dest, *s);
  delete(s);
}

void strremove(_string &dest, uint32 start, uint32 length) {
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

bool stricmp(char *dest, char *src) {
int i, sl = strlen(dest);
  if(sl != strlen(src))return false;
  for(i=0;i<sl;i++) {
    if(dest[i] >= 'A' && dest[i] <= 'Z') {
      if(dest[i] != src[i] && dest[i] + 0x20 != src[i])return false;
    } else if(dest[i] >='a' && dest[i] <= 'z') {
      if(dest[i] != src[i] && dest[i] - 0x20 != src[i])return false;
    } else {
      if(dest[i] != src[i])return false;
    }
  }
  return true;
}

void strlower(char *str) {
int i, sl = strlen(str);
  for(i=0;i<sl;i++) {
    if(str[i] >= 'A' && str[i] <= 'Z')str[i] += 0x20;
  }
}

void strupper(char *str) {
int i, sl = strlen(str);
  for(i=0;i<sl;i++) {
    if(str[i] >= 'a' && str[i] <= 'z')str[i] -= 0x20;
  }
}

uint32 strpos(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return null;
  for(i=0;i<=ssl-ksl;i++) {
    if(!memcmp(str+i, key, ksl))return i;
  }
  return null;
}

uint32 strqpos(char *str, char *key) {
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

void strtr(char *dest, char *before, char *after) {
int i, l, sl = strlen(dest), bsl = strlen(before), asl = strlen(after);
  if((bsl != asl) || bsl == 0)return;
  for(i=0;i<sl;i++) {
    for(l=0;l<bsl;l++) {
      if(dest[i] == before[l])dest[i] = after[l];
    }
  }
}

uint32 strbegin(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return 1;
  if(!memcmp(str, key, ksl))return 0;
  return 1;
}

uint32 stribegin(char *str, char *key) {
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

uint32 strend(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return 1;
  if(!memcmp(str + ssl - ksl, key, ksl))return 0;
  return 1;
}

uint32 striend(char *str, char *key) {
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

void strltrim(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!strbegin(str, key)) {
    for(i=0;i<ssl-ksl;i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}

void striltrim(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!stribegin(str, key)) {
    for(i=0;i<ssl-ksl;i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}

void strrtrim(char *str, char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!strend(str, key)) {
    str[ssl - ksl] = 0;
  }
}

void strirtrim(char *str, char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!striend(str, key)) {
    str[ssl - ksl] = 0;
  }
}

/* does not work on type char* because function increases string length */
void strquote(_string &str) {
static string t;
  t = "\"";
  t += str;
  t += "\"";
  str = t;
}

bool strunquote(char *str) {
int i, ssl = strlen(str);
/* make sure string is long enough to have quotes */
  if(ssl < 2)return false;

/* make sure string actually has quotes */
  if(str[0] == '\"' && str[ssl - 1] == '\"');
  else if(str[0] == '\'' && str[ssl - 1] == '\'');
  else return false;

/* now remove them */
  for(i=0;i<ssl;i++) {
    str[i] = str[i + 1];
  }
  str[i - 2] = 0;

  return true;
}

uint32 strhex(char *str) {
uint32 r = 0, m = 0;
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

int strdec(char *str) {
uint32 m = 1;
int i, r = 0, ssl = strlen(str);
uint8 x;
  for(i=0;i<ssl;i++) {
    if(str[i] >= '0' && str[i] <= '9');
    else if(str[i] == '-' && i == 0);
    else break;
  }
  for(--i;i>=0;i--, m*=10) {
    x = str[i];
    if(x >= '0' && x <= '9')x -= '0';
    else if(i == 0 && str[i] == '-') {
      r *= -1;
      return r;
    }
    else return r;
    r += x * m;
  }
  return r;
}

uint32 strbin(char *str) {
uint32 r = 0, m = 0;
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

#include "libstring_math.cpp"
#include "libstring_split.cpp"
#include "libstring_replace.cpp"
#include "libstring_sprintf.cpp"
