#ifdef SYSTEM_CPP

Configuration config;

Configuration::Configuration() {
  controller_port1 = Input::DeviceJoypad;
  controller_port2 = Input::DeviceJoypad;
  expansion_port   = System::ExpansionBSX;
  region           = System::Autodetect;

  cpu.version         = 2;
  cpu.ntsc_clock_rate = 21477272;
  cpu.pal_clock_rate  = 21281370;
  cpu.alu_mul_delay   = 2;
  cpu.alu_div_delay   = 2;
  cpu.wram_init_value = 0x55;

  smp.ntsc_clock_rate = 24607104;  //32040.5 * 768
  smp.pal_clock_rate  = 24607104;

  ppu.bg1_enabled[0] = ppu.bg1_enabled[1] = true;
  ppu.bg2_enabled[0] = ppu.bg2_enabled[1] = true;
  ppu.bg3_enabled[0] = ppu.bg3_enabled[1] = true;
  ppu.bg4_enabled[0] = ppu.bg4_enabled[1] = true;
  ppu.oam_enabled[0] = ppu.oam_enabled[1] = ppu.oam_enabled[2] = ppu.oam_enabled[3] = true;

  ppu1.version = 1;
  ppu2.version = 3;

  superfx.speed = 0;  //0 = auto-select, 1 = force 10.74MHz, 2 = force 21.48MHz
}

#endif
