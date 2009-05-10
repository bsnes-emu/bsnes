#include <zlib/unzip.h>

#define ZIP_MAX_FILE_NAME PATH_MAX

class ZipReader : public Reader {
public:
  unsigned size();
  uint8_t* read(unsigned length = 0);
  bool ready();

  ZipReader(const char *fn);
  ~ZipReader();

private:
  unzFile zipfile;
  unsigned filesize;
  bool zipready;
  char cname[PATH_MAX];
};
