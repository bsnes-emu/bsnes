#include "../base.h"

bool FileReader::open(char *fn) {
/* is the filename too short to be a file? */
  if(strlen(fn) < 4)return false;

int i;
  for(i=strlen(fn) - 1;i>=0;i--) {
    if(fn[i] == '.')break;
  }
  if(fn[i] != '.')return false;

char *filetype = fn + i;
/* make sure we support this file format before loading it */
  if(strcmp(filetype, ".smc"))return false;

  fp = fopen(fn, "rb");
  if(!fp)return false;

  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  if(!strcmp(filetype, ".smc")) {
  /* remove header if it exists */
    if((fsize & 0xfff) == 0x200) {
      fsize -= 0x200;
      fseek(fp, 0x200, SEEK_SET);
    }
  }

/* empty file? */
  if(fsize == 0) {
    fclose(fp);
    return false;
  }

  return true;
}

void FileReader::read(uint8 **buffer, uint32 length) {
uint8 *data;
  if(length == 0) {
  /* read the entire file into RAM */
    data = (uint8*)memalloc(fsize);
    fread(data, 1, fsize, fp);
  } else if(length > fsize) {
  /* read the entire file into RAM, pad the rest with 0x00s */
    data = (uint8*)memalloc(length);
    memset(data, 0, length);
    fread(data, 1, fsize, fp);
  } else { //fsize >= length
  /* read as much of the file as possible, truncate the rest */
    data = (uint8*)memalloc(length);
    fread(data, 1, length, fp);
  }
  *buffer = data;
}

uint32 FileReader::size() {
  return fsize;
}

void FileReader::close() {
  if(fp)fclose(fp);
}
