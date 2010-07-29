inline void serial_enter() { serial.enter(); }

void Serial::create() {
  if(thread) co_delete(thread);
  thread = co_create(65536 * sizeof(void*), serial_enter);
  frequency = cartridge.serial_baud_rate() * 8;
  clock = 0;
}

void Serial::step(unsigned clocks) {
  clock += clocks * (uint64)cpu.frequency;
}

void Serial::synchronize_cpu() {
  if(clock >= 0 && scheduler.sync != Scheduler::SynchronizeMode::All) co_switch(cpu.thread);
}
