struct Registers {
  struct DMA {
    VariadicNatural source;
    VariadicNatural target;
    VariadicNatural length;
    uint32 data;
    struct Control {
      uint2 targetmode;
      uint2 sourcemode;
      uint1 repeat;
      uint1 size;
      uint1 drq;
      uint2 timingmode;
      uint1 irq;
      uint1 enable;
    } control;

    //internal
    bool pending;
    struct Run {
      VariadicNatural target;
      VariadicNatural source;
      VariadicNatural length;
    } run;
  } dma[4];

  struct Timer {
    uint16 period;
    uint16 reload;
    bool pending;
    struct Control {
      uint2 frequency;
      uint1 cascade;
      uint1 irq;
      uint1 enable;
    } control;
  } timer[4];

  struct Serial {
    uint16 data[4];
    struct Control {
      uint1 shiftclockselect;
      uint1 shiftclockfrequency;
      uint1 transferenablereceive;
      uint1 transferenablesend;
      uint1 startbit;
      uint1 transferlength;
      uint1 irqenable;
    } control;
    uint8 data8;
  } serial;

  struct Keypad {
    struct Control {
      uint1 flag[10];
      uint1 enable;
      uint1 condition;
    } control;
  } keypad;

  struct Joybus {
    struct Settings {
      uint1 sc;
      uint1 sd;
      uint1 si;
      uint1 so;
      uint1 scmode;
      uint1 sdmode;
      uint1 simode;
      uint1 somode;
      uint1 irqenable;
      uint2 mode;
    } settings;
    struct Control {
      uint1 resetsignal;
      uint1 receivecomplete;
      uint1 sendcomplete;
      uint1 irqenable;
    } control;
    uint32 receive;
    uint32 transmit;
    struct Status {
      uint1 receiveflag;
      uint1 sendflag;
      uint2 generalflag;
    } status;
  } joybus;

  uint1 ime;

  struct IRQ {
    uint16 enable;
    uint16 flag;
  } irq;

  struct Wait {
    struct Control {
      uint2 nwait[4];
      uint1 swait[4];
      uint2 phi;
      uint1 prefetch;
      uint1 gametype;
    } control;
  } wait;

  struct Memory {
    struct Control {
      uint1 disable;
      uint3 unknown1;
      uint1 ewram;
      uint4 ewramwait;
      uint4 unknown2;
    } control;
  } memory;

  uint1 postboot;
  enum class Mode : uint { Normal, Halt, Stop } mode;
  uint clock;
} regs;
