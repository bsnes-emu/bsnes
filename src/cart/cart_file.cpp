#include "../reader/filereader.h"

#if defined(GZIP_SUPPORT)
  #include "../reader/gzreader.h"
  #include "../reader/zipreader.h"
#endif

#if defined(JMA_SUPPORT)
  #include "../reader/jmareader.h"
#endif

char* Cartridge::modify_extension(char *filename, const char *extension) {
int i;
  for(i = strlen(filename); i >= 0; i--) {
    if(filename[i] == '.') break;
    if(filename[i] == '/') break;
    if(filename[i] == '\\') break;
  }
  if(i > 0 && filename[i] == '.') filename[i] = 0;
  strcat(filename, ".");
  strcat(filename, extension);
  return filename;
}

char* Cartridge::get_save_filename(const char *source, const char *extension) {
  strcpy(savefn, source);
  for(char *p = savefn; *p; p++) { if(*p == '\\') *p = '/'; }
  modify_extension(savefn, extension);

//override path with user-specified folder, if one was defined
  if(config::path.save != "") {
  lstring part;
    split(part, "/", savefn);
  string fn = (const char*)config::path.save;
    if(strend(fn, "/") == false) strcat(fn, "/");
    strcat(fn, part[count(part) - 1]);
    strcpy(savefn, fn);

  //resolve relative path, if found
    if(strbegin(fn, "./") == true) {
      ltrim(fn, "./");
      strcpy(savefn, config::path.base);
      strcat(savefn, fn);
    }
  }

  return savefn;
}

bool Cartridge::load_file(const char *fn, uint8 *&data, uint &size) {
  dprintf("* Loading \"%s\"...", fn);

  if(fexists(fn) == false) return false;

  switch(Reader::detect(fn)) {
    default:
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
