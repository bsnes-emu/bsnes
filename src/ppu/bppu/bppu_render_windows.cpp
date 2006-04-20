void bPPU::build_window_table(uint8 bg, bool mainscreen) {
uint8  set = 1, clr = 0;
uint8 *wtbl = (mainscreen == true) ? window[bg].main : window[bg].sub;
  if(bg != COL) {
    if(mainscreen == true && regs.window_enabled[bg] == false) {
      memset(wtbl, 0, 256);
      return;
    }
    if(mainscreen == false && regs.sub_window_enabled[bg] == false) {
      memset(wtbl, 0, 256);
      return;
    }
  } else {
    switch((mainscreen == true) ? regs.color_mask : regs.colorsub_mask) {
    case 0: //always
      memset(wtbl, 1, 256);
      return;
    case 3: //never
      memset(wtbl, 0, 256);
      return;
    case 1: //inside window only
      set = 1;
      clr = 0;
      break;
    case 2: //outside window only
      set = 0;
      clr = 1;
      break;
    }
  }

uint16 window1_left  = regs.window1_left;
uint16 window1_right = regs.window1_right;
uint16 window2_left  = regs.window2_left;
uint16 window2_right = regs.window2_right;
  if(regs.window1_enabled[bg] == false && regs.window2_enabled[bg] == false) {
    memset(wtbl, clr, 256);
    return;
  }

  if(regs.window1_enabled[bg] == true && regs.window2_enabled[bg] == false) {
    if(regs.window1_invert[bg] == true) {
      set ^= clr; clr ^= set; set ^= clr;
    }
    for(int x = 0; x < 256; x++) {
      wtbl[x] = (x >= window1_left && x <= window1_right) ? set : clr;
    }
    return;
  }

  if(regs.window1_enabled[bg] == false && regs.window2_enabled[bg] == true) {
    if(regs.window2_invert[bg] == true) {
      set ^= clr; clr ^= set; set ^= clr;
    }
    for(int x = 0; x < 256; x++) {
      wtbl[x] = (x >= window2_left && x <= window2_right) ? set : clr;
    }
    return;
  }

//(regs.window1_enabled[bg] == true && regs.window2_enabled[bg] == true)
int w1_mask, w2_mask; //1 = masked, 0 = not masked
  for(int x = 0; x < 256; x++) {
    w1_mask = (x >= window1_left && x <= window1_right);
    if(regs.window1_invert[bg] == true)w1_mask = !w1_mask;

    w2_mask = (x >= window2_left && x <= window2_right);
    if(regs.window2_invert[bg] == true)w2_mask = !w2_mask;

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

void bPPU::build_window_tables(uint8 bg) {
  build_window_table(bg, true);
  build_window_table(bg, false);
}
