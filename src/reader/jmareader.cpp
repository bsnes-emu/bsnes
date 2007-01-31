//created by Nach

#include "jmareader.h"
#include "jma/jma.h"

uint32 JMAReader::size() {
  return fsize;
}

#define MAXROM 0x800000

uint8 *JMAReader::read(uint32 length)
{
  uint8 *data;
  if (!fsize) { return 0; }
  if (length <= fsize)
  {
    //read the entire file into RAM
    data = (uint8*)malloc(fsize);
    JMAFile.extract_file(cname, data);
  }
  else if (length > fsize)
  {
    //read the entire file into RAM, pad the rest with 0x00s
    data = (uint8*)malloc(length);
    memset(data, 0, length);
    JMAFile.extract_file(cname, data);
  }
  return data;
}

JMAReader::JMAReader(const char *fn) : JMAFile(fn), fsize(0)
{
  std::vector<JMA::jma_public_file_info> file_info = JMAFile.get_files_info();
  for (std::vector<JMA::jma_public_file_info>::iterator i = file_info.begin(); i != file_info.end(); i++)
  {
    //Check for valid ROM based on size
    if ((i->size <= MAXROM+512) && (i->size > fsize))
    {
      cname = i->name;
      fsize = i->size;
    }
  }
}
