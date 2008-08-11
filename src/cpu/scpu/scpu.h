class sCPU : public CPU {
public:
  void enter();

  #include "core/core.h"
  #include "dma/dma.h"
  #include "memory/memory.h"
  #include "mmio/mmio.h"
  #include "timing/timing.h"

  struct {
    bool wai;
    bool irq;
    uint16 irq_vector;
  } event;

  struct {
    unsigned nmi_hold;
    unsigned irq_hold;

    unsigned nmi_fire;
    unsigned irq_fire;
    unsigned irq_delay;
    unsigned hw_math;

    alwaysinline void set(uint &ctr, uint clocks) {
      if(clocks >= ctr) { ctr = clocks; }
    }

    alwaysinline void sub(uint &ctr, uint clocks) {
      if(ctr >= clocks) {
        ctr -= clocks;
      } else {
        ctr  = 0;
      }
    }
  } counter;

  enum DMA_State { DMA_Inactive, DMA_Run, DMA_CPUsync };

  struct {
    //core
    uint8 opcode;
    bool in_opcode;

    unsigned clock_count;

    //timing
    uint16 vcounter, hcounter;
    uint16 field_lines, line_clocks;

    bool line_rendered;
    uint16 line_render_position;

    bool dram_refreshed;
    uint16 dram_refresh_position;

    bool hdmainit_triggered;
    uint16 hdmainit_trigger_position;

    bool hdma_triggered;

    uint16 irq_delay;

    bool nmi_valid;
    bool nmi_line;
    bool nmi_transition;
    bool nmi_pending;

    uint16 virq_trigger_pos, hirq_trigger_pos;
    bool irq_valid;
    bool irq_line;
    bool irq_transition;
    bool irq_pending;

    //dma
    unsigned dma_counter;
    unsigned dma_clocks;
    bool dma_pending;
    bool hdma_pending;
    bool hdma_mode; //0 = init, 1 = run
    DMA_State dma_state;

    //mmio

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
    uint8 mul_a, mul_b;

    //$4204-$4206
    uint16 div_a;
    uint8  div_b;

    //$4207-$420a
    uint16 hirq_pos, virq_pos;

    //$4214-$4217
    uint16 r4214;
    uint16 r4216;

    //$4218-$421f
    uint8 joy1l, joy1h;
    uint8 joy2l, joy2h;
    uint8 joy3l, joy3h;
    uint8 joy4l, joy4h;
  } status;

  void power();
  void reset();

  sCPU();
  ~sCPU();
};
