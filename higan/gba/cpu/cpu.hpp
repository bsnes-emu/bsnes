struct CPU : Processor::ARM7TDMI, Thread, IO {
  struct Interrupt { enum : uint {
    VBlank       = 0x0001,
    HBlank       = 0x0002,
    VCoincidence = 0x0004,
    Timer0       = 0x0008,
    Timer1       = 0x0010,
    Timer2       = 0x0020,
    Timer3       = 0x0040,
    Serial       = 0x0080,
    DMA0         = 0x0100,
    DMA1         = 0x0200,
    DMA2         = 0x0400,
    DMA3         = 0x0800,
    Keypad       = 0x1000,
    Cartridge    = 0x2000,
  };};

  inline auto clock() const -> uint { return context.clock; }
  inline auto halted() const -> bool { return context.halted; }
  inline auto stopped() const -> bool { return context.stopped; }

  //cpu.cpp
  static auto Enter() -> void;
  auto main() -> void;

  auto step(uint clocks) -> void override;

  auto power() -> void;

  //prefetch.cpp
  auto prefetchSync(uint32 addr) -> void;
  auto prefetchStep(uint clocks) -> void;
  auto prefetchWait() -> void;
  auto prefetchRead() -> uint16;

  //bus.cpp
  auto sleep() -> void override;
  auto get(uint mode, uint32 addr) -> uint32 override;
  auto set(uint mode, uint32 addr, uint32 word) -> void override;
  auto _wait(uint mode, uint32 addr) -> uint;

  //io.cpp
  auto readIO(uint32 addr) -> uint8;
  auto writeIO(uint32 addr, uint8 byte) -> void;

  auto readIWRAM(uint mode, uint32 addr) -> uint32;
  auto writeIWRAM(uint mode, uint32 addr, uint32 word) -> void;

  auto readEWRAM(uint mode, uint32 addr) -> uint32;
  auto writeEWRAM(uint mode, uint32 addr, uint32 word) -> void;

  //dma.cpp
  auto dmaVblank() -> void;
  auto dmaHblank() -> void;
  auto dmaHDMA() -> void;

  //timer.cpp
  auto runFIFO(uint n) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint8 iwram[ 32 * 1024];
  uint8 ewram[256 * 1024];

//private:
  struct DMA {
    //dma.cpp
    inline auto run() -> bool;
    auto transfer() -> void;

    uint2 id;

    boolean active;
    integer waiting;

    uint2 targetMode;
    uint2 sourceMode;
    uint1 repeat;
    uint1 size;
    uint1 drq;
    uint2 timingMode;
    uint1 irq;
    uint1 enable;

    VariadicNatural source;
    VariadicNatural target;
    VariadicNatural length;
    uint32 data;

    struct Latch {
      VariadicNatural target;
      VariadicNatural source;
      VariadicNatural length;
    } latch;
  } dma[4];

  struct Timer {
    //timer.cpp
    inline auto run() -> void;
    auto step() -> void;

    uint2 id;

    boolean pending;

    uint16 period;
    uint16 reload;

    uint2 frequency;
    uint1 cascade;
    uint1 irq;
    uint1 enable;
  } timer[4];

  struct Serial {
    uint1 shiftClockSelect;
    uint1 shiftClockFrequency;
    uint1 transferEnableReceive;
    uint1 transferEnableSend;
    uint1 startBit;
    uint1 transferLength;
    uint1 irqEnable;

    uint16 data[4];
    uint8 data8;
  } serial;

  struct Keypad {
    //keypad.cpp
    auto run() -> void;

    uint1 enable;
    uint1 condition;
    uint1 flag[10];
  } keypad;

  struct Joybus {
    uint1 sc;
    uint1 sd;
    uint1 si;
    uint1 so;
    uint1 scMode;
    uint1 sdMode;
    uint1 siMode;
    uint1 soMode;
    uint1 siIRQEnable;
    uint2 mode;

    uint1 resetSignal;
    uint1 receiveComplete;
    uint1 sendComplete;
    uint1 resetIRQEnable;

    uint32 receive;
    uint32 transmit;

    uint1 receiveFlag;
    uint1 sendFlag;
    uint2 generalFlag;
  } joybus;

  struct IRQ {
    uint1 ime;
    uint16 enable;
    uint16 flag;
  } irq;

  struct Wait {
    uint2 nwait[4];
    uint1 swait[4];
    uint2 phi;
    uint1 prefetch;
    uint1 gameType;
  } wait;

  struct Memory {
    uint1 disable;
    uint3 unknown1;
    uint1 ewram = 1;
    uint4 ewramWait = 13;
    uint4 unknown2;
  } memory;

  struct {
    uint16 slot[8];
    uint32 addr;       //read location of slot buffer
    uint32 load;       //write location of slot buffer
    integer wait = 1;  //number of clocks before next slot load

    inline auto empty() const { return addr == load; }
    inline auto full() const { return load - addr == 16; }
  } prefetch;

  struct Context {
    natural clock;
    boolean halted;
    boolean stopped;
    boolean booted;  //set to true by the GBA BIOS
    boolean dmaActive;
  } context;
};

extern CPU cpu;
