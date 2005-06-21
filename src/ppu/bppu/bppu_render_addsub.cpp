namespace bPPUAddSubTables {
  uint8 adjust_buffer_full[96] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
  };
  uint8 adjust_buffer_half[96] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
  };
};

inline uint16 bPPU::addsub_pixels(int cdest_index, int cdest_bg, int csrc_index, int csrc_bg) {
int r, g, b;
uint16 cdest = get_palette(cdest_index);
uint16 csrc  = get_palette(csrc_index);
uint16 res;
//oam palettes 0-3 are not affected by color add/sub
  if(cdest_bg == OAM) {
    if(cdest_index < 192) {
      return cdest;
    }
  }
  if(csrc_bg == OAM) {
    if(csrc_index < 192) {
      return csrc;
    }
  }

  switch(regs.color_mode) {
  case 0: //COLORMODE_ADD:
    if(regs.color_halve == true) {
      r = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) ));
      g = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) ));
      b = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) ));
    }
    break;
  case 1: //COLORMODE_SUB:
    if(regs.color_halve == true) {
      r = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest      ) & 31) - ((csrc      ) & 31) )) >> 1;
      g = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) - ((csrc >>  5) & 31) )) >> 1;
      b = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) - ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest      ) & 31) - ((csrc      ) & 31) ));
      g = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) - ((csrc >>  5) & 31) ));
      b = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) - ((csrc >> 10) & 31) ));
    }
    break;
  }
  return ((r) | (g << 5) | (b << 10));
}

inline uint16 bPPU::addsub_pixel(int cdest_index, int cdest_bg) {
int r, g, b;
uint16 cdest = get_palette(cdest_index);
uint16 csrc  = (regs.color_r) | (regs.color_g << 5) | (regs.color_b << 10);
uint16 res;
//only oam palettes 4-7 are affected by color add/sub
  if(cdest_bg == OAM) {
    if(cdest_index < 192) {
      return cdest;
    }
  }

  switch(regs.color_mode) {
  case 0: //COLORMODE_ADD:
    if(regs.color_halve == true && regs.addsub_mode == 0) {
      r = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) ));
      g = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) ));
      b = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) ));
    }
    break;
  case 1: //COLORMODE_SUB:
    if(regs.color_halve == true && regs.addsub_mode == 0) {
      r = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest      ) & 31) - ((csrc      ) & 31) )) >> 1;
      g = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) - ((csrc >>  5) & 31) )) >> 1;
      b = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) - ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest      ) & 31) - ((csrc      ) & 31) ));
      g = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) - ((csrc >>  5) & 31) ));
      b = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) - ((csrc >> 10) & 31) ));
    }
    break;
  }
  return ((r) | (g << 5) | (b << 10));
}
