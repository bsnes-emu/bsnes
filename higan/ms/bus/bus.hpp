struct Bus {
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;

  auto in(uint8 addr) -> uint8;
  auto out(uint8 addr, uint8 data) -> void;

private:
  uint8 ram[0x2000];
};

extern Bus bus;
