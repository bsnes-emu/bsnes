#ifndef NALL_STRING_TRIM_CPP
#define NALL_STRING_TRIM_CPP

char* ltrim(char *str, const char *key) {
  if(!key || !*key) return str;
  while(strbegin(str, key)) strcpy(str, str + strlen(key));
  return str;
}

char* rtrim(char *str, const char *key) {
  if(!key || !*key) return str;
  while(strend(str, key)) str[strlen(str) - strlen(key)] = 0;
  return str;
}

char* trim(char *str, const char *key) {
  return ltrim(rtrim(str, key), key);
}

char* ltrim_once(char *str, const char *key) {
  if(!key || !*key) return str;
  if(strbegin(str, key)) strcpy(str, str + strlen(key));
  return str;
}

char* rtrim_once(char *str, const char *key) {
  if(!key || !*key) return str;
  if(strend(str, key)) str[strlen(str) - strlen(key)] = 0;
  return str;
}

char* trim_once(char *str, const char *key) {
  return ltrim_once(rtrim_once(str, key), key);
}

#endif //ifndef NALL_STRING_TRIM_CPP
