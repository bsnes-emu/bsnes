struct VBRBus : Bus {
  void init();
};

struct SA1Bus : Bus {
  void init();
};

struct VSPROM : Memory {
  unsigned size() const;
  alwaysinline uint8 read(unsigned);
  alwaysinline void write(unsigned, uint8);
};

struct CPUIRAM : Memory {
  unsigned size() const;
  alwaysinline uint8 read(unsigned);
  alwaysinline void write(unsigned, uint8);
};

struct SA1IRAM : Memory {
  unsigned size() const;
  alwaysinline uint8 read(unsigned);
  alwaysinline void write(unsigned, uint8);
};

struct SA1BWRAM : Memory {
  unsigned size() const;
  alwaysinline uint8 read(unsigned);
  alwaysinline void write(unsigned, uint8);
};

struct CC1BWRAM : Memory {
  unsigned size() const;
  alwaysinline uint8 read(unsigned);
  alwaysinline void write(unsigned, uint8);
  bool dma;
};

struct BitmapRAM : Memory {
  unsigned size() const;
  alwaysinline uint8 read(unsigned);
  alwaysinline void write(unsigned, uint8);
};

namespace memory {
  extern StaticRAM iram;

  extern VSPROM vsprom;
  extern CPUIRAM cpuiram;
  extern SA1IRAM sa1iram;
  extern SA1BWRAM sa1bwram;
  extern CC1BWRAM cc1bwram;
  extern BitmapRAM bitmapram;
};
