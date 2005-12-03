class Reader {
public:
enum {
  RF_NORMAL = 0,
  RF_GZ     = 1,
  RF_ZIP    = 2,
  RF_JMA    = 3
};
//attemps to determine filetype by extension,
//RF_NORMAL is returned on failure as a failsafe
  static uint32 detect(char *fn);
  virtual uint32 size() = 0;
//return is 0 on failure, caller must deallocate memory manually
  virtual uint8 *read(uint32 length = 0) = 0;
};

class Writer {
public:
  virtual void write(uint8 *buffer, uint32 length) = 0;
};

#include "filereader.h"
#ifdef GZIP_SUPPORT
  #include "gzreader.h"
  #include "zipreader.h"
#endif
#ifdef JMA_SUPPORT
  #include "jmareader.h"
#endif
