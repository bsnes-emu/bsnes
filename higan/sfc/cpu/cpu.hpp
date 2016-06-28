struct CPU : Processor::R65816, Thread, PPUcounter {
  auto interruptPending() const -> bool override;
  auto pio() const -> uint8;
  auto joylatch() const -> bool;

  CPU();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeSMP() -> void;
  auto synchronizePPU() -> void;
  auto synchronizeCoprocessors() -> void;
  auto synchronizePeripherals() -> void;

  auto portRead(uint2 port) const -> uint8;
  auto portWrite(uint2 port, uint8 data) -> void;

  static auto Enter() -> void;
  auto main() -> void;
  auto load(Markup::Node) -> bool;
  auto power() -> void;
  auto reset() -> void;

  //dma.cpp
  auto dmaAddClocks(uint clocks) -> void;
  auto dmaTransferValid(uint8 bbus, uint24 abus) -> bool;
  auto dmaAddressValid(uint24 abus) -> bool;
  auto dmaRead(uint24 abus) -> uint8;
  auto dmaWrite(bool valid, uint addr = 0, uint8 data = 0) -> void;
  auto dmaTransfer(bool direction, uint8 bbus, uint24 abus) -> void;

  inline auto dmaAddressB(uint n, uint channel) -> uint8;
  inline auto dmaAddress(uint n) -> uint24;
  inline auto hdmaAddress(uint n) -> uint24;
  inline auto hdmaIndirectAddress(uint n) -> uint24;

  inline auto dmaEnabledChannels() -> uint;
  inline auto hdmaActive(uint n) -> bool;
  inline auto hdmaActiveAfter(uint s) -> bool;
  inline auto hdmaEnabledChannels() -> uint;
  inline auto hdmaActiveChannels() -> uint;

  auto dmaRun() -> void;
  auto hdmaUpdate(uint n) -> void;
  auto hdmaRun() -> void;
  auto hdmaInitReset() -> void;
  auto hdmaInit() -> void;

  //memory.cpp
  auto io() -> void override;
  auto read(uint24 addr) -> uint8 override;
  auto write(uint24 addr, uint8 data) -> void override;
  alwaysinline auto speed(uint24 addr) const -> uint;
  auto readDisassembler(uint24 addr) -> uint8 override;

  //mmio.cpp
  auto readAPU(uint24 addr, uint8 data) -> uint8;
  auto readCPU(uint24 addr, uint8 data) -> uint8;
  auto readDMA(uint24 addr, uint8 data) -> uint8;
  auto writeAPU(uint24 addr, uint8 data) -> void;
  auto writeCPU(uint24 addr, uint8 data) -> void;
  auto writeDMA(uint24 addr, uint8 data) -> void;

  //timing.cpp
  auto dmaCounter() const -> uint;

  auto addClocks(uint clocks) -> void;
  auto scanline() -> void;

  alwaysinline auto aluEdge() -> void;
  alwaysinline auto dmaEdge() -> void;
  alwaysinline auto lastCycle() -> void;

  //irq.cpp
  alwaysinline auto pollInterrupts() -> void;
  auto nmitimenUpdate(uint8 data) -> void;
  auto rdnmi() -> bool;
  auto timeup() -> bool;

  alwaysinline auto nmiTest() -> bool;
  alwaysinline auto irqTest() -> bool;

  //joypad.cpp
  auto stepAutoJoypadPoll() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint8 wram[128 * 1024];
  vector<Thread*> coprocessors;
  vector<Thread*> peripherals;

privileged:
  uint version = 2;  //allowed: 1, 2

  struct Status {
    bool interruptPending;

    uint clockCount;
    uint lineClocks;

    //timing
    bool irqLock;

    uint dramRefreshPosition;
    bool dramRefreshed;

    uint hdmaInitPosition;
    bool hdmaInitTriggered;

    uint hdmaPosition;
    bool hdmaTriggered;

    bool nmiValid;
    bool nmiLine;
    bool nmiTransition;
    bool nmiPending;
    bool nmiHold;

    bool irqValid;
    bool irqLine;
    bool irqTransition;
    bool irqPending;
    bool irqHold;

    bool powerPending;
    bool resetPending;

    //DMA
    bool dmaActive;
    uint dmaCounter;
    uint dmaClocks;
    bool dmaPending;
    bool hdmaPending;
    bool hdmaMode;  //0 = init, 1 = run

    //auto joypad polling
    bool autoJoypadActive;
    bool autoJoypadLatch;
    uint autoJoypadCounter;
    uint autoJoypadClock;

    //MMIO
    //$2140-217f
    uint8 port[4];

    //$2181-$2183
    uint17 wramAddress;

    //$4016-$4017
    bool joypadStrobeLatch;
    uint32 joypad1_bits;
    uint32 joypad2_bits;

    //$4200
    bool nmiEnabled;
    bool hirqEnabled;
    bool virqEnabled;
    bool autoJoypadPoll;

    //$4201
    uint8 pio;

    //$4202-$4203
    uint8 wrmpya;
    uint8 wrmpyb;

    //$4204-$4206
    uint16 wrdiva;
    uint8 wrdivb;

    //$4207-$420a
    uint9 hirqPos;
    uint9 virqPos;

    //$420d
    uint romSpeed;

    //$4214-$4217
    uint16 rddiv;
    uint16 rdmpy;

    //$4218-$421f
    uint16 joy1;
    uint16 joy2;
    uint16 joy3;
    uint16 joy4;
  } status;

  struct ALU {
    uint mpyctr;
    uint divctr;
    uint shift;
  } alu;

  struct Channel {
    //$420b
    bool dmaEnabled;

    //$420c
    bool hdmaEnabled;

    //$43x0
    bool direction;
    bool indirect;
    bool unused;
    bool reverseTransfer;
    bool fixedTransfer;
    uint3 transferMode;

    //$43x1
    uint8 targetAddress;

    //$43x2-$43x3
    uint16 sourceAddress;

    //$43x4
    uint8 sourceBank;

    //$43x5-$43x6
    union {
      uint16 transferSize = 0;
      uint16_t indirectAddress;
    };

    //$43x7
    uint8 indirectBank;

    //$43x8-$43x9
    uint16 hdmaAddress;

    //$43xa
    uint8 lineCounter;

    //$43xb/$43xf
    uint8 unknown;

    //internal state
    bool hdmaCompleted;
    bool hdmaDoTransfer;
  } channel[8];

  struct Pipe {
    bool valid;
    uint addr;
    uint8 data;
  } pipe;
};

extern CPU cpu;
