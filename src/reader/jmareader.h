#include "jma/jma.h"

class JMAReader : public Reader {
public:
  unsigned size();
  uint8_t* read(unsigned length = 0);

  JMAReader(const char *fn);

private:
  JMA::jma_open JMAFile;
  uint32 filesize;
  std::string cname;
};
