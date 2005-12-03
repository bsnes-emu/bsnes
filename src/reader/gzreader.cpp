uint32 GZReader::size() {
  return fsize;
}

//This function will allocate memory even if open() fails.
//This is needed so that when SRAM files do not exist, the
//memory for the SRAM data will be allocated still.
//The memory is flushed to 0x00 when no file is opened.
uint8 *GZReader::read(uint32 length) {
uint8 *data;
  if(length == 0) {
  //read the entire file into RAM
    data = (uint8*)memalloc(fsize);
    memset(data, 0, fsize);
    if(gp)gzread(gp, data, fsize);
  } else if(length > fsize) {
  //read the entire file into RAM, pad the rest with 0x00s
    data = (uint8*)memalloc(length);
    memset(data, 0, length);
    if(gp)gzread(gp, data, fsize);
  } else { //fsize >= length
  //read only what was requested
    data = (uint8*)memalloc(length);
    memset(data, 0, length);
    if(gp)gzread(gp, data, length);
  }
  return data;
}

bool GZReader::open(char *fn) {
FILE *fp;
  fp = fopen(fn, "rb");
  if(!fp)return false;

  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);

  if(fsize < 4) {
    fclose(fp);
    fp = 0;
    return false;
  }

uint32 gzsize;
  fseek(fp, -4, SEEK_END); //jump to 4 bytes before end
  gzsize  = fgetc(fp);
  gzsize |= fgetc(fp) <<  8;
  gzsize |= fgetc(fp) << 16;
  gzsize |= fgetc(fp) << 24;

  fclose(fp);
  fp = 0;

  gp = gzopen(fn, "rb");
  if(!gp)return false;

  if(!gzdirect(gp)) {
    fsize = gzsize;
  }

//empty file?
  if(fsize == 0) {
    gzclose(gp);
    gp = 0;
    return false;
  }

  return true;
}

void GZReader::close() {
  if(gp) {
    gzclose(gp);
    gp = 0;
  }
}
