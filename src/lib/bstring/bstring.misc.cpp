#ifdef BSTRING_H

char chrlower(char c) {
  return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

char chrupper(char c) {
  return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
}

int stricmp(const char *dest, const char *src) {
  while(*dest) {
    if(chrlower(*dest) != chrlower(*src)) break;
    dest++;
    src++;
  }

  return (int)chrlower(*dest) - (int)chrlower(*src);
}

void strcpy(string &dest, const char *src) {
int srclen = strlen(src);
  dest.reserve(srclen);
  strcpy(dest(), src);
}

void strcat(string &dest, const char *src) {
int srclen  = strlen(src);
int destlen = strlen(dest);
  dest.reserve(srclen + destlen);
  strcat(dest(), src);
}

string substr(const char *src, uint start, uint length) {
string dest;
  if(length == 0) {
  //copy entire string
    strcpy(dest, src + start);
  } else {
  //copy partial string
    strlcpy(dest, src + start, length + 1);
  }
  return dest;
}

char* strlower(char *str) {
uint i = 0;
  while(str[i]) {
    str[i] = chrlower(str[i]);
    i++;
  }
  return str;
}

char* strupper(char *str) {
uint i = 0;
  while(str[i]) {
    str[i] = chrupper(str[i]);
    i++;
  }
  return str;
}

string& strlower(string &str) {
  strlower(str());
  return str;
}

string& strupper(string &str) {
  strupper(str());
  return str;
}

int strpos(const char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl) return -1;
  for(int i = 0; i <= ssl - ksl; i++) {
    if(!memcmp(str + i, key, ksl)) {
      return i;
    }
  }
  return -1;
}

int qstrpos(const char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl) return -1;
  for(int i = 0; i <= ssl - ksl;) {
  uint8 x = str[i];
    if(x == '\"' || x == '\'') {
    uint8 z = i++;
      while(str[i] != x && i < ssl) i++;
      if(i >= ssl) i = z;
    }
    if(!memcmp(str + i, key, ksl)) {
      return i;
    } else {
      i++;
    }
  }
  return -1;
}

char* strtr(char *dest, const char *before, const char *after) {
int sl = strlen(dest), bsl = strlen(before), asl = strlen(after);
  if(bsl != asl || bsl == 0) return dest; //patterns must be the same length for 1:1 replace
  for(int i = 0; i < sl; i++) {
    for(int l = 0; l < bsl; l++) {
      if(dest[i] == before[l]) {
        dest[i] = after[l];
        break;
      }
    }
  }
  return dest;
}

string& strtr(string &dest, const char *before, const char *after) {
  strtr(dest(), before, after);
  return dest;
}

bool strbegin(const char *str, const char *key) {
int i, ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  return (!memcmp(str, key, ksl));
}

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

bool strend(const char *str, const char *key) {
int ssl = strlen(str), ksl = strlen(key);
  if(ksl > ssl)return false;
  return (!memcmp(str + ssl - ksl, key, ksl));
}

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

uint strhex(const char *str) {
uint r = 0, m = 0;
int i = 0, ssl = strlen(str);
uint8 x;
bool negate = (str[0] == '-');
  if(negate)i++;
  for(; i < ssl; i++) {
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
    else break;
    r |= x << m;
  }
  return !negate ? r : (uint)-r;
}

uint strdec(const char *str) {
uint m = 1;
int i = 0, r = 0, ssl = strlen(str);
uint8 x;
bool negate = (str[0] == '-');
  if(negate)i++;
  for(; i < ssl; i++) {
    if(str[i] >= '0' && str[i] <= '9');
    else break;
  }
  for(--i; i >= 0; i--, m *= 10) {
    x = str[i];
    if(x >= '0' && x <= '9')x -= '0';
    else break;
    r += x * m;
  }
  return !negate ? r : (uint)-r;
}

uint strbin(const char *str) {
uint r = 0, m = 0;
int i = 0, ssl = strlen(str);
uint8 x;
bool negate = (str[0] == '-');
  if(negate)i++;
  for(; i < ssl; i++) {
    if(str[i] == '0' || str[i] == '1');
    else break;
  }
  for(--i; i >= 0; i--, m++) {
    x = str[i];
    if(str[i] == '0' || str[i] == '1')x -= '0';
    else break;
    r |= x << m;
  }
  return !negate ? r : (uint)-r;
}

string &utoa(string &str, uint num) {
  str.reserve(16);
  sprintf(str(), "%u", num);
  return str;
}

string &itoa(string &str, uint num) {
  str.reserve(16);
  sprintf(str(), "%d", num);
  return str;
}

string &htoa(string &str, uint num) {
  str.reserve(16);
  sprintf(str(), "%x", num);
  return str;
}

string &btoa(string &str, uint num) {
  str.reserve(64);

char *pstr = str();
uint mask = 0x80000000;
  while(mask && (num & mask) == 0)mask >>= 1;
  while(mask > 1) {
    *pstr++ = (num & mask) ? '1' : '0';
    mask >>= 1;
  }
  *pstr++ = (num & mask) ? '1' : '0';
  *pstr = 0;

  return str;
}

bool fread(string &str, const char *filename) {
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

#endif //ifdef BSTRING_H
