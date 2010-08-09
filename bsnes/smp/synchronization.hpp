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
