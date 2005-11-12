//called once at the start of every rendered scanline
void bPPU::update_bg_info() {
  for(int bg=0;bg<4;bg++) {
    bg_info[bg].th = (regs.bg_tilesize[bg]) ? 4 : 3;
    bg_info[bg].tw = (line.hires) ? 4 : bg_info[bg].th;
    bg_info[bg].mx = (bg_info[bg].th == 4) ? (line.width << 1) : line.width;
    bg_info[bg].my = bg_info[bg].mx;
    if(regs.bg_scsize[bg] & 0x01)bg_info[bg].mx <<= 1;
    if(regs.bg_scsize[bg] & 0x02)bg_info[bg].my <<= 1;
    bg_info[bg].mx--;
    bg_info[bg].my--;
  }
}

uint16 bPPU::bg_get_tile(uint8 bg, uint16 x, uint16 y) {
uint16 map_index, pos;
  x &= bg_info[bg].mx;
  y &= bg_info[bg].my;

//32 = SC width, height; tilemap data is stored as uint16
//width(32) * height(32) * sizeof(uint16) = 2048
  switch(regs.bg_scsize[bg]) {
  case SC_32x32:map_index = 0; break;
  case SC_64x32:map_index = ((x >> bg_info[bg].th) / 32) * 2048; break;
  case SC_32x64:map_index = ((y >> bg_info[bg].th) / 32) * 2048; break;
  case SC_64x64:map_index = ((x >> bg_info[bg].th) / 32) * 2048 +
                            ((y >> bg_info[bg].th) / 32) * 4096; break;
  }

  pos = ((((y >> bg_info[bg].th) & 31) * 32) + ((x >> bg_info[bg].tw) & 31)) << 1;
  return read16(vram, regs.bg_scaddr[bg] + map_index + pos);
}

