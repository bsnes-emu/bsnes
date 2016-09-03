struct APU_Bus : Processor::Z80::Bus {
};

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

struct BusAPU : Processor::Z80::Bus {
  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;
  auto in(uint8 addr) -> uint8 override;
  auto out(uint8 addr, uint8 data) -> void override;
};

extern Bus bus;
extern BusAPU busAPU;
