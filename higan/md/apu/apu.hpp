//Zilog Z80

struct APU : Processor::Z80, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto enable(bool) -> void;
  auto power() -> void;
  auto reset() -> void;

  auto setNMI(bool value) -> void;
  auto setINT(bool value) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct State {
    uint1 enabled;
    uint1 nmiLine;
    uint1 intLine;
  } state;
};

extern APU apu;
