void bPPU::build_window_table(uint8 bg, bool mainscreen) {
uint8 set = true, clr = false;
uint8 *wtbl;
  if(mainscreen == true) {
    wtbl = (uint8*)window_cache[bg].main;
  } else {
    wtbl = (uint8*)window_cache[bg].sub;
  }

  if(bg != COL) {
    if(mainscreen == true && regs.window_enabled[bg] == false) {
      memset(wtbl, 0, line.width);
      return;
    }
    if(mainscreen == false && regs.sub_window_enabled[bg] == false) {
      memset(wtbl, 0, line.width);
      return;
    }
  } else {
  uint8 mask;
    if(mainscreen == true) {
      mask = regs.color_mask;
    } else {
      mask = regs.colorsub_mask;
    }

    if(mask == 0) {
    //always
      memset(wtbl, 1, line.width);
      return;
    }

    if(mask == 3) {
    //never
      memset(wtbl, 0, line.width);
      return;
    }

    if(mask == 1) {
    //inside window only
      set = 1;
      clr = 0;
    } else { //mask == 2
    //outside window only
      set = 0;
      clr = 1;
    }
  }

uint16 window1_left, window1_right, window2_left, window2_right;
int    w1_mask, w2_mask; //1 = masked, 0 = not masked
int    x;
bool   r;
  window1_left  = regs.window1_left;
  window1_right = regs.window1_right;
  window2_left  = regs.window2_left;
  window2_right = regs.window2_right;

  if(line.width == 512) {
    window1_left  <<= 1;
    window1_right <<= 1;
    window2_left  <<= 1;
    window2_right <<= 1;
  }

  if(regs.window1_enabled[bg] == false && regs.window2_enabled[bg] == false) {
    memset(wtbl, clr, line.width);
  } else if(regs.window1_enabled[bg] == true && regs.window2_enabled[bg] == false) {
    if(regs.window1_invert[bg] == false) {
      for(x=0;x<line.width;x++) {
        wtbl[x] = (x >= window1_left && x <= window1_right) ? set : clr;
      }
    } else {
      for(x=0;x<line.width;x++) {
        wtbl[x] = (x <  window1_left || x >  window1_right) ? set : clr;
      }
    }
  } else if(regs.window1_enabled[bg] == false && regs.window2_enabled[bg] == true) {
    if(regs.window2_invert[bg] == false) {
      for(x=0;x<line.width;x++) {
        wtbl[x] = (x >= window2_left && x <= window2_right) ? set : clr;
      }
    } else {
      for(x=0;x<line.width;x++) {
        wtbl[x] = (x <  window2_left || x >  window2_right) ? set : clr;
      }
    }
  } else { //if(regs.window1_enabled[bg] == true && regs.window2_enabled[bg] == true) {
    for(x=0;x<line.width;x++) {
      if(regs.window1_invert[bg] == false) {
        w1_mask = (x >= window1_left && x <= window1_right);
      } else {
        w1_mask = (x <  window1_left || x >  window1_right);
      }

      if(regs.window2_invert[bg] == false) {
        w2_mask = (x >= window2_left && x <= window2_right);
      } else {
        w2_mask = (x <  window2_left || x >  window2_right);
      }

      switch(regs.window_mask[bg]) {
      case 0: //WINDOWMASK_OR:
        wtbl[x] = ((w1_mask | w2_mask) == 1) ? set : clr;
        break;
      case 1: //WINDOWMASK_AND:
        wtbl[x] = ((w1_mask & w2_mask) == 1) ? set : clr;
        break;
      case 2: //WINDOWMASK_XOR:
        wtbl[x] = ((w1_mask ^ w2_mask) == 1) ? set : clr;
        break;
      case 3: //WINDOWMASK_XNOR:
        wtbl[x] = ((w1_mask ^ w2_mask) == 0) ? set : clr;
        break;
      }
    }
  }
}

void bPPU::build_window_tables(uint8 bg) {
  if(window_cache[bg].main_dirty == true) {
    window_cache[bg].main_dirty = false;
    build_window_table(bg, true);
  }

  if(window_cache[bg].sub_dirty == true) {
    window_cache[bg].sub_dirty = false;
    build_window_table(bg, false);
  }
}
