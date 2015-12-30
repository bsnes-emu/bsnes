class Sprite {
  Sprite(PPU& self);

  auto frame() -> void;
  auto update_list(uint addr, uint8 data) -> void;
  auto address_reset() -> void;
  auto set_first() -> void;
  alwaysinline auto on_scanline(uint sprite) -> bool;
  auto render() -> void;

  auto serialize(serializer&) -> void;

  PPU& self;

  bool priority0_enable;
  bool priority1_enable;
  bool priority2_enable;
  bool priority3_enable;

  struct Regs {
    uint priority0;
    uint priority1;
    uint priority2;
    uint priority3;

    uint base_size;
    uint nameselect;
    uint tiledata_addr;
    uint first_sprite;

    bool main_enable;
    bool sub_enable;

    bool interlace;

    bool time_over;
    bool range_over;
  } regs;

  struct List {
    uint width;
    uint height;
    uint x;
    uint y;
    uint character;
    bool use_nameselect;
    bool vflip;
    bool hflip;
    uint palette;
    uint priority;
    bool size;
  } list[128];
  bool list_valid;

  uint8 itemlist[32];
  struct TileList {
    uint x;
    uint y;
    uint priority;
    uint palette;
    uint tile;
    bool hflip;
  } tilelist[34];

  struct Output {
    uint8 palette[256];
    uint8 priority[256];
  } output;

  LayerWindow window;

  friend class PPU;
};
