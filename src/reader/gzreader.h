#include "zlib/zlib.h"

class GZReader : public Reader {
private:
gzFile gp;
uint32 fsize;

public:
  uint32 size();
  uint8 *read(uint32 length = 0);
  bool   open(char *fn);
  void   close();

  GZReader() { gp = 0; fsize = 0; }
  ~GZReader() { if(gp)gzclose(gp); }
};
