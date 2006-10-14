/*
  libfile : version 0.05 ~byuu (10/12/06)
*/

#ifndef __LIBFILE
#define __LIBFILE

/*****
 * file object
 *****/

class file {
public:
enum { mode_read, mode_write, mode_readwrite, mode_writeread };
enum { seek_start, seek_end, seek_back, seek_forward };
  virtual void   read(uint8 *data, uint length) = 0;
  virtual uint8  read() = 0;

  virtual void   write(uint8 *data, uint length) = 0;
  virtual void   write(uint8 data) = 0;

  virtual uint32 crc32();

  virtual void   seek(uint offset, uint mode = seek_start) = 0;
  virtual void   truncate(uint size) = 0;
  virtual uint   offset() = 0;
  virtual uint   size() = 0;
  virtual bool   eof() = 0;

  virtual bool   open(const char *filename, uint mode) = 0;
  virtual bool   open() = 0;
  virtual bool   flush() = 0;
  virtual bool   close() = 0;
};

inline uint32 file::crc32() {
uint pos = offset(), i = size();
  seek(0);
uint32 crc32 = 0xffffffff;
  while(i--) {
    crc32 = crc32_adjust(crc32, read());
  }
  seek(pos);
  return ~crc32;
}

/*****
 * c++ wrappers
 *****/

inline void   fread(file &s, uint8 *data, uint length) { s.read(data, length); }
inline uint8  fread(file &s) { return s.read(); }
inline uint8  fgetc(file &s) { return s.read(); }

inline void   fwrite(file &s, uint8 *data, uint length) { s.write(data, length); }
inline void   fwrite(file &s, uint8 data) { s.write(data); }
inline void   fputc(file &s, uint8 data) { s.write(data); }

inline uint32 fcrc32(file &s) { return s.crc32(); }

inline void   fseek(file &s, uint offset, uint mode = file::seek_start) { s.seek(offset, mode); }
inline void   ftruncate(file &s, uint size) { s.truncate(size); }
inline uint   ftell(file &s) { return s.offset(); }
inline uint   fsize(file &s) { return s.size(); }
inline bool   feof(file &s) { return s.eof(); }

inline bool   fopen(file &s, const char *filename, uint mode) { return s.open(filename, mode); }
inline bool   fopen(file &s) { return s.open(); }
inline bool   fflush(file &s) { return s.flush(); }
inline bool   fclose(file &s) { return s.close(); }


/*****
 * endian wrappers
 *****/

inline uint8 fgetlb(file &s) { return fgetc(s); }
inline uint8 fgetmb(file &s) { return fgetc(s); }

inline uint16 fgetlw(file &s) {
  return (fgetc(s)) | (fgetc(s) << 8);
}

inline uint16 fgetmw(file &s) {
  return (fgetc(s) << 8) | (fgetc(s) << 8);
}

inline uint32 fgetld(file &s) {
  return (fgetc(s)) | (fgetc(s) << 8) | (fgetc(s) << 16) | (fgetc(s) << 24);
}

inline uint32 fgetmd(file &s) {
  return (fgetc(s) << 24) | (fgetc(s) << 16) | (fgetc(s) << 8) | (fgetc(s));
}

inline uint64 fgetlq(file &s) {
  return ((uint64)fgetc(s) <<  0) | ((uint64)fgetc(s) <<  8) |
         ((uint64)fgetc(s) << 16) | ((uint64)fgetc(s) << 24) |
         ((uint64)fgetc(s) << 32) | ((uint64)fgetc(s) << 40) |
         ((uint64)fgetc(s) << 48) | ((uint64)fgetc(s) << 56);
}

inline uint64 fgetmq(file &s) {
  return ((uint64)fgetc(s) << 56) | ((uint64)fgetc(s) << 48) |
         ((uint64)fgetc(s) << 40) | ((uint64)fgetc(s) << 32) |
         ((uint64)fgetc(s) << 24) | ((uint64)fgetc(s) << 16) |
         ((uint64)fgetc(s) <<  8) | ((uint64)fgetc(s) <<  0);
}

inline void fputlb(file &s, uint8 data) { fputc(s, data); }
inline void fputmb(file &s, uint8 data) { fputc(s, data); }

