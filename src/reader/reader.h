class Reader {
public:
  virtual uint32 size() = 0;
  virtual void   read(uint8 **buffer, uint32 length = 0) = 0;
};

class Writer {
public:
  virtual void write(uint8 *buffer, uint32 length) = 0;
};

#include "filereader.h"
