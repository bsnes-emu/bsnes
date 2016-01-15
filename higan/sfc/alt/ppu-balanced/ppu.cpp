#include <sfc/sfc.hpp>

namespace SuperFamicom {

PPU ppu;

#include "memory/memory.cpp"
#include "mmio/mmio.cpp"
#include "render/render.cpp"
#include "serialization.cpp"

PPU::PPU() {
  output = new uint32[512 * 512]();
  output += 16 * 512;  //overscan offset

  alloc_tiledata_cache();

  for(uint l : range(16)) {
    for(uint i : range(4096)) {
      mosaic_table[l][i] = (i / (l + 1)) * (l + 1);
    }
  }

  layer_enabled[BG1][0] = true;
  layer_enabled[BG1][1] = true;
  layer_enabled[BG2][0] = true;
  layer_enabled[BG2][1] = true;
  layer_enabled[BG3][0] = true;
  layer_enabled[BG3][1] = true;
  layer_enabled[BG4][0] = true;
  layer_enabled[BG4][1] = true;
  layer_enabled[OAM][0] = true;
  layer_enabled[OAM][1] = true;
  layer_enabled[OAM][2] = true;
  layer_enabled[OAM][3] = true;
  frameskip = 0;
  framecounter = 0;
}

PPU::~PPU() {
  output -= 16 * 512;
  delete[] output;
  free_tiledata_cache();
}

auto PPU::step(uint clocks) -> void {
  clock += clocks;
}

auto PPU::synchronizeCPU() -> void {
  if(CPU::Threaded == true) {
    if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
  } else {
    while(clock >= 0) cpu.enter();
  }
}

auto PPU::Enter() -> void { ppu.enter(); }

auto PPU::enter() -> void {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    //H =    0 (initialize)
    scanline();
    add_clocks(10);

    //H =   10 (cache mode7 registers + OAM address reset)
    cache.m7_hofs = regs.m7_hofs;
    cache.m7_vofs = regs.m7_vofs;
    cache.m7a = regs.m7a;
    cache.m7b = regs.m7b;
    cache.m7c = regs.m7c;
    cache.m7d = regs.m7d;
    cache.m7x = regs.m7x;
    cache.m7y = regs.m7y;
    if(vcounter() == (!overscan() ? 225 : 240)) {
      if(regs.display_disabled == false) {
        regs.oam_addr = regs.oam_baseaddr << 1;
        regs.oam_firstsprite = (regs.oam_priority == false) ? 0 : (regs.oam_addr >> 2) & 127;
      }
    }
    add_clocks(502);

    //H =  512 (render)
    render_scanline();
    add_clocks(640);

    //H = 1152 (cache OBSEL)
    if(cache.oam_basesize != regs.oam_basesize) {
      cache.oam_basesize = regs.oam_basesize;
      sprite_list_valid = false;
    }
    cache.oam_nameselect = regs.oam_nameselect;
    cache.oam_tdaddr = regs.oam_tdaddr;
    add_clocks(lineclocks() - 1152);  //seek to start of next scanline

  }
}

auto PPU::add_clocks(uint clocks) -> void {
  tick(clocks);
  step(clocks);
  synchronizeCPU();
}

auto PPU::scanline() -> void {
  line = vcounter();

  if(line == 0) {
    frame();

    //RTO flag reset
    regs.time_over  = false;
    regs.range_over = false;
  }

  if(line == 1) {
    //mosaic reset
    for(int bg = BG1; bg <= BG4; bg++) regs.bg_y[bg] = 1;
    regs.mosaic_countdown = regs.mosaic_size + 1;
    regs.mosaic_countdown--;
  } else {
    for(int bg = BG1; bg <= BG4; bg++) {
      if(!regs.mosaic_enabled[bg] || !regs.mosaic_countdown) regs.bg_y[bg] = line;
    }
    if(!regs.mosaic_countdown) regs.mosaic_countdown = regs.mosaic_size + 1;
    regs.mosaic_countdown--;
  }

  if(line == 241) {
    scheduler.exit(Scheduler::ExitReason::FrameEvent);
  }
}

auto PPU::render_scanline() -> void {
  if(line >= 1 && line < (!overscan() ? 225 : 240)) {
    if(framecounter) return;
    render_line_oam_rto();
    render_line();
  }
}

auto PPU::frame() -> void {
  if(field() == 0) {
    display.interlace = regs.interlace;
    regs.scanlines = (regs.overscan == false) ? 224 : 239;
  }

  framecounter = (frameskip == 0 ? 0 : (framecounter + 1) % frameskip);
}

auto PPU::enable() -> void {
  function<auto (uint, uint8) -> uint8> reader{&PPU::mmio_read, (PPU*)&ppu};
  function<auto (uint, uint8) -> void> writer{&PPU::mmio_write, (PPU*)&ppu};

  bus.map(reader, writer, 0x00, 0x3f, 0x2100, 0x213f);
  bus.map(reader, writer, 0x80, 0xbf, 0x2100, 0x213f);
}

