struct IO {
  virtual auto portRead(uint16 addr) -> uint8 = 0;
  virtual auto portWrite(uint16 addr, uint8 data) -> void = 0;
};

struct InternalRAM {
  auto power() -> void;

  auto read(uint16 addr, uint size = Byte) -> uint32;
  auto write(uint16 addr, uint8 data) -> void;

private:
  uint8 memory[65536];
};

struct Bus {
  auto power() -> void;

  auto read(uint20 addr) -> uint8;
  auto write(uint20 addr, uint8 data) -> void;

  auto map(IO* io, uint16_t lo, maybe<uint16_t> hi = nothing) -> void;
  auto portRead(uint16 addr) -> uint8;
  auto portWrite(uint16 addr, uint8 data) -> void;

private:
  IO* port[64 * 1024] = {nullptr};
};

extern InternalRAM iram;
extern Bus bus;
