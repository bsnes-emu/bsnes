class FileReader : public Reader {
private:
  FILE *fp;
  unsigned filesize;

public:
  unsigned size();
  uint8_t* read(unsigned length = 0);
  bool ready();

  FileReader(const char *fn);
  ~FileReader();
};

class FileWriter : public Writer {
private:
  FILE *fp;

public:
  void write(uint8_t *buffer, unsigned length);
  bool ready();

  FileWriter(const char *fn);
  ~FileWriter();
};
