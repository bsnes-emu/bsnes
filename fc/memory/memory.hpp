struct Bus {
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
};

extern Bus bus;
