#ifndef NALL_CONFIG_HPP
#define NALL_CONFIG_HPP

#include <nall/file.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>

namespace nall {
  namespace configuration_traits {
    template<typename T> struct is_boolean { enum { value = false }; };
    template<> struct is_boolean<bool> { enum { value = true }; };

    template<typename T> struct is_signed { enum { value = false }; };
    template<> struct is_signed<signed> { enum { value = true }; };

    template<typename T> struct is_unsigned { enum { value = false }; };
    template<> struct is_unsigned<unsigned> { enum { value = true }; };

    template<typename T> struct is_string { enum { value = false }; };
    template<> struct is_string<string> { enum { value = true }; };
  }

  class configuration {
  public:
    enum type_t { boolean_t, signed_t, unsigned_t, string_t, unknown_t };
    struct item_t {
      uintptr_t data;
      string name;
      string desc;
      type_t type;

      string get() {
        switch(type) {
          case boolean_t:  return (*(bool*)data == false ? "false" : "true");
          case signed_t:   return string() << (int)*(signed*)data;
          case unsigned_t: return string() << (int)*(unsigned*)data;
          case string_t:   return string() << "\"" << *(string*)data << "\"";
        }
        return "???";
      }

      void set(string s) {
        switch(type) {
          case boolean_t:  *(bool*)data = (s == "true");      break;
          case signed_t:   *(signed*)data = s;                break;
          case unsigned_t: *(unsigned*)data = s;              break;
          case string_t:   trim(s, "\""); *(string*)data = s; break;
        }
      }
    };
    vector<item_t> list;

    template<typename T>
    void attach(T &data, const char *name, const char *desc = "") {
      unsigned n = list.size();
      list[n].data = (uintptr_t)&data;
      list[n].name = name;
      list[n].desc = desc;

      if(configuration_traits::is_boolean<T>::value) list[n].type = boolean_t;
      else if(configuration_traits::is_signed<T>::value) list[n].type = signed_t;
      else if(configuration_traits::is_unsigned<T>::value) list[n].type = unsigned_t;
      else if(configuration_traits::is_string<T>::value) list[n].type = string_t;
      else list[n].type = unknown_t;
    }

    bool load(const char *filename) {
      string data;
      if(fread(data, filename) == true) {
        replace(data, "\r", "");
        lstring line;
        split(line, "\n", data);

        for(unsigned i = 0; i < count(line); i++) {
          int position = qstrpos(line[i], "#");
          if(position >= 0) line[i][position] = 0;
          if(qstrpos(line[i], " = ") < 0) continue;

          lstring part;
          qsplit(part, " = ", line[i]);
          trim(part[0]);
          trim(part[1]);

          for(unsigned n = 0; n < list.size(); n++) {
            if(part[0] == list[n].name) {
              list[n].set(part[1]);
              break;
            }
          }
        }

        return true;
      } else {
        return false;
      }
    }

    bool save(const char *filename) {
      file fp;
      if(fp.open(filename, file::mode_write)) {
        for(unsigned i = 0; i < list.size(); i++) {
          fp.print(string() << list[i].name << " = " << list[i].get() << " # " << list[i].desc << "\r\n");
        }

        fp.close();
        return true;
      } else {
        return false;
      }
    }
  };
}

#endif
