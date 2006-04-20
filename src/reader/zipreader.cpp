//created by Nach

uint32 ZipReader::size() {
  return fsize;
}

#define MAXROM 0x800000

uint8 *ZipReader::read(uint32 length)
{
  uint8 *data;
  if (!fsize) { return 0; }
  if (length <= fsize)
  {
    //read the entire file into RAM
    data = (uint8*)malloc(fsize);
    unzReadCurrentFile(zipfile, data, fsize);
  }
  else if (length > fsize)
  {
    //read the entire file into RAM, pad the rest with 0x00s
    data = (uint8*)malloc(length);
    memset(data, 0, length);
    unzReadCurrentFile(zipfile, data, fsize);
  }
  return data;
}

ZipReader::ZipReader(char *fn) : fsize(0)
{
  unz_file_info cFileInfo; //Create variable to hold info for a compressed file
  char cFileName[sizeof(cname)];

  if (zipfile = unzOpen(fn)) //Open zip file
  {
    for (int cFile = unzGoToFirstFile(zipfile); cFile == UNZ_OK; cFile = unzGoToNextFile(zipfile))
    {
      //Gets info on current file, and places it in cFileInfo
      unzGetCurrentFileInfo(zipfile, &cFileInfo, cFileName, sizeof(cname), 0, 0, 0, 0);

      if ((cFileInfo.uncompressed_size <= MAXROM+512) && (cFileInfo.uncompressed_size > fsize))
      {
        strcpy(cname, cFileName);
        fsize = cFileInfo.uncompressed_size;
      }
    }

    if (fsize)
    {
      unzLocateFile(zipfile, cname, 1);
      unzOpenCurrentFile(zipfile);
    }
  }
}
