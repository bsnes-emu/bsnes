struct SuperFXBus : Bus {
  void init();
};

struct SuperFXGSUROM : Memory {
  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};

struct SuperFXGSURAM : Memory {
  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};

struct SuperFXCPUROM : Memory {
  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};

struct SuperFXCPURAM : Memory {
  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};
