//this should be reset once every scanline
void bPPU::clear_pixel_cache(void) {
  for(int i=0;i<512;i++) {
    pixel_cache[i].color_main = 0;
    pixel_cache[i].color_sub  = 0;
    pixel_cache[i].src_main   = BACK;
    pixel_cache[i].src_sub    = BACK;
    pixel_cache[i].blend_type = BLENDTYPE_BACK;
  }
}

//this should be reset once every scanline
void bPPU::clear_layer_cache(void) {
  memset(&layer_cache, 0, 512 * 12);
}

void bPPU::init_tiledata_cache(void) {
  bg_tiledata[TILE_2BIT]       = (uint8*)malloc(262144);
  bg_tiledata[TILE_4BIT]       = (uint8*)malloc(131072);
  bg_tiledata[TILE_8BIT]       = (uint8*)malloc( 65536);
  bg_tiledata_state[TILE_2BIT] = (uint8*)malloc(  4096);
  bg_tiledata_state[TILE_4BIT] = (uint8*)malloc(  2048);
  bg_tiledata_state[TILE_8BIT] = (uint8*)malloc(  1024);
}

void bPPU::clear_tiledata_cache(void) {
  memset(bg_tiledata[TILE_2BIT],       0, 262144);
  memset(bg_tiledata[TILE_4BIT],       0, 131072);
  memset(bg_tiledata[TILE_4BIT],       0,  65536);
  memset(bg_tiledata_state[TILE_2BIT], 0,   4096);
  memset(bg_tiledata_state[TILE_4BIT], 0,   2048);
  memset(bg_tiledata_state[TILE_8BIT], 0,   1024);
}
