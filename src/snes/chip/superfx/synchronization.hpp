inline void superfx_enter() { superfx.enter(); }

void SuperFX::create() {
  if(thread) co_delete(thread);
  thread = co_create(65536 * sizeof(void*), superfx_enter);
  frequency = system.region() == System::Region::NTSC ? config.cpu.ntsc_clock_rate : config.cpu.pal_clock_rate;
  clock = 0;
}

void SuperFX::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
}

void SuperFX::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
