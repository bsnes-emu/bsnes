//Zilog Z80

struct CPU : Processor::Z80, Processor::Z80::Bus, Thread {
  //cpu.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto synchronizing() const -> bool override;

  auto pollPause() -> void;
  auto setNMI(bool value) -> void;
  auto setINT(bool value) -> void;

  auto power() -> void;

  //bus.cpp
  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;

  auto in(uint8 addr) -> uint8 override;
  auto out(uint8 addr, uint8 data) -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  vector<Thread*> peripherals;

private:
  uint8 ram[8 * 1024];

  struct State {
    bool nmiLine = 0;
    bool intLine = 0;
  } state;
};

extern CPU cpu;
