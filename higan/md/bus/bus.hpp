struct Bus {
  auto readByte(uint24 addr) -> uint16;
  auto readWord(uint24 addr) -> uint16;
  auto writeByte(uint24 addr, uint16 data) -> void;
  auto writeWord(uint24 addr, uint16 data) -> void;

  auto readIO(uint24 addr) -> uint16;
  auto writeIO(uint24 addr, uint16 data) -> void;

private:
  uint8 ram[64 * 1024];
};

extern Bus bus;
