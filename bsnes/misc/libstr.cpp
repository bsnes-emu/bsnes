#include "../base.h"
#include "libvlist.cpp"

typedef struct {
char *s;
ulong size;
}string;

typedef struct {
vectorlist list;
ulong count;
}stringlist;

/***********************
string library functions
***********************/

void strresize(string *str, ulong size) {
char *t;
int sl;
  if(str->size == size)return;
  sl = strlen(str->s);
  t = (char*)malloc(size + 1);
  strncpy(t, str->s, size);
  free(str->s);
  str->s = t;
  str->size = size;
}

string *newstr(void) {
string *s;
  s = (string*)malloc(sizeof(string));
  s->size = 16;
  s->s = (char*)malloc(s->size + 1);
  *s->s = 0;
  return s;
}

void memfree(string *s) {
  if(s->s)free(s->s);
  free(s);
}

/***************************
stringlist library functions
***************************/

void stradd(stringlist *sl, ulong num) {
int i;
string *s;
  while(sl->count < (num + 1)) {
    s = newstr();
    sl->list.add((ulong)s);
    sl->count++;
  }
}

string *strget(stringlist *sl, ulong num) {
string *s;
  if(sl->count < (num + 1)) { stradd(sl, num); }
  s = (string*)sl->list.get(num);
  return s;
}

stringlist *newstrlist(void) {
stringlist *sl;
  sl = (stringlist*)malloc(sizeof(stringlist));
  sl->count = 0;
  stradd(sl, 1);
  return sl;
}

ulong count(stringlist *sl) {
  return sl->count;
}

/****************************
string manipulation functions
****************************/

char *strptr(string *s) { return s->s; }
char *strptr(stringlist *sl, ulong num) { return strget(sl, num)->s; }

void strcpy(string *dest, char *src) {
int srclen = strlen(src);
  if(srclen > dest->size) { strresize(dest, srclen); }
  strncpy(dest->s, src, dest->size);
}
void strcpy(string *dest, string *src) { strcpy(dest, src->s); }
void strcpy(stringlist *dest, ulong num, char *src) { strcpy(strget(dest, num), src); }
void strcpy(stringlist *dest, ulong num, string *src) { strcpy(strget(dest, num), src->s); }
void strcpy(stringlist *dest, ulong destnum, stringlist *src, ulong srcnum) { strcpy(strget(dest, destnum), strptr(src, srcnum)); }

void strset(string *dest, ulong pos, byte c) {
char *s;
  if(pos > dest->size) { strresize(dest, pos); }
  dest->s[pos] = c;
}
void strset(stringlist *dest, ulong num, ulong pos, byte c) { strset(strget(dest, num), pos, c); }

void strcat(string *dest, char *src) {
int srclen, destlen;
  srclen  = strlen(src);
  destlen = strlen(dest->s);
  if(srclen + destlen > dest->size) { strresize(dest, srclen + destlen); }
  strncat(dest->s, src, srclen + destlen);
}
void strcat(string *dest, string *src) { strcat(dest, src->s); }
void strcat(stringlist *dest, ulong num, char *src) { strcat(strget(dest, num), src); }
void strcat(stringlist *dest, ulong num, string *src) { strcat(strget(dest, num), src->s); }
void strcat(stringlist *dest, ulong destnum, stringlist *src, ulong srcnum) { strcat(strget(dest, destnum), strptr(src, srcnum)); }

ulong strlen(string *s) { return strlen(s->s); }
ulong strlen(stringlist *sl, ulong num) { return strlen(strget(sl, num)->s); }

void strinsert(string *dest, char *src, ulong pos) {
string *s = newstr();
  strcpy(s, strptr(dest)+pos);
  strset(dest, pos, 0);
  strcat(dest, src);
  strcat(dest, s);
  memfree(s);
}
void strinsert(string *dest, string *src, ulong pos) { strinsert(dest, src->s, pos); }
void strinsert(stringlist *dest, ulong num, char *src, ulong pos) { strinsert(strget(dest, num), src, pos); }
void strinsert(stringlist *dest, ulong num, string *src, ulong pos) { strinsert(strget(dest, num), src->s, pos); }
void strinsert(stringlist *dest, ulong destnum, stringlist *src, ulong srcnum, ulong pos) { strinsert(strget(dest, destnum), strptr(src, srcnum), pos); }

