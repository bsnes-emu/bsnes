//Hudson Soft HuC6280

struct CPU : Processor::HuC6280, Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void override;

  auto power() -> void;
  auto lastCycle() -> void override;

  //io.cpp
  auto read(uint21 addr) -> uint8 override;
  auto write(uint21 addr, uint8 data) -> void override;

  //timer.cpp
  auto timerStep(uint clocks) -> void;

  vector<Thread*> peripherals;

  struct IRQ {
    enum class Line : uint { External, VDC, Timer };

    //irq.cpp
    auto pending() const -> bool;
    auto vector() const -> uint16;
    auto poll() -> void;
    auto level(Line, bool = 1) -> void;

  private:
    bool disableExternal;
    bool disableVDC;
    bool disableTimer;

    bool pendingExternal;
    bool pendingVDC;
    bool pendingTimer;

    bool   pendingIRQ;
    uint16 pendingVector;

    friend class CPU;
  } irq;

  struct Timer {
    //timer.cpp
    auto start() -> void;
    auto step(uint clocks) -> void;

  private:
    bool  enable;
    uint7 latch;
    uint7 value;
    uint  clock;

    friend class CPU;
  } timer;

private:
  uint8 ram[0x2000];
};

extern CPU cpu;
