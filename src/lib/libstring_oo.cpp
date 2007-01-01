const char *string::operator()() {
  return s;
}

char &string::operator[](const uint index) {
  reserve(index);
  return s[index];
}

string &string::operator=(int num) {
  strcpy(*this, strfmt("%d", num));
  return *this;
}

string &string::operator=(const char *str) {
  strcpy(*this, str);
  return *this;
}

string &string::operator=(string &str) {
  strcpy(*this, str);
  return *this;
}

string &string::operator+=(int num) {
  strcat(*this, strfmt("%d", num));
  return *this;
}

string &string::operator+=(const char *str) {
  strcat(*this, str);
  return *this;
}

string &string::operator+=(string &str) {
  strcat(*this, str);
  return *this;
}

bool string::operator==(const char *str) { return strcmp(strptr(*this), str)         == 0; }
bool string::operator==(string &str)     { return strcmp(strptr(*this), strptr(str)) == 0; }
bool string::operator!=(const char *str) { return strcmp(strptr(*this), str)         != 0; }
bool string::operator!=(string &str)     { return strcmp(strptr(*this), strptr(str)) != 0; }
bool string::operator<(const char *str)  { return strcmp(strptr(*this), str)          < 0; }
bool string::operator<(string &str)      { return strcmp(strptr(*this), strptr(str))  < 0; }
bool string::operator<=(const char *str) { return strcmp(strptr(*this), str)         <= 0; }
bool string::operator<=(string &str)     { return strcmp(strptr(*this), strptr(str)) <= 0; }
bool string::operator>(const char *str)  { return strcmp(strptr(*this), str)          > 0; }
bool string::operator>(string &str)      { return strcmp(strptr(*this), strptr(str))  > 0; }
bool string::operator>=(const char *str) { return strcmp(strptr(*this), str)         >= 0; }
bool string::operator>=(string &str)     { return strcmp(strptr(*this), strptr(str)) >= 0; }

string string::operator+(int num) {
string temp(*this);
  strcat(temp, strfmt("%d", num));
  return temp;
}

string string::operator+(const char *str) {
string temp(*this);
  strcat(temp, str);
  return temp;
}

string string::operator+(string &str) {
string temp(*this);
  strcat(temp, str);
  return temp;
}

//

string operator+(int x, string &y) {
string temp(strfmt("%d", x));
  strcat(temp, y);
  return temp;
}

string operator+(const char *x, string &y) {
string temp(x);
  strcat(temp, y);
  return temp;
}
