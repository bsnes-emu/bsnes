struct Registers {
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
    Interrupt& operator=(uint16 source);
  } irq_enable, irq_flag;
} regs;
