struct CPU : Processor {
  #include "core/core.hpp"
  #include "memory/memory.hpp"
  uint8 ram[0x0800];

  struct Status {
    bool nmi_line;
    bool irq_line;

    bool controller_latch;
    unsigned controller_port0;
    unsigned controller_port1;
  } status;

  static void Main();
  void main();
  void add_clocks(unsigned clocks);
  void interrupt(uint16 vector);

  void power();
  void reset();

  uint8 ram_read(uint16 addr);
  void ram_write(uint16 addr, uint8 data);

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  void oam_dma(uint16 addr);

bool trace;
};

extern CPU cpu;
