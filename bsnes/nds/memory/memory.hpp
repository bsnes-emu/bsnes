struct Memory {
  virtual ~Memory();
  virtual uint32 read(uint32 addr, uint32 size) = 0;
  virtual void write(uint32 addr, uint32 size, uint32 word) = 0;
};

struct StaticMemory : Memory {
  uint8_t *data;
  unsigned size;
  
  uint8& operator[](uint32 addr);
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  StaticMemory();
  ~StaticMemory();
};

// Slot 2 SRAM - 8-bit; bytes only
struct ByteMemory : Memory {
  uint8_t *data;
  unsigned size;
  
  uint8& operator[](uint32 addr);
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  ByteMemory();
  ~ByteMemory();
};

// VRAM, Palettes - 16-bit; halves and words
struct HalfMemory : Memory {
  uint16_t *data;
  unsigned size;
  
  uint16& operator[](uint32 addr);
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  HalfMemory();
  ~HalfMemory();
};

// OAM, BIOS - 32-bit; halves and words
struct WordMemory : Memory {
  uint32_t *data;
  unsigned size;
  
  uint32& operator[](uint32 addr);
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  WordMemory();
  ~WordMemory();
};

// EWRAM - 16-bit; all sizes
struct SDRAM : Memory {
  uint16_t *data;
  unsigned size;
  
  uint16& operator[](uint32 addr);
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  SDRAM();
  ~SDRAM();
};

// TCM, IWRAM - 32-bit; all sizes
struct SRAM : Memory {
  uint32_t *data;
  unsigned size;
  
  uint32& operator[](uint32 addr);
  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  SRAM();
  ~SRAM();
};
