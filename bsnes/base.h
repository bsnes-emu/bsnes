//#define PUBLIC_DOMAIN

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define null 0xffffffff
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  ulong;
typedef void (*vfunc)(void);

#define SH_2   1
#define SH_4   2
#define SH_8   3
#define SH_16  4
#define SH_32  5
#define SH_64  6
#define SH_128 7
#define SH_256 8

/*************************
 *** general functions ***
 ************************/

void alert(char *s, ...);

/************************
 *** joypad functions ***
 ***********************/

void UpdateJoypad(void);

typedef struct {
  byte a, b, x, y;
  byte l, r;
  byte select, start;
  byte up, down, left, right;

  byte read_pos;
}joypad_state;

/******************
 *** deprecated ***
 *****************/

typedef struct {
  char  rom_name[4096], sram_name[4096];
  ulong sram_save_tick_count;
}emustate;

/***********************
 *** video functions ***
 **********************/

void video_setmode(bool fullscreen, word width, word height);
void video_setsnesmode(void);

//global export: render
typedef struct {
  word  width, height;
  word  snes_width, snes_height;
  bool  fullscreen;
  bool  show_menu;

  byte  frame_skip;
  byte  frame_count;

  bool  bg1_enabled[3], bg2_enabled[3], bg3_enabled[3], bg4_enabled[3], oam_enabled[5];
}videostate;

/***************************
 *** debugging functions ***
 **************************/

#define DEBUGMODE_NOROM      0
#define DEBUGMODE_DISABLED   1
#define DEBUGMODE_WAIT       2
#define DEBUGMODE_RUN        3
#define DEBUGMODE_STEP       4
#define DEBUGMODE_PROCEED    5

#define DEBUG_BGENABLED_ALL  0
#define DEBUG_BGENABLED_PRI0 1
#define DEBUG_BGENABLED_PRI1 2
#define DEBUG_BGENABLED_PRI2 3
#define DEBUG_BGENABLED_PRI3 4

void debug_set_state(byte state);
#define debug_get_state() debugger.mode
void dprintf(char *s, ...);
void debug_refresh_mem(void);
void debug_refresh_bp(void);
void debug_update_cycles(void);

typedef struct {
  byte mode;

  bool  trace_enabled;
  ulong mem_ptr;     //position of wram ptr for debug window
  bool  disas_op;
  bool  refresh_mem;
  bool  refresh_bp;

  FILE *trace_fp;
}debugstate;

/*********************
 *** ppu functions ***
 ********************/

#define BG1     0x00
#define BG2     0x01
#define BG3     0x02
#define BG4     0x03
#define OAM     0x04
#define SS_BG1  0x80
#define SS_BG2  0x81
#define SS_BG3  0x82
#define SS_BG4  0x83
#define SS_OAM  0x84
#define BACK    0x05

#define CLIPMODE_IN  0
#define CLIPMODE_OUT 1

#define WINDOWMASK_OR   0
#define WINDOWMASK_AND  1
#define WINDOWMASK_XOR  2
#define WINDOWMASK_XNOR 3

#define COLORMODE_ADD 0
#define COLORMODE_SUB 1

