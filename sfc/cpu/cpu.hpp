struct CPU : Processor::R65816, Thread, public PPUcounter {
  enum : bool { Threaded = true };

  CPU();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeSMP() -> void;
  auto synchronizePPU() -> void;
  auto synchronizeCoprocessors() -> void;
  auto synchronizeDevices() -> void;

  auto portRead(uint2 port) const -> uint8;
  auto portWrite(uint2 port, uint8 data) -> void;

  auto pio() -> uint8;
  auto joylatch() -> bool;
  alwaysinline auto interrupt_pending() -> bool { return status.interrupt_pending; }

  auto enter() -> void;
  auto enable() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  uint8 wram[128 * 1024] = {0};
  vector<Thread*> coprocessors;

privileged:
  uint cpu_version = 2;  //allowed: 1, 2

  #include "dma/dma.hpp"
  #include "memory/memory.hpp"
  #include "mmio/mmio.hpp"
  #include "timing/timing.hpp"

  struct Status {
    bool interrupt_pending;

    uint clock_count;
    uint line_clocks;

    //timing
    bool irq_lock;

    uint dram_refresh_position;
    bool dram_refreshed;

    uint hdma_init_position;
    bool hdma_init_triggered;

    uint hdma_position;
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

    //DMA
    bool dma_active;
    uint dma_counter;
    uint dma_clocks;
    bool dma_pending;
    bool hdma_pending;
    bool hdma_mode;  //0 = init, 1 = run

    //auto joypad polling
    bool auto_joypad_active;
    bool auto_joypad_latch;
    uint auto_joypad_counter;
    uint auto_joypad_clock;

    //MMIO
    //$2140-217f
    uint8 port[4];

    //$2181-$2183
    uint17 wram_addr;

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
    uint8 wrdivb;

    //$4207-$420a
    uint9 hirq_pos;
    uint9 virq_pos;

    //$420d
    uint rom_speed;

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

  static auto Enter() -> void;
  auto op_step() -> void;

  struct Debugger {
    hook<void (uint24)> op_exec;
    hook<void (uint24, uint8)> op_read;
    hook<void (uint24, uint8)> op_write;
    hook<void ()> op_nmi;
    hook<void ()> op_irq;
  } debugger;
};

extern CPU cpu;
