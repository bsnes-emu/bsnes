#include "../reader/filereader.h"

#if defined(GZIP_SUPPORT)
  #include "../reader/gzreader.h"
  #include "../reader/zipreader.h"
#endif

#if defined(JMA_SUPPORT)
  #include "../reader/jmareader.h"
#endif

bool Cartridge::load_file(const char *fn, uint8 *&data, uint &size) {
  dprintf("* Loading \"%s\"...", fn);

  if(fexists(fn) == false) {
    alert("Error: file '%s' not found!", fn);
    return false;
  }

  switch(Reader::detect(fn)) {

  case Reader::RF_NORMAL: {
  FileReader ff(fn);
    if(!ff.ready()) {
      alert("Error loading image file (%s)!", fn);
      return false;
    }
    size = ff.size();
    data = ff.read();
  } break;

#ifdef GZIP_SUPPORT
  case Reader::RF_GZ: {
  GZReader gf(fn);
    if(!gf.ready()) {
      alert("Error loading image file (%s)!", fn);
      return false;
    }
    size = gf.size();
    data = gf.read();
  } break;

  case Reader::RF_ZIP: {
  ZipReader zf(fn);
    size = zf.size();
    data = zf.read();
  } break;
#endif

#ifdef JMA_SUPPORT
  case Reader::RF_JMA: {
    try {
    JMAReader jf(fn);
      size = jf.size();
      data = jf.read();
    } catch(JMA::jma_errors jma_error) {
      alert("Error loading image file (%s)!", fn);
      return false;
    }
  } break;
#endif

  }

  return true;
}

bool Cartridge::save_file(const char *fn, uint8 *data, uint size) {
FileWriter ff(fn);
  if(!ff.ready())return false;
  ff.write(data, size);
  return true;
}
