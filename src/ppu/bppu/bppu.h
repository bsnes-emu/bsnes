//#define _BPPU_OLDRENDER

class bPPU;

class bPPUMMIO : public MMIO {
public:
bPPU *ppu;
  uint8 read (uint32 addr);
  void  write(uint32 addr, uint8 value);
  bPPUMMIO(bPPU *_ppu);
};

class bPPU : public PPU {
public:
uint8 *vram, *oam, *cgram;

enum { BG1 = 0, BG2 = 1, BG3 = 2, BG4 = 3, OAM = 4, BACK = 5 };
enum { SC_32x32 = 0, SC_32x64 = 1, SC_64x32 = 2, SC_64x64 = 3 };

struct {
//$2100
  bool   display_disabled;
  uint8  display_brightness;

//$2101
  uint8  oam_basesize;
  uint8  oam_nameselect;
  uint16 oam_tdaddr;

//$2102-$2103
  uint8  oam_addrl, oam_addrh;
  uint16 oam_addr;
  uint8  oam_latchdata;

//$2105
  bool   bg_tilesize[4];
  bool   bg3_priority;
  uint8  bg_mode;

//$2106
  uint8  mosaic_size;
  bool   mosaic_enabled[4];

//$2107-$210a
  uint16 bg_scaddr[4];
  uint8  bg_scsize[4];

//$210b-$210c
  uint16 bg_tdaddr[4];

//$210d-$2114
  uint16 bg_hofs[4];
  uint16 bg_vofs[4];

//$2115
  bool   vram_incmode;
  uint8  vram_mapping;
  uint8  vram_incsize;

//$2116-$2117
  uint16 vram_addr;

//$211a
  uint8  mode7_repeat;
  bool   mode7_vflip;
  bool   mode7_hflip;

//$211b-$2120
  uint16 m7a, m7b, m7c, m7d, m7x, m7y;

//$2121
  uint16 cgram_addr;

//$2123-$2125
  bool   bg_window1_enabled[5];
  bool   bg_window1_invert [5];
  bool   bg_window2_enabled[5];
  bool   bg_window2_invert [5];
  bool   color_window1_enabled, color_window1_invert;
  bool   color_window2_enabled, color_window2_invert;

//$2126-$2129
  uint8  window1_left, window1_right;
  uint8  window2_left, window2_right;

//$212a-$212b
  uint8  bg_window_mask[5];
  uint8  color_window_mask;

//$212c-$212d
  bool   bg_enabled[5], bgsub_enabled[5];

//$212e-$212f
  bool   bg_window_enabled[5], bgsub_window_enabled[5];

//$2130
  uint8  color_mask, colorsub_mask;
  bool   addsub_mode;

//$2131
  bool   color_mode, color_halve;
  bool   bg_color_enabled[6];

//$2132
  uint8  color_r, color_g, color_b;

//$2133
  bool   mode7_extbg;
  bool   overscan;
  uint16 scanlines;
  bool   oam_halve;
  bool   interlace;

//$2137
  uint16 hcounter, vcounter;
  bool   latch_hcounter, latch_vcounter;
  bool   counters_latched;

//$2139-$213a
  uint16 vram_readbuffer;
}regs;
  uint8  vram_read  (uint16 addr);
  void   vram_write (uint16 addr, uint8 value);
  uint8  oam_read   (uint16 addr);
  void   oam_write  (uint16 addr, uint8 value);
  uint8  cgram_read (uint16 addr);
  void   cgram_write(uint16 addr, uint8 value);

  uint16 get_vram_address();