auto PPU::power() -> void {
  for(auto& n : vram) n = 0x00;
  for(auto& n : oam) n = 0x00;
  for(auto& n : cgram) n = 0x00;
  flush_tiledata_cache();

  region = (system.region() == System::Region::NTSC ? 0 : 1);  //0 = NTSC, 1 = PAL

  regs.ioamaddr   = 0x0000;
  regs.icgramaddr = 0x01ff;

  //$2100
  regs.display_disabled   = true;
  regs.display_brightness = 15;

  //$2101
  regs.oam_basesize    = 0;
  regs.oam_nameselect  = 0;
  regs.oam_tdaddr      = 0x0000;

  cache.oam_basesize   = 0;
  cache.oam_nameselect = 0;
  cache.oam_tdaddr     = 0x0000;

  //$2102-$2103
  regs.oam_baseaddr    = 0x0000;
  regs.oam_addr        = 0x0000;
  regs.oam_priority    = false;
  regs.oam_firstsprite = 0;

  //$2104
  regs.oam_latchdata = 0x00;

  //$2105
  regs.bg_tilesize[BG1] = 0;
  regs.bg_tilesize[BG2] = 0;
  regs.bg_tilesize[BG3] = 0;
  regs.bg_tilesize[BG4] = 0;
  regs.bg3_priority     = 0;
  regs.bg_mode          = 0;

  //$2106
  regs.mosaic_size         = 0;
  regs.mosaic_enabled[BG1] = false;
  regs.mosaic_enabled[BG2] = false;
  regs.mosaic_enabled[BG3] = false;
  regs.mosaic_enabled[BG4] = false;
  regs.mosaic_countdown    = 0;

  //$2107-$210a
  regs.bg_scaddr[BG1] = 0x0000;
  regs.bg_scaddr[BG2] = 0x0000;
  regs.bg_scaddr[BG3] = 0x0000;
  regs.bg_scaddr[BG4] = 0x0000;
  regs.bg_scsize[BG1] = SC_32x32;
  regs.bg_scsize[BG2] = SC_32x32;
  regs.bg_scsize[BG3] = SC_32x32;
  regs.bg_scsize[BG4] = SC_32x32;

  //$210b-$210c
  regs.bg_tdaddr[BG1] = 0x0000;
  regs.bg_tdaddr[BG2] = 0x0000;
  regs.bg_tdaddr[BG3] = 0x0000;
  regs.bg_tdaddr[BG4] = 0x0000;

  //$210d-$2114
  regs.bg_ofslatch = 0x00;
  regs.m7_hofs = regs.m7_vofs = 0x0000;
  regs.bg_hofs[BG1] = regs.bg_vofs[BG1] = 0x0000;
  regs.bg_hofs[BG2] = regs.bg_vofs[BG2] = 0x0000;
  regs.bg_hofs[BG3] = regs.bg_vofs[BG3] = 0x0000;
  regs.bg_hofs[BG4] = regs.bg_vofs[BG4] = 0x0000;

  //$2115
  regs.vram_incmode = 1;
  regs.vram_mapping = 0;
  regs.vram_incsize = 1;

  //$2116-$2117
  regs.vram_addr = 0x0000;

  //$211a
  regs.mode7_repeat = 0;
  regs.mode7_vflip  = false;
  regs.mode7_hflip  = false;

  //$211b-$2120
  regs.m7_latch = 0x00;
  regs.m7a = 0x0000;
  regs.m7b = 0x0000;
  regs.m7c = 0x0000;
  regs.m7d = 0x0000;
  regs.m7x = 0x0000;
  regs.m7y = 0x0000;

  //$2121
  regs.cgram_addr = 0x0000;

  //$2122
  regs.cgram_latchdata = 0x00;

  //$2123-$2125
  regs.window1_enabled[BG1] = false;
  regs.window1_enabled[BG2] = false;
  regs.window1_enabled[BG3] = false;
  regs.window1_enabled[BG4] = false;
  regs.window1_enabled[OAM] = false;
  regs.window1_enabled[COL] = false;

  regs.window1_invert [BG1] = false;
  regs.window1_invert [BG2] = false;
  regs.window1_invert [BG3] = false;
  regs.window1_invert [BG4] = false;
  regs.window1_invert [OAM] = false;
  regs.window1_invert [COL] = false;

  regs.window2_enabled[BG1] = false;
  regs.window2_enabled[BG2] = false;
  regs.window2_enabled[BG3] = false;
  regs.window2_enabled[BG4] = false;
  regs.window2_enabled[OAM] = false;
  regs.window2_enabled[COL] = false;

  regs.window2_invert [BG1] = false;
  regs.window2_invert [BG2] = false;
  regs.window2_invert [BG3] = false;
  regs.window2_invert [BG4] = false;
  regs.window2_invert [OAM] = false;
  regs.window2_invert [COL] = false;

  //$2126-$2129
  regs.window1_left  = 0x00;
  regs.window1_right = 0x00;
  regs.window2_left  = 0x00;
  regs.window2_right = 0x00;

  //$212a-$212b
  regs.window_mask[BG1] = 0;
  regs.window_mask[BG2] = 0;
  regs.window_mask[BG3] = 0;
  regs.window_mask[BG4] = 0;
  regs.window_mask[OAM] = 0;
  regs.window_mask[COL] = 0;

  //$212c-$212d
  regs.bg_enabled[BG1]    = false;
  regs.bg_enabled[BG2]    = false;
  regs.bg_enabled[BG3]    = false;
  regs.bg_enabled[BG4]    = false;
  regs.bg_enabled[OAM]    = false;
  regs.bgsub_enabled[BG1] = false;
  regs.bgsub_enabled[BG2] = false;
  regs.bgsub_enabled[BG3] = false;
  regs.bgsub_enabled[BG4] = false;
  regs.bgsub_enabled[OAM] = false;

  //$212e-$212f
  regs.window_enabled[BG1]     = false;
  regs.window_enabled[BG2]     = false;
  regs.window_enabled[BG3]     = false;
  regs.window_enabled[BG4]     = false;
  regs.window_enabled[OAM]     = false;
  regs.sub_window_enabled[BG1] = false;
  regs.sub_window_enabled[BG2] = false;
  regs.sub_window_enabled[BG3] = false;
  regs.sub_window_enabled[BG4] = false;
  regs.sub_window_enabled[OAM] = false;

  //$2130
  regs.color_mask    = 0;
  regs.colorsub_mask = 0;
  regs.addsub_mode   = false;
  regs.direct_color  = false;

  //$2131
  regs.color_mode          = 0;
  regs.color_halve         = false;
  regs.color_enabled[BACK] = false;
  regs.color_enabled[OAM]  = false;
  regs.color_enabled[BG4]  = false;
  regs.color_enabled[BG3]  = false;
  regs.color_enabled[BG2]  = false;
  regs.color_enabled[BG1]  = false;

  //$2132
  regs.color_r   = 0x00;
  regs.color_g   = 0x00;
  regs.color_b   = 0x00;
  regs.color_rgb = 0x0000;

  //$2133
  regs.mode7_extbg   = false;
  regs.pseudo_hires  = false;
  regs.overscan      = false;
  regs.scanlines     = 224;
  regs.oam_interlace = false;
  regs.interlace     = false;

  //$2137
  regs.hcounter         = 0;
  regs.vcounter         = 0;
  regs.latch_hcounter   = 0;
  regs.latch_vcounter   = 0;
  regs.counters_latched = false;

  //$2139-$213a
  regs.vram_readbuffer = 0x0000;

  //$213e
  regs.time_over  = false;
  regs.range_over = false;

  reset();
}

