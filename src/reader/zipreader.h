#include "zlib/unzip.h"

//Could be up to 65536
#define ZIP_MAX_FILE_NAME 4096

class ZipReader : public Reader {
public:
  unsigned size();
  uint8_t* read(unsigned length = 0);
  bool ready();

  ZipReader(const char *fn);
  ~ZipReader();

private:
  unzFile zipfile;
  uint32 filesize;
  bool zipready;
  char cname[4096];
};
