struct PPU : Thread {
  inline auto rate() const -> uint { return Region::PAL() ? 5 : 4; }
  inline auto vlines() const -> uint { return Region::PAL() ? 312 : 262; }

  //ppu.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto step(uint clocks) -> void;

  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;

  auto power(bool reset) -> void;

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
    uint lx = 0;
    uint ly = 0;

    uint8 busData;

    union Union {
      auto& operator=(const Union& u) { value = u.value; return *this; }
      uint value = 0;
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

    bool nmiHold = 0;
    bool nmiFlag = 0;

    //$2000
    uint vramIncrement = 1;
    uint spriteAddress = 0;
    uint bgAddress = 0;
    uint spriteHeight = 0;
    bool masterSelect = 0;
    bool nmiEnable = 0;

    //$2001
    bool grayscale = 0;
    bool bgEdgeEnable = 0;
    bool spriteEdgeEnable = 0;
    bool bgEnable = 0;
    bool spriteEnable = 0;
    uint3 emphasis;

    //$2002
    bool spriteOverflow = 0;
    bool spriteZeroHit = 0;

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

    uint oamIterator = 0;
    uint oamCounter = 0;

    OAM oam[8];   //primary
    OAM soam[8];  //secondary
  } latch;

  uint8 ciram[2048];
  uint8 cgram[32];
  uint8 oam[256];

  uint32 buffer[256 * 262];
};

extern PPU ppu;
