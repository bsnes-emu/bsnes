#define RENDER_MAIN 0
#define RENDER_SUB  1

#include "ppu_render.cpp"

byte layer_bg_lookup_mode0[12] = {
  BG4, BG3, OAM, BG4, BG3, OAM, BG2, BG1, OAM, BG2, BG1, OAM
};

void ppu_render_line_mode0(void) {
  ppu_render_line_bg (7, 10, COLORDEPTH_4, BG1);
  ppu_render_line_bg (6,  9, COLORDEPTH_4, BG2);
  ppu_render_line_bg (1,  4, COLORDEPTH_4, BG3);
  ppu_render_line_bg (0,  3, COLORDEPTH_4, BG4);
  ppu_render_line_oam(2,  5, 8, 11);
  ppu_set_layer_pixels(12, layer_bg_lookup_mode0);
}

byte layer_bg_lookup_mode1_pri0[10] = {
  BG3, OAM, BG3, OAM, BG2, BG1, OAM, BG2, BG1, OAM
};

byte layer_bg_lookup_mode1_pri1[10] = {
  BG3, OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM, BG3
};

void ppu_render_line_mode1(void) {
  switch(ppu.bg_priority_mode) {
  case 0:
    ppu_render_line_bg (5, 8, COLORDEPTH_16, BG1);
    ppu_render_line_bg (4, 7, COLORDEPTH_16, BG2);
    ppu_render_line_bg (0, 2, COLORDEPTH_4,  BG3);
    ppu_render_line_oam(1, 3, 6, 9);
    ppu_set_layer_pixels(10, layer_bg_lookup_mode1_pri0);
    break;
  case 1:
    ppu_render_line_bg (4, 7, COLORDEPTH_16, BG1);
    ppu_render_line_bg (3, 6, COLORDEPTH_16, BG2);
    ppu_render_line_bg (0, 9, COLORDEPTH_4,  BG3);
    ppu_render_line_oam(1, 2, 5, 8);
    ppu_set_layer_pixels(10, layer_bg_lookup_mode1_pri1);
    break;
  }
}

byte layer_bg_lookup_mode2[8] = {
  OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM
};

void ppu_render_line_mode2(void) {
  ppu_render_line_bg (3, 6, COLORDEPTH_16, BG1);
  ppu_render_line_bg (2, 5, COLORDEPTH_16, BG2);
  ppu_render_line_oam(0, 1, 4, 7);
  ppu_set_layer_pixels(8, layer_bg_lookup_mode2);
}

byte layer_bg_lookup_mode3[8] = {
  OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM
};

void ppu_render_line_mode3(void) {
  ppu_render_line_bg (3, 6, COLORDEPTH_256, BG1);
  ppu_render_line_bg (2, 5, COLORDEPTH_16,  BG2);
  ppu_render_line_oam(0, 1, 4, 7);
  ppu_set_layer_pixels(8, layer_bg_lookup_mode3);
}

byte layer_bg_lookup_mode4[8] = {
  OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM
};

void ppu_render_line_mode4(void) {
  ppu_render_line_bg (3, 6, COLORDEPTH_256, BG1);
  ppu_render_line_bg (2, 5, COLORDEPTH_4,   BG2);
  ppu_render_line_oam(0, 1, 4, 7);
  ppu_set_layer_pixels(8, layer_bg_lookup_mode4);
}

byte layer_bg_lookup_mode5[8] = {
  OAM, OAM, BG2, BG1, OAM, BG2, BG1, OAM
};

void ppu_render_line_mode5(void) {
  ppu_render_line_bg (3, 6, COLORDEPTH_16, BG1);
  ppu_render_line_bg (2, 5, COLORDEPTH_4,  BG2);
  ppu_render_line_oam(0, 1, 4, 7);
  ppu_set_layer_pixels(8, layer_bg_lookup_mode5);
}

byte layer_bg_lookup_mode6[6] = {
  OAM, OAM, BG1, OAM, BG1, OAM
};

