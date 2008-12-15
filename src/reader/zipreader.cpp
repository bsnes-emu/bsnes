#ifdef READER_CPP

#include "zipreader.hpp"

unsigned ZipReader::size() {
  return filesize;
}

uint8_t* ZipReader::read(unsigned length) {
  if(!filesize) return 0;

  uint8_t *data = 0;
  if(length <= filesize) {
    //read the entire file into RAM
    data = new(zeromemory) uint8_t[filesize];
    unzReadCurrentFile(zipfile, data, filesize);
  } else { /* length > filesize */
    //read the entire file into RAM, pad the rest with 0x00s
    data = new(zeromemory) uint8_t[length];
    unzReadCurrentFile(zipfile, data, filesize);
  }

  return data;
}

bool ZipReader::ready() {
  return zipready;
}

ZipReader::ZipReader(const char *fn) : filesize(0), zipready(false) {
  unz_file_info cFileInfo;  //Create variable to hold info for a compressed file
  char cFileName[sizeof(cname)];

  if(zipfile = unzOpen(fn)) {  //Open zip file
    for(int cFile = unzGoToFirstFile(zipfile); cFile == UNZ_OK; cFile = unzGoToNextFile(zipfile)) {
      //Gets info on current file, and places it in cFileInfo
      unzGetCurrentFileInfo(zipfile, &cFileInfo, cFileName, sizeof(cname), 0, 0, 0, 0);

      //verify uncompressed file is not bigger than max ROM size
      if((cFileInfo.uncompressed_size <= 0x1000000 + 512) && (cFileInfo.uncompressed_size > filesize)) {
        strcpy(cname, cFileName);
        filesize = cFileInfo.uncompressed_size;
      }
    }

    if(filesize) {
      unzLocateFile(zipfile, cname, 1);
      unzOpenCurrentFile(zipfile);
      zipready = true;
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
