//Sony CXP1100Q-1

struct SMP : Processor::SPC700, Thread {
  auto readPort(uint2 port) const -> uint8;
  auto writePort(uint2 port, uint8 data) -> void;

  auto main() -> void;
  auto load(Markup::Node) -> bool;
  auto power() -> void;

  auto serialize(serializer&) -> void;

  uint8 iplrom[64];
  uint8 apuram[64 * 1024];

private:
  struct IO {
    //timing
    uint clockCounter;
    uint dspCounter;
    uint timerStep;

    //$00f0
    uint8 clockSpeed;
    uint8 timerSpeed;
    bool timersEnable;
    bool ramDisable;
    bool ramWritable;
    bool timersDisable;

    //$00f1
    bool iplromEnable;

    //$00f2
    uint8 dspAddr;

    //$00f8,$00f9
    uint8 ram00f8;
    uint8 ram00f9;
  } io;

  static auto Enter() -> void;

  //memory.cpp
  auto readRAM(uint16 addr) -> uint8;
  auto writeRAM(uint16 addr, uint8 data) -> void;

  auto readBus(uint16 addr) -> uint8;
  auto writeBus(uint16 addr, uint8 data) -> void;

  auto idle() -> void override;
  auto read(uint16 addr) -> uint8 override;
  auto write(uint16 addr, uint8 data) -> void override;

  auto readDisassembler(uint16 addr) -> uint8 override;

  //timing.cpp
  template<uint Frequency> struct Timer {
    uint8 stage0;
    uint8 stage1;
    uint8 stage2;
    uint4 stage3;
    bool line;
    bool enable;
    uint8 target;

    auto tick() -> void;
    auto synchronizeStage1() -> void;
  };

  Timer<192> timer0;
  Timer<192> timer1;
  Timer< 24> timer2;

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto cycleEdge() -> void;
};

extern SMP smp;