inline void fputlw(file &s, uint16 data) {
  fputc(s, data >> 0);
  fputc(s, data >> 8);
}

inline void fputmw(file &s, uint16 data) {
  fputc(s, data >> 8);
  fputc(s, data >> 0);
}

inline void fputld(file &s, uint32 data) {
  fputc(s, data >>  0);
  fputc(s, data >>  8);
  fputc(s, data >> 16);
  fputc(s, data >> 24);
}

inline void fputmd(file &s, uint32 data) {
  fputc(s, data >> 24);
  fputc(s, data >> 16);
  fputc(s, data >>  8);
  fputc(s, data >>  0);
}

inline void fputlq(file &s, uint64 data) {
  fputc(s, data >>  0);
  fputc(s, data >>  8);
  fputc(s, data >> 16);
  fputc(s, data >> 24);
  fputc(s, data >> 32);
  fputc(s, data >> 40);
  fputc(s, data >> 48);
  fputc(s, data >> 56);
}

inline void fputmq(file &s, uint64 data) {
  fputc(s, data >> 56);
  fputc(s, data >> 48);
  fputc(s, data >> 40);
  fputc(s, data >> 32);
  fputc(s, data >> 24);
  fputc(s, data >> 16);
  fputc(s, data >>  8);
  fputc(s, data >>  0);
}

/*****
 * ramfile
 *****/

class ramfile : public file {
private:
FILE *fp;
array<uint8> filedata;
char filename[1024];
uint filepos;
uint filesize;
uint filemode;
bool fileopen;
bool filevirtual;

public:
  void read(uint8 *data, uint length) {
    if(!fileopen || filemode == mode_write) { return; }

    memcpy(data, filedata.get(filepos + length) + filepos, length);
    filepos += length;
    if(filepos > filesize)filepos = filesize;
  }

  uint8 read() {
    if(!fileopen || filemode == mode_write) { return 0; }

    if(eof() == true) { return 0xff; }
    return filedata[filepos++];
  }

  void write(uint8 *data, uint length) {
    if(!fileopen || filemode == mode_read) { return; }

    memcpy(filedata.get(filepos + length) + filepos, data, length);
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
    case seek_end:     filepos  = filesize + offset; break;
    case seek_back:    filepos -= offset;            break;
    case seek_forward: filepos += offset;            break;
    }

    if(filemode == mode_read) {
      if(filepos > filesize)filepos = filesize;
    } else {
      if(filepos > filesize)filesize = filepos;
    }
  }

  void truncate(uint size) {
    filesize = size;
    if(filepos > filesize)filepos = filesize;
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

    strcpy(filename, fn ? fn : "");
    filevirtual = (*filename == 0);
    filemode = mode;
    switch(filemode) {
    case mode_read:
    case mode_readwrite:
      if(filevirtual == true) {
        filesize = 0;
      } else {
        fp = fopen(filename, "rb");
        if(!fp) { return false; }
        filesize = fsize(fp);
        fread(filedata.get(filesize), 1, filesize, fp);
        fclose(fp);
      }
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
      if(filevirtual == false) {
        fp = fopen(filename, "wb");
        if(!fp) { return false; }
        fwrite(filedata.get(filesize), 1, filesize, fp);
        fclose(fp);
      }
      break;
    }
    return true;
  }

  bool close() {
    if(!fileopen) { return false; }

  bool result = flush();
    fileopen = false;
    filedata.reset();
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

    if(eof() == true) { return 0xff; }
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
    case seek_start:   fseek(fp, offset, SEEK_SET); break;
    case seek_end:     fseek(fp, offset, SEEK_END); break;
    case seek_back:    fseek(fp, offset, SEEK_CUR); break;
    case seek_forward: fseek(fp, offset, SEEK_CUR); break;
    }
  }

  void truncate(uint size) {
    if(!fp) { return; }

    ftruncate(fp, size);
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

    if(feof(fp)) {
      seek(size(), seek_start);
      return true;
    }

    return false;
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

/*****
 * directory object
 *****/

class directory {
public:
  void open(const char *path) {}
  void close() {}
  uint read(char *filename, uint maxlength) { return 0; }
};

#endif
