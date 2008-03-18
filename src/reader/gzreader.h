#include "zlib/zlib.h"

class GZReader : public Reader {
private:
  gzFile gp;
  uint32 filesize;

public:
  unsigned size();
  uint8_t* read(unsigned length = 0);
  bool ready();

  GZReader(const char *fn);
  ~GZReader();
};
