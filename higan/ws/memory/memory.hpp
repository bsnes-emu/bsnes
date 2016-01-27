struct IO {
  static auto power() -> void;

  virtual auto in(uint8 addr) -> uint8 { return 0x00; }
  virtual auto out(uint8 addr, uint8 data) -> void {}
};

struct Bus {
  auto read(uint20 addr) -> uint8;
  auto write(uint20 addr, uint8 data) -> void;
};

extern uint8 iram[64 * 1024];
extern IO* io[256];
extern Bus bus;
