struct IO {
  static auto power() -> void;

  virtual auto portRead(uint16 addr) -> uint8;
  virtual auto portWrite(uint16 addr, uint8 data) -> void;
};

struct InternalRAM {
  auto power() -> void;

  auto read(uint16 addr, uint size = Byte) -> uint32;
  auto write(uint16 addr, uint8 data) -> void;

private:
  uint8 memory[65536];
};

struct Bus {
  auto read(uint20 addr) -> uint8;
  auto write(uint20 addr, uint8 data) -> void;
};

extern IO* iomap[64 * 1024];
extern InternalRAM iram;
extern Bus bus;
