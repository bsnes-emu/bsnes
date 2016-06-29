struct PPU : Thread, MMIO {
  auto enabled() const -> bool;

  static auto Enter() -> void;
  auto main() -> void;
  auto mode(uint) -> void;
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
    bool irq;  //STAT IRQ line
    uint lx;

    //$ff40  LCDC
    bool displayEnable;
    bool windowTilemapSelect;
    bool windowDisplayEnable;
    bool bgTiledataSelect;
    bool bgTilemapSelect;
    bool obSize;
    bool obEnable;
    bool bgEnable;

    //$ff41  STAT
    bool interruptLYC;
    bool interruptOAM;
    bool interruptVblank;
    bool interruptHblank;
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
    bool dmaActive;
    uint dmaClock;
    uint8 dmaBank;

    //$ff4a  WY
    uint8 wy;

    //$ff4b  WX
    uint8 wx;

    //$ff4f  VBK
    bool vramBank;

    //$ff68  BGPI
    bool bgpiIncrement;
    uint6 bgpi;

    //$ff6a  OBPI
    bool obpiIncrement;
    uint8 obpi;
  } status;

  uint32 screen[160 * 144];

  struct Pixel {
    uint16 color;
    uint8 palette;
    bool priority;
  };
  Pixel bg;
  Pixel ob;

  struct Sprite {
    uint x;
    uint y;
    uint tile;
    uint attr;
    uint data;
  };
  Sprite sprite[10];
  uint sprites;

  uint px;

  struct Background {
    uint attr;
    uint data;
  };
  Background background;
  Background window;
};

extern PPU ppu;
