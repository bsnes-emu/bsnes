struct CPU : Processor::MOS6502, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power() -> void;

  //memory.cpp
  auto readRAM(uint11 addr) -> uint8;
  auto writeRAM(uint11 addr, uint8 data) -> void;

  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

  auto readDebugger(uint16 addr) -> uint8 override;

  auto serialize(serializer&) -> void;

  //timing.cpp
  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;
  auto lastCycle() -> void override;
  auto nmi(uint16& vector) -> void override;

  auto oamdma() -> void;

  auto nmiLine(bool) -> void;
  auto irqLine(bool) -> void;
  auto apuLine(bool) -> void;

  auto rdyLine(bool) -> void;
  auto rdyAddr(bool valid, uint16 value = 0) -> void;

//protected:
  vector<Thread*> peripherals;

  uint8 ram[0x0800];

  struct IO {
    bool interruptPending;
    bool nmiPending;
    bool nmiLine;
    bool irqLine;
    bool apuLine;

    bool rdyLine;
    bool rdyAddrValid;
    uint16 rdyAddrValue;

    bool oamdmaPending;
    uint8 oamdmaPage;
  } io;
};

extern CPU cpu;
