inline void ppu_enter() { ppu.enter(); }

void PPU::create() {
  if(thread) co_delete(thread);
  thread = co_create(65536 * sizeof(void*), ppu_enter);
  frequency = system.region() == System::Region::NTSC ? config.cpu.ntsc_clock_rate : config.cpu.pal_clock_rate;
  clock = 0;
}

void PPU::step(unsigned clocks) {
  clock += clocks;
}

void PPU::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
