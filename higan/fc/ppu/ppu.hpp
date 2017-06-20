struct PPU : Thread {
  inline auto vlines() const -> uint { return Region::NTSC() ? 262 : 312; }

  //ppu.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;

  auto power() -> void;

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

  struct IO {
    //internal
    uint8 mdr;

    uint1 field;
    uint lx;
    uint ly;

    uint8 busData;

    union {
      uint value;
      NaturalBitField<uint, 0, 4> tileX;
      NaturalBitField<uint, 5, 9> tileY;
      NaturalBitField<uint,10,11> nametable;
      NaturalBitField<uint,10,10> nametableX;
      NaturalBitField<uint,11,11> nametableY;
      NaturalBitField<uint,12,14> fineY;
      NaturalBitField<uint, 0,14> address;
      NaturalBitField<uint, 0, 7> addressLo;
      NaturalBitField<uint, 8,14> addressHi;
      NaturalBitField<uint,15,15> latch;
      NaturalBitField<uint,16,18> fineX;
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
  } io;

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
  } latch;

  uint8 ciram[2048];
  uint8 cgram[32];
  uint8 oam[256];

  uint32 buffer[256 * 262];
};

extern PPU ppu;
