inline void msu1_enter() { msu1.enter(); }

void MSU1::create() {
  if(thread) co_delete(thread);
  thread = co_create(65536 * sizeof(void*), msu1_enter);
  frequency = 44100;
  clock = 0;
}

void MSU1::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
}

void MSU1::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