void bPPU::render_line_bg(uint8 bg, uint8 color_depth, uint8 pri0_pos, uint8 pri1_pos) {
  if(regs.bg_enabled[bg] == false && regs.bgsub_enabled[bg] == false) {
    return;
  }

bool   bg_enabled     = regs.bg_enabled[bg];
bool   bgsub_enabled  = regs.bgsub_enabled[bg];

uint16 opt_valid_bit  = (bg == BG1) ? 0x2000 : ((bg == BG2) ? 0x4000 : 0x0000);
uint8  bgpal_index    = (regs.bg_mode == 0) ? (bg << 5) : 0;

uint8  pal_size       = (color_depth == 2) ? 256 : ((color_depth == 1) ? 16 : 4);
uint16 tile_mask      = (color_depth == 2) ? 0x03ff : ((color_depth == 1) ? 0x07ff : 0x0fff);
//4 + color_depth = >>(4-6) -- / {16, 32, 64 } bytes/tile
//index is a tile number count to add to base tile number
uint   tiledata_index = regs.bg_tdaddr[bg] >> (4 + color_depth);

uint8 *bg_td          = (uint8*)bg_tiledata[color_depth];
uint8 *bg_td_state    = (uint8*)bg_tiledata_state[color_depth];

uint8  tile_width     = bg_info[bg].tw;
uint8  tile_height    = bg_info[bg].th;
uint16 mask_x         = bg_info[bg].mx; //screen width  mask
uint16 mask_y         = bg_info[bg].my; //screen height mask

uint x = 0;
uint y = line.y;
  if(line.interlace && line.hires) {
    y = (y << 1) + line.interlace_field;
  }

uint16 hscroll = (line.hires) ? (regs.bg_hofs[bg] << 1) : regs.bg_hofs[bg];
uint16 vscroll = (line.interlace && line.hires) ? (regs.bg_vofs[bg] << 1) : regs.bg_vofs[bg];
  hscroll &= mask_x;
  vscroll &= mask_y;

uint16 *mtable = (uint16*)mosaic_table[(regs.mosaic_enabled[bg]) ? regs.mosaic_size : 0];
uint16  mosaic_x;
uint16  mosaic_y;

uint16 hval, vval;
uint16 t, tile_pri, tile_num;
uint8  pal_index, pal_num;
uint8 *tile_ptr;
uint   xpos, ypos;
uint16 hoffset, voffset, opt_x, col;
bool   mirror_x, mirror_y;
bool   is_opt_mode = (regs.bg_mode == 2 || regs.bg_mode == 4 || regs.bg_mode == 6);

  build_window_tables(bg);
uint8 *wt_main = window_cache[bg].main;
uint8 *wt_sub  = window_cache[bg].sub;

int32 prev_x = -1, prev_y = -1;
  for(x=0;x<line.width;x++) {
    hoffset = x + hscroll;
    voffset = y + vscroll;

    if(is_opt_mode) {
      opt_x = (x + (hscroll & 7));

    //tile 0 is unaffected by OPT mode...
      if(opt_x >= 8) {
        hval = bg_get_tile(BG3,
          (opt_x - 8) + (regs.bg_hofs[BG3] & ~7),
          regs.bg_vofs[BG3]);

        vval = bg_get_tile(BG3,
          (opt_x - 8) + (regs.bg_hofs[BG3] & ~7),
          regs.bg_vofs[BG3] + 8);

        if(regs.bg_mode == 4) {
          if(hval & opt_valid_bit) {
            if(!(hval & 0x8000)) {
              hoffset = opt_x + (hval & ~7);
            } else {
              voffset = y + hval;
            }
          }
        } else {
          if(hval & opt_valid_bit) {
            hoffset = opt_x + (hval & ~7);
          }
          if(vval & opt_valid_bit) {
            voffset = y + vval;
          }
        }
      }
    }

    mosaic_x = mtable[hoffset & mask_x];
    mosaic_y = mtable[voffset & mask_y];

    if((mosaic_x >> 3) != prev_x || (mosaic_y >> 3) != prev_y) {
      prev_x = (mosaic_x >> 3);
      prev_y = (mosaic_y >> 3);

      t = bg_get_tile(bg, mosaic_x, mosaic_y);

      mirror_y  = !!(t & 0x8000);
      mirror_x  = !!(t & 0x4000);

      tile_pri = (t & 0x2000) ? pri1_pos : pri0_pos;
      tile_num = t;

      if(tile_width  == 4) { //16x16 horizontal tile mirroring
        if(!!(mosaic_x & 8) != mirror_x)tile_num++;
      }

      if(tile_height == 4) { //16x16 vertical tile mirroring
        if(!!(mosaic_y & 8) != mirror_y)tile_num += 16;
      }

      tile_num &= 0x03ff;
      tile_num += tiledata_index;
      tile_num &= tile_mask;

      if(bg_td_state[tile_num] == 1) {
        render_bg_tile(color_depth, tile_num);
      }

      pal_num   = ((t >> 10) & 7);
      pal_index = (pal_num * pal_size) + bgpal_index;

      ypos = mosaic_y & 7;
      if(mirror_y)ypos ^= 7; //invert y tile pos

      tile_ptr = (uint8*)bg_td + (tile_num * 64) + (ypos * 8);
    }

    xpos = mosaic_x & 7;
    if(mirror_x)xpos ^= 7; //invert x tile pos
    col = *(tile_ptr + xpos);
    if(col) {
      if(regs.direct_color == true && bg == BG1 && (regs.bg_mode == 3 || regs.bg_mode == 4)) {
        col = get_direct_color(pal_num, col);
      } else {
        col = get_palette(col + pal_index);
      }

      if(bg_enabled == true && !wt_main[x]) {
        if(pixel_cache[x].pri_main < tile_pri) {
          pixel_cache[x].pri_main = tile_pri;
          pixel_cache[x].bg_main  = 0x80 | bg;
          pixel_cache[x].src_main = col;
          pixel_cache[x].color_exempt = false;
        }
      }
      if(bgsub_enabled == true && !wt_sub[x]) {
        if(pixel_cache[x].pri_sub < tile_pri) {
          pixel_cache[x].pri_sub = tile_pri;
          pixel_cache[x].bg_sub  = 0x80 | bg;
          pixel_cache[x].src_sub = col;
        }
      }
    }
  }
}
