#include "../base.h"
#include "filereader.cpp"
#ifdef GZIP_SUPPORT
  #include "gzreader.cpp"
  #include "zipreader.cpp"
#endif
#ifdef JMA_SUPPORT
  #include "jmareader.cpp"
#endif

uint Reader::detect(const char *fn) {
int len = strlen(fn);
  if(len >= 4 && !stricmp(fn + len - 3, ".gz")) {
    return RF_GZ;
  } else if(len >= 5 && !stricmp(fn + len - 4, ".zip")) {
    return RF_ZIP;
  } else if(len >= 5 && !stricmp(fn + len - 4, ".jma")) {
    return RF_JMA;
  } else {
    return RF_NORMAL;
  }
}
