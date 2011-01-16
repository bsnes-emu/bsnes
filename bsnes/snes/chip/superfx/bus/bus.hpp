struct SuperFXCPUROM : Memory {
  unsigned size() const;
  uint8 read(unsigned);
  void write(unsigned, uint8);
};

struct SuperFXCPURAM : Memory {
  unsigned size() const;
  uint8 read(unsigned);
  void write(unsigned, uint8);
};

namespace memory {
  extern SuperFXCPUROM fxrom;
  extern SuperFXCPURAM fxram;
}
