//Motorola 68000

struct CPU : Processor::M68K, Thread {
  static auto Enter() -> void;
  auto boot() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;

  auto power() -> void;
  auto reset() -> void;

  auto read(bool word, uint24 addr) -> uint16 override;
  auto write(bool word, uint24 addr, uint16 data) -> void override;
};

extern CPU cpu;
