//created by Nach

#include "zlib/unzip.h"

//Could be up to 65536
#define ZIP_MAX_FILE_NAME 4096

class ZipReader : public Reader {
private:
unzFile zipfile;
uint32 fsize;
char cname[4096];

public:
  uint32 size();
  uint8 *read(uint32 length = 0);

  ZipReader(const char *fn);
  ~ZipReader()
  {
    if (zipfile)
    {
      unzCloseCurrentFile(zipfile);
      unzClose(zipfile);
    }
  }
};
