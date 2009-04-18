#ifdef SA1_CPP

//==========================
//SA-1 opcode core functions
//==========================

void SA1::op_io() {
  add_clocks(2);
  if(regs.wai) scheduler.sync_copcpu();
  cycle_edge();
}

uint8_t SA1::op_read(unsigned addr) {
  add_clocks(bus_speed(addr));
  scheduler.sync_copcpu();
  regs.mdr = sa1bus.read(addr);
  cycle_edge();
  return regs.mdr;
}

void SA1::op_write(unsigned addr, uint8_t data) {
  add_clocks(bus_speed(addr));
  scheduler.sync_copcpu();
  sa1bus.write(addr, regs.mdr = data);
  cycle_edge();
}

void SA1::cycle_edge() {
  switch(dma.mode) {
    case DMA::Normal: dma_normal(); break;
    case DMA::CC1:    dma_cc1();    break;
    case DMA::CC2:    dma_cc2();    break;
  }
}

//$[00-3f:80-bf]:[8000-ffff]
//$[c0-ff]      :[0000-ffff]
#define ROM(n) ( \
   ((n & 0x408000) == 0x008000) \
|| ((n & 0xc00000) == 0xc00000) \
)

//$[00-3f|80-bf]:[0000-07ff]
//$[00-3f|80-bf]:[3000-37ff]
#define IRAM(n) ( \
   ((n & 0x40f800) == 0x000000) \
|| ((n & 0x40f800) == 0x003000) \
)

unsigned SA1::bus_speed(unsigned addr) {
  if(IRAM(addr)) return 2;
  if(ROM(addr)) return (ROM(cpu.regs.bus) ? 4 : 2);
  return 4;  //MMIO, BW-RAM
}

#undef ROM
#undef IRAM

#endif