void strremove(string *dest, ulong start, ulong length = 0) {
int destlen;
char *s;
int i, sl = strlen(dest->s);
  if(start > dest->size) { strresize(dest, start); }
  if(!length) {
    strset(dest, start, 0);
    return;
  }
  s = strptr(dest);
  for(i=start;i<sl;i++) { s[i] = s[i+length]; }
  s[i] = 0;
}
void strremove(stringlist *sl, ulong num, ulong start, ulong length = 0) { strremove(strget(sl, num), start, length); }

ulong strcmp(string *dest, char *src) { return strcmp(dest->s, src); }
ulong strcmp(string *dest, string *src) { return strcmp(dest->s, src->s); }
ulong strcmp(stringlist *dest, ulong num, char *src) { return strcmp(strget(dest, num)->s, src); }
ulong strcmp(stringlist *dest, ulong num, string *src) { return strcmp(strget(dest, num)->s, src->s); }
ulong strcmp(stringlist *dest, ulong destnum, stringlist *src, ulong srcnum) { return strcmp(strget(dest, destnum)->s, strptr(src, srcnum)); }

ulong stricmp(char *dest, char *src) {
int i, sl = strlen(dest);
  if(sl != strlen(src))return 1;
  for(i=0;i<sl;i++) {
    if(dest[i] >= 'A' && dest[i] <= 'Z') {
      if(dest[i]!=src[i] && dest[i]+0x20!=src[i])return 1;
    } else if(dest[i] >='a' && dest[i] <= 'z') {
      if(dest[i]!=src[i] && dest[i]-0x20!=src[i])return 1;
    } else {
      if(dest[i] != src[i])return 1;
    }
  }
  return 0;
}
ulong stricmp(string *dest, char *src) { return stricmp(dest->s, src); }
ulong stricmp(string *dest, string *src) { return stricmp(dest->s, src->s); }
ulong stricmp(stringlist *dest, ulong num, char *src) { return stricmp(strget(dest, num)->s, src); }
ulong stricmp(stringlist *dest, ulong num, string *src) { return stricmp(strget(dest, num)->s, src->s); }
ulong stricmp(stringlist *dest, ulong destnum, stringlist *src, ulong srcnum) { return stricmp(strget(dest, destnum)->s, strptr(src, srcnum)); }

void strlower(char *str) {
int i, sl = strlen(str);
  for(i=0;i<sl;i++) { if(str[i] >= 'A' && str[i] <= 'Z')str[i] += 0x20; }
}
void strlower(string *str) { strlower(str->s); }
void strlower(stringlist *sl, ulong num) { strlower(strptr(sl, num)); }

void strupper(char *str) {
int i, sl = strlen(str);
  for(i=0;i<sl;i++) { if(str[i] >='a' && str[i] <='z')str[i] -= 0x20; }
}
void strupper(string *str) { strupper(str->s); }
void strupper(stringlist *sl, ulong num) { strupper(strptr(sl, num)); }

ulong strpos(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return null;
  for(i=0;i<=ssl-ksl;i++) {
    if(!memcmp(str+i, key, ksl))return i;
  }
  return null;
}
ulong strpos(string *dest, char *src) { return strpos(dest->s, src); }
ulong strpos(string *dest, string *src) { return strpos(dest->s, src->s); }
ulong strpos(stringlist *dest, ulong num, char *src) { return strpos(strget(dest, num)->s, src); }
ulong strpos(stringlist *dest, ulong num, string *src) { return strpos(strget(dest, num)->s, src->s); }
ulong strpos(stringlist *dest, ulong destnum, stringlist *src, ulong srcnum) { return strpos(strget(dest, destnum)->s, strptr(src, srcnum)); }

