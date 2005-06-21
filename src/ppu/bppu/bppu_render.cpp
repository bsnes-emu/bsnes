#include "bppu_render_cache.cpp"
#include "bppu_render_windows.cpp"
#include "bppu_render_bg.cpp"
#include "bppu_render_oam.cpp"
#include "bppu_render_mode7.cpp"
#include "bppu_render_addsub.cpp"
#include "bppu_render_line.cpp"

/*
Mode 0: ->
     1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12
  BG4B, BG3B, OAM0, BG4A, BG3A, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3
*/
inline void bPPU::render_line_mode0() {
  render_line_bg(BG1, COLORDEPTH_4, 8, 11);
  render_line_bg(BG2, COLORDEPTH_4, 7, 10);
  render_line_bg(BG3, COLORDEPTH_4, 2,  5);
  render_line_bg(BG4, COLORDEPTH_4, 1,  4);
  render_line_oam(3, 6, 9, 12);
}

/*
Mode 1 (pri=0): ->
     1,    2,    3,    4,    5,    6,    7,    8,    9,   10
  BG3B, OAM0, BG3A, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3

Mode 1 (pri=1): ->
     1,    2,    3,    4,    5,    6,    7,    8,    9,   10
  BG3B, OAM0, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3, BG3A
*/
inline void bPPU::render_line_mode1() {
  switch(regs.bg3_priority) {
  case 0:
    render_line_bg(BG1, COLORDEPTH_16, 6,  9);
    render_line_bg(BG2, COLORDEPTH_16, 5,  8);
    render_line_bg(BG3, COLORDEPTH_4,  1,  3);
    render_line_oam(2, 4, 7, 10);
    break;
  case 1:
    render_line_bg(BG1, COLORDEPTH_16, 5,  8);
    render_line_bg(BG2, COLORDEPTH_16, 4,  7);
    render_line_bg(BG3, COLORDEPTH_4,  1, 10);
    render_line_oam(2, 3, 6, 9);
    break;
  }
}

/*
Mode 2: ->
     1,    2,    3,    4,    5,    6,    7,    8
  OAM0, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3
*/
inline void bPPU::render_line_mode2() {
  render_line_bg(BG1, COLORDEPTH_16, 4, 7);
  render_line_bg(BG2, COLORDEPTH_16, 3, 6);
  render_line_oam(1, 2, 5, 8);
}

/*
Mode 3: ->
     1,    2,    3,    4,    5,    6,    7,    8
  OAM0, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3
*/
inline void bPPU::render_line_mode3() {
  render_line_bg(BG1, COLORDEPTH_256, 4, 7);
  render_line_bg(BG2, COLORDEPTH_16,  3, 6);
}

/*
Mode 4: ->
     1,    2,    3,    4,    5,    6,    7,    8
  OAM0, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3
*/
inline void bPPU::render_line_mode4() {
  render_line_bg(BG1, COLORDEPTH_256, 4, 7);
  render_line_bg(BG2, COLORDEPTH_4,   3, 6);
  render_line_oam(1, 2, 5, 8);
}

/*
Mode 5: ->
     1,    2,    3,    4,    5,    6,    7,    8
  OAM0, OAM1, BG2B, BG1B, OAM2, BG2A, BG1A, OAM3
*/
inline void bPPU::render_line_mode5() {
  render_line_bg(BG1, COLORDEPTH_16, 4, 7);
  render_line_bg(BG2, COLORDEPTH_4,  3, 6);
  render_line_oam(1, 2, 5, 8);
}

/*
Mode 6: ->
     1,    2,    3,    4,    5,    6
  OAM0, OAM1, BG1B, OAM2, BG1A, OAM3
*/
inline void bPPU::render_line_mode6() {
  render_line_bg(BG1, COLORDEPTH_16, 3, 5);
  render_line_oam(1, 2, 4, 6);
}

/*
Mode7: ->
     1,    2,    3,    4,    5
  OAM0, BG1n, OAM1, OAM2, OAM3

Mode 7 (extbg): ->
     1,    2,    3,    4,    5,    6
  BG2B, OAM0, OAM1, BG2A, OAM2, OAM3
*/
inline void bPPU::render_line_mode7() {
  if(regs.mode7_extbg == false) {
    render_line_mode7(1, 0, 0); //bg2 priorities are ignored
    render_line_oam(0, 2, 3, 4);
  } else {
    render_line_mode7(0, 0, 3); //bg1 priority is ignored
    render_line_oam(1, 2, 4, 5);
  }
}

void bPPU::render_line() {
  _screen_width    = (regs.bg_mode == 5 || regs.bg_mode == 6)?512:256;
  _interlace       = clock->interlace();
  _interlace_field = clock->interlace_field();

  if(regs.display_disabled == true) {
    memset(output->buffer + (((_y << 1) + _interlace_field) << 9), 0, 1024);
    return;
  }

  clear_pixel_cache();
  build_color_window_tables();
  switch(regs.bg_mode) {
  case 0:render_line_mode0();break;
  case 1:render_line_mode1();break;
  case 2:render_line_mode2();break;
  case 3:render_line_mode3();break;
  case 4:render_line_mode4();break;
  case 5:render_line_mode5();break;
  case 6:render_line_mode6();break;
  case 7:render_line_mode7();break;
  }
  render_line_output();
}
