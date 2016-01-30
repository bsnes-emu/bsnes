struct IO {
  static auto power() -> void;

  virtual auto portRead(uint16 addr) -> uint8;
  virtual auto portWrite(uint16 addr, uint8 data) -> void;
};

struct Bus {
  auto read(uint20 addr) -> uint8;
  auto write(uint20 addr, uint8 data) -> void;
};

extern uint8 iram[64 * 1024];
extern IO* iomap[64 * 1024];
extern Bus bus;
