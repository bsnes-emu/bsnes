#ifdef READER_CPP

#include "filereader.h"

unsigned FileReader::size() {
  return filesize;
}

//This function will allocate memory even if open() fails.
//This is needed so that when SRAM files do not exist, the
//memory for the SRAM data will be allocated still.
//The memory is flushed to 0x00 when no file is opened.
uint8_t* FileReader::read(unsigned length) {
  uint8_t *data = 0;

  if(length == 0) {
    //read the entire file into RAM
    data = (uint8*)malloc(filesize);
    memset(data, 0, filesize);
    if(fp) fread(data, 1, filesize, fp);
  } else if(length > filesize) {
    //read the entire file into RAM, pad the rest with 0x00s
    data = (uint8*)malloc(length);
    memset(data, 0, length);
    if(fp)fread(data, 1, filesize, fp);
  } else { //filesize >= length
    //read only what was requested
    data = (uint8*)malloc(length);
    memset(data, 0, length);
    if(fp)fread(data, 1, length, fp);
  }
  return data;
}

bool FileReader::ready() {
  return (fp != 0);
}

FileReader::FileReader(const char *fn) {
  fp = fopen(fn, "rb");
  if(!fp) return;

  fseek(fp, 0, SEEK_END);
  filesize = ftell(fp);
  rewind(fp);

  //empty file?
  if(filesize == 0) {
    fclose(fp);
    fp = 0;
  }
}

FileReader::~FileReader() {
  if(fp) {
    fclose(fp);
    fp = 0;
  }
}

void FileWriter::write(uint8_t *buffer, unsigned length) {
  if(!fp) return;
  fwrite(buffer, 1, length, fp);
}

bool FileWriter::ready() {
  return (fp != 0);
}

FileWriter::FileWriter(const char *fn) {
  fp = fopen(fn, "wb");
}

FileWriter::~FileWriter() {
  if(fp) {
    fclose(fp);
    fp = 0;
  }
}

#endif //ifdef READER_CPP
