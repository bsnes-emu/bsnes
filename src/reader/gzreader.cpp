#ifdef READER_CPP

#include "gzreader.hpp"

unsigned GZReader::size() {
  return filesize;
}

//This function will allocate memory even if open() fails.
//This is needed so that when SRAM files do not exist, the
//memory for the SRAM data will be allocated still.
//The memory is flushed to 0x00 when no file is opened.
uint8_t* GZReader::read(unsigned length) {
  uint8_t *data = 0;

  if(length == 0) {
    //read the entire file into RAM
    data = new(zeromemory) uint8_t[filesize];
    if(gp) gzread(gp, data, filesize);
  } else if(length > filesize) {
    //read the entire file into RAM, pad the rest with 0x00s
    data = new(zeromemory) uint8_t[length];
    if(gp) gzread(gp, data, filesize);
  } else { //filesize >= length
    //read only what was requested
    data = new(zeromemory) uint8_t[length];
    if(gp) gzread(gp, data, length);
  }

  return data;
}

bool GZReader::ready() {
  return (gp != 0);
}

GZReader::GZReader(const char *fn) : gp(0) {
  #if !defined(_WIN32)
  fp = fopen(fn, "rb");
  #else
  fp = _wfopen(utf16_t(fn), L"rb");
  #endif
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);

  if(filesize < 4) {
    //too small to be a valid GZ archive
    fclose(fp);
    fp = 0;
    return;
  }

  fseek(fp, -4, SEEK_END);
  unsigned gzsize;
  gzsize  = fgetc(fp);
  gzsize |= fgetc(fp) << 8;
  gzsize |= fgetc(fp) << 16;
  gzsize |= fgetc(fp) << 24;
  fseek(fp, 0, SEEK_SET);

  //zlib does not support UTF-8 filenames on Windows,
  //thus _wfopen() wrapper above + fileno() wrapper here.
  gp = gzdopen(fileno(fp), "rb");
  if(!gp) return;

  if(gzdirect(gp) == false) filesize = gzsize;

  if(filesize == 0) {
    //archive is empty
    gzclose(gp);
    gp = 0;
    return;
  }
}

GZReader::~GZReader() {
  if(gp) {
    gzclose(gp);
    gp = 0;
  }
}

#endif //ifdef READER_CPP
