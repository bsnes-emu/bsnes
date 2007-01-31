#include "filereader.h"

uint32 FileReader::size() {
  return fsize;
}

//This function will allocate memory even if open() fails.
//This is needed so that when SRAM files do not exist, the
//memory for the SRAM data will be allocated still.
//The memory is flushed to 0x00 when no file is opened.
uint8 *FileReader::read(uint32 length) {
uint8 *data;
  if(length == 0) {
  //read the entire file into RAM
    data = (uint8*)malloc(fsize);
    memset(data, 0, fsize);
    if(fp)fread(data, 1, fsize, fp);
  } else if(length > fsize) {
  //read the entire file into RAM, pad the rest with 0x00s
    data = (uint8*)malloc(length);
    memset(data, 0, length);
    if(fp)fread(data, 1, fsize, fp);
  } else { //fsize >= length
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
  if(!fp)return;

  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

//empty file?
  if(fsize == 0) {
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

void FileWriter::write(uint8 *buffer, uint32 length) {
  if(!fp)return;

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
