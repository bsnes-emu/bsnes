//called once at the start of every rendered scanline
void bPPU::update_bg_info() {
uint hires = (regs.bg_mode == 5 || regs.bg_mode == 6);
uint width = (!hires) ? 256 : 512;

  for(int bg = 0; bg < 4; bg++) {
    bg_info[bg].th = (regs.bg_tilesize[bg]) ? 4 : 3;
    bg_info[bg].tw = (hires) ? 4 : bg_info[bg].th;

    bg_info[bg].mx = (bg_info[bg].th == 4) ? (width << 1) : width;
    bg_info[bg].my = bg_info[bg].mx;
    if(regs.bg_scsize[bg] & 0x01)bg_info[bg].mx <<= 1;
    if(regs.bg_scsize[bg] & 0x02)bg_info[bg].my <<= 1;
    bg_info[bg].mx--;
    bg_info[bg].my--;

    bg_info[bg].scy = (regs.bg_scsize[bg] & 0x02) ? (32 << 5) : 0;
    bg_info[bg].scx = (regs.bg_scsize[bg] & 0x01) ? (32 << 5) : 0;
    if(regs.bg_scsize[bg] == 3)bg_info[bg].scy <<= 1;
  }
}

uint16 bPPU::bg_get_tile(uint8 bg, uint16 x, uint16 y) {
  x = (x & bg_info[bg].mx) >> bg_info[bg].tw;
  y = (y & bg_info[bg].my) >> bg_info[bg].th;

uint16 pos = ((y & 0x1f) << 5) + (x & 0x1f);
  if(y & 0x20)pos += bg_info[bg].scy;
  if(x & 0x20)pos += bg_info[bg].scx;

uint16 addr = regs.bg_scaddr[bg] + (pos << 1);
  return (vram_read(addr + 0) << 0) | (vram_read(addr + 1) << 8);
}

#define setpixel_main(x) \
  if(pixel_cache[x].pri_main < tile_pri) { \
    pixel_cache[x].pri_main = tile_pri; \
    pixel_cache[x].bg_main  = bg; \
    pixel_cache[x].src_main = col; \
    pixel_cache[x].ce_main  = false; \
  }

#define setpixel_sub(x) \
  if(pixel_cache[x].pri_sub < tile_pri) { \
    pixel_cache[x].pri_sub = tile_pri; \
    pixel_cache[x].bg_sub  = bg; \
    pixel_cache[x].src_sub = col; \
    pixel_cache[x].ce_sub  = false; \
  }

