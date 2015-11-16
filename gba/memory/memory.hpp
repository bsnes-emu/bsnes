struct Memory {
  virtual auto read(uint mode, uint32 addr) -> uint32 = 0;
  virtual auto write(uint mode, uint32 addr, uint32 word) -> void = 0;
};

struct MMIO : Memory {
  virtual auto read(uint32 addr) -> uint8 = 0;
  virtual auto write(uint32 addr, uint8 data) -> void = 0;
  auto read(uint mode, uint32 addr) -> uint32 final;
  auto write(uint mode, uint32 addr, uint32 word) -> void final;
};

struct Bus {
  static auto mirror(uint32 addr, uint32 size) -> uint32;

  auto power() -> void;

  Memory* mmio[0x400] = {nullptr};
};

extern Bus bus;