typedef struct {
  word *screen;            //internal buffer used to render 512x448 screen
  byte *vram;              //65536 bytes
  byte *cgram;             //512 bytes
  byte *oam;               //544 bytes mirrored as 1024 bytes (512-543 are mirrored to 544-1023)
  word *light_table;       //16 * 32768 -- applies display_brightness to bgr555 color data
//ppu.mosaic_table[ppu.mosaic_size][x] == (x / (ppu.mosaic_size + 1)) * (ppu.mosaic_size + 1)
  word *mosaic_table[16];

  ulong ppu_cycles, ppu_prev_cycles; //used in ppu_update_scanline()

  bool display_disable;    //$2100 bit 7
  byte display_brightness; //$2100 bits 0-3

  byte visible_scanlines;  //$2133 bit 2 (NTSC/PAL mode) -- value = 224 or 240
  byte toggle_visible_scanlines; //do not allow change midframe

  bool interlace;
  byte interlace_frame;    //0 or 1, used to alternate between scanlines rendered

  bool bg_enabled[5];
  bool ss_bg_enabled[5];
  byte mosaic_size;
  bool mosaic_enabled[4];
  word bg_tilemap_loc[4];
  byte bg_tile_size[4];
  byte bg_tilemap_size[4];
  word bg_tiledata_loc[4];
  word oam_tiledata_loc;
  byte bg_priority_mode;
  byte oam_base_size;
  byte oam_name_sel;
  byte bg_mode;

  word hline_pos;          //0-255: inside vblank, 256-339: outside vblank
  word vline_pos;          //0-223: inside vblank, 224-261: outside vblank, 180 cycles/scanline
  bool irq_triggered;      //for $4211 read
  bool virq_triggered;     //prevent recursive calling
  bool hirq_triggered;     //prevent recursive calling
  word vram_write_pos;     //holds value at $2116
  word vram_inc_size;      //amount to increment vram_write_pos by
  byte vram_inc_reg;       //increment on 2118 (0) or 2119 (1)
  word cgram_write_pos;    //holds value at $2121 (multiplied by 2)
  word oam_write_pos;      //holds value at $2102/$2103 (multiplied by 2)
  ulong wram_write_pos;    //holds value at $2181-$2183

  word bg_hscroll_pos[4];  //$210d-$2114
  word bg_vscroll_pos[4];
  word m7hofs, m7vofs;

  byte mul_a, mul_b;
  word div_a;
  byte div_b;
  word r_4214, r_4216;

  word  smul_a;
  byte  smul_b;
  ulong smul_r;

  bool bg_window1_enabled[5];
  bool bg_window2_enabled[5];
  byte bg_window1_clipmode[5];
  byte bg_window2_clipmode[5];

  byte window1_left, window1_right;
  byte window2_left, window2_right;

  bool bg_windowing_enabled[5];
  bool ss_bg_windowing_enabled[5];
  byte bg_window_mask[5];

  bool color_window1_enabled,  color_window2_enabled;
  bool color_window1_clipmode, color_window2_clipmode;
  byte color_window_mask;
  byte color_mask, ss_color_mask;
  byte addsub_mode;
  byte color_mode, color_halve;
  bool bg_color_enabled[6];
  byte color_r, color_g, color_b;

  byte toggle_active_hdma_channels;
  byte active_hdma_channels;
  word hdma_scanlines_remaining[8];
  word hdma_index_pointer[8];
  bool hdma_completed[8];

  bool vcounter_enabled, hcounter_enabled;
  word hirq_pos, virq_pos;

  bool auto_joypad_read;
  byte joypad_strobe_value;

  byte latch_toggle;
  word latch_vpos, latch_hpos;

  word m7a, m7b, m7c, m7d, m7x, m7y;

  bool mode7_vflip, mode7_hflip;
}ppustate;

/*********************
 *** cpu functions ***
 ********************/

#define MEMSPEED_SLOWROM 0
#define MEMSPEED_FASTROM 1

#define PF_N 0x80
#define PF_V 0x40
#define PF_M 0x20
#define PF_X 0x10
#define PF_D 0x08
#define PF_I 0x04
#define PF_Z 0x02
#define PF_C 0x01

#define BP_OFF   0
#define BP_READ  1
#define BP_WRITE 2
#define BP_EXEC  4
#define BP_VAL   8

/************************
 *** memory functions ***
 ***********************/

#define MEMMAP_HIROM 0x01
#define MEMMAP_LOROM 0x02

#define MEMACCESS_NORMAL   0
#define MEMACCESS_CPU      1
#define MEMACCESS_DEBUGGER 1

enum {
  MEMMODE_NONE = 0,  //(no address translation)
  MEMMODE_DP,        //dp
  MEMMODE_DPX,       //dp,x
  MEMMODE_DPY,       //dp,y
  MEMMODE_IDP,       //(dp)
  MEMMODE_IDPX,      //(dp,x)
  MEMMODE_IDPY,      //(dp),y
  MEMMODE_ILDP,      //[dp]
  MEMMODE_ILDPY,     //[dp],y
  MEMMODE_ADDR,      //addr
  MEMMODE_ADDRX,     //addr,x
  MEMMODE_ADDRY,     //addr,y
  MEMMODE_IADDRX,    //(addr,x)
  MEMMODE_ILADDR,    //[addr]
  MEMMODE_LONG,      //long
  MEMMODE_LONGX,     //long, x
  MEMMODE_SR,        //sr,s
  MEMMODE_ISRY,      //(sr,s),y
//exchanges pbr for dbr, disables address mirroring
  MEMMODE_ADDR_PC,   //addr
  MEMMODE_IADDR_PC,  //(addr)
  MEMMODE_IADDRX_PC, //(addr,x)
  MEMMODE_ILADDR_PC  //[addr]
};

#define MEMSIZE_BYTE 1
#define MEMSIZE_WORD 2
#define MEMSIZE_LONG 3

/***********************
 *** misc. functions ***
 **********************/

//main.cpp
void InitSNES(void);
extern vfunc RunSNES;
void RunSNES_NoDebug(void);
void RunSNES_Debug(void);

//cpu/g65816_ops_stack.cpp
ulong g65816_stackread(byte size);
void g65816_stackwrite(byte size, ulong value);

//cpu/d65816.cpp
void disas_g65816_op(void);

//ppu/mmio.cpp
byte mmio_read(word addr);
void mmio_write(word addr, byte value);

//ppu/ppu.cpp
void ppu_render_scanline(void);
void ppu_update_scanline(void);
byte oam_read(word addr);
void oam_write(word addr, byte value);
void PPUInit(byte first_time);

//win/render.cpp
void UpdateDisplay(void);

//misc/libstr.cpp
ulong strhex(char *str);
ulong strdec(char *str);
