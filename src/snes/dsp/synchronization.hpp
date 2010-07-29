inline void dsp_enter() { dsp.enter(); }

void DSP::create() {
  if(thread) co_delete(thread);
  thread = co_create(65536 * sizeof(void*), dsp_enter);
  frequency = system.region() == System::Region::NTSC ? config.smp.ntsc_clock_rate : config.smp.pal_clock_rate;
  clock = 0;
}

void DSP::step(unsigned clocks) {
  clock += clocks;
}

void DSP::synchronize_smp() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(smp.thread);
}
