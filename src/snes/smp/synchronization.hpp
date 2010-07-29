inline void smp_enter() { smp.enter(); }

void SMP::create() {
  if(thread) co_delete(thread);
  thread = co_create(65536 * sizeof(void*), smp_enter);
  frequency = system.region() == System::Region::NTSC ? config.smp.ntsc_clock_rate : config.smp.pal_clock_rate;
  clock = 0;
}

void SMP::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
  dsp.clock -= clocks;
}

void SMP::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}

void SMP::synchronize_dsp() {
  if(dsp.clock < 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(dsp.thread);
}
