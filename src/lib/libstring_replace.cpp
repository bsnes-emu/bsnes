string &replace(string &str, const char *key, const char *token) {
int  i, z, ksl = strlen(key), tsl = strlen(token), ssl = strlen(str);
uint replace_count = 0, size = ssl;
char *data;
  if(ksl > ssl)return str;
  if(tsl > ksl) {                      //the new string may be longer than the old string...
    for(i = 0; i <= ssl - ksl;) {      //so let's find out how big of a string we'll need...
      if(!memcmp(str.s + i, key, ksl)) {
        replace_count++;
        i += ksl;
      } else i++;
    }
    size = ssl + ((tsl - ksl) * replace_count);
    str.reserve(size);
  }
  data = (char*)malloc(size + 1);
  for(i = z = 0; i < ssl;) {
    if(i <= ssl - ksl) {
      if(!memcmp(str.s + i, key, ksl)) {
        memcpy(data + z, token, tsl);
        z += tsl;
        i += ksl;
      } else data[z++] = str.s[i++];
    } else data[z++] = str.s[i++];
  }
  data[z] = 0;
  strcpy(str, data);
  free(data);
  return str;
}
string &replace(string &str, const char *key, const string &token) { return replace(str, key, strptr(token)); }

string &qreplace(string &str, const char *key, const char *token) {
int   i, l, z, ksl = strlen(key), tsl = strlen(token), ssl = strlen(str);
uint  replace_count = 0, size = ssl;
uint8 x;
char *data;
  if(ksl > ssl)return str;
  if(tsl > ksl) {
    for(i = 0; i <= ssl - ksl;) {
      x = str.s[i];
      if(x == '\"' || x == '\'') {
        l = i;
        i++;
        while(str.s[i++] != x) {
          if(i == ssl) {
            i = l;
            break;
          }
        }
      }
      if(!memcmp(str.s + i, key, ksl)) {
        replace_count++;
        i += ksl;
      } else i++;
    }
    size = ssl + ((tsl - ksl) * replace_count);
    str.reserve(size);
  }
  data = (char*)malloc(size + 1);
  for(i = z = 0; i < ssl;) {
    x = str.s[i];
    if(x == '\"' || x == '\'') {
      l = i++;
      while(str.s[i] != x && i < ssl)i++;
      if(i >= ssl)i = l;
      else {
        memcpy(data + z, str.s + l, i - l);
        z += i - l;
      }
    }
    if(i <= ssl - ksl) {
      if(!memcmp(str.s + i, key, ksl)) {
        memcpy(data + z, token, tsl);
        z += tsl;
        i += ksl;
        replace_count++;
      } else data[z++] = str.s[i++];
    } else data[z++] = str.s[i++];
  }
  data[z] = 0;
  strcpy(str, data);
  free(data);
  return str;
}
string &qreplace(string &str, const char *key, const string &token) { return qreplace(str, key, strptr(token)); }
