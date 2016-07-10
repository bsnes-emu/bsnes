//Motorola 68000

struct CPU : Processor::M68K, Thread {
  static auto Enter() -> void;
  auto boot() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;

  auto power() -> void;
  auto reset() -> void;

  auto read(uint32 addr) -> uint8 override;
  auto write(uint32 addr, uint8 data) -> void override;
};

extern CPU cpu;
