bool bPPU::windows_not_obstructing(uint8 layer, uint8 bg, uint16 x) {
uint8  w1_mask, w2_mask; //1 = masked, 0 = not masked
uint16 window1_left, window1_right, window2_left, window2_right;
  if(layer == PPU_MAIN) {
    if(regs.bg_window_enabled[bg] == false)return true;
  } else if(layer == PPU_SUB) {
    if(regs.bgsub_window_enabled[bg] == false)return true;
  }

  window1_left  = regs.window1_left;
  window1_right = regs.window1_right;
  window2_left  = regs.window2_left;
  window2_right = regs.window2_right;

  if(regs.bg_mode == 5 || regs.bg_mode == 6) {
    window1_left  <<= 1;
    window1_right <<= 1;
    window2_left  <<= 1;
    window2_right <<= 1;
  }

  if(regs.bg_window1_enabled[bg] == true && regs.bg_window2_enabled[bg] == false) {
    if(regs.bg_window1_invert[bg] == false) {
      if(x >= window1_left && x <= window1_right)return false;
      return true;
    } else {
      if(x <  window1_left || x >  window1_right)return false;
      return true;
    }
  } else if(regs.bg_window2_enabled[bg] == true && regs.bg_window1_enabled[bg] == false) {
    if(regs.bg_window2_invert[bg] == false) {
      if(x >= window2_left && x <= window2_right)return false;
      return true;
    } else {
      if(x <  window2_left || x >  window2_right)return false;
      return true;
    }
  } else if(regs.bg_window1_enabled[bg] == true && regs.bg_window2_enabled[bg] == true) {
    if(regs.bg_window1_invert[bg] == false) {
      if(x >= window1_left && x <= window1_right)w1_mask = 1;
      else w1_mask = 0;
    } else {
      if(x <  window1_left || x >  window1_right)w1_mask = 1;
      else w1_mask = 0;
    }

    if(regs.bg_window2_invert[bg] == false) {
      if(x >= window2_left && x <= window2_right)w2_mask = 1;
      else w2_mask = 0;
    } else {
      if(x <  window2_left || x >  window2_right)w2_mask = 1;
      else w2_mask = 0;
    }

    switch(regs.bg_window_mask[bg]) {
    case WINDOWMASK_OR:
      if((w1_mask | w2_mask) == 1)return false;
      return true;
    case WINDOWMASK_AND:
      if((w1_mask & w2_mask) == 1)return false;
      return true;
    case WINDOWMASK_XOR:
      if((w1_mask ^ w2_mask) == 1)return false;
      return true;
    case WINDOWMASK_XNOR:
      if((w1_mask ^ w2_mask) == 0)return false;
      return true;
    }
  }
  return true;
}

bool bPPU::color_windows_not_obstructing(uint16 x, uint8 color_mask_type) {
uint8  w1_mask, w2_mask; //1 = masked, 0 = not masked
uint8  color_mask;
bool   r;
uint16 window1_left, window1_right, window2_left, window2_right;
  if(color_mask_type == PPU_MAIN) {
    color_mask = regs.color_mask;
  } else {
    color_mask = regs.colorsub_mask;
  }

  if(color_mask == 0)return false;
  if(color_mask == 3)return true;

  window1_left  = regs.window1_left;
  window1_right = regs.window1_right;
  window2_left  = regs.window2_left;
  window2_right = regs.window2_right;

  if(regs.bg_mode == 5 || regs.bg_mode == 6) {
    window1_left  <<= 1;
    window1_right <<= 1;
    window2_left  <<= 1;
    window2_right <<= 1;
  }

  if(regs.color_window1_enabled == false && regs.color_window2_enabled == false) {
    r = true;
  } else if(regs.color_window1_enabled == true && regs.color_window2_enabled == false) {
    if(regs.color_window1_invert == false) {
      if(x >= window1_left && x <= window1_right)r = false;
      else r = true;
    } else {
      if(x <  window1_left || x >  window1_right)r = false;
      else r = true;
    }
  } else if(regs.color_window1_enabled == false && regs.color_window2_enabled == true) {
    if(regs.color_window2_invert == false) {
      if(x >= window2_left && x <= window2_right)r = false;
      else r = true;
    } else {
      if(x <  window2_left || x >  window2_right)r = false;
      else r = true;
    }
  } else if(regs.color_window1_enabled == true && regs.color_window2_enabled == true) {
    if(regs.color_window1_invert == false) {
      if(x >= window1_left && x <= window1_right)w1_mask = 1;
      else w1_mask = 0;
    } else {
      if(x <  window1_left || x >  window1_right)w1_mask = 1;
      else w1_mask = 0;
    }

    if(regs.color_window2_invert == false) {
      if(x >= window2_left && x <= window2_right)w2_mask = 1;
      else w2_mask = 0;
    } else {
      if(x <  window2_left || x >  window2_right)w2_mask = 1;
      else w2_mask = 0;
    }

    switch(regs.color_window_mask) {
    case WINDOWMASK_OR:
      if((w1_mask | w2_mask) == 1)r = false;
      else r = true;
      break;
    case WINDOWMASK_AND:
      if((w1_mask & w2_mask) == 1)r = false;
      else r = true;
      break;
    case WINDOWMASK_XOR:
      if((w1_mask ^ w2_mask) == 1)r = false;
      else r = true;
      break;
    case WINDOWMASK_XNOR:
      if((w1_mask ^ w2_mask) == 0)r = false;
      else r = true;
      break;
    }
  }

  if(color_mask == 2) {
    r = (r == true)?false:true;
  }

  return r;
}
