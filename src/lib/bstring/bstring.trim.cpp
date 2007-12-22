#ifdef BSTRING_H

char* ltrim(char *str, const char *key) {
  if(!key || !*key) return str;
  while(strbegin(str, key)) {
  string temp = str;
    strcpy(str, temp() + strlen(key));
  }
  return str;
}

char* rtrim(char *str, const char *key) {
  if(!key || !*key) return str;
  while(strend(str, key)) {
    str[strlen(str) - strlen(key)] = 0;
  }
  return str;
}

char* trim(char *str, const char *key) {
  return ltrim(rtrim(str, key), key);
}

string& ltrim(string &str, const char *key) {
  ltrim(str(), key);
  return str;
}

string& rtrim(string &str, const char *key) {
  rtrim(str(), key);
  return str;
}

string& trim(string &str, const char *key) {
  trim(str(), key);
  return str;
}

#endif //ifdef BSTRING_H
