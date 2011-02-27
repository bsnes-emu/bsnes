#ifndef NALL_STRING_CORE_HPP
#define NALL_STRING_CORE_HPP

namespace nall {

void string::reserve(unsigned size_) {
  if(size_ > size) {
    size = size_;
    data = (char*)realloc(data, size + 1);
    data[size] = 0;
  }
}

string& string::assign(const char *s) {
  unsigned length = strlen(s);
  reserve(length);
  strcpy(data, s);
  return *this;
}

string& string::append(const char *s) {
  unsigned length = strlen(data) + strlen(s);
  reserve(length);
  strcat(data, s);
  return *this;
}

string& string::append(bool value) { append(value ? "true" : "false"); return *this; }
string& string::append(signed int value) { append(integer(value)); return *this; }
string& string::append(unsigned int value) { append(decimal(value)); return *this; }
string& string::append(double value) { append(fp(value)); return *this; }

string::operator const char*() const {
  return data;
}

char* string::operator()() {
  return data;
}

char& string::operator[](int index) {
  reserve(index);
  return data[index];
}

bool string::operator==(const char *str) const { return strcmp(data, str) == 0; }
bool string::operator!=(const char *str) const { return strcmp(data, str) != 0; }
bool string::operator< (const char *str) const { return strcmp(data, str)  < 0; }
bool string::operator<=(const char *str) const { return strcmp(data, str) <= 0; }
bool string::operator> (const char *str) const { return strcmp(data, str)  > 0; }
bool string::operator>=(const char *str) const { return strcmp(data, str) >= 0; }

string& string::operator=(const string &value) {
  assign(value);
  return *this;
}

string& string::operator=(string &&source) {
  if(data) free(data);
  size = source.size;
  data = source.data;
  source.data = 0;
  source.size = 0;
  return *this;
}

static void istring(string &output) {
}

template<typename T, typename... Args>
static void istring(string &output, const T &value, Args&&... args) {
  output.append(value);
  istring(output, std::forward<Args>(args)...);
}

template<typename... Args> string::string(Args&&... args) {
  size = 64;
  data = (char*)malloc(size + 1);
  *data = 0;
  istring(*this, std::forward<Args>(args)...);
}

string::string(const string &value) {
  size = strlen(value);
  data = strdup(value);
}

string::string(string &&source) {
  size = source.size;
  data = source.data;
  source.data = 0;
}

string::~string() {
  if(data) free(data);
}

bool string::readfile(const string &filename) {
  assign("");

  #if !defined(_WIN32)
  FILE *fp = fopen(filename, "rb");
  #else
  FILE *fp = _wfopen(utf16_t(filename), L"rb");
  #endif
  if(!fp) return false;

  fseek(fp, 0, SEEK_END);
  unsigned size = ftell(fp);
  rewind(fp);
  char *fdata = new char[size + 1];
  unsigned unused = fread(fdata, 1, size, fp);
  fclose(fp);
  fdata[size] = 0;
  assign(fdata);
  delete[] fdata;

  return true;
}

optional<unsigned> lstring::find(const char *key) const {
  for(unsigned i = 0; i < size(); i++) {
    if(operator[](i) == key) return { true, i };
  }
  return { false, 0 };
}

inline lstring::lstring() {
}

inline lstring::lstring(std::initializer_list<string> list) {
  for(const string *s = list.begin(); s != list.end(); ++s) {
    operator<<(*s);
  }
}

}

#endif
