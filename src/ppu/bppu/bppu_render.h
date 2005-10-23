//bppu_render.cpp
int  _screen_width;
bool _interlace;
int  _interlace_field;

inline void render_line_mode0();
inline void render_line_mode1();
inline void render_line_mode2();
inline void render_line_mode3();
inline void render_line_mode4();
inline void render_line_mode5();
inline void render_line_mode6();
inline void render_line_mode7();

//bppu_render_cache.cpp
enum { COLORDEPTH_4 = 0, COLORDEPTH_16 = 1, COLORDEPTH_256 = 2 };
enum { TILE_2BIT = 0, TILE_4BIT = 1, TILE_8BIT = 2 };

enum { PC_BG1 = 0x80, PC_BG2 = 0x81, PC_BG3 = 0x82, PC_BG4 = 0x83, PC_OAM = 0x84, PC_BACK = 0x00 };
struct _pixel {
//bgr555 color data for main/subscreen pixels: 0x0000 = transparent / use palette color # 0
//needs to be bgr555 instead of palette index for direct color mode ($2130 bit 0) to work
uint16 src_main, src_sub;
//indicates source of palette # for main/subscreen (BG1-4, OAM, or back)
uint8  bg_main,  bg_sub;
//true when bg_main == OAM && palette index >= 192, disables color add/sub effects
uint8  color_exempt;
//priority level of src_n. to set src_n,
//the priority of the pixel must be >pri_n
uint8  pri_main, pri_sub;
} pixel_cache[512];

uint8 *bg_tiledata[3];
uint8 *bg_tiledata_state[3];

void render_bg_tile(uint8 color_depth, uint16 tile_num);
inline void clear_pixel_cache();
inline void init_tiledata_cache();
inline void clear_tiledata_cache();

//bppu_render_windows.cpp
struct _window {
  bool  main_dirty, sub_dirty;
  uint8 main[512],  sub[512];
} window_cache[6];

void build_window_table(uint8 bg, bool mainscreen);
void build_window_tables(uint8 bg);
inline void clear_window_cache();

//bppu_render_bg.cpp
void render_line_bg(uint8 bg, uint8 color_depth, uint8 pri0_pos, uint8 pri1_pos);

//bppu_render_oam.cpp
sprite_item *spr;

uint8 oam_itemlist[32];
struct oam_tileitem {
  uint16 x, y, pri, pal, tile;
  bool   hflip;
} oam_tilelist[34];

enum { OAM_PRI_NONE = 4 };
uint8 oam_line_pal[512], oam_line_pri[512];

bool is_sprite_on_scanline();
void load_oam_tiles();
void render_oam_tile(int tile_num);
void render_line_oam(uint8 pri0_pos, uint8 pri1_pos, uint8 pri2_pos, uint8 pri3_pos);

//bppu_render_mode7.cpp
void render_line_mode7(uint8 bg, uint8 pri0_pos, uint8 pri1_pos);

//bppu_render_addsub.cpp
inline uint16 addsub_pixels(uint32 cdest, uint32 csrc);
inline uint16 addsub_pixel (uint32 cdest);

//bppu_render_line.cpp
enum { BLENDTYPE_BACK = 0, BLENDTYPE_MAIN = 1, BLENDTYPE_SUB = 2, BLENDTYPE_COMBINE = 3 };

inline uint16 get_palette(uint8 index);
inline uint16 get_direct_color(uint8 p, uint8 t);
inline uint16 get_pixel(int x);
inline void render_line_output();
