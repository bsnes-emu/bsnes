class PPU : public Processor, public PPUcounter, public MMIO {
public:
  enum : bool { Threaded = true };
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();

  void latch_counters();
  bool interlace() const;
  bool overscan() const;
  bool hires() const;

  uint8 mmio_read(unsigned addr);
  void mmio_write(unsigned addr, uint8 data);

  void enter();
  void power();
  void reset();

  void serialize(serializer&);
  PPU();
  ~PPU();

private:
  enum : unsigned { BG1 = 0, BG2 = 1, BG3 = 2, BG4 = 3, OAM = 4, COL = 5, BACK = 5 };
  uint16 *surface;
  uint16 *output;

  //background.cpp

  //memory.cpp
  uint16 get_vram_addr();
  uint8 vram_read(unsigned addr);
  void vram_write(unsigned addr, uint8 data);
  uint8 oam_read(unsigned addr);
  void oam_write(unsigned addr, uint8 data);
  uint8 cgram_read(unsigned addr);
  void cgram_write(unsigned addr, uint8 data);

  //mmio.cpp
  struct Regs {
    //internal
    unsigned width;
    unsigned height;

    //$2100
    bool display_disabled;
    unsigned display_brightness;

    //$2101
    unsigned oam_basesize;
    unsigned oam_nameselect;
    uint16 oam_tdaddr;

    //$2102-$2103
    uint16 oam_baseaddr;
    uint16 oam_addr;
    bool oam_priority;
    unsigned oam_firstsprite;

    //$2104
    uint8 oam_latchdata;

    //$2105
    bool bg_tilesize[4];
    bool bg3_priority;
    unsigned bg_mode;

    //$2106
    unsigned mosaic_size;
    bool mosaic_enabled[4];

    //$2107-$210a
    uint16 bg_scaddr[4];
    unsigned bg_scsize[4];

    //$210b-$210c
    uint16 bg_tdaddr[4];

    //$2115
    bool vram_incmode;
    unsigned vram_mapping;
    unsigned vram_incsize;

    //$2116-$2117
    uint16 vram_addr;

    //$2121
    uint16 cgram_addr;

    //$2122
    uint8 cgram_latchdata;

    //$2139-$213a
    uint16 vram_readbuffer;
  } regs;

  void mmio_reset();

  //ppu.cpp
  static void Enter();
  void add_clocks(unsigned clocks);
  void scanline();
  void frame();
  void render_scanline();

  //screen.cpp
  uint16 **light_table;
  void screen_render();

  friend class Video;
};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern PPUDebugger ppu;
#else
  extern PPU ppu;
#endif
