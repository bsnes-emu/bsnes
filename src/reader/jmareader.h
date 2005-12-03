//created by Nach

#include "jma/jma.h"

class JMAReader : public Reader {
private:
JMA::jma_open JMAFile;
uint32 fsize;
std::string cname;

public:
  uint32 size();
  uint8 *read(uint32 length = 0);

  JMAReader(char *fn);
  ~JMAReader() { }
};
