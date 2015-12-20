struct CPU : Processor::R65816, Thread, public PPUcounter {
  enum : bool { Threaded = true };

  CPU();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeSMP() -> void;
  auto synchronizePPU() -> void;
  auto synchronizeCoprocessors() -> void;
  auto synchronizeDevices() -> void;

  auto pio() -> uint8;
  auto joylatch() -> bool;
  auto interrupt_pending() -> bool;
  auto port_read(uint8 port) -> uint8;
  auto port_write(uint8 port, uint8 data) -> void;
  auto mmio_read(uint addr, uint8 data) -> uint8;
  auto mmio_write(uint addr, uint8 data) -> void;

  auto op_io() -> void;
  auto op_read(uint addr) -> uint8;
  auto op_write(uint addr, uint8 data) -> void;

  auto enter() -> void;
  auto enable() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto serialize(serializer&) -> void;

  uint8 wram[128 * 1024];
  vector<Thread*> coprocessors;

private:
  //cpu
  static auto Enter() -> void;

  //timing
  auto queue_event(uint id) -> void;
  auto last_cycle() -> void;
  auto add_clocks(uint clocks) -> void;
  auto scanline() -> void;
  auto run_auto_joypad_poll() -> void;

  struct QueueEvent {
    enum : uint {
      DramRefresh,
      HdmaRun,
    };
  };
  nall::priority_queue<uint> queue;

  //memory
  auto speed(uint addr) const -> uint;

  //dma
  auto dma_transfer_valid(uint8 bbus, uint abus) -> bool;
  auto dma_addr_valid(uint abus) -> bool;
  auto dma_read(uint abus) -> uint8;
  auto dma_write(bool valid, uint addr, uint8 data) -> void;
  auto dma_transfer(bool direction, uint8 bbus, uint abus) -> void;
  auto dma_bbus(uint i, uint index) -> uint8;
  auto dma_addr(uint i) -> uint;
  auto hdma_addr(uint i) -> uint;
  auto hdma_iaddr(uint i) -> uint;
  auto dma_run() -> void;
  auto hdma_active_after(uint i) -> bool;
  auto hdma_update(uint i) -> void;
  auto hdma_run() -> void;
  auto hdma_init() -> void;
  auto dma_reset() -> void;

  //registers
  uint8 port_data[4];

  struct Channel {
    bool dma_enabled;
    bool hdma_enabled;

    bool direction;
    bool indirect;
    bool unused;
    bool reverse_transfer;
    bool fixed_transfer;
    uint8 transfer_mode;

    uint8 dest_addr;
    uint16 source_addr;
    uint8 source_bank;

    union {
      uint16 transfer_size;
      uint16 indirect_addr;
    };

    uint8 indirect_bank;
    uint16 hdma_addr;
    uint8 line_counter;
    uint8 unknown;

    bool hdma_completed;
    bool hdma_do_transfer;
  } channel[8];

  struct Status {
    bool nmi_valid;
    bool nmi_line;
    bool nmi_transition;
    bool nmi_pending;

    bool irq_valid;
    bool irq_line;
    bool irq_transition;
    bool irq_pending;

    bool irq_lock;
    bool hdma_pending;

    uint wram_addr;

    bool joypad_strobe_latch;

    bool nmi_enabled;
    bool virq_enabled;
    bool hirq_enabled;
    bool auto_joypad_poll_enabled;

    uint8 pio;

    uint8 wrmpya;
    uint8 wrmpyb;
    uint16 wrdiva;
    uint8 wrdivb;

    uint16 htime;
    uint16 vtime;

    uint rom_speed;

    uint16 rddiv;
    uint16 rdmpy;

    uint8 joy1l, joy1h;
    uint8 joy2l, joy2h;
    uint8 joy3l, joy3h;
    uint8 joy4l, joy4h;
  } status;
};

extern CPU cpu;
