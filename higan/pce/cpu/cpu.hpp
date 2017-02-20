//Hudson Soft HuC6280

struct CPU : Processor::HuC6280, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto power() -> void;
  auto lastCycle() -> void override;

  //memory.cpp
  auto load() -> void;
  auto save() -> void;

  //io.cpp
  auto read(uint8 bank, uint13 addr) -> uint8 override;
  auto read_(uint8 bank, uint13 addr) -> uint8;
  auto write(uint8 bank, uint13 addr, uint8 data) -> void override;
  auto store(uint2 addr, uint8 data) -> void override;

  //timer.cpp
  auto timerStep(uint clocks) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  vector<Thread*> peripherals;

private:
  uint8 ram[0x8000];  //PC Engine = 8KB, SuperGrafx = 32KB
  uint8 bram[0x800];  //PC Engine CD-ROM Backup RAM = 2KB

  struct IRQ {
    //irq.cpp
    auto pending() const -> bool;
    auto vector() const -> uint16;
    auto poll() -> void;

  private:
    bool   disableExternal;
    bool   disableVDC;
    bool   disableTimer;

    bool   pendingIRQ;
    uint16 pendingVector;

    friend class CPU;
  } irq;

  struct Timer {
    inline auto irqLine() const { return line; }

    //timer.cpp
    auto start() -> void;
    auto step(uint clocks) -> void;

  private:
    bool  enable;
    uint7 latch;
    uint7 value;
    uint  clock;

    bool  line;

    friend class CPU;
  } timer;

  struct IO {
    uint8 mdr;
  } io;
};

extern CPU cpu;
