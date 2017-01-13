//Zilog Z80

struct CPU : Processor::Z80, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto pollPause() -> void;
  auto setNMI(bool value) -> void;
  auto setINT(bool value) -> void;

  auto power() -> void;

  vector<Thread*> peripherals;

private:
  struct State {
    boolean nmiLine;
    boolean intLine;
  } state;
};

extern CPU cpu;
