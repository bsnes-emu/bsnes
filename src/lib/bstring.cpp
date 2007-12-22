#include "bstring.h"

#include "bstring/bstring.list.cpp"
#include "bstring/bstring.strl.cpp"
#include "bstring/bstring.math.cpp"
#include "bstring/bstring.misc.cpp"
#include "bstring/bstring.replace.cpp"
#include "bstring/bstring.split.cpp"
#include "bstring/bstring.trim.cpp"

void string::reserve(uint size_) {
  if(size_ > size) {
    size = size_;
    data = (char*)realloc(data, size + 1);
    data[size] = 0;
  }
}

//implicit-conversion, read-only
string::operator const char*() const {
  return data;
}

//explicit-coversion, read-write
char* string::operator()() {
  return data;
}

//index, read-write
char& string::operator[](const uint index) {
  reserve(index);
  return data[index];
}

string& string::operator=(const int num) {
  itoa(*this, num);
  return *this;
}

string& string::operator=(const char *str) {
  strcpy(*this, str);
  return *this;
}

string& string::operator=(const string &str) {
  strcpy(*this, str);
  return *this;
}

string& string::operator<<(const int num) {
string temp(num);
  strcat(*this, temp);
  return *this;
}

string& string::operator<<(const char *str) {
  strcat(*this, str);
  return *this;
}

bool string::operator==(const char *str) { return strcmp(data, str) == 0; }
bool string::operator!=(const char *str) { return strcmp(data, str) != 0; }
bool string::operator< (const char *str) { return strcmp(data, str)  < 0; }
bool string::operator<=(const char *str) { return strcmp(data, str) <= 0; }
bool string::operator> (const char *str) { return strcmp(data, str)  > 0; }
bool string::operator>=(const char *str) { return strcmp(data, str) >= 0; }

string::string() {
  size = 64;
  data = (char*)malloc(size + 1);
  *data = 0;
}

string::string(const int source) {
  size = 64;
  data = (char*)malloc(size + 1);
  *data = 0;
  itoa(*this, source);
}

string::string(const char *source) {
  size = strlen(source);
  data = (char*)malloc(size + 1);
  strcpy(data, source);
}

string::string(const string &source) {
  size = strlen(source);
  data = (char*)malloc(size + 1);
  strcpy(data, source);
}

string::~string() {
  safe_free(data);
}
