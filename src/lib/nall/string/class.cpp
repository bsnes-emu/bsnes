#ifdef NALL_STRING_CPP

size_t count(nall::lstring &str) {
  return str.size();
}

int find(nall::lstring &str, const char *key) {
  for(size_t i = 0; i < count(str); i++) {
    if(str[i] == key) return i;
  }
  return -1;
}

namespace nall {

void string::reserve(size_t size_) {
  if(size_ > size) {
    size = size_;
    data = (char*)realloc(data, size + 1);
    data[size] = 0;
  }
}

//implicit-conversion, read-only
string::operator int() const {
  return strdec(data);
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
char& string::operator[](int index) {
  reserve(index);
  return data[index];
}

string& string::operator=(int num) {
  operator=(strdec(num));
  return *this;
}

string& string::operator=(double num) {
  operator=(strdouble(num));
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

string& string::operator<<(int num) {
string temp(num);
  strcat(*this, temp);
  return *this;
}

string& string::operator<<(const char *str) {
  strcat(*this, str);
  return *this;
}

string& string::operator<<(const string &str) {
  strcat(*this, str);
  return *this;
}

bool string::operator==(const char *str) const { return strcmp(data, str) == 0; }
bool string::operator!=(const char *str) const { return strcmp(data, str) != 0; }
bool string::operator< (const char *str) const { return strcmp(data, str)  < 0; }
bool string::operator<=(const char *str) const { return strcmp(data, str) <= 0; }
bool string::operator> (const char *str) const { return strcmp(data, str)  > 0; }
bool string::operator>=(const char *str) const { return strcmp(data, str) >= 0; }

string::string() {
  size = 64;
  data = (char*)malloc(size + 1);
  *data = 0;
}

string::string(int source) {
  size = 64;
  data = (char*)malloc(size + 1);
  *data = 0;
  operator=(strdec(source));
}

string::string(double source) {
  size = 64;
  data = (char*)malloc(size + 1);
  *data = 0;
  operator=(strdouble(source));
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
  if(data) free(data);
}

} //namespace nall

void strcpy(nall::string &dest, const char *src) {
  int srclen = strlen(src);
  dest.reserve(srclen);
  strcpy(dest(), src);
}

void strcat(nall::string &dest, const char *src) {
  int srclen  = strlen(src);
  int destlen = strlen(dest);
  dest.reserve(srclen + destlen);
  strcat(dest(), src);
}

size_t strlcpy(nall::string &dest, const char *src, size_t length) {
  dest.reserve(length);
  return strlcpy(dest(), src, length);
}

size_t strlcat(nall::string &dest, const char *src, size_t length) {
  dest.reserve(length);
  return strlcat(dest(), src, length);
}

nall::string substr(const char *src, size_t start, size_t length) {
nall::string dest;
  if(length == 0) { //copy entire string
    strcpy(dest, src + start);
  } else { //copy partial string
    strlcpy(dest, src + start, length + 1);
  }
  return dest;
}

/* very simplistic wrappers to return nall::string& instead of char* type */

nall::string& strlower(nall::string &str) { strlower(str()); return str; }
nall::string& strupper(nall::string &str) { strupper(str()); return str; }
nall::string& strtr(nall::string &dest, const char *before, const char *after) { strtr(dest(), before, after); return dest; }
nall::string& ltrim(nall::string &str, const char *key) { ltrim(str(), key); return str; }
nall::string& rtrim(nall::string &str, const char *key) { rtrim(str(), key); return str; }
nall::string& trim (nall::string &str, const char *key) { trim (str(), key); return str; }
nall::string& ltrim_once(nall::string &str, const char *key) { ltrim_once(str(), key); return str; }
nall::string& rtrim_once(nall::string &str, const char *key) { rtrim_once(str(), key); return str; }
nall::string& trim_once (nall::string &str, const char *key) { trim_once (str(), key); return str; }

/* arithmetic <> string */

nall::string strhex(uintmax_t value) {
nall::string temp;
  temp.reserve(strhex(0, value));
  strhex(temp(), value);
  return temp;
}

nall::string strdec(intmax_t value) {
nall::string temp;
  temp.reserve(strdec(0, value));
  strdec(temp(), value);
  return temp;
}

nall::string strbin(uintmax_t value) {
nall::string temp;
  temp.reserve(strbin(0, value));
  strbin(temp(), value);
  return temp;
}

nall::string strdouble(double value) {
nall::string temp;
  temp.reserve(strdouble(0, value));
  strdouble(temp(), value);
  return temp;
}

/* ... */

bool fread(nall::string &str, const char *filename) {
  strcpy(str, "");

FILE *fp = fopen(filename, "rb");
  if(!fp)return false;

  fseek(fp, 0, SEEK_END);
size_t size = ftell(fp);
  rewind(fp);
char *fdata = (char*)malloc(size + 1);
  fread(fdata, 1, size, fp);
  fclose(fp);
  fdata[size] = 0;
  strcpy(str, fdata);
  free(fdata);

  return true;
}

#endif //ifdef NALL_STRING_CPP
