char &string::operator[](const uint index) {
  reserve(index);
  return s[index];
}

string &string::operator=(const int num) {
  strcpy(*this, strfmt("%d", num));
  return *this;
}

string &string::operator=(const char *str) {
  strcpy(*this, str);
  return *this;
}

string &string::operator=(const string &str) {
  strcpy(*this, str);
  return *this;
}

string& string::operator<<(const int num) {
  strcat(*this, strfmt("%d", num));
  return *this;
}

string& string::operator<<(const char* str) {
  strcat(*this, str);
  return *this;
}

string& string::operator<<(const string& str) {
  strcat(*this, str);
  return *this;
}

bool string::operator==(const char   *str) { return strcmp(strptr(*this), str)         == 0; }
bool string::operator==(const string &str) { return strcmp(strptr(*this), strptr(str)) == 0; }
bool string::operator!=(const char   *str) { return strcmp(strptr(*this), str)         != 0; }
bool string::operator!=(const string &str) { return strcmp(strptr(*this), strptr(str)) != 0; }
bool string::operator< (const char   *str) { return strcmp(strptr(*this), str)          < 0; }
bool string::operator< (const string &str) { return strcmp(strptr(*this), strptr(str))  < 0; }
bool string::operator<=(const char   *str) { return strcmp(strptr(*this), str)         <= 0; }
bool string::operator<=(const string &str) { return strcmp(strptr(*this), strptr(str)) <= 0; }
bool string::operator> (const char   *str) { return strcmp(strptr(*this), str)          > 0; }
bool string::operator> (const string &str) { return strcmp(strptr(*this), strptr(str))  > 0; }
bool string::operator>=(const char   *str) { return strcmp(strptr(*this), str)         >= 0; }
bool string::operator>=(const string &str) { return strcmp(strptr(*this), strptr(str)) >= 0; }
