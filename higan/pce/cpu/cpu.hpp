//Hudson Soft HuC6280

struct CPU : Processor::HuC6280, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;

  auto power() -> void;

  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;
  auto lastCycle() -> void override;

  auto disassembleRead(uint16 addr) -> uint8 override;

  vector<Thread*> peripherals;
};

extern CPU cpu;
