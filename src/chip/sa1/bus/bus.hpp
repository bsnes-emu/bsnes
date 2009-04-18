class SA1Bus : public Bus {
public:
  void init();
};

struct CPUIRAM : Memory {
  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};

struct CPUBWRAM : Memory {
  bool cc1dma;

  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};

struct SA1IRAM : Memory {
  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};

struct SA1BWRAM : Memory {
  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};

struct SA1BitmapRAM : Memory {
  unsigned size() const;
  uint8_t read(unsigned);
  void write(unsigned, uint8_t);
};

namespace memory {
  namespace cpu {
    extern CPUIRAM iram;
    extern CPUBWRAM bwram;
  }

  namespace sa1 {
    extern SA1IRAM iram;
    extern SA1BWRAM bwram;
    extern SA1BitmapRAM bitmapram;
  }
}
