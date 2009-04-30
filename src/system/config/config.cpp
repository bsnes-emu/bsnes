Config::Config() {
  controller_port1 = System::Input::DeviceJoypad;
  controller_port2 = System::Input::DeviceJoypad;
  expansion_port   = System::ExpansionBSX;
  region           = System::Autodetect;

  cpu.version         = 2;
  cpu.ntsc_clock_rate = 21477272;
  cpu.pal_clock_rate  = 21281370;
  cpu.alu_mul_delay   = 2;
  cpu.alu_div_delay   = 2;
  cpu.wram_init_value = 0x55;

  smp.ntsc_clock_rate = 32041 * 768;
  smp.pal_clock_rate  = 32041 * 768;

  ppu1.version = 1;
  ppu2.version = 3;
}
