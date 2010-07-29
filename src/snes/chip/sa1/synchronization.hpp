inline void sa1_enter() { sa1.enter(); }

void SA1::create() {
  if(thread) co_delete(thread);
  thread = co_create(65536 * sizeof(void*), sa1_enter);
  frequency = system.region() == System::Region::NTSC ? config.cpu.ntsc_clock_rate : config.cpu.pal_clock_rate;
  clock = 0;
}

void SA1::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
}

void SA1::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
