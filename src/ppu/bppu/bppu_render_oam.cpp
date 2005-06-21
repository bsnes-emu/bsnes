void bPPU::set_sprite_attributes(uint8 sprite_num) {
uint32 t;
uint8  size, b;
uint16 x;
  t = *((uint32*)oam + sprite_num);
  b = oam[512 + (sprite_num >> 2)];

  switch(sprite_num & 3) {
  case 0: size = !!(b & 0x02); x = (b & 0x01)?0x100:0; break;
  case 1: size = !!(b & 0x08); x = (b & 0x04)?0x100:0; break;
  case 2: size = !!(b & 0x20); x = (b & 0x10)?0x100:0; break;
  case 3: size = !!(b & 0x80); x = (b & 0x40)?0x100:0; break;
  }

  current_sprite.num       = sprite_num;
  current_sprite.priority  = (t >> 28) & 3;
  current_sprite.x         = x | (t & 0xff);
  current_sprite.y         = ((t >> 8) + 1) & 0xff;
  current_sprite.v_flip    = !!(t & 0x80000000);
  current_sprite.h_flip    = !!(t & 0x40000000);
  current_sprite.palette   = (t >> 25) & 7;
  current_sprite.character = (t >> 16) & 0x01ff;

//size: 0 = small, 1 = large
  switch(regs.oam_basesize) {
  case 0:
    if(!size) { current_sprite.width =  8; current_sprite.height =  8; }
    else      { current_sprite.width = 16; current_sprite.height = 16; }
    break;
  case 1:
    if(!size) { current_sprite.width =  8; current_sprite.height =  8; }
    else      { current_sprite.width = 32; current_sprite.height = 32; }
    break;
  case 2:
    if(!size) { current_sprite.width =  8; current_sprite.height =  8; }
    else      { current_sprite.width = 64; current_sprite.height = 64; }
    break;
  case 3:
    if(!size) { current_sprite.width = 16; current_sprite.height = 16; }
    else      { current_sprite.width = 32; current_sprite.height = 32; }
    break;
  case 4:
    if(!size) { current_sprite.width = 16; current_sprite.height = 16; }
    else      { current_sprite.width = 64; current_sprite.height = 64; }
    break;
  case 5:
    if(!size) { current_sprite.width = 32; current_sprite.height = 32; }
    else      { current_sprite.width = 64; current_sprite.height = 64; }
    break;
  case 6:
    if(!size) { current_sprite.width = 16; current_sprite.height = 32; }
    else      { current_sprite.width = 32; current_sprite.height = 64; }
    break;
  case 7:
    if(!size) { current_sprite.width = 16; current_sprite.height = 32; }
    else      { current_sprite.width = 32; current_sprite.height = 32; }
    break;
  }
}

void bPPU::render_oam_sprite() {
uint16 pos, col, chr, tiledata_inc;
uint8  d0, d1, d2, d3, pal_index;
int    x, y, z, x1, mx, mask, p;
int    tile_width;
  tile_width = current_sprite.width >> 3; //e.x. 16x16 sprite = 2x2 tiles

  if(_interlace == true && _screen_width == 512) {
    y = (_y << 1) + _interlace_field;
  } else {
    y = _y;
  }

  x = current_sprite.x;
  if(_screen_width == 512) {
    x <<= 1;
  }

  if(current_sprite.v_flip) {
    y = ((current_sprite.height - 1) - (_y - current_sprite.y));
  } else {
    y = (_y - current_sprite.y);
  }
  y &= 255;
  if(regs.oam_halve == true) {
    y <<= 1;
    y += _interlace_field;
  }

  chr = current_sprite.character;
  tiledata_inc = (chr & 0x100)?(regs.oam_nameselect << 13):0;
  chr += (y >> 3) << 4;
  pal_index = (current_sprite.palette << 4);
  for(x1=0;x1<tile_width;x1++) {
    if(current_sprite.h_flip)mx = (tile_width - 1) - x1;
    else                     mx = x1;
    pos = regs.oam_tdaddr + ((chr + mx) << 5) + ((y & 7) << 1) + tiledata_inc;
    d0 = vram[pos     ];
    d1 = vram[pos +  1];
    d2 = vram[pos + 16];
    d3 = vram[pos + 17];
    for(z=0;z<8;z++) {
      if(current_sprite.h_flip) {
        mask = 0x01 << z;
      } else {
        mask = 0x80 >> z;
      }
      x &= 511;
      if(x < _screen_width) {
        col = 0;
        if(d0 & mask)col += 1;
        if(d1 & mask)col += 2;
        if(d2 & mask)col += 4;
        if(d3 & mask)col += 8;
        if(col) {
          col += pal_index;
          col += 128;
          if(oam_line_pri[x] == OAM_PRI_NONE) {
            oam_line_pal[x] = col;
            oam_line_pri[x] = current_sprite.priority;
          }
          if(_screen_width == 512) {
            if(oam_line_pri[x + 1] == OAM_PRI_NONE) {
              oam_line_pal[x + 1] = col;
              oam_line_pri[x + 1] = current_sprite.priority;
            }
          }
        }
      }
      x += (_screen_width == 512)?2:1;
    }
  }
}

void bPPU::render_line_oam(uint8 pri0_pos, uint8 pri1_pos, uint8 pri2_pos, uint8 pri3_pos) {
int s;
bool _bg_enabled    = regs.bg_enabled[OAM];
bool _bgsub_enabled = regs.bgsub_enabled[OAM];
  if(_bg_enabled == false && _bgsub_enabled == false)return;

uint8 *wt_main = main_windowtable[OAM];
uint8 *wt_sub  = sub_windowtable[OAM];
  build_window_tables(OAM);

  memset(oam_line_pri, OAM_PRI_NONE, 512);
  for(s=0;s<128;s++) {
    set_sprite_attributes(s);
    if(regs.oam_halve == false) {
      if(_y >= current_sprite.y && _y < (current_sprite.y + current_sprite.height)) {
        render_oam_sprite();
      } else if((current_sprite.y + current_sprite.height) >= 256 && _y < ((current_sprite.y + current_sprite.height) & 255)) {
        render_oam_sprite();
      }
    } else {
      if(_y >= current_sprite.y && _y < (current_sprite.y + (current_sprite.height >> 1))) {
        render_oam_sprite();
      } else if((current_sprite.y + current_sprite.height) >= 256 && _y < ((current_sprite.y + (current_sprite.height >> 1)) & 255)) {
        render_oam_sprite();
      }
    }
  }

int _pri;
  for(int x=0;x<_screen_width;x++) {
    if(oam_line_pri[x] == OAM_PRI_NONE)continue;

    switch(oam_line_pri[x]) {
    case 0:_pri = pri0_pos;break;
    case 1:_pri = pri1_pos;break;
    case 2:_pri = pri2_pos;break;
    case 3:_pri = pri3_pos;break;
    }

    if(main_colorwindowtable[x]) {
      if(_bg_enabled == true && !wt_main[x]) {
        if(pixel_cache[x].pri_main < _pri) {
          pixel_cache[x].pri_main = _pri;
          pixel_cache[x].bg_main  = PC_OAM;
          pixel_cache[x].src_main = oam_line_pal[x];
        }
      }
      if(_bgsub_enabled == true && !wt_sub[x]) {
        if(pixel_cache[x].pri_sub < _pri) {
          pixel_cache[x].pri_sub = _pri;
          pixel_cache[x].bg_sub  = PC_OAM;
          pixel_cache[x].src_sub = oam_line_pal[x];
        }
      }
    }
  }
}
