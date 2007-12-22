/*
  bconfig : version 0.16 ~byuu (2007-11-28)
  license: public domain
*/

#ifndef BCONFIG_H
#define BCONFIG_H

#include "bbase.h"
#include "barray.h"
#include "bstring.h"

class Setting;

class Config {
public:
  array<Setting*> list;
  uint list_count;

  string data;
  lstring line, part, subpart;

  bool load(const char *fn);
  bool save(const char *fn);
  void add(Setting *setting) { list[list_count++] = setting; }

  Config() : list_count(0) {}
};

class Setting {
public:
  enum Type {
    Integer,
    String,
  } type;
  char *name, *desc, *def;

  virtual void set(const char *input) = 0;
  virtual void get(string &output)    = 0;
};

class IntegerSetting : public Setting {
public:
  enum Format {
    Boolean,
    Decimal,
    Hex,
  } ifmt;
  uint data, idef;

  void set(const char *input) {
    if(ifmt == Boolean) { data = !strcmp(input, "true"); }
    if(ifmt == Decimal) { data = strdec(input); }
    if(ifmt == Hex)     { data = strhex(input + (stribegin(input, "0x") ? 2 : 0)); }
  }

  void get(string &output) {
    output.reserve(64);
    if(ifmt == Boolean) { sprintf(output(), "%s", data ? "true" : "false"); }
    if(ifmt == Decimal) { sprintf(output(), "%d", data); }
    if(ifmt == Hex)     { sprintf(output(), "0x%x", data); }
  }

  uint operator()() { return data; }
  operator uint() { return data; }
  template<typename T> IntegerSetting &operator=(T x) { data = uint(x); return *this; }
  template<typename T> bool operator==(T x) { return (T(data) == x); }
  template<typename T> bool operator!=(T x) { return (T(data) != x); }
  template<typename T> bool operator>=(T x) { return (T(data) >= x); }
  template<typename T> bool operator> (T x) { return (T(data) >  x); }
  template<typename T> bool operator<=(T x) { return (T(data) <= x); }
  template<typename T> bool operator< (T x) { return (T(data) <  x); }

  IntegerSetting(Config *parent, const char *r_name, const char *r_desc, Format r_format, uint r_data) {
    type = Setting::Integer;
    name = strdup(r_name);
    desc = strdup(r_desc);
    ifmt = r_format;
    data = idef = r_data;
  string t;
    get(t);
    def = strdup(t);

    if(parent) { parent->add(this); }
  }

  ~IntegerSetting() {
    safe_free(name);
    safe_free(desc);
    safe_free(def);
  }
};

class StringSetting : public Setting {
public:
  string data;
  void set(const char *input) { data = input; trim(data(), "\""); }
  void get(string &output) { output = string() << "\"" << data << "\""; }

  char* operator()() { return data(); }
  operator const char*() { return data; }
  StringSetting &operator=(const char *x) { data = x; return *this; }
  bool operator==(const char *x) { return !strcmp(data, x); }
  bool operator!=(const char *x) { return  strcmp(data, x); }

  StringSetting(Config *parent, const char *r_name, const char *r_desc, const char *r_data) {
    type = Setting::String;
    name = strdup(r_name);
    desc = strdup(r_desc);
    data = r_data;
  string t;
    get(t);
    def = strdup(t);

    if(parent) { parent->add(this); }
  }

  ~StringSetting() {
    safe_free(name);
    safe_free(desc);
    safe_free(def);
  }
};

inline bool Config::load(const char *fn) {
FILE *fp = fopen(fn, "rb");
  if(!fp)return false;

//load the config file into memory
  fseek(fp, 0, SEEK_END);
int fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
char *buffer = (char*)malloc(fsize + 1);
  fread(buffer, 1, fsize, fp);
  fclose(fp);
  *(buffer + fsize) = 0;
  strcpy(data, buffer);
  free(buffer);

//split the file into lines
  replace(data, "\r\n", "\n");
  qreplace(data, "\t", "");
  qreplace(data, " ", "");
  split(line, "\n", data);

  for(int i = 0; i < count(line); i++) {
    if(strlen(line[i]) == 0)continue;
    if(*line[i] == '#')continue;

    qsplit(part, "=", line[i]);
    for(int l = 0; l < list_count; l++) {
      if(!strcmp(list[l]->name, part[0])) {
        list[l]->set(part[1]);
        break;
      }
    }
  }

  return true;
}

inline bool Config::save(const char *fn) {
FILE *fp;
string t;
  fp = fopen(fn, "wb");
  if(!fp)return false;

  for(int i = 0; i < list_count; i++) {
    strcpy(data, list[i]->desc);
    replace(data, "\r\n", "\n");
    split(line, "\n", data);
    for(int l = 0; l < count(line); l++) {
      if(line[l] != "") { fprintf(fp, "# %s\r\n", line[l]()); }
    }

    fprintf(fp, "# (default = %s)\r\n", list[i]->def);
    list[i]->get(t);
    fprintf(fp, "%s = %s\r\n\r\n", list[i]->name, t());
  }

  fclose(fp);
  return true;
}

#endif //ifndef BCONFIG_H
