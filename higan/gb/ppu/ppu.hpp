struct PPU : Thread, MMIO {
  static auto Enter() -> void;
  auto main() -> void;
  auto stat() -> void;
  auto coincidence() -> bool;
  auto refresh() -> void;
  auto step(uint clocks) -> void;

  auto hflip(uint data) const -> uint;

  //io.cpp
  auto vramAddress(uint16 addr) const -> uint;
  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

  //dmg.cpp
  auto readTileDMG(bool select, uint x, uint y, uint& data) -> void;
  auto scanlineDMG() -> void;
  auto runDMG() -> void;
  auto runBackgroundDMG() -> void;
  auto runWindowDMG() -> void;
  auto runObjectsDMG() -> void;

  //cgb.cpp
  auto readTileCGB(bool select, uint x, uint y, uint& attr, uint& data) -> void;
  auto scanlineCGB() -> void;
  auto runCGB() -> void;
  auto runBackgroundCGB() -> void;
  auto runWindowCGB() -> void;
  auto runObjectsCGB() -> void;

  auto power() -> void;

  auto serialize(serializer&) -> void;

  uint8 vram[16384];  //GB = 8192, GBC = 16384
  uint8 oam[160];
  uint8 bgp[4];
  uint8 obp[2][4];
  uint8 bgpd[64];
  uint8 obpd[64];

  function<auto () -> void> scanline;
  function<auto () -> void> run;

  struct Status {
    bool irq = 0;  //STAT IRQ line
    uint lx = 0;

    //$ff40  LCDC
    bool displayEnable = 0;
    bool windowTilemapSelect = 0;
    bool windowDisplayEnable = 0;
    bool bgTiledataSelect = 0;
    bool bgTilemapSelect = 0;
    bool obSize = 0;
    bool obEnable = 0;
    bool bgEnable = 0;

    //$ff41  STAT
    bool interruptLYC = 0;
    bool interruptOAM = 0;
    bool interruptVblank = 0;
    bool interruptHblank = 0;
    uint2 mode;

    //$ff42  SCY
    uint8 scy;

    //$ff43  SCX
    uint8 scx;

    //$ff44  LY
    uint8 ly;

    //$ff45  LYC
    uint8 lyc;

    //$ff46  DMA
    bool dmaActive = 0;
    uint dmaClock = 0;
    uint8 dmaBank;

    //$ff4a  WY
    uint8 wy;

    //$ff4b  WX
    uint8 wx;

    //$ff4f  VBK
    bool vramBank = 0;

    //$ff68  BGPI
    bool bgpiIncrement = 0;
    uint6 bgpi;

    //$ff6a  OBPI
    bool obpiIncrement = 0;
    uint8 obpi;
  } status;

  uint32 screen[160 * 144];

  struct Pixel {
    uint16 color;
    uint8 palette;
    bool priority = 0;
  };
  Pixel bg;
  Pixel ob;

  struct Sprite {
    uint x = 0;
    uint y = 0;
    uint tile = 0;
    uint attr = 0;
    uint data = 0;
  };
  Sprite sprite[10];
  uint sprites = 0;

  uint px = 0;

  struct Background {
    uint attr = 0;
    uint data = 0;
  };
  Background background;
  Background window;
};

extern PPU ppu;
