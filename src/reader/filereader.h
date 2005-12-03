class FileReader : public Reader {
private:
FILE  *fp;
uint32 fsize;

public:
  uint32 size();
  uint8 *read(uint32 length = 0);
  bool   open(char *fn);
  void   close();

  FileReader() { fp = 0; fsize = 0; }
  ~FileReader() { if(fp)fclose(fp); }
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
