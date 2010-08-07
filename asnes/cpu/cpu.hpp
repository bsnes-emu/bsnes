class CPU : public Processor, public PPUCounter, public MMIO, public CPUcore {
public:
  //synchronization
  array<Processor*> coprocessors;
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_smp();
  void synchronize_ppu();
  void synchronize_coprocessor();

  static void Enter();
  void enter();
  debugvirtual void op_step();
  void op_irq();
  bool interrupt_pending() { return status.interrupt_pending; }

  uint8 cpu_version;

  #include "dma/dma.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"
  #include "timing/timing.hpp"

  struct Status {
    bool interrupt_pending;
    uint16 interrupt_vector;

    unsigned clock_count;
    unsigned line_clocks;

    //======
    //timing
    //======

    bool irq_lock;

    unsigned dram_refresh_position;
    bool dram_refreshed;

    unsigned hdma_init_position;
    bool hdma_init_triggered;

    unsigned hdma_position;
    bool hdma_triggered;

    bool nmi_valid;
    bool nmi_line;
    bool nmi_transition;
    bool nmi_pending;
    bool nmi_hold;

    bool irq_valid;
    bool irq_line;
    bool irq_transition;
    bool irq_pending;
    bool irq_hold;

    bool reset_pending;

    //===
    //DMA
    //===

    bool dma_active;
    unsigned dma_counter;
    unsigned dma_clocks;
    bool dma_pending;
    bool hdma_pending;
    bool hdma_mode;  //0 = init, 1 = run

    //====
    //MMIO
    //====

    //$2181-$2183
    uint32 wram_addr;

    //$4016-$4017
    bool joypad_strobe_latch;
    uint32 joypad1_bits;
    uint32 joypad2_bits;

    //$4200
    bool nmi_enabled;
    bool hirq_enabled, virq_enabled;
    bool auto_joypad_poll;

    //$4201
    uint8 pio;

    //$4202-$4203
    uint8 wrmpya;
    uint8 wrmpyb;

    //$4204-$4206
    uint16 wrdiva;
    uint8  wrdivb;

    //$4207-$420a
    uint16 hirq_pos, virq_pos;

    //$420d
    unsigned rom_speed;

    //$4214-$4217
    uint16 rddiv;
    uint16 rdmpy;

    //$4218-$421f
    uint8 joy1l, joy1h;
    uint8 joy2l, joy2h;
    uint8 joy3l, joy3h;
    uint8 joy4l, joy4h;
  } status;

  struct ALU {
    unsigned mpyctr;
    unsigned divctr;
    unsigned shift;
  } alu;

  void power();
  void reset();

  void serialize(serializer&);
  CPU();
  ~CPU();

  friend class CPUDebugger;
};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern CPUDebugger cpu;
#else
  extern CPU cpu;
#endif
