#ifndef NALL_DICTIONARY_HPP
#define NALL_DICTIONARY_HPP

#include <nall/array.hpp>
#include <nall/string.hpp>
#include <nall/utility.hpp>

namespace nall {

class dictionary : noncopyable {
public:
  const char* operator[](const char *input) const {
    for(unsigned i = 0; i < index_input.size(); i++) {
      if(!strcmp(input, index_input[i])) return index_output[i];
    }
    return input; //no match, return input rather than null string
  }

  bool import(const char *filename) {
    string data;
    if(fread(data, filename) == false) return false;
    replace(data, "\r", "");

    lstring line;
    split(line, "\n", data);
    for(unsigned i = 0; i < count(line); i++) {
      lstring part;
      //format: "Input" = "Output"
      qsplit(part, "=", line[i]);
      if(count(part) != 2) continue;

      //remove whitespace
      trim(part[0]);
      trim(part[1]);

      //remove quotes
      trim_once(part[0], "\"");
      trim_once(part[1], "\"");

      unsigned n = index_input.size();
      index_input[n]  = strdup(part[0]);
      index_output[n] = strdup(part[1]);
    }
  }

  void reset() {
    for(unsigned i = 0; i < index_input.size(); i++) {
      free(index_input[i]);
      free(index_output[i]);
    }
    index_input.reset();
    index_output.reset();
  }

  ~dictionary() {
    reset();
  }

private:
  array<char*> index_input;
  array<char*> index_output;
};

} //namespace nall

#endif //ifndef NALL_DICTIONARY_HPP
