struct SDD1 {
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto read(uint addr) -> uint8;
  auto write(uint addr, uint8 data) -> void;

  auto mmc_read(uint addr) -> uint8;

  auto mcurom_read(uint addr) -> uint8;
  auto mcurom_write(uint addr, uint8 data) -> void;

  auto mcuram_read(uint addr) -> uint8;
  auto mcuram_write(uint addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  MappedRAM rom;
  MappedRAM ram;

private:
  uint8 sdd1_enable;  //channel bit-mask
  uint8 xfer_enable;  //channel bit-mask
  bool dma_ready;     //used to initialize decompression module
  uint mmc[4];        //memory map controller ROM indices

  struct {
    uint addr;        //$43x2-$43x4 -- DMA transfer address
    uint16 size;      //$43x5-$43x6 -- DMA transfer size
  } dma[8];

public:
  #include "decomp.hpp"
  Decomp decomp;
};

extern SDD1 sdd1;
