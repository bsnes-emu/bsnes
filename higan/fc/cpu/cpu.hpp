struct CPU : Processor::MOS6502, Thread {
  inline auto rate() const -> uint { return Region::PAL() ? 16 : 12; }

  //cpu.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power(bool reset) -> void;

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

  uint8 ram[0x800];

  struct IO {
    bool interruptPending = 0;
    bool nmiPending = 0;
    bool nmiLine = 0;
    bool irqLine = 0;
    bool apuLine = 0;

    bool rdyLine = 0;
    bool rdyAddrValid = 0;
    uint16 rdyAddrValue;

    bool oamdmaPending = 0;
    uint8 oamdmaPage;
  } io;
};

extern CPU cpu;
