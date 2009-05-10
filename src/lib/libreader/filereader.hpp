class FileReader : public Reader {
public:
  unsigned size();
  uint8_t* read(unsigned length = 0);
  bool ready();

  FileReader(const char *fn);
  ~FileReader();

private:
  file fp;
};
