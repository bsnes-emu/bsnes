#include <nall/file.hpp>
#include <nall/platform.hpp>
#include <nall/stdint.hpp>
#include <nall/string.hpp>
using namespace nall;

class Reader {
public:
  enum Type {
    Unknown,
    Normal,
    GZIP,
    ZIP,
    JMA,
  };

  static Type detect(const char *fn, bool inspectheader);
  virtual unsigned size() = 0;
  virtual uint8_t* read(unsigned length = 0) = 0;
  virtual bool ready() { return true; }  //can only call read() when ready() returns true
};
