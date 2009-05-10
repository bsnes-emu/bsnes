#ifdef READER_CPP
#include "jmareader.hpp"

unsigned JMAReader::size() {
  return filesize;
}

uint8_t* JMAReader::read(unsigned length) {
  uint8_t *data = 0;
  if(!filesize) return 0;

  if(length <= filesize) {
    //read the entire file into RAM
    data = new(zeromemory) uint8_t[filesize];
    JMAFile.extract_file(cname, data);
  } else if(length > filesize) {
    //read the entire file into RAM, pad the rest with 0x00s
    data = new(zeromemory) uint8_t[length];
    JMAFile.extract_file(cname, data);
  }

  return data;
}

JMAReader::JMAReader(const char *fn) : JMAFile(fn), filesize(0) {
  std::vector<JMA::jma_public_file_info> file_info = JMAFile.get_files_info();
  for(std::vector<JMA::jma_public_file_info>::iterator i = file_info.begin(); i != file_info.end(); i++) {
    //Check for valid ROM based on size
    if((i->size <= 0x1000000 + 512) && (i->size > filesize)) {
      cname = i->name;
      filesize = i->size;
    }
  }
}

#endif //ifdef READER_CPP