void bPPU::render_line_bg(uint8 bg, uint8 color_depth, uint8 pri0_pos, uint8 pri1_pos) {
  if(regs.bg_enabled[bg] == false && regs.bgsub_enabled[bg] == false) {
    return;
  }

//are layers disabled by user?
  if(render_enabled(bg, 0) == false)pri0_pos = 0;
  if(render_enabled(bg, 1) == false)pri1_pos = 0;
//nothing to render?
  if(!pri0_pos && !pri1_pos)return;

bool   bg_enabled    = regs.bg_enabled[bg];
bool   bgsub_enabled = regs.bgsub_enabled[bg];

uint16 opt_valid_bit = (bg == BG1) ? 0x2000 : (bg == BG2) ? 0x4000 : 0x0000;
uint8  bgpal_index   = (regs.bg_mode == 0) ? (bg << 5) : 0;

uint8  pal_size  = 2 << color_depth;      //<<2 (*4), <<4 (*16), <<8 (*256)
uint16 tile_mask = 0x0fff >> color_depth; //0x0fff, 0x07ff, 0x03ff
//4 + color_depth = >>(4-6) -- / {16, 32, 64 } bytes/tile
//index is a tile number count to add to base tile number
uint   tiledata_index = regs.bg_tdaddr[bg] >> (4 + color_depth);

uint8 *bg_td       = bg_tiledata[color_depth];
uint8 *bg_td_state = bg_tiledata_state[color_depth];

uint8  tile_width  = bg_info[bg].tw;
uint8  tile_height = bg_info[bg].th;
uint16 mask_x      = bg_info[bg].mx; //screen width  mask
uint16 mask_y      = bg_info[bg].my; //screen height mask

uint16 y       = regs.bg_y[bg];
uint16 hscroll = regs.bg_hofs[bg];
uint16 vscroll = regs.bg_vofs[bg];

uint hires = (regs.bg_mode == 5 || regs.bg_mode == 6);
uint width = (!hires) ? 256 : 512;

  if(hires) {
    hscroll <<= 1;
    if(regs.interlace) {
      y = (y << 1) + line.interlace_field;
    }
  }

uint16 *mtable = mosaic_table[(regs.mosaic_enabled[bg]) ? regs.mosaic_size : 0];

uint16 hval, vval;
uint16 t, tile_pri, tile_num;
uint8  pal_index, pal_num;
uint8 *tile_ptr;
uint   xpos, ypos;
uint16 hoffset, voffset, opt_x, col;
bool   mirror_x, mirror_y;
bool   is_opt_mode = (config::ppu.opt_enable == true) && (regs.bg_mode == 2 || regs.bg_mode == 4 || regs.bg_mode == 6);

  build_window_tables(bg);
uint8 *wt_main = window[bg].main;
uint8 *wt_sub  = window[bg].sub;

uint16 prev_x = 0xffff, prev_y = 0xffff;
  for(uint16 x = 0; x < width; x++) {
    hoffset = mtable[x] + hscroll;
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

    hoffset &= mask_x;
    voffset &= mask_y;

    if((hoffset >> 3) != prev_x || (voffset >> 3) != prev_y) {
      prev_x = (hoffset >> 3);
      prev_y = (voffset >> 3);

      t = bg_get_tile(bg, hoffset, voffset);

      mirror_y = !!(t & 0x8000);
      mirror_x = !!(t & 0x4000);

      tile_pri = (t & 0x2000) ? pri1_pos : pri0_pos;
      tile_num = t;

      if(tile_width  == 4) { //16x16 horizontal tile mirroring
        if(!!(hoffset & 8) != mirror_x)tile_num++;
      }

      if(tile_height == 4) { //16x16 vertical tile mirroring
        if(!!(voffset & 8) != mirror_y)tile_num += 16;
      }

      tile_num &= 0x03ff;
      tile_num += tiledata_index;
      tile_num &= tile_mask;

      if(bg_td_state[tile_num] == 1) {
        render_bg_tile(color_depth, tile_num);
      }

      pal_num   = ((t >> 10) & 7);
      pal_index = bgpal_index + (pal_num << pal_size);

      ypos = voffset & 7;
      if(mirror_y)ypos ^= 7; //invert y tile pos

      tile_ptr = bg_td + (tile_num * 64) + (ypos * 8);
    }

    xpos = hoffset & 7;
    if(mirror_x)xpos ^= 7; //invert x tile pos
    col = *(tile_ptr + xpos);
    if(col) {
      if(regs.direct_color == true && bg == BG1 && (regs.bg_mode == 3 || regs.bg_mode == 4)) {
        col = get_direct_color(pal_num, col);
      } else {
        col = get_palette(col + pal_index);
      }

      if(!hires) {
        if(bg_enabled    == true && !wt_main[x]) { setpixel_main(x); }
        if(bgsub_enabled == true && !wt_sub[x])  { setpixel_sub(x);  }
      } else {
      int hx = x >> 1;
        if(x & 1) {
          if(bg_enabled    == true && !wt_main[hx]) { setpixel_main(hx); }
        } else {
          if(bgsub_enabled == true && !wt_sub[hx])  { setpixel_sub(hx);  }
        }
      }
    }
  }
}

#undef setpixel_main
#undef setpixel_sub
