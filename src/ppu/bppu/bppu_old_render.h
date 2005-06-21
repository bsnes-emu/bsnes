//bppu_render.cpp
void render_line_mode0();
void render_line_mode1();
void render_line_mode2();
void render_line_mode3();
void render_line_mode4();
void render_line_mode5();
void render_line_mode6();
void render_line_mode7();

//bppu_render_cache.cpp
enum { BLENDTYPE_BACK = 0, BLENDTYPE_MAIN = 1, BLENDTYPE_SUB = 2, BLENDTYPE_COMBINE = 3 };
enum { COLORDEPTH_4 = 0, COLORDEPTH_16 = 1, COLORDEPTH_256 = 2 };
enum { TILE_2BIT = 0, TILE_4BIT = 1, TILE_8BIT = 2 };

struct {
  uint8 color_main, color_sub;
  uint8 src_main,   src_sub;
  uint8 blend_type;
}pixel_cache[512];
uint8 layer_cache[512 * 12];

uint8 *bg_tiledata[3];
uint8 *bg_tiledata_state[3];

void clear_pixel_cache(void);
void clear_layer_cache(void);
void init_tiledata_cache(void);
void clear_tiledata_cache(void);

//bppu_render_windows.cpp
enum { WINDOWMASK_OR = 0, WINDOWMASK_AND = 1, WINDOWMASK_XOR = 2, WINDOWMASK_XNOR = 3 };

bool windows_not_obstructing(uint8 layer, uint8 bg, uint16 x);
bool color_windows_not_obstructing(uint16 x, uint8 color_mask_type);

//bppu_render_main.cpp
enum {
  SH_2    =  1, SH_4    =  2, SH_8    =  3, SH_16   =  4,
  SH_32   =  5, SH_64   =  6, SH_128  =  7, SH_256  =  8,
  SH_512  =  9, SH_1024 = 10, SH_2048 = 11, SH_4096 = 12
};
enum { COLORMODE_ADD = 0, COLORMODE_SUB = 1 };
enum { PPU_MAIN = 0, PPU_SUB = 1 };
enum { OAM_PRI_NONE = 4 };
uint8 oam_line_pal[512], oam_line_pri[512];

struct {
  byte num;
  byte width, height;
  word x, y;
  word character;
  byte v_flip, h_flip;
  byte palette;
  byte priority;
}current_sprite;

void   render_line_to_output();
inline uint16 addsub_pixels(uint8 x, uint8 cdest_index, uint8 cdest_bg, uint8 csrc_index, uint8 csrc_bg);
inline uint16 addsub_pixel(uint8 x, uint8 cdest_index, uint8 cdest_bg);
void   render_bg_tile(uint8 color_depth, uint8 bg, uint16 tile_num);
void   set_pixel(uint8 bg, uint16 x, uint8 pal_index);
void   set_layer_pixels(uint8 layer_count, uint8 *layer_bg_lookup);
void   set_sprite_attributes(uint8 sprite_num);
void   render_oam_sprite(void);
void   render_line_oam(uint8 layer_pos_pri0, uint8 layer_pos_pri1, uint8 layer_pos_pri2, uint8 layer_pos_pri3);
void   render_line_bg(uint8 layer_pos_pri0, uint8 layer_pos_pri1, uint8 color_depth, uint8 bg);

//bppu_render_mode7.cpp
void render_line_m7(uint8 layer_pos_bg1, uint8 layer_pos_bg2_pri0, uint8 layer_pos_bg2_pri1);
