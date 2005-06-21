void bPPU::render_line_bg(uint8 bg, uint8 color_depth, uint8 pri0_pos, uint8 pri1_pos) {
  if(regs.bg_enabled[bg] == false && regs.bgsub_enabled[bg] == false) {
    return;
  }

int x;
int  _scaddr        = regs.bg_scaddr[bg];
int  _tdaddr        = regs.bg_tdaddr[bg];
bool _bg_enabled    = regs.bg_enabled[bg];
bool _bgsub_enabled = regs.bgsub_enabled[bg];

uint16 opt_valid_bit; //offset-per-tile valid flag bit
  if(bg == BG1) {
    opt_valid_bit = 0x2000;
  } else if(bg == BG2) {
    opt_valid_bit = 0x4000;
  } else {
    opt_valid_bit = 0x0000;
  }

uint8 pal_size, tiledata_size;
  switch(color_depth) {
  case COLORDEPTH_4:
    pal_size      =   4;
    tiledata_size =   4; //<<4=*16
    break;
  case COLORDEPTH_16:
    pal_size      =  16;
    tiledata_size =   5; //<<5=*32
    break;
  case COLORDEPTH_256:
    pal_size      = 256;
    tiledata_size =   6; //<<6=*64
    break;
  }

uint8 *bg_td, *bg_td_state;
  bg_td       = (uint8*)bg_tiledata[color_depth];
  bg_td_state = (uint8*)bg_tiledata_state[color_depth];

uint16 screen_width, screen_height;
  screen_width  = _screen_width;
  screen_height = _screen_width; //this is correct -- ppu tilemap is a perfect square

uint8 tile_size, tile_width, tile_height;
  tile_size   = (regs.bg_tilesize[bg])?4:3; //<<4=*16, <<3=*8
  tile_width  = tile_size;
  tile_height = tile_size;

int screen_x, screen_y;
  if(_interlace == true && _screen_width == 512) {
    screen_y = (_y << 1) + _interlace_field;
  } else {
    screen_y = _y;
  }

//Modes 5 and 6 seem to force 16-width tiles due to having twice the resolution.
//The tile size attribute in $2105 has no effect on tile width.
  if(_screen_width == 512) {
    tile_width = 4; //<<4=*16
  }

  if(tile_size == 4) { //16x16 tile size
    screen_width  <<= 1;
    screen_height <<= 1;
  }

  if(regs.bg_scsize[bg] & 0x01)screen_width  <<= 1;
  if(regs.bg_scsize[bg] & 0x02)screen_height <<= 1;

uint16 screen_width_mask, screen_height_mask;
  screen_width_mask  = screen_width  - 1;
  screen_height_mask = screen_height - 1;

int bg_x, bg_y;
uint16 vscroll, hscroll;
  if(_screen_width == 512) {
    hscroll = (regs.bg_hofs[bg] << 1) & screen_width_mask;
  } else {
    hscroll = regs.bg_hofs[bg] & screen_width_mask;
  }
  bg_x = hscroll;

  if(_screen_width == 512 && _interlace == true) {
    vscroll = (regs.bg_vofs[bg] << 1) & screen_height_mask;
  } else {
    vscroll = regs.bg_vofs[bg] & screen_height_mask;
  }
  bg_y = (screen_y + vscroll) & screen_height_mask;

uint16 *mtable;
int mosaic_x, mosaic_y;
  if(regs.mosaic_enabled[bg] == true) {
    mtable = (uint16*)mosaic_table[regs.mosaic_size];
  } else {
    mtable = (uint16*)mosaic_table[0];
  }
  mosaic_x = mtable[bg_x];
  mosaic_y = mtable[bg_y];

uint8  tile_x;
uint16 t, base_xpos, base_pos, pos;
uint16 tile_num;
int mirror_x, mirror_y;
uint8  pal_index;
uint8 *tile_ptr;
int    xpos, ypos;
uint16 map_index, hoffset, voffset, col;

uint8 *wt_main = main_windowtable[bg];
uint8 *wt_sub  = sub_windowtable[bg];
  build_window_tables(bg);
  for(screen_x=0;screen_x<_screen_width;screen_x++) {
    if(wt_main[screen_x] && wt_sub[screen_x])continue;

    if(regs.bg_mode == 2 || regs.bg_mode == 4 || regs.bg_mode == 6) {
      if(regs.bg_mode == 6) {
        tile_x = (mtable[screen_x + (hscroll & 15)] >> 4);
      } else {
        tile_x = (mtable[screen_x + (hscroll &  7)] >> 3);
      }
      hoffset = hscroll;
      voffset = vscroll;
      if(tile_x != 0) {
        tile_x = (tile_x - 1) & 31;
        if(regs.bg_mode == 4) {
          pos = regs.bg_scaddr[BG3] + (tile_x << 1);
          t   = *((uint16*)vram + (pos >> 1));
          if(t & opt_valid_bit) {
            if(!(t & 0x8000)) {
              hoffset = ((t & 0x1ff8) | (hscroll & 7)) & screen_width_mask;
            } else {
              voffset = (t & 0x1fff) & screen_height_mask;
            }
          }
        } else {
          pos = regs.bg_scaddr[BG3] + (tile_x << 1);
          t   = *((uint16*)vram + (pos >> 1));
          if(t & opt_valid_bit) {
            hoffset = ((t & 0x1ff8) | (hscroll & 7)) & screen_width_mask;
          }
          pos = regs.bg_scaddr[BG3] + 64 + (tile_x << 1);
          t   = *((uint16*)vram + (pos >> 1));
          if(t & opt_valid_bit) {
            voffset = (t & 0x1fff) & screen_height_mask;
          }
        }
      }
      mosaic_x = mtable[(screen_x + hoffset) & screen_width_mask ];
      mosaic_y = mtable[(screen_y + voffset) & screen_height_mask];
    }

    switch(regs.bg_scsize[bg]) {
    case 0:
      map_index = 0;
      break;
    case 1:
      map_index = ((mosaic_x >> tile_size) >> 5) << 11;
      break;
    case 2:
      map_index = ((mosaic_y >> tile_size) >> 5) << 11;
      break;
    case 3:
      map_index = ((mosaic_x >> tile_size) >> 5) << 11 |
                  ((mosaic_y >> tile_size) >> 5) << 12;
      break;
    }

    base_xpos = ((mosaic_x >> 3) & 31);
    base_pos  = (((mosaic_y >> tile_height) & 31) << 5) + ((mosaic_x >> tile_width) & 31);
    pos       = _scaddr + map_index + (base_pos << 1);
    t         = *((uint16*)vram + (pos >> 1));
    mirror_y  = (t & 0x8000)?1:0;
    mirror_x  = (t & 0x4000)?1:0;

int _pri;
    _pri = (t & 0x2000) ? pri1_pos : pri0_pos;

    tile_num = t & 0x03ff;
    if(tile_width == 4) {
      if(((mosaic_x & 15) >= 8 && !mirror_x) ||
         ((mosaic_x & 15) <  8 &&  mirror_x))tile_num++;
      tile_num &= 0x03ff;
    }
    if(tile_height == 4) {
      if(((mosaic_y & 15) >= 8 && !mirror_y) ||
         ((mosaic_y & 15) <  8 &&  mirror_y))tile_num += 16;
      tile_num &= 0x03ff;
    }
    tile_num += (_tdaddr >> tiledata_size);

    if(bg_td_state[tile_num] == 1) {
      render_bg_tile(color_depth, tile_num);
    }

    pal_index = ((t >> 10) & 7) * pal_size;

    if(mirror_y) { ypos = (7 - (mosaic_y & 7)); }
    else         { ypos = (    (mosaic_y & 7)); }

//loop while we are rendering from the same tile, as there's no need to do all of the above work
//unless we have rendered all of the visible tile, taking mosaic into account.
    tile_ptr = (uint8*)bg_td + (tile_num << 6) + (ypos << 3);
    while(1) {
      if(mirror_x) { xpos = (7 - (mosaic_x & 7)); }
      else         { xpos = (    (mosaic_x & 7)); }
      col = *(tile_ptr + xpos);
      if(col && main_colorwindowtable[screen_x]) {
        if(_bg_enabled == true && !wt_main[screen_x]) {
          if(pixel_cache[screen_x].pri_main < _pri) {
            pixel_cache[screen_x].pri_main = _pri;
            pixel_cache[screen_x].bg_main  = 0x80 | bg;
            pixel_cache[screen_x].src_main = col + pal_index;
          }
        }
        if(_bgsub_enabled == true && !wt_sub[screen_x]) {
          if(pixel_cache[screen_x].pri_sub < _pri) {
            pixel_cache[screen_x].pri_sub = _pri;
            pixel_cache[screen_x].bg_sub  = 0x80 | bg;
            pixel_cache[screen_x].src_sub = col + pal_index;
          }
        }
      }

      bg_x++;
      bg_x &= screen_width_mask;
      mosaic_x = mtable[bg_x];

      if(base_xpos != ((mosaic_x >> 3) & 31))break;
      screen_x++;
      if(screen_x >= _screen_width)break;
    }
  }
}