  void   mmio_w2100(uint8 value); //INIDISP
  void   mmio_w2101(uint8 value); //OBSEL
  void   mmio_w2102(uint8 value); //OAMADDL
  void   mmio_w2103(uint8 value); //OAMADDH
  void   mmio_w2104(uint8 value); //OAMDATA
  void   mmio_w2105(uint8 value); //BGMODE
  void   mmio_w2106(uint8 value); //MOSAIC
  void   mmio_w2107(uint8 value); //BG1SC
  void   mmio_w2108(uint8 value); //BG2SC
  void   mmio_w2109(uint8 value); //BG3SC
  void   mmio_w210a(uint8 value); //BG4SC
  void   mmio_w210b(uint8 value); //BG12NBA
  void   mmio_w210c(uint8 value); //BG34NBA
  void   mmio_w210d(uint8 value); //BG1HOFS
  void   mmio_w210e(uint8 value); //BG1VOFS
  void   mmio_w210f(uint8 value); //BG2HOFS
  void   mmio_w2110(uint8 value); //BG2VOFS
  void   mmio_w2111(uint8 value); //BG3HOFS
  void   mmio_w2112(uint8 value); //BG3VOFS
  void   mmio_w2113(uint8 value); //BG4HOFS
  void   mmio_w2114(uint8 value); //BG4VOFS
  void   mmio_w2115(uint8 value); //VMAIN
  void   mmio_w2116(uint8 value); //VMADDL
  void   mmio_w2117(uint8 value); //VMADDH
  void   mmio_w2118(uint8 value); //VMDATAL
  void   mmio_w2119(uint8 value); //VMDATAH
  void   mmio_w211a(uint8 value); //M7SEL
  void   mmio_w211b(uint8 value); //M7A
  void   mmio_w211c(uint8 value); //M7B
  void   mmio_w211d(uint8 value); //M7C
  void   mmio_w211e(uint8 value); //M7D
  void   mmio_w211f(uint8 value); //M7X
  void   mmio_w2120(uint8 value); //M7Y
  void   mmio_w2121(uint8 value); //CGADD
  void   mmio_w2122(uint8 value); //CGDATA
  void   mmio_w2123(uint8 value); //W12SEL
  void   mmio_w2124(uint8 value); //W34SEL
  void   mmio_w2125(uint8 value); //WOBJSEL
  void   mmio_w2126(uint8 value); //WH0
  void   mmio_w2127(uint8 value); //WH1
  void   mmio_w2128(uint8 value); //WH2
  void   mmio_w2129(uint8 value); //WH3
  void   mmio_w212a(uint8 value); //WBGLOG
  void   mmio_w212b(uint8 value); //WOBJLOG
  void   mmio_w212c(uint8 value); //TM
  void   mmio_w212d(uint8 value); //TS
  void   mmio_w212e(uint8 value); //TMW
  void   mmio_w212f(uint8 value); //TSW
  void   mmio_w2130(uint8 value); //CGWSEL
  void   mmio_w2131(uint8 value); //CGADDSUB
  void   mmio_w2132(uint8 value); //COLDATA
  void   mmio_w2133(uint8 value); //SETINI
  uint8  mmio_r2134();            //MPYL
  uint8  mmio_r2135();            //MPYM
  uint8  mmio_r2136();            //MPYH
  uint8  mmio_r2137();            //SLHV
  uint8  mmio_r2138();            //OAMDATAREAD
  uint8  mmio_r2139();            //VMDATALREAD
  uint8  mmio_r213a();            //VMDATAHREAD
  uint8  mmio_r213b();            //CGDATAREAD
  uint8  mmio_r213c();            //OPHCT
  uint8  mmio_r213d();            //OPVCT
  uint8  mmio_r213e();            //STAT77
  uint8  mmio_r213f();            //STAT78

  void   latch_counters();

/* PPU render functions */

#ifdef _BPPU_OLDRENDER
#include "bppu_old_render.h"
#else
#include "bppu_render.h"
#endif

uint16 *light_table;
uint16 *mosaic_table[16];
  void   render_line();

/* Required functions */
  void   run();
  void   scanline();
  void   frame();
  void   power();
  void   reset();

  bPPU();
  ~bPPU();
};
