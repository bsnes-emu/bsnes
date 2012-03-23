struct CPU : Thread {
  #include "core/core.hpp"
  #include "memory/memory.hpp"
  uint8 ram[0x0800];

  struct Status {
    bool interrupt_pending;
    bool nmi_pending;
    bool nmi_line;
    bool irq_line;
    bool irq_apu_line;

    bool rdy_line;
    optional<uint16> rdy_addr;

    bool oam_dma_pending;
    uint8 oam_dma_page;

    bool controller_latch;
    unsigned controller_port0;
    unsigned controller_port1;
  } status;

  static void Main();
  void main();
  void add_clocks(unsigned clocks);

  void power();
  void reset();

  uint8 mdr() const;
  void set_rdy_line(bool);
  void set_rdy_addr(optional<uint16>);

  uint8 ram_read(uint16 addr);
  void ram_write(uint16 addr, uint8 data);

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  void oam_dma();

  void serialize(serializer&);

//internal:
  bool trace;
};

extern CPU cpu;
