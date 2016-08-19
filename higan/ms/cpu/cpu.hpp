//Zilog Z80

struct CPU : Processor::Z80, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto wait() -> void override;
  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;

  auto power() -> void;
  auto reset() -> void;

private:
  uint8 ram[0x2000];  //8KB
};

extern CPU cpu;
