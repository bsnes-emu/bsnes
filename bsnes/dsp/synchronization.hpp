void DSP::step(unsigned clocks) {
  clock += clocks;
}

void DSP::synchronize_smp() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(smp.thread);
}
