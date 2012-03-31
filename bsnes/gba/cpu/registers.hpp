struct Registers {
  bool ime;

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
  } irq_enable, irq_flag;

  bool postboot;
  enum class Mode : unsigned { Normal, Halt, Stop } mode;
} regs;
