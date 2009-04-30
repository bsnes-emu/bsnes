#include <zlib/zlib.h>

class GZReader : public Reader {
private:
  FILE *fp;
  gzFile gp;
  unsigned filesize;

public:
  unsigned size();
  uint8_t* read(unsigned length = 0);
  bool ready();

  GZReader(const char *fn);
  ~GZReader();
};
