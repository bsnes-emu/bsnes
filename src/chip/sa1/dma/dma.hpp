struct DMA {
  enum CDEN { DmaNormal = 0, DmaCharConversion = 1 };
  enum SD { SourceROM = 0, SourceBWRAM = 1, SourceIRAM = 2 };
  enum DD { DestIRAM = 0, DestBWRAM = 1 };

  enum Mode { Inactive, Normal, CC1, CC2 } mode;
  unsigned clocks;
  bool tile;
  unsigned line;
} dma;

void dma_normal();
void dma_cc1();
uint8_t dma_cc1_read(unsigned addr);
void dma_cc2();
