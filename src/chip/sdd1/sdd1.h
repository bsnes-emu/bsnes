#include "sdd1emu.h"

class SDD1 : public MMIO {
public:
SDD1emu sdd1emu;

struct {
  uint32 index[4];      //memory mapping registers
  uint8  buffer[65536]; //pointer to decompressed S-DD1 data,
                        //max. DMA length is 65536
  uint16 buffer_index;  //DMA read index into S-DD1 decompression buffer
  uint16 buffer_size;
  bool   active[8];     //true when DMA channel should pass through S-DD1
  bool   dma_active;
} sdd1;
  void   init();
  void   enable();
  void   power();
  void   reset();
  uint32 offset(uint32 addr);

  void   dma_begin(uint8 channel, uint32 addr, uint16 length);
  bool   dma_active();
  uint8  dma_read();

  uint8  mmio_read (uint16 addr);
  void   mmio_write(uint16 addr, uint8 data);

  SDD1();
};

extern SDD1 sdd1;
