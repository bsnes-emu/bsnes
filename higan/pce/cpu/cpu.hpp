//Hudson Soft HuC6280

struct CPU : Processor::HuC6280, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;

  auto power() -> void;

  auto read(uint21 addr) -> uint8 override;
  auto write(uint21 addr, uint8 data) -> void override;
  auto lastCycle() -> void override;

  vector<Thread*> peripherals;

private:
  uint8 ram[0x2000];
};

extern CPU cpu;