void ppu_render_line_mode6(void) {
  ppu_render_line_bg (2, 4, COLORDEPTH_16, BG1);
  ppu_render_line_oam(0, 1, 3, 5);
  ppu_set_layer_pixels(8, layer_bg_lookup_mode6);
}

byte layer_bg_lookup_mode7[5] = {
  OAM, BG1, OAM, OAM, OAM
};

byte layer_bg_lookup_mode7_extbg[6] = {
  BG2, OAM, OAM, BG2, OAM, OAM
};

void ppu_render_line_mode7(void) {
  if(ppu.mode7_extbg == false) {
    ppu_render_line_m7 (1, 0, 0); //bg2 priorities are ignored
    ppu_render_line_oam(0, 2, 3, 4);
    ppu_set_layer_pixels(5, layer_bg_lookup_mode7);
  } else {
    ppu_render_line_m7 (0, 0, 3); //bg1 priority is ignored
    ppu_render_line_oam(1, 2, 4, 5);
    ppu_set_layer_pixels(6, layer_bg_lookup_mode7_extbg);
  }
}

void ppu_render_scanline(void) {
int x, y;
  ppu.vline_pos = snes_time->vscan_pos;
  ppu.hirq_triggered = false;

//new screen initialize
  if(ppu.vline_pos == 0) {
    hdma_initialize();
    ppu.virq_triggered = false;
    gx816->nmi_pin = 1;
  }

//screen refresh
  if(ppu.vline_pos == ppu.visible_scanlines) {
    if(render.frame_count == 0) {
      UpdateDisplay();
    }
    render.frame_count++;
    if(render.frame_count >= render.frame_skip) {
      render.frame_count = 0;
    }
  }

//automatic joypad read
  if(ppu.vline_pos == (ppu.visible_scanlines + 1) && ppu.auto_joypad_read == true) {
    UpdateJoypad();
  }

  y = ppu.vline_pos;
  if(y > 0 && y < ppu.visible_scanlines && (render.frame_skip == 0 || render.frame_count == 0)) {
    if(ppu.display_disable == true) {
      memset(ppu.screen + (y << 1) * 512, 0, 2048);
    } else {
      ppu_clear_layer_cache();
      ppu_clear_pixel_cache();
      switch(ppu.bg_mode) {
      case 0:ppu_render_line_mode0();break;
      case 1:ppu_render_line_mode1();break;
      case 2:ppu_render_line_mode2();break;
      case 3:ppu_render_line_mode3();break;
      case 4:ppu_render_line_mode4();break;
      case 5:ppu_render_line_mode5();break;
      case 6:ppu_render_line_mode6();break;
      case 7:ppu_render_line_mode7();break;
      }
      ppu_render_line_to_screen();
    }
  }
}

void ppu_update_scanline(void) {
static bool hdma_triggered = false;
word current_vscan_pos;
//starting a new screen?
  if(snes_time->vscan_wrapped == true) {
    snes_time->vscan_wrapped = false;
  }

  if(snes_time->hscan_wrapped == true) {
    snes_time->hscan_wrapped = false;
    hdma_triggered           = false;
    ppu_render_scanline();
  }

  if(snes_time->hscan_pos >= 278 && hdma_triggered == false) {
    hdma_update();
    hdma_triggered = true;
  }

  if(gx816->cpu_state == CPUSTATE_STP)return;

  if(!(gx816->regs.p & PF_I)) {
    if(ppu.vcounter_enabled == true && ppu.hcounter_enabled == true) {
      if(snes_time->vscan_pos == ppu.virq_pos && ppu.virq_triggered == false) {
        if(snes_time->hscan_pos >= ppu.hirq_pos && ppu.hirq_triggered == false) {
          ppu.irq_triggered      = true;
          ppu.virq_triggered     = true;
          ppu.hirq_triggered     = true;
          gx816->InvokeIRQ(0xffee);
        }
      }
    } else if(ppu.vcounter_enabled == true) {
      if(snes_time->vscan_pos == ppu.virq_pos && ppu.virq_triggered == false) {
        ppu.irq_triggered      = true;
        ppu.virq_triggered     = true;
        gx816->InvokeIRQ(0xffee);
      }
    } else if(ppu.hcounter_enabled == true) {
      if(snes_time->hscan_pos >= ppu.hirq_pos && ppu.hirq_triggered == false) {
        ppu.irq_triggered      = true;
        ppu.hirq_triggered     = true;
        gx816->InvokeIRQ(0xffee);
      }
    }
  }
}

