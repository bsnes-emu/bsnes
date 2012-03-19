struct Memory {
  virtual uint32 read(uint32 addr, uint32 size) = 0;
  virtual void write(uint32 addr, uint32 size, uint32 word) = 0;
};

struct StaticMemory {
  uint8_t *data;
  unsigned size;

  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  StaticMemory();
  ~StaticMemory();
};

struct Bus : Memory {
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
};

extern Bus bus;
