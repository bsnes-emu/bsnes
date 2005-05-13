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
  uint32 size();
  void   read(uint8 **buffer, uint32 length = 0);
  bool   open(char *fn);
  void   close();

  FileReader() { fp = 0; fsize = 0; }
  ~FileReader() { if(fp)fclose(fp); }
};