ulong strpos_q(char *str, char *key) {
int i, z, ssl = strlen(str), ksl = strlen(key);
byte x;
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
ulong strpos_q(string *dest, char *src) { return strpos_q(dest->s, src); }
ulong strpos_q(string *dest, string *src) { return strpos_q(dest->s, src->s); }
ulong strpos_q(stringlist *dest, ulong num, char *src) { return strpos_q(strget(dest, num)->s, src); }
ulong strpos_q(stringlist *dest, ulong num, string *src) { return strpos_q(strget(dest, num)->s, src->s); }
ulong strpos_q(stringlist *dest, ulong destnum, stringlist *src, ulong srcnum) { return strpos_q(strget(dest, destnum)->s, strptr(src, srcnum)); }

void strtr(char *dest, char *before, char *after) {
int i, l, sl = strlen(dest), bsl = strlen(before), asl = strlen(after);
  if((bsl != asl) || bsl == 0)return;
  for(i=0;i<sl;i++) {
    for(l=0;l<bsl;l++) {
      if(dest[i] == before[l])dest[i] = after[l];
    }
  }
}
void strtr(string *dest, char *before, char *after) { strtr(dest->s, before, after); }
void strtr(stringlist *dest, ulong num, char *before, char *after) { strtr(strptr(dest, num), before, after); }

ulong strbegin(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return 1;
  if(!memcmp(str, key, ksl))return 0;
  return 1;
}
ulong strbegin(string *str, char *key) { return strbegin(str->s, key); }
ulong strbegin(stringlist *str, ulong num, char *key) { return strbegin(strptr(str, num), key); }

ulong stribegin(char *str, char *key) {
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
ulong stribegin(string *str, char *key) { return stribegin(str->s, key); }
ulong stribegin(stringlist *str, ulong num, char *key) { return stribegin(strptr(str, num), key); }

ulong strend(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return 1;
  if(!memcmp(str + ssl - ksl, key, ksl))return 0;
  return 1;
}
ulong strend(string *str, char *key) { return strend(str->s, key); }
ulong strend(stringlist *str, ulong num, char *key) { return strend(strptr(str, num), key); }

ulong striend(char *str, char *key) {
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
ulong striend(string *str, char *key) { return striend(str->s, key); }
ulong striend(stringlist *str, ulong num, char *key) { return striend(strptr(str, num), key); }

void strltrim(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!strbegin(str, key)) {
    for(i=0;i<ssl-ksl;i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}
void strltrim(string *str, char *key) { strltrim(str->s, key); }
void strltrim(stringlist *str, ulong num, char *key) { strltrim(strptr(str, num), key); }

void striltrim(char *str, char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!stribegin(str, key)) {
    for(i=0;i<ssl-ksl;i++)str[i] = str[i + ksl];
    str[i] = 0;
  }
}
void striltrim(string *str, char *key) { striltrim(str->s, key); }
void striltrim(stringlist *str, ulong num, char *key) { striltrim(strptr(str, num), key); }

void strrtrim(char *str, char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!strend(str, key)) {
    str[ssl - ksl] = 0;
  }
}
void strrtrim(string *str, char *key) { strrtrim(str->s, key); }
void strrtrim(stringlist *str, ulong num, char *key) { strrtrim(strptr(str, num), key); }

void strirtrim(char *str, char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return;
  if(!striend(str, key)) {
    str[ssl - ksl] = 0;
  }
}
void strirtrim(string *str, char *key) { strirtrim(str->s, key); }
void strirtrim(stringlist *str, ulong num, char *key) { strirtrim(strptr(str, num), key); }

ulong strhex(char *str) {
ulong r = 0, m = 0;
int i, ssl = strlen(str);
byte x;
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
ulong strhex(string *str) { return strhex(str->s); }
ulong strhex(stringlist *str, ulong num) { return strhex(strptr(str, num)); }

ulong strdec(char *str) {
ulong r = 0, m = 1;
int i, ssl = strlen(str);
byte x;
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
ulong strdec(string *str) { return strdec(str->s); }
ulong strdec(stringlist *str, ulong num) { return strdec(strptr(str, num)); }

ulong strbin(char *str) {
ulong r = 0, m = 0;
int i, ssl = strlen(str);
byte x;
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
ulong strbin(string *str) { return strbin(str->s); }
ulong strbin(stringlist *str, ulong num) { return strbin(strptr(str, num)); }

#include "libstr_math.cpp"
#include "libstr_split.cpp"
#include "libstr_replace.cpp"
#include "libstr_sprintf.cpp"
