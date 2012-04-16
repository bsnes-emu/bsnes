struct Registers {
  struct DMAControl {
    uint2 targetmode;
    uint2 sourcemode;
    uint1 repeat;
    uint1 size;
    uint1 drq;
    uint2 timingmode;
    uint1 irq;
    uint1 enable;

    operator uint16() const;
    uint16 operator=(uint16 source);
    DMAControl& operator=(const DMAControl&) = delete;
  };

  struct DMA {
    uint32 source;
    uint32 target;
    uint16 length;
    DMAControl control;

    //internal
    struct Run {
      uint32 target;
      uint32 source;
      uint16 length;
    } run;
  } dma[4];

  struct TimerControl {
    uint2 frequency;
    uint1 cascade;
    uint1 irq;
    uint1 enable;

    operator uint8() const;
    uint8 operator=(uint8 source);
    TimerControl& operator=(const TimerControl&) = delete;
  };

  struct Timer {
    uint16 period;
    uint16 reload;
    TimerControl control;
  } timer[4];

  struct KeypadControl {
    uint1 flag[10];
    uint1 enable;
    uint1 condition;

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
    bool timer[4];
    bool serial;
    bool dma[4];
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
    uint2 nwait[4];
    uint2 swait[4];
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
  unsigned clock;
} regs;
