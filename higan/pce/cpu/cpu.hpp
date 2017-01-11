//Hudson Soft HuC6280

struct CPU : Processor::HuC6280, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;

  vector<Thread*> peripherals;
};

extern CPU cpu;
