#ifdef CART_CPP

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

//remove directory information and file extension ("/foo/bar.ext" -> "bar")
char* Cartridge::get_base_filename(char *filename) {
  //remove extension
  for(int i = strlen(filename) - 1; i >= 0; i--) {
    if(filename[i] == '.') {
      filename[i] = 0;
      break;
    }
  }

  //remove directory information
  for(int i = strlen(filename) - 1; i >= 0; i--) {
    if(filename[i] == '/' || filename[i] == '\\') {
      i++;
      char *output = filename;
      while(true) {
        *output++ = filename[i];
        if(!filename[i]) break;
        i++;
      }
      break;
    }
  }
}

char* Cartridge::get_path_filename(char *filename, const char *path, const char *source, const char *extension) {
  strcpy(filename, source);
  for(char *p = filename; *p; p++) { if(*p == '\\') *p = '/'; }
  modify_extension(filename, extension);

  //override path with user-specified folder, if one was defined
  if(*path) {
    lstring part;
    split(part, "/", filename);
    string fn = path;
    if(strend(fn, "/") == false) strcat(fn, "/");
    strcat(fn, part[count(part) - 1]);
    strcpy(filename, fn);

    //resolve relative path, if found
    if(strbegin(fn, "./") == true) {
      ltrim(fn, "./");
      strcpy(filename, config::path.base);
      strcat(filename, fn);
    }
  }

  return filename;
}

char* Cartridge::get_patch_filename(const char *source, const char *extension) {
  return get_path_filename(patchfn, config::path.patch, source, extension);
}

char* Cartridge::get_save_filename(const char *source, const char *extension) {
  return get_path_filename(savefn, config::path.save, source, extension);
}

char* Cartridge::get_cheat_filename(const char *source, const char *extension) {
  return get_path_filename(cheatfn, config::path.cheat, source, extension);
}

bool Cartridge::load_file(const char *fn, uint8 *&data, uint &size, CompressionMode compression) {
  dprintf("* Loading \"%s\" ...", fn);

  if(fexists(fn) == false) return false;

  Reader::Type filetype = Reader::Normal;
  if(compression == CompressionInspect) filetype = Reader::detect(fn, true);
  if(compression == CompressionAuto) filetype = Reader::detect(fn, config::file.autodetect_type);

  switch(filetype) {
    default:
      dprintf("* Warning: filetype detected as unsupported compression type.");
      dprintf("* Will attempt to load as uncompressed file -- may fail.");
    case Reader::Normal: {
      FileReader ff(fn);
      if(!ff.ready()) {
        alert("Error loading image file (%s)!", fn);
        return false;
      }
      size = ff.size();
      data = ff.read();
    } break;

    #ifdef GZIP_SUPPORT
    case Reader::GZIP: {
      GZReader gf(fn);
      if(!gf.ready()) {
        alert("Error loading image file (%s)!", fn);
        return false;
      }
      size = gf.size();
      data = gf.read();
    } break;

    case Reader::ZIP: {
      ZipReader zf(fn);
      size = zf.size();
      data = zf.read();
    } break;
    #endif

    #ifdef JMA_SUPPORT
    case Reader::JMA: {
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

bool Cartridge::apply_patch(const uint8_t *pdata, const unsigned psize, uint8_t *&data, unsigned &size) {
  uint8_t *outdata = 0;
  unsigned outsize;
  ups patcher;
  ups::result result = patcher.apply(pdata, psize, data, size, outdata, outsize);

  bool apply = false;
  if(result == ups::ok) apply = true;
  if(config::file.bypass_patch_crc32 == true) {
    if(result == ups::input_crc32_invalid) apply = true;
    if(result == ups::output_crc32_invalid) apply = true;
  }

  if(apply == true) {
    free(data);
    data = (uint8_t*)malloc(size = outsize);
    memcpy(data, outdata, outsize);
  } else {
    dprintf("* Warning: patch application failed!");
  }

  if(outdata) delete[] outdata;
}

bool Cartridge::save_file(const char *fn, uint8 *data, uint size) {
  FileWriter ff(fn);
  if(!ff.ready())return false;
  ff.write(data, size);
  return true;
}

#endif //ifdef CART_CPP
