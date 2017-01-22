struct HitachiDSP : Processor::HG51B, Thread {
  MappedRAM rom;
  MappedRAM ram;

  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;

  //HG51B read/write
  auto read(uint24 addr) -> uint8 override;
  auto write(uint24 addr, uint8 data) -> void override;

  //CPU ROM read/write
  auto romRead(uint24 addr, uint8 data) -> uint8;
  auto romWrite(uint24 addr, uint8 data) -> void;

  //CPU RAM read/write
  auto ramRead(uint24 addr, uint8 data) -> uint8;
  auto ramWrite(uint24 addr, uint8 data) -> void;

  //HG51B data RAM read/write
  auto dramRead(uint24 addr, uint8 data) -> uint8;
  auto dramWrite(uint24 addr, uint8 data) -> void;

  //CPU MMIO read/write
  auto dspRead(uint24 addr, uint8 data) -> uint8;
  auto dspWrite(uint24 addr, uint8 data) -> void;

  auto firmware() const -> vector<uint8>;
  auto serialize(serializer&) -> void;

  uint Frequency;
  uint Roms;

  struct MMIO {
    bool dma;  //true during DMA transfers

    uint24 dmaSource;       //$1f40-$1f42
    uint24 dmaLength;       //$1f43-$1f44
    uint24 dmaTarget;       //$1f45-$1f47
    uint8  r1f48;           //$1f48
    uint24 programOffset;   //$1f49-$1f4b
    uint8  r1f4c;           //$1f4c
    uint16 pageNumber;      //$1f4d-$1f4e
    uint8  programCounter;  //$1f4f
    uint8  r1f50;           //$1f50
    uint8  r1f51;           //$1f51
    uint8  r1f52;           //$1f52
    uint8  vector[32];      //$1f60-$1f7f
  } mmio;
};

extern HitachiDSP hitachidsp;
