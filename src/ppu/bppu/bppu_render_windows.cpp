void bPPU::build_window_table(uint8 bg, uint8 *wtbl, bool mainscreen) {
  if(mainscreen == true && regs.bg_window_enabled[bg] == false) {
    memset(wtbl, 0, _screen_width);
    return;
  }
  if(mainscreen == false && regs.bgsub_window_enabled[bg] == false) {
    memset(wtbl, 0, _screen_width);
    return;
  }

uint16 window1_left, window1_right, window2_left, window2_right;
int    w1_mask, w2_mask; //1 = masked, 0 = not masked
int    x;
bool   r;
  window1_left  = regs.window1_left;
  window1_right = regs.window1_right;
  window2_left  = regs.window2_left;
  window2_right = regs.window2_right;

  if(_screen_width == 512) {
    window1_left  <<= 1;
    window1_right <<= 1;
    window2_left  <<= 1;
    window2_right <<= 1;
  }

  if(regs.bg_window1_enabled[bg] == false && regs.bg_window2_enabled[bg] == false) {
    memset(wtbl, 0, _screen_width);
  } else if(regs.bg_window1_enabled[bg] == true && regs.bg_window2_enabled[bg] == false) {
    if(regs.bg_window1_invert[bg] == false) {
      for(x=0;x<_screen_width;x++) {
        wtbl[x] = (x >= window1_left && x <= window1_right)?true:false;
      }
    } else {
      for(x=0;x<_screen_width;x++) {
        wtbl[x] = (x <  window1_left || x >  window1_right)?true:false;
      }
    }
  } else if(regs.bg_window1_enabled[bg] == false && regs.bg_window2_enabled[bg] == true) {
    if(regs.bg_window2_invert[bg] == false) {
      for(x=0;x<_screen_width;x++) {
        wtbl[x] = (x >= window2_left && x <= window2_right)?true:false;
      }
    } else {
      for(x=0;x<_screen_width;x++) {
        wtbl[x] = (x <  window2_left || x >  window2_right)?true:false;
      }
    }
  } else { //if(regs.bg_window1_enabled[bg] == true && regs.bg_window2_enabled[bg] == true) {
    for(x=0;x<_screen_width;x++) {
      if(regs.bg_window1_invert[bg] == false) {
        w1_mask = (x >= window1_left && x <= window1_right);
      } else {
        w1_mask = (x <  window1_left || x >  window1_right);
      }

      if(regs.bg_window2_invert[bg] == false) {
        w2_mask = (x >= window2_left && x <= window2_right);
      } else {
        w2_mask = (x <  window2_left || x >  window2_right);
      }

      switch(regs.bg_window_mask[bg]) {
      case 0: //WINDOWMASK_OR:
        wtbl[x] = ((w1_mask | w2_mask) == 1)?true:false;
        break;
      case 1: //WINDOWMASK_AND:
        wtbl[x] = ((w1_mask & w2_mask) == 1)?true:false;
        break;
      case 2: //WINDOWMASK_XOR:
        wtbl[x] = ((w1_mask ^ w2_mask) == 1)?true:false;
        break;
      case 3: //WINDOWMASK_XNOR:
        wtbl[x] = ((w1_mask ^ w2_mask) == 0)?true:false;
        break;
      }
    }
  }
}

void bPPU::build_window_tables(uint8 bg) {
  build_window_table(bg, main_windowtable[bg], true);
  build_window_table(bg, sub_windowtable[bg],  false);
}

void bPPU::build_color_window_table(uint8 *wtbl, uint8 mask) {
  if(mask == 0) {
  //always
    memset(wtbl, 1, _screen_width);
    return;
  }

  if(mask == 3) {
  //never
    memset(wtbl, 0, _screen_width);
    return;
  }

int _true, _false;
  if(mask == 1) {
  //inside window only
    _true  = 1;
    _false = 0;
  } else { //mask == 2
  //outside window only
    _true  = 0;
    _false = 1;
  }

uint16 window1_left, window1_right, window2_left, window2_right;
int    w1_mask, w2_mask; //1 = masked, 0 = not masked
int    x;
bool   r;
  window1_left  = regs.window1_left;
  window1_right = regs.window1_right;
  window2_left  = regs.window2_left;
  window2_right = regs.window2_right;

  if(_screen_width == 512) {
    window1_left  <<= 1;
    window1_right <<= 1;
    window2_left  <<= 1;
    window2_right <<= 1;
  }

  if(regs.color_window1_enabled == false && regs.color_window2_enabled == false) {
    memset(wtbl, _false, _screen_width);
  } else if(regs.color_window1_enabled == true && regs.color_window2_enabled == false) {
    if(regs.color_window1_invert == false) {
      for(x=0;x<_screen_width;x++) {
        wtbl[x] = (x >= window1_left && x <= window1_right)?_true:_false;
      }
    } else {
      for(x=0;x<_screen_width;x++) {
        wtbl[x] = (x <  window1_left || x >  window1_right)?_true:_false;
      }
    }
  } else if(regs.color_window1_enabled == false && regs.color_window2_enabled == true) {
    if(regs.color_window2_invert == false) {
      for(x=0;x<_screen_width;x++) {
        wtbl[x] = (x >= window2_left && x <= window2_right)?_true:_false;
      }
    } else {
      for(x=0;x<_screen_width;x++) {
        wtbl[x] = (x <  window2_left || x >  window2_right)?_true:_false;
      }
    }
  } else { //if(regs.color_window1_enabled == true && regs.color_window2_enabled == true) {
    for(x=0;x<_screen_width;x++) {
      if(regs.color_window1_invert == false) {
        w1_mask = (x >= window1_left && x <= window1_right);
      } else {
        w1_mask = (x <  window1_left || x >  window1_right);
      }

      if(regs.color_window2_invert == false) {
        w2_mask = (x >= window2_left && x <= window2_right);
      } else {
        w2_mask = (x <  window2_left || x >  window2_right);
      }

      switch(regs.color_window_mask) {
      case 0: //WINDOWMASK_OR:
        wtbl[x] = ((w1_mask | w2_mask) == 1)?_true:_false;
        break;
      case 1: //WINDOWMASK_AND:
        wtbl[x] = ((w1_mask & w2_mask) == 1)?_true:_false;
        break;
      case 2: //WINDOWMASK_XOR:
        wtbl[x] = ((w1_mask ^ w2_mask) == 1)?_true:_false;
        break;
      case 3: //WINDOWMASK_XNOR:
        wtbl[x] = ((w1_mask ^ w2_mask) == 0)?_true:_false;
        break;
      }
    }
  }
}

void bPPU::build_color_window_tables() {
  build_color_window_table(main_colorwindowtable, regs.color_mask);
  build_color_window_table(sub_colorwindowtable,  regs.colorsub_mask);
}
