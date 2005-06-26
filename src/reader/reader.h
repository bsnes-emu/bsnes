class Reader {
public:
  virtual uint32 size() = 0;
  virtual void   read(uint8 **buffer, uint32 length = 0) = 0;
};

class FileReader : public Reader {
private:
FILE  *fp;
uint32 fsize;

public:
enum {
  TYPE_ROM  = 0,
  TYPE_SRAM = 1
};
  uint32 size();
  void   read(uint8 **buffer, uint32 length = 0);
  bool   open(uint8 type, char *fn);
  void   close();

  FileReader() { fp = 0; fsize = 0; }
  ~FileReader() { if(fp)fclose(fp); }
};

class Writer {
public:
  virtual void write(uint8 *buffer, uint32 length) = 0;
};

class FileWriter : public Writer {
private:
FILE *fp;

public:
  void write(uint8 *buffer, uint32 length);
  bool open(char *fn);
  void close();

  FileWriter() { fp = 0; }
  ~FileWriter() { if(fp)fclose(fp); }
};
