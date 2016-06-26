struct PPU : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto tick() -> void;

  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;

  auto power() -> void;
  auto reset() -> void;

  //memory.cpp
  auto readCIRAM(uint11 addr) -> uint8;
  auto writeCIRAM(uint11 addr, uint8 data) -> void;

  auto readCGRAM(uint5 addr) -> uint8;
  auto writeCGRAM(uint5 addr, uint8 data) -> void;

  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

  //render.cpp
  auto enable() const -> bool;
  auto nametableAddress() const -> uint;
  auto scrollX() const -> uint;
  auto scrollY() const -> uint;

  auto loadCHR(uint16 addr) -> uint8;

  auto scrollX_increment() -> void;
  auto scrollY_increment() -> void;

  auto renderPixel() -> void;
  auto renderSprite() -> void;
  auto renderScanline() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Registers {
    //internal
    uint8 mdr;

    bool field;
    uint lx;
    uint ly;

    uint8 busData;

    bool addressLatch;

    uint15 vaddr;
    uint15 taddr;
    uint8 xaddr;

    bool nmiHold;
    bool nmiFlag;

    //$2000
    bool nmiEnable;
    bool masterSelect;
    uint spriteHeight;
    uint bgAddress;
    uint spriteAddress;
    uint vramIncrement;

    //$2001
    uint3 emphasis;
    bool spriteEnable;
    bool bgEnable;
    bool spriteEdgeEnable;
    bool bgEdgeEnable;
    bool grayscale;

    //$2002
    bool spriteZeroHit;
    bool spriteOverflow;

    //$2003
    uint8 oamAddress;
  } r;

  struct OAM {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint8 id = 64;
    uint8 y = 0xff;
    uint8 tile = 0xff;
    uint8 attr = 0xff;
    uint8 x = 0xff;

    uint8 tiledataLo = 0;
    uint8 tiledataHi = 0;
  };

  struct Latches {
    uint16 nametable;
    uint16 attribute;
    uint16 tiledataLo;
    uint16 tiledataHi;

    uint oamIterator;
    uint oamCounter;

    OAM oam[8];   //primary
    OAM soam[8];  //secondary
  } l;

  uint8 ciram[2048];
  uint8 cgram[32];
  uint8 oam[256];

  uint32 buffer[256 * 262];
};

extern PPU ppu;
