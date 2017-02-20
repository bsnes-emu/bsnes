//Zilog Z80

struct APU : Processor::Z80, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto enable(bool) -> void;
  auto power() -> void;

  auto setNMI(bool value) -> void;
  auto setINT(bool value) -> void;

private:
  struct State {
    boolean enabled;
    boolean nmiLine;
    boolean intLine;
  } state;
};

extern APU apu;
