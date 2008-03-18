#ifdef READER_CPP

#include "zipreader.h"

unsigned ZipReader::size() {
  return filesize;
}

#define MAXROM 0x800000

uint8_t* ZipReader::read(unsigned length) {
  uint8_t *data = 0;

  if(!filesize) return 0;

  if(length <= filesize) {
    //read the entire file into RAM
    data = (uint8_t*)malloc(filesize);
    unzReadCurrentFile(zipfile, data, filesize);
  } else if(length > filesize) {
    //read the entire file into RAM, pad the rest with 0x00s
    data = (uint8_t*)malloc(length);
    memset(data, 0, length);
    unzReadCurrentFile(zipfile, data, filesize);
  }

  return data;
}

ZipReader::ZipReader(const char *fn) : filesize(0) {
  unz_file_info cFileInfo; //Create variable to hold info for a compressed file
  char cFileName[sizeof(cname)];

  if(zipfile = unzOpen(fn)) { //Open zip file
    for(int cFile = unzGoToFirstFile(zipfile); cFile == UNZ_OK; cFile = unzGoToNextFile(zipfile)) {
      //Gets info on current file, and places it in cFileInfo
      unzGetCurrentFileInfo(zipfile, &cFileInfo, cFileName, sizeof(cname), 0, 0, 0, 0);

      if((cFileInfo.uncompressed_size <= MAXROM+512) && (cFileInfo.uncompressed_size > filesize)) {
        strcpy(cname, cFileName);
        filesize = cFileInfo.uncompressed_size;
      }
    }

    if(filesize) {
      unzLocateFile(zipfile, cname, 1);
      unzOpenCurrentFile(zipfile);
    }
  }
}

ZipReader::~ZipReader() {
  if(zipfile) {
    unzCloseCurrentFile(zipfile);
    unzClose(zipfile);
  }
}

#endif //ifdef READER_CPP
