/*
  libfile : version 0.02 ~byuu (06/17/06)
*/

#ifndef __LIBFILE
#define __LIBFILE

/*****
 * file base class
 *****/

class file {
public:
enum { mode_read, mode_write, mode_readwrite, mode_writeread };
enum { seek_start, seek_end, seek_back, seek_forward };
  virtual void   read(uint8 *data, uint length) = 0;
  virtual uint8  read() = 0;

  virtual void   write(uint8 *data, uint length) = 0;
  virtual void   write(uint8 data) = 0;

  virtual void   seek(uint offset, uint mode = seek_start) = 0;
  virtual uint   offset() = 0;
  virtual uint   size() = 0;
  virtual bool   eof() = 0;

  virtual bool   open(const char *filename, uint mode) = 0;
  virtual bool   open() = 0;
  virtual bool   flush() = 0;
  virtual bool   close() = 0;
};

/*****
 * c wrappers
 *****/

void   fread(file *s, uint8 *data, uint length);
uint8  fread(file *s);
uint8  fgetc(file *s);

void   fwrite(file *s, uint8 *data, uint length);
void   fwrite(file *s, uint8 data);
void   fputc(file *s, uint8 data);

void   fseek(file *s, uint offset, uint mode = file::seek_start);
uint   foffset(file *s);
uint   ftell(file *s);
uint   fsize(file *s);
bool   feof(file *s);

bool   fopen(file *s, const char *filename, uint mode);
bool   fopen(file *s);
bool   fflush(file *s);
bool   fclose(file *s);

/*****
 * c++ wrappers
 *****/

void   fread(file &s, uint8 *data, uint length);
uint8  fread(file &s);
uint8  fgetc(file &s);

void   fwrite(file &s, uint8 *data, uint length);
void   fwrite(file &s, uint8 data);
void   fputc(file &s, uint8 data);

void   fseek(file &s, uint offset, uint mode = file::seek_start);
uint   foffset(file &s);
uint   ftell(file &s);
uint   fsize(file &s);
bool   feof(file &s);

bool   fopen(file &s, const char *filename, uint mode);
bool   fopen(file &s);
bool   fflush(file &s);
bool   fclose(file &s);

/*****
 * ramfile
 *****/

class ramfile : public file {
private:
FILE *fp;
vector<uint8> filedata;
char filename[1024];
uint filepos;
uint filesize;
uint filemode;
bool fileopen;

public:
  void read(uint8 *data, uint length) {
    if(!fileopen || filemode == mode_write) { return; }

    filedata.read(filepos, data, length);
    filepos += length;
    if(filepos > filesize)filepos = filesize;
  }

  uint8 read() {
    if(!fileopen || filemode == mode_write) { return 0; }

  uint8 r = filedata[filepos++];
    if(filepos > filesize)filepos = filesize;
    return r;
  }

  void write(uint8 *data, uint length) {
    if(!fileopen || filemode == mode_read) { return; }

    filedata.write(filepos, data, length);
    filepos += length;
    if(filepos > filesize)filesize = filepos;
  }

  void write(uint8 data) {
    if(!fileopen || filemode == mode_read) { return; }

    filedata[filepos++] = data;
    if(filepos > filesize)filesize = filepos;
  }

  void seek(uint offset, uint mode = seek_start) {
    if(!fileopen) { return; }

    switch(mode) {
    case seek_start:   filepos  = offset;            break;
    case seek_end:     filepos  = filesize - offset; break;
    case seek_back:    filepos -= offset;            break;
    case seek_forward: filepos += offset;            break;
    }

    if(filemode == mode_read) {
      if(filepos > filesize)filepos = filesize;
    } else {
      if(filepos > filesize)filesize = filepos;
    }
  }

  uint offset() {
    if(!fileopen) { return 0; }

    return filepos;
  }

  uint size() {
    if(!fileopen) { return 0; }

    return filesize;
  }

  bool eof() {
    if(!fileopen) { return true; }

    return (filepos >= filesize);
  }

  bool open(const char *fn, uint mode) {
    if(fileopen) { return false; }

    strcpy(filename, fn);
    filemode = mode;
    switch(filemode) {
    case mode_read:
    case mode_readwrite:
      fp = fopen(filename, "rb");
      if(!fp) { return false; }
      filesize = fsize(fp);
      fread(filedata.handle(filesize), 1, filesize, fp);
      fclose(fp);
      break;
    default:
      filesize = 0;
      break;
    }
    filepos  = 0;
    fileopen = true;
    return true;
  }

  bool open() {
    return fileopen;
  }

  bool flush() {
    if(!fileopen) { return false; }

    switch(filemode) {
    case mode_readwrite:
    case mode_write:
    case mode_writeread:
      fp = fopen(filename, "wb");
      if(!fp) { return false; }
      fwrite(filedata.handle(filesize), 1, filesize, fp);
      fclose(fp);
      break;
    }
    return true;
  }

  bool close() {
    if(!fileopen) { return false; }

  bool result = flush();
    fileopen = false;
    filedata.release();
    return result;
  }

  ramfile() {
    fileopen = false;
  }

  ~ramfile() {
    if(fileopen) { close(); }
  }
};

/*****
 * diskfile
 *****/

class diskfile : public file {
private:
FILE *fp;
uint filemode;

public:
  void read(uint8 *data, uint length) {
    if(!fp || filemode == mode_write) { return; }

    fread(data, 1, length, fp);
  }

  uint8 read() {
    if(!fp || filemode == mode_write) { return 0; }

    return fgetc(fp);
  }

  void write(uint8 *data, uint length) {
    if(!fp || filemode == mode_read) { return; }

    fwrite(data, 1, length, fp);
  }

  void write(uint8 data) {
    if(!fp || filemode == mode_read) { return; }

    fputc(data, fp);
  }

  void seek(uint offset, uint mode = seek_start) {
    if(!fp) { return; }

    switch(mode) {
    default:
    case seek_start:   fseek(fp,  offset, SEEK_SET); break;
    case seek_end:     fseek(fp,  offset, SEEK_END); break;
    case seek_back:    fseek(fp, -offset, SEEK_CUR); break;
    case seek_forward: fseek(fp,  offset, SEEK_CUR); break;
    }
  }

  uint offset() {
    if(!fp) { return 0; }

    return ftell(fp);
  }

  uint size() {
    if(!fp) { return 0; }

  uint pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
  uint filesize = ftell(fp);
    fseek(fp, pos, SEEK_SET);
    return filesize;
  }

  bool eof() {
    if(!fp) { return true; }

    return feof(fp);
  }

  bool open(const char *filename, uint mode) {
    if(fp) { return false; }

    filemode = mode;
  char m[8];
    switch(filemode) {
    default:
    case mode_read:      strcpy(m, "rb");  break;
    case mode_write:     strcpy(m, "wb");  break;
    case mode_readwrite: strcpy(m, "rb+"); break;
    case mode_writeread: strcpy(m, "wb+"); break;
    }
    fp = fopen(filename, m);
    if(!fp) { return false; }
    return true;
  }

  bool open() {
    return (fp != 0);
  }

  bool flush() {
    if(!fp) { return false; }

    fflush(fp);
    return true;
  }

  bool close() {
    if(!fp) { return false; }

    fclose(fp);
    fp = 0;
    return true;
  }

  diskfile() {
    fp = 0;
  }

  ~diskfile() {
    if(fp) { fclose(fp); }
  }
};

#endif
