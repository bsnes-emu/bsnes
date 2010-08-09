#ifdef PPU_CPP

#include "render.cpp"

bool PPUDebugger::property(unsigned id, string &name, string &value) {
  unsigned n = 0;

  //internal
  if(id == n++) { name = "S-PPU1 MDR"; value = string("0x", strhex<2>(ppu1_mdr())); return true; }
  if(id == n++) { name = "S-PPU2 MDR"; value = string("0x", strhex<2>(ppu2_mdr())); return true; }

  //$2100
  if(id == n++) { name = "$2100"; value = ""; return true; }
  if(id == n++) { name = "Display Disable"; value = display_disable(); return true; }
  if(id == n++) { name = "Display Brightness"; value = display_brightness(); return true; }

  //$2101
  if(id == n++) { name = "$2101"; value = ""; return true; }
  if(id == n++) { name = "OAM Base Size"; value = oam_base_size(); return true; }
  if(id == n++) { name = "OAM Name Select"; value = oam_name_select(); return true; }
  if(id == n++) { name = "OAM Name Base Address"; value = string("0x", strhex<4>(oam_name_base_address())); return true; }

  //$2102-$2103
  if(id == n++) { name = "$2102-$2103"; value = ""; return true; }
  if(id == n++) { name = "OAM Base Address"; value = string("0x", strhex<4>(oam_base_address())); return true; }
  if(id == n++) { name = "OAM Priority"; value = oam_priority(); return true; }

  //$2105
  if(id == n++) { name = "$2105"; value = ""; return true; }
  if(id == n++) { name = "BG1 Tile Size"; value = bg1_tile_size() ? "16x16" : "8x8"; return true; }
  if(id == n++) { name = "BG2 Tile Size"; value = bg2_tile_size() ? "16x16" : "8x8"; return true; }
  if(id == n++) { name = "BG3 Tile Size"; value = bg3_tile_size() ? "16x16" : "8x8"; return true; }
  if(id == n++) { name = "BG4 Tile Size"; value = bg4_tile_size() ? "16x16" : "8x8"; return true; }
  if(id == n++) { name = "BG3 Priority"; value = bg3_priority(); return true; }
  if(id == n++) { name = "BG Mode"; value = bg_mode(); return true; }

  //$2106
  if(id == n++) { name = "$2106"; value = ""; return true; }
  if(id == n++) { name = "Mosaic Size"; value = mosaic_size(); return true; }
  if(id == n++) { name = "BG1 Mosaic Enable"; value = bg1_mosaic_enable(); return true; }
  if(id == n++) { name = "BG2 Mosaic Enable"; value = bg2_mosaic_enable(); return true; }
  if(id == n++) { name = "BG3 Mosaic Enable"; value = bg3_mosaic_enable(); return true; }
  if(id == n++) { name = "BG4 Mosaic Enable"; value = bg4_mosaic_enable(); return true; }

  static char screen_size[4][8] = { "32x32", "32x64", "64x32", "64x64" };

  //$2107
  if(id == n++) { name = "$2107"; value = ""; return true; }
  if(id == n++) { name = "BG1 Screen Address"; value = string("0x", strhex<4>(bg1_screen_address())); return true; }
  if(id == n++) { name = "BG1 Screen Size"; value = screen_size[bg1_screen_size()]; return true; }

  //$2108
  if(id == n++) { name = "$2108"; value = ""; return true; }
  if(id == n++) { name = "BG2 Screen Address"; value = string("0x", strhex<4>(bg2_screen_address())); return true; }
  if(id == n++) { name = "BG2 Screen Size"; value = screen_size[bg2_screen_size()]; return true; }

  //$2109
  if(id == n++) { name = "$2109"; value = ""; return true; }
  if(id == n++) { name = "BG3 Screen Address"; value = string("0x", strhex<4>(bg3_screen_address())); return true; }
  if(id == n++) { name = "BG3 Screen Size"; value = screen_size[bg3_screen_size()]; return true; }

  //$210a
  if(id == n++) { name = "$210a"; value = ""; return true; }
  if(id == n++) { name = "BG4 Screen Address"; value = string("0x", strhex<4>(bg4_screen_address())); return true; }
  if(id == n++) { name = "BG4 Screen Size"; value = screen_size[bg4_screen_size()]; return true; }

  //$210b
  if(id == n++) { name = "$210b"; value = ""; return true; }
  if(id == n++) { name = "BG1 Name Base Address"; value = string("0x", strhex<4>(bg1_name_base_address())); return true; }
  if(id == n++) { name = "BG2 Name Base Address"; value = string("0x", strhex<4>(bg2_name_base_address())); return true; }

  //$210c
  if(id == n++) { name = "$210c"; value = ""; return true; }
  if(id == n++) { name = "BG3 Name Base Address"; value = string("0x", strhex<4>(bg3_name_base_address())); return true; }
  if(id == n++) { name = "BG4 Name Base Address"; value = string("0x", strhex<4>(bg4_name_base_address())); return true; }

  //$210d
  if(id == n++) { name = "$210d"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 Scroll H-offset"; value = mode7_hoffset(); return true; }
  if(id == n++) { name = "BG1 Scroll H-offset"; value = bg1_hoffset(); return true; }

  //$210e
  if(id == n++) { name = "$210e"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 Scroll V-offset"; value = mode7_voffset(); return true; }
  if(id == n++) { name = "BG1 Scroll V-offset"; value = bg1_voffset(); return true; }

  //$210f
  if(id == n++) { name = "$210f"; value = ""; return true; }
  if(id == n++) { name = "BG2 Scroll H-offset"; value = bg2_hoffset(); return true; }

  //$2110
  if(id == n++) { name = "$2110"; value = ""; return true; }
  if(id == n++) { name = "BG2 Scroll V-offset"; value = bg2_voffset(); return true; }

  //$2111
  if(id == n++) { name = "$2111"; value = ""; return true; }
  if(id == n++) { name = "BG3 Scroll H-offset"; value = bg3_hoffset(); return true; }

  //$2112
  if(id == n++) { name = "$2112"; value = ""; return true; }
  if(id == n++) { name = "BG3 Scroll V-offset"; value = bg3_voffset(); return true; }

  //$2113
  if(id == n++) { name = "$2113"; value = ""; return true; }
  if(id == n++) { name = "BG4 Scroll H-offset"; value = bg4_hoffset(); return true; }

  //$2114
  if(id == n++) { name = "$2114"; value = ""; return true; }
  if(id == n++) { name = "BG4 Scroll V-offset"; value = bg4_voffset(); return true; }

  //$2115
  if(id == n++) { name = "$2115"; value = ""; return true; }
  if(id == n++) { name = "VRAM Increment Mode"; value = (unsigned)vram_increment_mode(); return true; }
  if(id == n++) { name = "VRAM Increment Formation"; value = vram_increment_formation(); return true; }
  if(id == n++) { name = "VRAM Increment Size"; value = vram_increment_size(); return true; }

  //$2116-$2117
  if(id == n++) { name = "$2116-$2117"; value = ""; return true; }
  if(id == n++) { name = "VRAM Address"; value = string("0x", strhex<4>(vram_address())); return true; }

  //$211a
  if(id == n++) { name = "$211a"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 Repeat"; value = mode7_repeat(); return true; }
  if(id == n++) { name = "Mode 7 V-flip"; value = mode7_vflip(); return true; }
  if(id == n++) { name = "Mode 7 H-flip"; value = mode7_hflip(); return true; }

  //$211b
  if(id == n++) { name = "$211b"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 A"; value = mode7_a(); return true; }

  //$211c
  if(id == n++) { name = "$211c"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 B"; value = mode7_b(); return true; }

  //$211d
  if(id == n++) { name = "$211d"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 C"; value = mode7_c(); return true; }

  //$211e
  if(id == n++) { name = "$211e"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 D"; value = mode7_d(); return true; }

  //$211f
  if(id == n++) { name = "$211f"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 X"; value = mode7_x(); return true; }

  //$2120
  if(id == n++) { name = "$2120"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 Y"; value = mode7_y(); return true; }

  //$2121
  if(id == n++) { name = "$2121"; value = ""; return true; }
  if(id == n++) { name = "CGRAM Address"; value = string("0x", strhex<4>(cgram_address())); return true; }

  //$2123
  if(id == n++) { name = "$2123"; value = ""; return true; }
  if(id == n++) { name = "BG1 Window 1 Enable"; value = bg1_window1_enable(); return true; }
  if(id == n++) { name = "BG1 Window 1 Invert"; value = bg1_window1_invert(); return true; }
  if(id == n++) { name = "BG1 Window 2 Enable"; value = bg1_window2_enable(); return true; }
  if(id == n++) { name = "BG1 Window 2 Invert"; value = bg1_window2_invert(); return true; }
  if(id == n++) { name = "BG2 Window 1 Enable"; value = bg2_window1_enable(); return true; }
  if(id == n++) { name = "BG2 Window 1 Invert"; value = bg2_window1_invert(); return true; }
  if(id == n++) { name = "BG2 Window 2 Enable"; value = bg2_window2_enable(); return true; }
  if(id == n++) { name = "BG2 Window 2 Invert"; value = bg2_window2_invert(); return true; }

  //$2124
  if(id == n++) { name = "$2124"; value = ""; return true; }
  if(id == n++) { name = "BG3 Window 1 Enable"; value = bg3_window1_enable(); return true; }
  if(id == n++) { name = "BG3 Window 1 Invert"; value = bg3_window1_invert(); return true; }
  if(id == n++) { name = "BG3 Window 2 Enable"; value = bg3_window2_enable(); return true; }
  if(id == n++) { name = "BG3 Window 2 Invert"; value = bg3_window2_invert(); return true; }
  if(id == n++) { name = "BG4 Window 1 Enable"; value = bg4_window1_enable(); return true; }
  if(id == n++) { name = "BG4 Window 1 Invert"; value = bg4_window1_invert(); return true; }
  if(id == n++) { name = "BG4 Window 2 Enable"; value = bg4_window2_enable(); return true; }
  if(id == n++) { name = "BG4 Window 2 Invert"; value = bg4_window2_invert(); return true; }

  //$2125
  if(id == n++) { name = "$2125"; value = ""; return true; }
  if(id == n++) { name = "OAM Window 1 Enable"; value = oam_window1_enable(); return true; }
  if(id == n++) { name = "OAM Window 1 Invert"; value = oam_window1_invert(); return true; }
  if(id == n++) { name = "OAM Window 2 Enable"; value = oam_window2_enable(); return true; }
  if(id == n++) { name = "OAM Window 2 Invert"; value = oam_window2_invert(); return true; }
  if(id == n++) { name = "Color Window 1 Enable"; value = color_window1_enable(); return true; }
  if(id == n++) { name = "Color Window 1 Invert"; value = color_window1_invert(); return true; }
  if(id == n++) { name = "Color Window 2 Enable"; value = color_window2_enable(); return true; }
  if(id == n++) { name = "Color Window 2 Invert"; value = color_window2_invert(); return true; }

  //$2126
  if(id == n++) { name = "$2126"; value = ""; return true; }
  if(id == n++) { name = "Window 1 Left"; value = window1_left(); return true; }

  //$2127
  if(id == n++) { name = "$2127"; value = ""; return true; }
  if(id == n++) { name = "Window 1 Right"; value = window1_right(); return true; }

  //$2128
  if(id == n++) { name = "$2128"; value = ""; return true; }
  if(id == n++) { name = "Window 2 Left"; value = window2_left(); return true; }

  //$2129
  if(id == n++) { name = "$2129"; value = ""; return true; }
  if(id == n++) { name = "Window 2 Right"; value = window2_right(); return true; }

  static char window_mask_mode[4][8] = { "OR", "AND", "XOR", "XNOR" };

  //$212a
  if(id == n++) { name = "$212a"; value = ""; return true; }
  if(id == n++) { name = "BG1 Window Mask"; value = window_mask_mode[bg1_window_mask()]; return true; }
  if(id == n++) { name = "BG2 Window Mask"; value = window_mask_mode[bg2_window_mask()]; return true; }
  if(id == n++) { name = "BG3 Window Mask"; value = window_mask_mode[bg3_window_mask()]; return true; }
  if(id == n++) { name = "BG4 Window Mask"; value = window_mask_mode[bg4_window_mask()]; return true; }

  //$212b
  if(id == n++) { name = "$212b"; value = ""; return true; }
  if(id == n++) { name = "OAM Window Mask"; value = window_mask_mode[oam_window_mask()]; return true; }
  if(id == n++) { name = "Color Window Mask"; value = window_mask_mode[color_window_mask()]; return true; }

  //$212c
  if(id == n++) { name = "$212c"; value = ""; return true; }
  if(id == n++) { name = "BG1 Mainscreen Enable"; value = bg1_mainscreen_enable(); return true; }
  if(id == n++) { name = "BG2 Mainscreen Enable"; value = bg2_mainscreen_enable(); return true; }
  if(id == n++) { name = "BG3 Mainscreen Enable"; value = bg3_mainscreen_enable(); return true; }
  if(id == n++) { name = "BG4 Mainscreen Enable"; value = bg4_mainscreen_enable(); return true; }
  if(id == n++) { name = "OAM Mainscreen Enable"; value = oam_mainscreen_enable(); return true; }

  //$212d
  if(id == n++) { name = "$212d"; value = ""; return true; }
  if(id == n++) { name = "BG1 Subscreen Enable"; value = bg1_subscreen_enable(); return true; }
  if(id == n++) { name = "BG2 Subscreen Enable"; value = bg2_subscreen_enable(); return true; }
  if(id == n++) { name = "BG3 Subscreen Enable"; value = bg3_subscreen_enable(); return true; }
  if(id == n++) { name = "BG4 Subscreen Enable"; value = bg4_subscreen_enable(); return true; }
  if(id == n++) { name = "OAM Subscreen Enable"; value = oam_subscreen_enable(); return true; }

  //$212e
  if(id == n++) { name = "$212e"; value = ""; return true; }
  if(id == n++) { name = "BG1 Mainscreen Window Enable"; value = bg1_mainscreen_window_enable(); return true; }
  if(id == n++) { name = "BG2 Mainscreen Window Enable"; value = bg2_mainscreen_window_enable(); return true; }
  if(id == n++) { name = "BG3 Mainscreen Window Enable"; value = bg3_mainscreen_window_enable(); return true; }
  if(id == n++) { name = "BG4 Mainscreen Window Enable"; value = bg4_mainscreen_window_enable(); return true; }
  if(id == n++) { name = "OAM Mainscreen Window Enable"; value = oam_mainscreen_window_enable(); return true; }

  //$212f
  if(id == n++) { name = "$212f"; value = ""; return true; }
  if(id == n++) { name = "BG1 Subscreen Window Enable"; value = bg1_subscreen_window_enable(); return true; }
  if(id == n++) { name = "BG2 Subscreen Window Enable"; value = bg2_subscreen_window_enable(); return true; }
  if(id == n++) { name = "BG3 Subscreen Window Enable"; value = bg3_subscreen_window_enable(); return true; }
  if(id == n++) { name = "BG4 Subscreen Window Enable"; value = bg4_subscreen_window_enable(); return true; }
  if(id == n++) { name = "OAM Subscreen Window Enable"; value = oam_subscreen_window_enable(); return true; }

  static char color_window_mask_mode[4][32] = { "Always", "Never", "Inside Window Only", "Outside Window Only" };

  //$2130
  if(id == n++) { name = "$2130"; value = ""; return true; }
  if(id == n++) { name = "Color Mainscreen Window Mask"; value = color_window_mask_mode[color_mainscreen_window_mask()]; return true; }
  if(id == n++) { name = "Color Subscreen Window Mask"; value = color_window_mask_mode[color_subscreen_window_mask()]; return true; }
  if(id == n++) { name = "Color Add/Subtract Mode"; value = !color_add_subtract_mode() ? "Fixed Color" : "Subscreen"; return true; }
  if(id == n++) { name = "Direct Color"; value = direct_color(); return true; }

  //$2131
  if(id == n++) { name = "$2131"; value = ""; return true; }
  if(id == n++) { name = "Color Mode"; value = !color_mode() ? "Add" : "Subtract"; return true; }
  if(id == n++) { name = "Color Halve"; value = color_halve(); return true; }
  if(id == n++) { name = "BG1 Color Enable"; value = bg1_color_enable(); return true; }
  if(id == n++) { name = "BG2 Color Enable"; value = bg2_color_enable(); return true; }
  if(id == n++) { name = "BG3 Color Enable"; value = bg3_color_enable(); return true; }
  if(id == n++) { name = "BG4 Color Enable"; value = bg4_color_enable(); return true; }
  if(id == n++) { name = "OAM Color Enable"; value = oam_color_enable(); return true; }
  if(id == n++) { name = "Back Color Enable"; value = back_color_enable(); return true; }

  //$2132
  if(id == n++) { name = "$2132"; value = ""; return true; }
  if(id == n++) { name = "Color Constant - Blue"; value = color_constant_blue(); return true; }
  if(id == n++) { name = "Color Constant - Green"; value = color_constant_green(); return true; }
  if(id == n++) { name = "Color Constant - Red"; value = color_constant_red(); return true; }

  //$2133
  if(id == n++) { name = "$2133"; value = ""; return true; }
  if(id == n++) { name = "Mode 7 EXTBG"; value = mode7_extbg(); return true; }
  if(id == n++) { name = "Pseudo Hires"; value = pseudo_hires(); return true; }
  if(id == n++) { name = "Overscan"; value = overscan_enable(); return true; }
  if(id == n++) { name = "OAM Interlace"; value = oam_interlace(); return true; }
  if(id == n++) { name = "Interlace"; value = interlace_enable(); return true; }

  //$213c
  if(id == n++) { name = "$213c"; value = ""; return true; }
  if(id == n++) { name = "H-counter"; value = hcounter(); return true; }

  //$213d
  if(id == n++) { name = "$213d"; value = ""; return true; }
  if(id == n++) { name = "V-counter"; value = vcounter(); return true; }

  //$213e
  if(id == n++) { name = "$213e"; value = ""; return true; }
  if(id == n++) { name = "Range Over"; value = range_over(); return true; }
  if(id == n++) { name = "Time Over"; value = time_over(); return true; }
  if(id == n++) { name = "S-PPU1 Version"; value = ppu1_version(); return true; }

  //$213f
  if(id == n++) { name = "$213f"; value = ""; return true; }
  if(id == n++) { name = "Field"; value = field(); return true; }
  if(id == n++) { name = "Region"; value = !region() ? "NTSC" : "PAL"; return true; }
  if(id == n++) { name = "S-PPU2 Version"; value = ppu2_version(); return true; }

  return false;
}

uint8 PPUDebugger::vram_mmio_read(uint16 addr) {
  uint8 data = PPU::vram_mmio_read(addr);
  debugger.breakpoint_test(Debugger::Breakpoint::Source::VRAM, Debugger::Breakpoint::Mode::Read, addr, data);
  return data;
}

void PPUDebugger::vram_mmio_write(uint16 addr, uint8 data) {
  PPU::vram_mmio_write(addr, data);
  debugger.breakpoint_test(Debugger::Breakpoint::Source::VRAM, Debugger::Breakpoint::Mode::Write, addr, data);
}

uint8 PPUDebugger::oam_mmio_read(uint16 addr) {
  uint8 data = PPU::oam_mmio_read(addr);
  debugger.breakpoint_test(Debugger::Breakpoint::Source::OAM, Debugger::Breakpoint::Mode::Read, addr, data);
  return data;
}

void PPUDebugger::oam_mmio_write(uint16 addr, uint8 data) {
  PPU::oam_mmio_write(addr, data);
  debugger.breakpoint_test(Debugger::Breakpoint::Source::OAM, Debugger::Breakpoint::Mode::Write, addr, data);
}

uint8 PPUDebugger::cgram_mmio_read(uint16 addr) {
  uint8 data = PPU::cgram_mmio_read(addr);
  debugger.breakpoint_test(Debugger::Breakpoint::Source::CGRAM, Debugger::Breakpoint::Mode::Read, addr, data);
  return data;
}

void PPUDebugger::cgram_mmio_write(uint16 addr, uint8 data) {
  PPU::cgram_mmio_write(addr, data);
  debugger.breakpoint_test(Debugger::Breakpoint::Source::CGRAM, Debugger::Breakpoint::Mode::Write, addr, data);
}

PPUDebugger::PPUDebugger() {
  bg1_enabled[0] = bg1_enabled[1] = true;
  bg2_enabled[0] = bg2_enabled[1] = true;
  bg3_enabled[0] = bg3_enabled[1] = true;
  bg4_enabled[0] = bg4_enabled[1] = true;
  oam_enabled[0] = oam_enabled[1] = oam_enabled[2] = oam_enabled[3] = true;
}

//===========
//PPUDebugger
//===========

//internal
unsigned PPUDebugger::ppu1_mdr() { return regs.ppu1_mdr; }
unsigned PPUDebugger::ppu2_mdr() { return regs.ppu2_mdr; }

//$2100
bool PPUDebugger::display_disable() { return regs.display_disabled; }
unsigned PPUDebugger::display_brightness() { return regs.display_brightness; }

//$2101
unsigned PPUDebugger::oam_base_size() { return regs.oam_basesize; }
unsigned PPUDebugger::oam_name_select() { return regs.oam_nameselect; }
unsigned PPUDebugger::oam_name_base_address() { return regs.oam_tdaddr; }

//$2102-$2103
unsigned PPUDebugger::oam_base_address() { return regs.oam_baseaddr; }
bool PPUDebugger::oam_priority() { return regs.oam_priority; }

//$2105
bool PPUDebugger::bg1_tile_size() { return regs.bg_tilesize[BG1]; }
bool PPUDebugger::bg2_tile_size() { return regs.bg_tilesize[BG2]; }
bool PPUDebugger::bg3_tile_size() { return regs.bg_tilesize[BG3]; }
bool PPUDebugger::bg4_tile_size() { return regs.bg_tilesize[BG4]; }
bool PPUDebugger::bg3_priority() { return regs.bg3_priority; }
unsigned PPUDebugger::bg_mode() { return regs.bg_mode; }

//$2106
unsigned PPUDebugger::mosaic_size() { return regs.mosaic_size; }
bool PPUDebugger::bg1_mosaic_enable() { return regs.mosaic_enabled[BG1]; }
bool PPUDebugger::bg2_mosaic_enable() { return regs.mosaic_enabled[BG2]; }
bool PPUDebugger::bg3_mosaic_enable() { return regs.mosaic_enabled[BG3]; }
bool PPUDebugger::bg4_mosaic_enable() { return regs.mosaic_enabled[BG4]; }

//$2107
unsigned PPUDebugger::bg1_screen_address() { return regs.bg_scaddr[BG1]; }
unsigned PPUDebugger::bg1_screen_size() { return regs.bg_scsize[BG1]; }

//$2108
unsigned PPUDebugger::bg2_screen_address() { return regs.bg_scaddr[BG2]; }
unsigned PPUDebugger::bg2_screen_size() { return regs.bg_scsize[BG2]; }

//$2109
unsigned PPUDebugger::bg3_screen_address() { return regs.bg_scaddr[BG3]; }
unsigned PPUDebugger::bg3_screen_size() { return regs.bg_scsize[BG3]; }

//$210a
unsigned PPUDebugger::bg4_screen_address() { return regs.bg_scaddr[BG4]; }
unsigned PPUDebugger::bg4_screen_size() { return regs.bg_scsize[BG4]; }

//$210b
unsigned PPUDebugger::bg1_name_base_address() { return regs.bg_tdaddr[BG1]; }
unsigned PPUDebugger::bg2_name_base_address() { return regs.bg_tdaddr[BG2]; }

//$210c
unsigned PPUDebugger::bg3_name_base_address() { return regs.bg_tdaddr[BG3]; }
unsigned PPUDebugger::bg4_name_base_address() { return regs.bg_tdaddr[BG4]; }

//$210d
unsigned PPUDebugger::mode7_hoffset() { return regs.m7_hofs & 0x1fff; }
unsigned PPUDebugger::bg1_hoffset() { return regs.bg_hofs[BG1] & 0x03ff; }

//$210e
unsigned PPUDebugger::mode7_voffset() { return regs.m7_vofs & 0x1fff; }
unsigned PPUDebugger::bg1_voffset() { return regs.bg_vofs[BG1] & 0x03ff; }

//$210f
unsigned PPUDebugger::bg2_hoffset() { return regs.bg_hofs[BG2] & 0x03ff; }

//$2110
unsigned PPUDebugger::bg2_voffset() { return regs.bg_vofs[BG2] & 0x03ff; }

//$2111
unsigned PPUDebugger::bg3_hoffset() { return regs.bg_hofs[BG3] & 0x03ff; }

//$2112
unsigned PPUDebugger::bg3_voffset() { return regs.bg_vofs[BG3] & 0x03ff; }

//$2113
unsigned PPUDebugger::bg4_hoffset() { return regs.bg_hofs[BG4] & 0x03ff; }

//$2114
unsigned PPUDebugger::bg4_voffset() { return regs.bg_vofs[BG4] & 0x03ff; }

//$2115
bool PPUDebugger::vram_increment_mode() { return regs.vram_incmode; }
unsigned PPUDebugger::vram_increment_formation() { return regs.vram_mapping; }
unsigned PPUDebugger::vram_increment_size() { return regs.vram_incsize; }

//$2116-$2117
unsigned PPUDebugger::vram_address() { return regs.vram_addr; }

//$211a
unsigned PPUDebugger::mode7_repeat() { return regs.mode7_repeat; }
bool PPUDebugger::mode7_vflip() { return regs.mode7_vflip; }
bool PPUDebugger::mode7_hflip() { return regs.mode7_hflip; }

//$211b
unsigned PPUDebugger::mode7_a() { return regs.m7a; }

//$211c
unsigned PPUDebugger::mode7_b() { return regs.m7b; }

//$211d
unsigned PPUDebugger::mode7_c() { return regs.m7c; }

//$211e
unsigned PPUDebugger::mode7_d() { return regs.m7d; }

//$211f
unsigned PPUDebugger::mode7_x() { return regs.m7x; }

//$2120
unsigned PPUDebugger::mode7_y() { return regs.m7y; }

//$2121
unsigned PPUDebugger::cgram_address() { return regs.cgram_addr; }

//$2123
bool PPUDebugger::bg1_window1_enable() { return regs.window1_enabled[BG1]; }
bool PPUDebugger::bg1_window1_invert() { return regs.window1_invert [BG1]; }
bool PPUDebugger::bg1_window2_enable() { return regs.window2_enabled[BG1]; }
bool PPUDebugger::bg1_window2_invert() { return regs.window2_invert [BG1]; }
bool PPUDebugger::bg2_window1_enable() { return regs.window1_enabled[BG2]; }
bool PPUDebugger::bg2_window1_invert() { return regs.window1_invert [BG2]; }
bool PPUDebugger::bg2_window2_enable() { return regs.window2_enabled[BG2]; }
bool PPUDebugger::bg2_window2_invert() { return regs.window2_invert [BG2]; }

//$2124
bool PPUDebugger::bg3_window1_enable() { return regs.window1_enabled[BG3]; }
bool PPUDebugger::bg3_window1_invert() { return regs.window1_invert [BG3]; }
bool PPUDebugger::bg3_window2_enable() { return regs.window2_enabled[BG3]; }
bool PPUDebugger::bg3_window2_invert() { return regs.window2_invert [BG3]; }
bool PPUDebugger::bg4_window1_enable() { return regs.window1_enabled[BG4]; }
bool PPUDebugger::bg4_window1_invert() { return regs.window1_invert [BG4]; }
bool PPUDebugger::bg4_window2_enable() { return regs.window2_enabled[BG4]; }
bool PPUDebugger::bg4_window2_invert() { return regs.window2_invert [BG4]; }

//$2125
bool PPUDebugger::oam_window1_enable() { return regs.window1_enabled[OAM]; }
bool PPUDebugger::oam_window1_invert() { return regs.window1_invert [OAM]; }
bool PPUDebugger::oam_window2_enable() { return regs.window2_enabled[OAM]; }
bool PPUDebugger::oam_window2_invert() { return regs.window2_invert [OAM]; }
bool PPUDebugger::color_window1_enable() { return regs.window1_enabled[COL]; }
bool PPUDebugger::color_window1_invert() { return regs.window1_invert [COL]; }
bool PPUDebugger::color_window2_enable() { return regs.window2_enabled[COL]; }
bool PPUDebugger::color_window2_invert() { return regs.window2_enabled[COL]; }

//$2126
unsigned PPUDebugger::window1_left() { return regs.window1_left; }

//$2127
unsigned PPUDebugger::window1_right() { return regs.window1_right; }

//$2128
unsigned PPUDebugger::window2_left() { return regs.window2_left; }

//$2129
unsigned PPUDebugger::window2_right() { return regs.window2_right; }

//$212a
unsigned PPUDebugger::bg1_window_mask() { return regs.window_mask[BG1]; }
unsigned PPUDebugger::bg2_window_mask() { return regs.window_mask[BG2]; }
unsigned PPUDebugger::bg3_window_mask() { return regs.window_mask[BG3]; }
unsigned PPUDebugger::bg4_window_mask() { return regs.window_mask[BG4]; }

//$212b
unsigned PPUDebugger::oam_window_mask() { return regs.window_mask[OAM]; }
unsigned PPUDebugger::color_window_mask() { return regs.window_mask[COL]; }

//$212c
bool PPUDebugger::bg1_mainscreen_enable() { return regs.bg_enabled[BG1]; }
bool PPUDebugger::bg2_mainscreen_enable() { return regs.bg_enabled[BG2]; }
bool PPUDebugger::bg3_mainscreen_enable() { return regs.bg_enabled[BG3]; }
bool PPUDebugger::bg4_mainscreen_enable() { return regs.bg_enabled[BG4]; }
bool PPUDebugger::oam_mainscreen_enable() { return regs.bg_enabled[OAM]; }

//$212d
bool PPUDebugger::bg1_subscreen_enable() { return regs.bgsub_enabled[BG1]; }
bool PPUDebugger::bg2_subscreen_enable() { return regs.bgsub_enabled[BG2]; }
bool PPUDebugger::bg3_subscreen_enable() { return regs.bgsub_enabled[BG3]; }
bool PPUDebugger::bg4_subscreen_enable() { return regs.bgsub_enabled[BG4]; }
bool PPUDebugger::oam_subscreen_enable() { return regs.bgsub_enabled[OAM]; }

//$212e
bool PPUDebugger::bg1_mainscreen_window_enable() { return regs.window_enabled[BG1]; }
bool PPUDebugger::bg2_mainscreen_window_enable() { return regs.window_enabled[BG2]; }
bool PPUDebugger::bg3_mainscreen_window_enable() { return regs.window_enabled[BG3]; }
bool PPUDebugger::bg4_mainscreen_window_enable() { return regs.window_enabled[BG4]; }
bool PPUDebugger::oam_mainscreen_window_enable() { return regs.window_enabled[OAM]; }

//$212f
bool PPUDebugger::bg1_subscreen_window_enable() { return regs.sub_window_enabled[BG1]; }
bool PPUDebugger::bg2_subscreen_window_enable() { return regs.sub_window_enabled[BG2]; }
bool PPUDebugger::bg3_subscreen_window_enable() { return regs.sub_window_enabled[BG3]; }
bool PPUDebugger::bg4_subscreen_window_enable() { return regs.sub_window_enabled[BG4]; }
bool PPUDebugger::oam_subscreen_window_enable() { return regs.sub_window_enabled[OAM]; }

//$2130
unsigned PPUDebugger::color_mainscreen_window_mask() { return regs.color_mask; }
unsigned PPUDebugger::color_subscreen_window_mask() { return regs.colorsub_mask; }
bool PPUDebugger::color_add_subtract_mode() { return regs.addsub_mode; }
bool PPUDebugger::direct_color() { return regs.direct_color; }

//$2131
bool PPUDebugger::color_mode() { return regs.color_mode; }
bool PPUDebugger::color_halve() { return regs.color_halve; }
bool PPUDebugger::bg1_color_enable() { return regs.color_enabled[BG1]; }
bool PPUDebugger::bg2_color_enable() { return regs.color_enabled[BG2]; }
bool PPUDebugger::bg3_color_enable() { return regs.color_enabled[BG3]; }
bool PPUDebugger::bg4_color_enable() { return regs.color_enabled[BG4]; }
bool PPUDebugger::oam_color_enable() { return regs.color_enabled[OAM]; }
bool PPUDebugger::back_color_enable() { return regs.color_enabled[BACK]; }

//$2132
unsigned PPUDebugger::color_constant_blue() { return regs.color_b; }
unsigned PPUDebugger::color_constant_green() { return regs.color_g; }
unsigned PPUDebugger::color_constant_red() { return regs.color_r; }

//$2133
bool PPUDebugger::mode7_extbg() { return regs.mode7_extbg; }
bool PPUDebugger::pseudo_hires() { return regs.pseudo_hires; }
bool PPUDebugger::overscan_enable() { return regs.overscan; }
bool PPUDebugger::oam_interlace() { return regs.oam_interlace; }
bool PPUDebugger::interlace_enable() { return regs.interlace; }

//$213c
unsigned PPUDebugger::hcounter() { return PPU::hcounter(); }

//$213d
unsigned PPUDebugger::vcounter() { return PPU::vcounter(); }

//$213e
bool PPUDebugger::range_over() { return regs.range_over; }
bool PPUDebugger::time_over() { return regs.time_over; }
unsigned PPUDebugger::ppu1_version() { return PPU::ppu1_version; }

//$213f
bool PPUDebugger::field() { return cpu.field(); }
bool PPUDebugger::region() { return PPU::region; }
unsigned PPUDebugger::ppu2_version() { return PPU::ppu2_version; }

#endif
