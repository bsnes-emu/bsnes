class FileReader : public Reader {
private:
FILE  *fp;
uint32 fsize;

public:
  uint32 size();
  uint8 *read(uint32 length = 0);
  bool   ready();

  FileReader(const char *fn);
  ~FileReader();
};

class FileWriter : public Writer {
private:
FILE *fp;

public:
  void write(uint8 *buffer, uint32 length);
  bool ready();

  FileWriter(const char *fn);
  ~FileWriter();
};
