struct Memory {
  ~Memory();

  auto operator[](uint addr) -> uint8&;
  auto allocate(uint size) -> void;
  auto copy(const uint8* data, uint size) -> void;
  auto free() -> void;

  uint8* data = nullptr;
  uint size = 0;
};

struct MMIO {
  virtual auto mmio_read(uint16 addr) -> uint8 = 0;
  virtual auto mmio_write(uint16 addr, uint8 data) -> void = 0;
};

struct Unmapped : MMIO {
  auto mmio_read(uint16) -> uint8 { return 0x00; }
  auto mmio_write(uint16, uint8) -> void {}
};

struct Bus {
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  MMIO* mmio[65536];
};

extern Unmapped unmapped;
extern Bus bus;
