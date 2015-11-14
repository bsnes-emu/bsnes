struct DMA {
  enum CDEN : uint { DmaNormal = 0, DmaCharConversion = 1 };
  enum SD : uint { SourceROM = 0, SourceBWRAM = 1, SourceIRAM = 2 };
  enum DD : uint { DestIRAM = 0, DestBWRAM = 1 };
  uint line;
} dma;

auto dma_normal() -> void;
auto dma_cc1() -> void;
auto dma_cc1_read(uint addr) -> uint8;
auto dma_cc2() -> void;
