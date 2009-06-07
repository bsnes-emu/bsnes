#include "libreader.hpp"
#define READER_CPP

#include "filereader.cpp"

#if defined(GZIP_SUPPORT)
  #include "gzreader.cpp"
  #include "zipreader.cpp"
#endif

#if defined(JMA_SUPPORT)
  #include "jmareader.cpp"
#endif

Reader::Type Reader::detect(const char *fn, bool inspectheader) {
  file fp;
  if(!fp.open(fn, file::mode_read)) return Unknown;

  uint8_t p[8];
  memset(p, 0, sizeof p);
  fp.read(p, 8);
  fp.close();

  if(inspectheader == true) {
    //inspect file header to determine type
    if(p[0] == 0x1f && p[1] == 0x8b && p[2] == 0x08 && p[3] <= 0x1f) return GZIP;
    if(p[0] == 0x50 && p[1] == 0x4b && p[2] == 0x03 && p[3] == 0x04) return ZIP;
    if(p[0] == 0x4a && p[1] == 0x4d && p[2] == 0x41 && p[3] == 0x00 && p[4] == 0x4e) return JMA;
  } else {
    //check file extension to determine type
    if(striend(fn, ".gz")) return GZIP;
    if(striend(fn, ".zip") || striend(fn, ".z")) return ZIP;
    if(striend(fn, ".jma")) return JMA;
  }
  return Normal;
}
