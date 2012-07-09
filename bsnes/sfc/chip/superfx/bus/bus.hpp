struct CPUROM : Memory {
  unsigned size() const;
  uint8 read(unsigned);
  void write(unsigned, uint8);
} cpurom;

struct CPURAM : Memory {
  unsigned size() const;
  uint8 read(unsigned);
  void write(unsigned, uint8);
} cpuram;
