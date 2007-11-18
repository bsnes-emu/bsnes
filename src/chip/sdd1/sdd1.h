#include "sdd1emu.h"

class SDD1 : public MMIO {
public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 mmio_read (uint addr);
  void  mmio_write(uint addr, uint8 data);

  void  dma_begin(uint8 channel, uint32 addr, uint16 length);
  bool  dma_active();
  uint8 dma_read();

  SDD1();

private:
  SDD1emu sdd1emu;

  struct {
    uint8  buffer[65536]; //pointer to decompressed S-DD1 data, max DMA length is 65536
    uint16 buffer_index;  //DMA read index into S-DD1 decompression buffer
    uint16 buffer_size;
    bool   dma_active;
  } sdd1;

  struct {
    uint8 r4800;
    uint8 r4801;
    uint8 r4804;
    uint8 r4805;
    uint8 r4806;
    uint8 r4807;
  } regs;
};

extern SDD1 sdd1;
