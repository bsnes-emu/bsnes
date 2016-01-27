struct CPU : Processor::V30MZ, Thread {
  static auto Enter() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto read(uint32 addr) -> uint8 override;
  auto write(uint32 addr, uint8 data) -> void override;
  auto power() -> void;

  auto ramRead(uint addr) -> uint8;
  auto ramWrite(uint addr, uint8 data) -> void;
};

extern CPU cpu;
