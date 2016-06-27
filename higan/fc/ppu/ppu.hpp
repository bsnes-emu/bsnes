struct PPU : Thread {
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

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
  auto loadCHR(uint16 addr) -> uint8;

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

    union {
      uint value;
      BitField<uint, 0, 4> tileX;
      BitField<uint, 5, 9> tileY;
      BitField<uint,10,11> nametable;
      BitField<uint,   10> nametableX;
      BitField<uint,   11> nametableY;
      BitField<uint,12,14> fineY;
      BitField<uint, 0,14> address;
      BitField<uint, 0, 7> addressLo;
      BitField<uint, 8,14> addressHi;
      BitField<uint,   15> latch;
      BitField<uint,16,18> fineX;
    } v, t;

    bool nmiHold;
    bool nmiFlag;

    //$2000
    uint vramIncrement;
    uint spriteAddress;
    uint bgAddress;
    uint spriteHeight;
    bool masterSelect;
    bool nmiEnable;

    //$2001
    bool grayscale;
    bool bgEdgeEnable;
    bool spriteEdgeEnable;
    bool bgEnable;
    bool spriteEnable;
    uint3 emphasis;

    //$2002
    bool spriteOverflow;
    bool spriteZeroHit;

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