auto PPU::reset() -> void {
  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill(output, 512 * 480 * sizeof(uint32));

  frame();

  //$2100
  regs.display_disabled = true;

  display.interlace = false;
  display.overscan  = false;
  regs.scanlines    = 224;

  memset(sprite_list, 0, sizeof(sprite_list));
  sprite_list_valid = false;

  //open bus support
  regs.ppu1_mdr = 0xff;
  regs.ppu2_mdr = 0xff;

  //bg line counters
  regs.bg_y[0] = 0;
  regs.bg_y[1] = 0;
  regs.bg_y[2] = 0;
  regs.bg_y[3] = 0;
}

auto PPU::layer_enable(uint layer, uint priority, bool enable) -> void {
  switch(layer * 4 + priority) {
  case  0: layer_enabled[BG1][0] = enable; break;
  case  1: layer_enabled[BG1][1] = enable; break;
  case  4: layer_enabled[BG2][0] = enable; break;
  case  5: layer_enabled[BG2][1] = enable; break;
  case  8: layer_enabled[BG3][0] = enable; break;
  case  9: layer_enabled[BG3][1] = enable; break;
  case 12: layer_enabled[BG4][0] = enable; break;
  case 13: layer_enabled[BG4][1] = enable; break;
  case 16: layer_enabled[OAM][0] = enable; break;
  case 17: layer_enabled[OAM][1] = enable; break;
  case 18: layer_enabled[OAM][2] = enable; break;
  case 19: layer_enabled[OAM][3] = enable; break;
  }
}

auto PPU::set_frameskip(uint frameskip_) -> void {
  frameskip = frameskip_;
  framecounter = 0;
}

}
