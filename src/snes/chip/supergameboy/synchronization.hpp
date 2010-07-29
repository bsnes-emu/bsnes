inline void supergameboy_enter() { supergameboy.enter(); }

void SuperGameBoy::create() {
  thread = co_create(65536 * sizeof(void*), supergameboy_enter);
  frequency = cartridge.supergameboy_version() == Cartridge::SuperGameBoyVersion::Version1 ? 2147727 : 2097152;
  if(frequency == 2147727) frequency = system.region() == System::Region::NTSC ? (config.cpu.ntsc_clock_rate / 10) : (config.cpu.pal_clock_rate / 10);
  clock = 0;
}

void SuperGameBoy::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
}

void SuperGameBoy::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