byte oam_read(word addr) {
byte r;
  addr &= 1023;
  if(addr >= 512) {
    addr &= 31;
    r = ppu.oam[addr + 512];
    debug_test_bp(BPSRC_OAM, BP_READ, addr + 512, r);
  } else {
    r = ppu.oam[addr];
    debug_test_bp(BPSRC_OAM, BP_READ, addr, r);
  }
  return r;
}

void oam_write(word addr, byte value) {
  addr &= 1023;
  if(addr >= 512) {
    addr &= 31;
    ppu.oam[addr + 512] = value;
    debug_test_bp(BPSRC_OAM, BP_WRITE, addr + 512, value);
  } else {
    ppu.oam[addr] = value;
    debug_test_bp(BPSRC_OAM, BP_WRITE, addr, value);
  }
}

void PPUInit(byte first_time) {
int i, l;
byte r, g, b;
double m;
word *ptr;
  if(first_time == 1) {
    ppu.screen      = (word*)malloc(512 * 478 * 2);
    ppu.vram        = (byte*)malloc(0x10000);
    ppu.cgram       = (byte*)malloc(512);
    ppu.oam         = (byte*)malloc(544);
    ppu.light_table = (word*)malloc(16 * 65536 * 2);
    ppu_init_tiledata_cache();

    for(l=0;l<16;l++) {
      ppu.mosaic_table[l] = (word*)malloc(4096 * 2);
      for(i=0;i<4096;i++) {
        ppu.mosaic_table[l][i] = (i / (l + 1)) * (l + 1);
      }
    }

    ptr = (word*)ppu.light_table;
    for(l=0;l<16;l++) {
      m = (double)l / 15.0;
      for(i=0;i<65536;i++) {
        r = (i      ) & 31;
        g = (i >>  5) & 31;
        b = (i >> 10) & 31;
        if(l == 0) { r = g = b = 0; }
        else if(l == 15);
        else {
          r = (double)r * m;
          g = (double)g * m;
          b = (double)b * m;
        }
        *ptr++ = (r) | (g << 5) | (b << 10);
      }
    }
  }
  ppu_clear_tiledata_cache();
  memset(ppu.screen, 0, 512 * 478 * 2);
  memset(ppu.vram,   0, 0x10000);
  memset(ppu.cgram,  0, 512);
  memset(ppu.oam,    0, 544);
  ppu.ppu_cycles         = 0;
  ppu.ppu_prev_cycles    = 0;
  ppu.display_disable    = true;
  ppu.display_brightness = 15;

//ppu.interlace/ppu.interlace_frame initialized in timing/timing.cpp
  ppu.overscan          = false;
  ppu.visible_scanlines = 224;
  ppu.sprite_halve      = false;
  ppu.hline_pos         = 0;
  ppu.vline_pos         = 0;
  ppu.irq_triggered     = false;
  ppu.virq_triggered    = false;
  ppu.hirq_triggered    = false;
  ppu.vram_write_pos    = 0;
  ppu.vram_read_buffer  = 0;
  ppu.vram_write_buffer = 0;
  ppu.cgram_write_pos   = 0;
  ppu.wram_write_pos    = 0;
  ppu.vram_remap_mode   = 0;
  ppu.vram_inc_size     = 2;
  ppu.vram_inc_reg      = 0;
  ppu.oam_write_pos     = 0;
  ppu.oam_tiledata_loc  = 0;

  ppu.bg_enabled[OAM]              = false;
  ppu.ss_bg_enabled[OAM]           = false;
  ppu.mosaic_size                  = 0;
  ppu.mosaic_enabled[BG4]          = false;
  ppu.mosaic_enabled[BG3]          = false;
  ppu.mosaic_enabled[BG2]          = false;
  ppu.mosaic_enabled[BG1]          = false;
  ppu.bg_windowing_enabled[OAM]    = false;
  ppu.ss_bg_windowing_enabled[OAM] = false;
  ppu.bg_window1_enabled[OAM]      = false;
  ppu.bg_window2_enabled[OAM]      = false;
  ppu.bg_window1_clipmode[OAM]     = false;
  ppu.bg_window2_clipmode[OAM]     = false;
  ppu.bg_window_mask[OAM]          = false;

  for(i=0;i<4;i++) {
    ppu.bg_enabled[i]              = false;
    ppu.ss_bg_enabled[i]           = false;
    ppu.bg_window1_enabled[i]      = false;
    ppu.bg_window2_enabled[i]      = false;
    ppu.bg_windowing_enabled[i]    = false;
    ppu.ss_bg_windowing_enabled[i] = false;
    ppu.bg_window1_clipmode[i]     = 0;
    ppu.bg_window2_clipmode[i]     = 0;
    ppu.bg_window_mask[i]          = 0;
    ppu.bg_tilemap_loc[i]          = 0;
    ppu.bg_tiledata_loc[i]         = 0;
    ppu.bg_hscroll_pos[i]          = 0;
    ppu.bg_vscroll_pos[i]          = 0;
  }
  ppu.bg_priority_mode    = 0;
  ppu.oam_base_size       = 0;
  ppu.oam_name_sel        = 0;
  ppu.bg_mode             = 0;

  ppu.mul_a               = 0;
  ppu.mul_b               = 0;
  ppu.div_a               = 0;
  ppu.div_b               = 0;
  ppu.r_4214              = 0;
  ppu.r_4216              = 0;
  ppu.r_2134              = 0;

  ppu.window1_left        = 0;
  ppu.window1_right       = 0;
  ppu.window2_left        = 0;
  ppu.window2_right       = 0;

  ppu.color_window1_enabled  = 0;
  ppu.color_window2_enabled  = 0;
  ppu.color_window1_clipmode = 0;
  ppu.color_window2_clipmode = 0;
  ppu.color_window_mask      = 0;
  ppu.color_mask             = 0;
  ppu.ss_color_mask          = 0;
  ppu.addsub_mode            = 0;
  ppu.color_mode             = 0;
  ppu.color_halve            = 0;
  for(i=0;i<6;i++) {
    ppu.bg_color_enabled[i]  = false;
  }
  ppu.color_r = 0;
  ppu.color_g = 0;
  ppu.color_b = 0;

  ppu.active_hdma_channels = 0;
  for(i=0;i<8;i++) {
    memset(&dma_channel[i], 0, sizeof(dmachannel));
    ppu.hdma_completed[i]           = false;
    ppu.hdma_scanlines_remaining[i] = 0;
    ppu.hdma_index_pointer[i]       = 0;
  }
  hdma_initialize();

  ppu.vcounter_enabled = false;
  ppu.hcounter_enabled = false;
  ppu.hirq_pos         = 0;
  ppu.virq_pos         = 0;

  ppu.auto_joypad_read    = false;
  ppu.joypad_strobe_value = 0;

  ppu.latch_toggle        = 0;
  ppu.latch_vpos          = 0;
  ppu.latch_hpos          = 0;

  ppu.m7a = ppu.m7b =
  ppu.m7c = ppu.m7d =
  ppu.m7x = ppu.m7y = 0;

  ppu.m7hofs = ppu.m7vofs = 0x0000;

  ppu.mode7_repeat = 0;
  ppu.mode7_extbg  = false;
  ppu.mode7_hflip  = false;
  ppu.mode7_vflip  = false;

  ppu.io4201 = 0xff;
  ppu.counter_latched = false;

  memset(ppu.mmio_mem_43xx, 0, 0x80);
}
