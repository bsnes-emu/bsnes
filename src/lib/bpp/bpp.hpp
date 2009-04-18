#include <nall/array.hpp>
#include <nall/file.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
#include <nall/vector.hpp>
using namespace nall;

class BPP {
public:
  bool process(const char *outputfn, const char *inputfn);

private:
  nall::file output;

  struct Global {
    string name;
    string data;
  };
  vector<Global> global;

  struct Macro {
    string name;
    unsigned args;
    string arg[8];
    string data;
  };
  vector<Macro> macro;

  bool process_macro(Macro &macro, const lstring &arg);
  bool process(const char *inputfn);
};
