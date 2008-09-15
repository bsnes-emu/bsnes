#ifndef NALL_CONFIG_HPP
#define NALL_CONFIG_HPP

#include <nall/array.hpp>
#include <nall/file.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>

namespace nall {

class setting;

class configuration {
public:
  array<setting*> list;

  bool load(const char *fn);
  bool save(const char *fn) const;
  void add(setting *setting_) { list.add(setting_); }
};

class setting {
public:
  enum setting_type {
    integral_type,
    string_type,
  } type;

  const char *name;
  const char *description;

  virtual void set(const char *input) = 0;
  virtual void get(string &output) const = 0;
  virtual void get_default(string &output) const = 0;
};

class integral_setting : public setting {
public:
  enum integral_type {
    boolean,
    decimal,
    hex,
  } type;

  intmax_t value;
  intmax_t default_value;

  void set(const char *input) {
    if(type == boolean) { value = !strcmp(input, "true"); }
    if(type == decimal) { value = strdec(input); }
    if(type == hex) { value = strhex(input); }
  }

  void get(string &output) const {
    if(type == boolean) { output = value ? "true" : "false"; }
    if(type == decimal) { output = strdec(value); }
    if(type == hex) { output = string() << "0x" << strhex(value); }
  }

  void get_default(string &output) const {
    if(type == boolean) { output = default_value ? "true" : "false"; }
    if(type == decimal) { output = strdec(default_value); }
    if(type == hex) { output = string() << "0x" << strhex(default_value); }
  }

  operator intmax_t() const { return value; }
  integral_setting& operator=(intmax_t value_) { value = value_; return *this; }

  integral_setting(const char *name_, const char *description_, integral_type type_, intmax_t value_) {
    initialize(name_, description_, type_, value_);
  }

  integral_setting(configuration &parent, const char *name_, const char *description_, integral_type type_, intmax_t value_) {
    initialize(name_, description_, type_, value_);
    parent.add(this);
  }

private:
  void initialize(const char *name_, const char *description_, integral_type type_, intmax_t value_) {
    setting::type = setting::integral_type;
    name = name_;
    description = description_;
    type = type_;
    value = default_value = value_;
  }
};

class string_setting : public setting {
public:
  string value;
  string default_value;

  void set(const char *input) { value = input; trim(value(), "\""); }
  void get(string &output) const { output = string() << "\"" << value << "\""; }
  void get_default(string &output) const { output = string() << "\"" << default_value << "\""; }

  operator const char*() const { return value; }
  string_setting& operator=(const char *value_) { value = value_; return *this; }
  bool operator==(const char *value_) const { return value == value_; }
  bool operator!=(const char *value_) const { return value != value_; }

  string_setting(const char *name_, const char *description_, const char *value_) {
    initialize(name_, description_, value_);
  }

  string_setting(configuration &parent, const char *name_, const char *description_, const char *value_) {
    initialize(name_, description_, value_);
    parent.add(this);
  }

private:
  void initialize(const char *name_, const char *description_, const char *value_) {
    setting::type = setting::string_type;
    name = name_;
    description = description_;
    value = default_value = value_;
  }
};

inline bool configuration::load(const char *fn) {
  //load the config file into memory
  string data;
  if(!fread(data, fn)) return false;

  //split the file into lines
  replace(data, "\r\n", "\n");
  qreplace(data, "\t", "");
  qreplace(data, " ", "");

  lstring line, part, subpart;
  split(line, "\n", data);

  for(unsigned i = 0; i < count(line); i++) {
    if(strlen(line[i]) == 0) continue;
    if(strbegin(line[i], "#")) continue;

    qsplit(part, "=", line[i]);
    for(unsigned l = 0; l < list.size(); l++) {
      if(part[0] == list[l]->name) {
        list[l]->set(part[1]);
        break;
      }
    }
  }

  return true;
}

inline bool configuration::save(const char *fn) const {
  file fp;
  if(!fp.open(fn, file::mode_write)) return false;

  for(unsigned i = 0; i < list.size(); i++) {
    string data;
    lstring line, part, subpart;
    strcpy(data, list[i]->description);
    replace(data, "\r\n", "\n");
    split(line, "\n", data);

    string temp;
    for(unsigned l = 0; l < count(line); l++) {
      if(line[l] != "") fp.print(string() << "# " << line[l] << "\r\n");
    }

    string default_, value_;
    list[i]->get_default(default_);
    fp.print(string() << "# (default = " << default_ << ")\r\n");
    list[i]->get(value_);
    fp.print(string() << list[i]->name << " = " << value_ << "\r\n\r\n");
  }

  fp.close();
  return true;
}

} //namespace nall

#endif //ifndef NALL_CONFIG_HPP
