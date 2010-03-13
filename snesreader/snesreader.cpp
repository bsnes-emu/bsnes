#include "snesreader.hpp"

#if defined(_WIN32)
  #define dllexport __declspec(dllexport)
#else
  #define dllexport
#endif

#include "fex/fex.h"
#include "libjma/jma.h"
extern "C" char* uncompressStream(int, int);  //micro-bunzip

#define QT_CORE_LIB
#include <QtGui>

#include <nall/file.hpp>
#include <nall/string.hpp>
using namespace nall;

#include "xml.cpp"

dllexport const char* snesreader_supported() {
  return "*.zip *.z *.7z *.rar *.gz *.bz2 *.jma";
}

void snesreader_apply_ips(const char *filename, uint8_t *&data, unsigned &size) {
  file fp;
  if(fp.open(filename, file::mode_read) == false) return;

  unsigned psize = fp.size();
  uint8_t *pdata = new uint8_t[psize];
  fp.read(pdata, psize);
  fp.close();

  if(psize < 8 || pdata[0] != 'P' || pdata[1] != 'A' || pdata[2] != 'T' || pdata[3] != 'C' || pdata[4] != 'H') { delete[] pdata; return; }

  unsigned outsize = 0;
  uint8_t *outdata = new uint8_t[16 * 1024 * 1024];
  memset(outdata, 0, 16 * 1024 * 1024);
  memcpy(outdata, data, size);

  unsigned offset = 5;
  while(offset < psize - 3) {
    unsigned addr;
    addr  = pdata[offset++] << 16;
    addr |= pdata[offset++] <<  8;
    addr |= pdata[offset++] <<  0;

    unsigned size;
    size  = pdata[offset++] << 8;
    size |= pdata[offset++] << 0;

    if(size == 0) {
      //RLE
      size  = pdata[offset++] << 8;
      size |= pdata[offset++] << 0;

      for(unsigned n = addr; n < addr + size;) {
        outdata[n++] = pdata[offset];
        if(n > outsize) outsize = n;
      }
      offset++;
    } else {
      //uncompressed
      for(unsigned n = addr; n < addr + size;) {
        outdata[n++] = pdata[offset++];
        if(n > outsize) outsize = n;
      }
    }
  }

  delete[] pdata;
  delete[] data;
  data = outdata;
  size = max(size, outsize);
}

bool snesreader_load_normal(const char *filename, uint8_t *&data, unsigned &size) {
  file fp;
  if(fp.open(filename, file::mode_read) == false) return false;
  size = fp.size();
  data = new uint8_t[size];
  fp.read(data, size);
  fp.close();
  return true;
}

#include "filechooser.cpp"

bool snesreader_load_fex(string &filename, uint8_t *&data, unsigned &size) {
  fex_t *fex;
  fex_open(&fex, filename);
  if(fex_done(fex)) { fex_close(fex); return false; }

  if(!fileChooser) fileChooser = new FileChooser;
  fileChooser->list.reset();

  while(fex_done(fex) == false) {
    fex_stat(fex);
    const char *name = fex_name(fex);
    //only add valid ROM extensions to list (ignore text files, save RAM files, etc)
    if(striend(name, ".sfc") || striend(name, ".smc")
    || striend(name, ".swc") || striend(name, ".fig")
    || striend(name, ".bs")  || striend(name, ".st")
    || striend(name, ".gb")  || striend(name, ".sgb") || striend(name, ".gbc")
    || striend(filename, ".gz")  //GZip files only contain a single file
    ) {
      fileChooser->list[fileChooser->list.size()] = name;
    }
    fex_next(fex);
  }

  string name = fileChooser->exec();
  if(name == "") { fex_close(fex); return false; }

  fex_rewind(fex);
  while(fex_done(fex) == false) {
    fex_stat(fex);
    if(name == fex_name(fex)) {
      size = fex_size(fex);
      data = new uint8_t[size];
      fex_read(fex, data, size);
      fex_close(fex);

      if(fileChooser->list.size() > 1) {
        strtr(name, "\\", "/");
        strtr(filename, "\\", "/");

        //retain only path from filename, "/foo/bar.7z" -> "/foo/"
        for(signed i = filename.length() - 1; i >= 0; i--) {
          if(filename[i] == '/') {
            filename[i + 1] = 0;
            break;
          }
        }

        //append only filename from archive, "foo/bar.sfc" -> "bar.sfc"
        lstring part;
        part.split("/", name);
        filename = string() << filename << part[part.size() - 1];
      }

      return true;
    }
    fex_next(fex);
  }

  fex_close(fex);
  return false;
}

bool snesreader_load_bz2(const char *filename, uint8_t *&data, unsigned &size) {
  //TODO: need a way to get the size of a bzip2 file, so we can pre-allocate
  //a buffer to decompress into memory. for now, use a temporary file.

  string name = "/tmp/.bz2_temporary_decompression_object";
  FILE *wr;
  wr = fopen_utf8(name, "wb");
  if(!wr) {
    //try the local directory
    name = ".bz2_temporary_decompression_object";
    wr = fopen_utf8(name, "wb");
    //can't get write access, so give up
    if(!wr) return false;
  }

  FILE *fp = fopen_utf8(filename, "rb");
  uncompressStream(fileno(fp), fileno(wr));
  fclose(fp);
  fclose(wr);

  bool success = snesreader_load_normal(name, data, size);
  unlink(name);
  return success;
}

bool snesreader_load_jma(const char *filename, uint8_t *&data, unsigned &size) {
  try {
    JMA::jma_open JMAFile(filename);
    std::string name;

    std::vector<JMA::jma_public_file_info> file_info = JMAFile.get_files_info();
    for(std::vector<JMA::jma_public_file_info>::iterator i = file_info.begin(); i != file_info.end(); i++) {
      name = i->name;
      size = i->size;
      break;
    }

    data = new uint8_t[size];
    JMAFile.extract_file(name, data);
    return true;
  } catch(JMA::jma_errors) {
    return false;
  }
}

dllexport bool snesreader_load(string &filename, string &xmldata, uint8_t *&data, unsigned &size) {
  if(file::exists(filename) == false) return false;
  xmldata = "";

  bool success = false;
  if(striend(filename, ".zip")
  || striend(filename, ".z")
  || striend(filename, ".7z")
  || striend(filename, ".rar")
  || striend(filename, ".gz")) {
    success = snesreader_load_fex(filename, data, size);
  } else if(striend(filename, ".bz2")) {
    success = snesreader_load_bz2(filename, data, size);
  } else if(striend(filename, ".jma")) {
    success = snesreader_load_jma(filename, data, size);
  } else {
    success = snesreader_load_normal(filename, data, size);
  }

  if(success == false) return false;

  //apply IPS patch, if it exists
  string patchname = filename;
  for(int i = patchname.length() - 1; i >= 0; i--) {
    if(patchname[i] == '.') { patchname[i] = 0; break; }
  }
  patchname << ".ips";
  if(file::exists(patchname)) snesreader_apply_ips(patchname, data, size);

  //remove copier header, if it exists
  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);

  xml.generate(xmldata, data, size);
  return true;
}
