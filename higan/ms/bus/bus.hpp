struct Bus : Processor::Z80::Bus {
  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;

  auto in(uint8 addr) -> uint8 override;
  auto out(uint8 addr, uint8 data) -> void override;

private:
  uint8 ram[0x2000];
};

extern Bus bus;
