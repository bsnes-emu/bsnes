#include "zlib/zlib.h"

class GZReader : public Reader {
private:
gzFile gp;
uint32 fsize;

public:
  uint32 size();
  uint8 *read(uint32 length = 0);
  bool   ready();

  GZReader(const char *fn);
  ~GZReader();
};
