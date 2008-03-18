#ifdef READER_CPP

#include "jmareader.h"
#include "jma/jma.h"

unsigned JMAReader::size() {
  return filesize;
}

#define MAXROM 0x800000

uint8_t* JMAReader::read(unsigned length) {
  uint8_t *data = 0;
  if(!filesize) return 0;

  if(length <= filesize) {
    //read the entire file into RAM
    data = (uint8_t*)malloc(filesize);
    JMAFile.extract_file(cname, data);
  } else if(length > filesize) {
    //read the entire file into RAM, pad the rest with 0x00s
    data = (uint8_t*)malloc(length);
    memset(data, 0, length);
    JMAFile.extract_file(cname, data);
  }

  return data;
}

JMAReader::JMAReader(const char *fn) : JMAFile(fn), filesize(0) {
  std::vector<JMA::jma_public_file_info> file_info = JMAFile.get_files_info();
  for(std::vector<JMA::jma_public_file_info>::iterator i = file_info.begin(); i != file_info.end(); i++) {
    //Check for valid ROM based on size
    if((i->size <= MAXROM + 512) && (i->size > filesize)) {
      cname = i->name;
      filesize = i->size;
    }
  }
}

#endif //ifdef READER_CPP
