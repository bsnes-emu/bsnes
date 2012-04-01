struct Registers {
  struct DMA {
    uint32 source;
    uint32 target;
    uint16 length;
    struct Control {
      uint2 targetmode;
      uint2 sourcemode;
      uint1 repeat;
      uint1 size;
      uint1 drq;
      uint2 timing;
      uint1 irq;
      uint1 enable;

      operator uint16() const;
      uint16 operator=(uint16 source);
      DMA& operator=(const DMA&) = delete;
    } control;
  } dma[4];

  struct TimerControl {
    uint2 prescalar;
    bool countup;
    bool irq;
    bool enable;

    operator uint16() const;
    uint16 operator=(uint16 source);
    TimerControl& operator=(const TimerControl&) = delete;
  };

  struct Timer {
    uint16 reload;
    TimerControl control;
  } timer[4];

  struct KeypadControl {
    bool a;
    bool b;
    bool select;
    bool start;
    bool right;
    bool left;
    bool up;
    bool down;
    bool r;
    bool l;
    bool enable;
    bool condition;

    operator uint16() const;
    uint16 operator=(uint16 source);
    KeypadControl& operator=(const KeypadControl&) = delete;
  };

  struct Keypad {
    KeypadControl control;
  } keypad;

  bool ime;

  struct Interrupt {
    bool vblank;
    bool hblank;
    bool vcoincidence;
    bool timer0;
    bool timer1;
    bool timer2;
    bool timer3;
    bool serial;
    bool dma0;
    bool dma1;
    bool dma2;
    bool dma3;
    bool keypad;
    bool cartridge;

    operator uint16() const;
    uint16 operator=(uint16 source);
    Interrupt& operator=(const Interrupt&) = delete;
  };

  struct IRQ {
    Interrupt enable;
    Interrupt flag;
  } irq;

  struct WaitControl {
    uint2 sram;
    uint2 wait0n;
    uint1 wait0s;
    uint2 wait1n;
    uint1 wait1s;
    uint2 wait2n;
    uint1 wait2s;
    uint2 phi;
    uint1 prefetch;
    uint1 gametype;

    operator uint16() const;
    uint16 operator=(uint16 source);
    WaitControl& operator=(const WaitControl&) = delete;
  };

  struct Wait {
    WaitControl control;
  } wait;

  struct MemoryControl {
    bool disable;
    uint3 unknown1;
    bool ewram;
    uint4 ewramwait;
    uint4 unknown2;

    operator uint32() const;
    uint32 operator=(uint32 source);
    MemoryControl& operator=(const MemoryControl&) = delete;
  };

  struct Memory {
    MemoryControl control;
  } memory;

  bool postboot;
  enum class Mode : unsigned { Normal, Halt, Stop } mode;
} regs;
