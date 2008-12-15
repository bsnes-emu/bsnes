#ifdef READER_CPP

#include "filereader.hpp"

unsigned FileReader::size() {
  return fp.size();
}

//This function will allocate memory even if open() fails.
//This is needed so that when SRAM files do not exist, the
//memory for the SRAM data will be allocated still.
//The memory is flushed to 0x00 when no file is opened.
uint8_t* FileReader::read(unsigned length) {
  uint8_t *data = 0;

  if(length == 0) {
    //read the entire file into RAM
    data = new(zeromemory) uint8_t[fp.size()];
    if(fp.open()) fp.read(data, fp.size());
  } else if(length > fp.size()) {
    //read the entire file into RAM, pad the rest with 0x00s
    data = new(zeromemory) uint8_t[length];
    if(fp.open()) fp.read(data, fp.size());
  } else { //filesize >= length
    //read only what was requested
    data = new(zeromemory) uint8_t[length];
    if(fp.open()) fp.read(data, length);
  }

  return data;
}

bool FileReader::ready() {
  return fp.open();
}

FileReader::FileReader(const char *fn) {
  if(!fp.open(fn, file::mode_read)) return;

  if(fp.size() == 0) {
    //empty file
    fp.close();
  }
}

FileReader::~FileReader() {
  if(fp.open()) fp.close();
}

#endif //ifdef READER_CPP
