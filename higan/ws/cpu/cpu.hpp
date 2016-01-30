struct CPU : Processor::V30MZ, Thread, IO {
  static auto Enter() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;

  auto wait(uint clocks = 1) -> void override;
  auto read(uint20 addr) -> uint8 override;
  auto write(uint20 addr, uint8 data) -> void override;
  auto in(uint16 port) -> uint8 override;
  auto out(uint16 port, uint8 data) -> void override;

  auto power() -> void;

  //memory.cpp
  auto ramRead(uint16 addr) -> uint8;
  auto ramWrite(uint16 addr, uint8 data) -> void;

  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;
};

extern CPU cpu;
