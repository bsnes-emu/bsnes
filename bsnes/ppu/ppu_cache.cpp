#define BLENDTYPE_BACK    0
#define BLENDTYPE_MAIN    1
#define BLENDTYPE_SUB     2
#define BLENDTYPE_COMBINE 3

#define COLORDEPTH_4   0
#define COLORDEPTH_16  1
#define COLORDEPTH_256 2

struct {
  byte color_main, color_sub;
  byte src_main,   src_sub;
  byte blend_type;
}ppu_pixel_cache[512];

byte ppu_layer_cache[512 * 12];

#define TILE_2BIT 0
#define TILE_4BIT 1
#define TILE_8BIT 2

byte *ppu_bg_tiledata[3];
byte *ppu_bg_tiledata_state[3];

//this should be reset once every scanline
void ppu_clear_pixel_cache(void) {
int i;
  for(i=0;i<render.snes_width;i++) {
    ppu_pixel_cache[i].color_main =
    ppu_pixel_cache[i].color_sub  = 0;
    ppu_pixel_cache[i].src_main   =
    ppu_pixel_cache[i].src_sub    = BACK;
    ppu_pixel_cache[i].blend_type = BLENDTYPE_BACK;
  }
}

//this should be reset once every scanline
void ppu_clear_layer_cache(void) {
  memset(&ppu_layer_cache, 0, render.snes_width * 12);
}

void ppu_init_tiledata_cache(void) {
  ppu_bg_tiledata[TILE_2BIT]       = (byte*)malloc(262144);
  ppu_bg_tiledata[TILE_4BIT]       = (byte*)malloc(131072);
  ppu_bg_tiledata[TILE_8BIT]       = (byte*)malloc( 65536);
  ppu_bg_tiledata_state[TILE_2BIT] = (byte*)malloc(  4096);
  ppu_bg_tiledata_state[TILE_4BIT] = (byte*)malloc(  2048);
  ppu_bg_tiledata_state[TILE_8BIT] = (byte*)malloc(  1024);
}

void ppu_clear_tiledata_cache(void) {
  memset(ppu_bg_tiledata[TILE_2BIT],       0, 262144);
  memset(ppu_bg_tiledata[TILE_4BIT],       0, 131072);
  memset(ppu_bg_tiledata[TILE_4BIT],       0,  65536);
  memset(ppu_bg_tiledata_state[TILE_2BIT], 0,   4096);
  memset(ppu_bg_tiledata_state[TILE_4BIT], 0,   2048);
  memset(ppu_bg_tiledata_state[TILE_8BIT], 0,   1024);
}
