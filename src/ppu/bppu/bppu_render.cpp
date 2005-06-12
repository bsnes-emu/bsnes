#include "bppu_render_cache.cpp"
#include "bppu_render_windows.cpp"
#include "bppu_render_main.cpp"
#include "bppu_render_mode7.cpp"

namespace bPPURenderTables {
enum { BG1 = 0, BG2 = 1, BG3 = 2, BG4 = 3, OAM = 4, BACK = 5 };
  uint8 lookup_mode0[12] = {
    BG4, BG3, OAM, BG4, BG3, OAM, BG2, BG1, OAM, BG2, BG1, OAM
  };
  uint8 lookup_mode1_pri0[10] = {
    BG3, OAM, BG3, OAM, BG2, BG1, OAM, BG2, BG1, OAM
  };
  uint8 lookup_mode1_pri1[10] = {
    BG3, OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM, BG3
  };
  uint8 lookup_mode2[8] = {
    OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM
  };
  uint8 lookup_mode3[8] = {
    OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM
  };
  uint8 lookup_mode4[8] = {
    OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM
  };
  uint8 lookup_mode5[8] = {
    OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM
  };
  uint8 lookup_mode6[6] = {
    OAM, OAM, BG1, OAM, BG1, OAM
  };
  uint8 lookup_mode7[5] = {
    OAM, BG1, OAM, OAM, OAM
  };
  uint8 lookup_mode7_extbg[6] = {
    BG2, OAM, OAM, BG2, OAM, OAM
  };
};

void bPPU::render_line_mode0() {
  render_line_bg (7, 10, COLORDEPTH_4, BG1);
  render_line_bg (6,  9, COLORDEPTH_4, BG2);
  render_line_bg (1,  4, COLORDEPTH_4, BG3);
  render_line_bg (0,  3, COLORDEPTH_4, BG4);
  render_line_oam(2,  5, 8, 11);
  set_layer_pixels(12, bPPURenderTables::lookup_mode0);
}

void bPPU::render_line_mode1() {
  switch(regs.bg3_priority) {
  case 0:
    render_line_bg (5, 8, COLORDEPTH_16, BG1);
    render_line_bg (4, 7, COLORDEPTH_16, BG2);
    render_line_bg (0, 2, COLORDEPTH_4,  BG3);
    render_line_oam(1, 3, 6, 9);
    set_layer_pixels(10, bPPURenderTables::lookup_mode1_pri0);
    break;
  case 1:
    render_line_bg (4, 7, COLORDEPTH_16, BG1);
    render_line_bg (3, 6, COLORDEPTH_16, BG2);
    render_line_bg (0, 9, COLORDEPTH_4,  BG3);
    render_line_oam(1, 2, 5, 8);
    set_layer_pixels(10, bPPURenderTables::lookup_mode1_pri1);
    break;
  }
}

void bPPU::render_line_mode2() {
  render_line_bg (3, 6, COLORDEPTH_16, BG1);
  render_line_bg (2, 5, COLORDEPTH_16, BG2);
  render_line_oam(0, 1, 4, 7);
  set_layer_pixels(8, bPPURenderTables::lookup_mode2);
}

void bPPU::render_line_mode3() {
  render_line_bg (3, 6, COLORDEPTH_256, BG1);
  render_line_bg (2, 5, COLORDEPTH_16,  BG2);
  render_line_oam(0, 1, 4, 7);
  set_layer_pixels(8, bPPURenderTables::lookup_mode3);
}

void bPPU::render_line_mode4() {
  render_line_bg (3, 6, COLORDEPTH_256, BG1);
  render_line_bg (2, 5, COLORDEPTH_4,   BG2);
  render_line_oam(0, 1, 4, 7);
  set_layer_pixels(8, bPPURenderTables::lookup_mode4);
}

void bPPU::render_line_mode5() {
  render_line_bg (3, 6, COLORDEPTH_16, BG1);
  render_line_bg (2, 5, COLORDEPTH_4,  BG2);
  render_line_oam(0, 1, 4, 7);
  set_layer_pixels(8, bPPURenderTables::lookup_mode5);
}

void bPPU::render_line_mode6() {
  render_line_bg (2, 4, COLORDEPTH_16, BG1);
  render_line_oam(0, 1, 3, 5);
  set_layer_pixels(8, bPPURenderTables::lookup_mode6);
}

void bPPU::render_line_mode7() {
  if(regs.mode7_extbg == false) {
    render_line_m7 (1, 0, 0); //bg2 priorities are ignored
    render_line_oam(0, 2, 3, 4);
    set_layer_pixels(5, bPPURenderTables::lookup_mode7);
  } else {
    render_line_m7 (0, 0, 3); //bg1 priority is ignored
    render_line_oam(1, 2, 4, 5);
    set_layer_pixels(6, bPPURenderTables::lookup_mode7_extbg);
  }
}

void bPPU::render_line(uint16 line) {
  if(regs.display_disabled == true) {
    memset(output->buffer + (line << 1) * 512, 0, 2048);
    return;
  }

  clear_layer_cache();
  clear_pixel_cache();
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
  render_line_to_output();
}
