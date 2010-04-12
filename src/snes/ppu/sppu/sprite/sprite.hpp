class Sprite {
public:
  sPPU &self;

  struct TileItem {
    uint16 x;
    uint16 y;
    uint16 priority;
    uint16 palette;
    uint16 tile;
    bool hflip;
  };

  struct SpriteItem {
    uint8 width;
    uint8 height;
    uint16 x;
    uint16 y;
    uint8 character;
    bool nameselect;
    bool vflip;
    bool hflip;
    uint8 priority;
    uint8 palette;
  } list[128];

  struct State {
    unsigned x;
    unsigned y;

    unsigned item_count;
    unsigned tile_count;
    uint8 output_palette[256];
    uint8 output_priority[256];
    uint8 item_list[32];
    TileItem tile_list[34];
    unsigned active_sprite;
  } t;

  struct {
    bool main_enabled;
    bool sub_enabled;
    bool interlace;

    uint8 base_size;
    uint8 nameselect;
    uint16 tiledata_addr;
    uint8 first_sprite;

    unsigned priority0;
    unsigned priority1;
    unsigned priority2;
    unsigned priority3;

    bool time_over;
    bool range_over;
  } regs;

  struct {
    struct {
      unsigned priority;  //0 = none (transparent)
      unsigned palette;
    } main, sub;
  } output;

  void address_reset();
  void frame();
  void scanline();
  void run();
  void reset();

  void serialize(serializer&);
  Sprite(sPPU &self);

private:
  bool on_scanline();
  void load_tiles();
  void render_tile(unsigned tile);
};
