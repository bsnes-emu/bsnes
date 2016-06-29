struct IO {
  virtual auto readIO(uint32 addr) -> uint8 = 0;
  virtual auto writeIO(uint32 addr, uint8 data) -> void = 0;
  auto readIO(uint mode, uint32 addr) -> uint32;
  auto writeIO(uint mode, uint32 addr, uint32 word) -> void;
};

struct Bus {
  static auto mirror(uint32 addr, uint32 size) -> uint32;

  auto power() -> void;

  IO* io[0x400] = {nullptr};
};

extern Bus bus;
