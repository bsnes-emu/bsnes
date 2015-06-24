struct Memory {
  virtual auto read(uint32 addr, uint32 size) -> uint32 = 0;
  virtual auto write(uint32 addr, uint32 size, uint32 word) -> void = 0;
};

struct MMIO : Memory {
  virtual auto read(uint32 addr) -> uint8 = 0;
  virtual auto write(uint32 addr, uint8 data) -> void = 0;
  auto read(uint32 addr, uint32 size) -> uint32;
  auto write(uint32 addr, uint32 size, uint32 word) -> void;
};

struct Bus : Memory {
  static auto mirror(uint32 addr, uint32 size) -> uint32;

  auto wait(uint32 addr, uint32 size) -> unsigned;
  auto idle(uint32 addr) -> void;
  auto read(uint32 addr, uint32 size) -> uint32;
  auto write(uint32 addr, uint32 size, uint32 word) -> void;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  Memory* mmio[0x400]{nullptr};
  bool idleflag{false};
};

extern Bus bus;
