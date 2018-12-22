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

  auto read(uint16 address) -> uint8 override;
  auto write(uint16 address, uint8 data) -> void override;

  auto in(uint8 address) -> uint8 override;
  auto out(uint8 address, uint8 data) -> void override;

  auto power() -> void;

  //coleco.cpp
  auto readColeco(uint16 address) -> uint8;
  auto writeColeco(uint16 address, uint8 data) -> void;

  auto inColeco(uint8 address) -> uint8;
  auto outColeco(uint8 address, uint8 data) -> void;

  //sega.cpp
  auto readSega(uint16 address) -> uint8;
  auto writeSega(uint16 address, uint8 data) -> void;

  auto inSega(uint8 address) -> uint8;
  auto outSega(uint8 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  vector<Thread*> peripherals;

private:
  Emulator::WritableMemory<uint8> ram;
  Emulator::WritableMemory<uint8> expansion;

  struct State {
    bool nmiLine = 0;
    bool intLine = 0;
  } state;

  struct Coleco {
    uint1 replaceBIOS;
    uint1 replaceRAM;
  } coleco;
};

extern CPU cpu;
