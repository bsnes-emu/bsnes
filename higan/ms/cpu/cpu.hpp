//Zilog Z80

struct CPU : Processor::Z80, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto synchronizing() const -> bool override;

  auto pollPause() -> void;
  auto setNMI(bool value) -> void;
  auto setINT(bool value) -> void;

  auto power() -> void;

  CPU();

  //serialization.cpp
  auto serialize(serializer&) -> void;

  vector<Thread*> peripherals;

private:
  struct State {
    bool nmiLine;
    bool intLine;
  } state;
};

extern CPU cpu;
