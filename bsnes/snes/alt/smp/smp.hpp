class SMP : public Processor {
public:
  static const uint8 iplrom[64];
  uint8 *apuram;

  enum : bool { Threaded = false };
  alwaysinline void synchronize_cpu();
  alwaysinline void synchronize_dsp();

  unsigned port_read(unsigned port);
  void port_write(unsigned port, unsigned data);

  unsigned mmio_read(unsigned addr);
  void mmio_write(unsigned addr, unsigned data);

  void enter();
  void power();
  void reset();

  void serialize(serializer&);
  SMP();
  ~SMP();

  #include "core.hpp"

  struct Flags {
    bool n, v, p, b, h, i, z, c;

    alwaysinline operator unsigned() const {
      return (n << 7) | (v << 6) | (p << 5) | (b << 4)
           | (h << 3) | (i << 2) | (z << 1) | (c << 0);
    };

    alwaysinline unsigned operator=(unsigned data) {
      n = data & 0x80; v = data & 0x40; p = data & 0x20; b = data & 0x10;
      h = data & 0x08; i = data & 0x04; z = data & 0x02; c = data & 0x01;
    }

    alwaysinline unsigned operator|=(unsigned data) { return operator=(operator unsigned() | data); }
    alwaysinline unsigned operator^=(unsigned data) { return operator=(operator unsigned() ^ data); }
    alwaysinline unsigned operator&=(unsigned data) { return operator=(operator unsigned() & data); }
  };

  unsigned opcode_number;
  unsigned opcode_cycle;

  struct Regs {
    uint16 pc;
    uint8 sp;
    union {
      uint16 ya;
      struct { uint8 order_lsb2(a, y); };
    };
    uint8 x;
    Flags p;
  } regs;

  struct Status {
    //$00f1
    bool iplrom_enable;

    //$00f2
    unsigned dsp_addr;

    //$00f8,$00f9
    unsigned ram00f8;
    unsigned ram00f9;
  } status;

  template<unsigned frequency>
  struct Timer {
    bool enable;
    uint8 target;
    uint8 stage1_ticks;
    uint8 stage2_ticks;
    uint8 stage3_ticks;

    void tick(unsigned clocks);
  };

  Timer<128> timer0;
  Timer<128> timer1;
  Timer< 16> timer2;
};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern SMPDebugger smp;
#else
  extern SMP smp;
#endif
