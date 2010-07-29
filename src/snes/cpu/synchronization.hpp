inline void cpu_enter() { cpu.enter(); }

void CPU::create() {
  if(thread) co_delete(thread);
  thread = co_create(65536 * sizeof(void*), cpu_enter);
  frequency = system.region() == System::Region::NTSC ? config.cpu.ntsc_clock_rate : config.cpu.pal_clock_rate;
  clock = 0;
  coprocessors.reset();
}

void CPU::step(unsigned clocks) {
  smp.clock -= clocks * (uint64)smp.frequency;
  ppu.clock -= clocks;
  for(unsigned i = 0; i < coprocessors.size(); i++) {
    Processor &chip = *coprocessors[i];
    chip.clock -= clocks * (uint64)chip.frequency;
  }
}

void CPU::synchronize_smp() {
  if(smp.clock < 0) co_switch(smp.thread);
}

void CPU::synchronize_ppu() {
  if(ppu.clock < 0) co_switch(ppu.thread);
}

void CPU::synchronize_coprocessor() {
  for(unsigned i = 0; i < coprocessors.size(); i++) {
    Processor &chip = *coprocessors[i];
    if(chip.clock < 0) co_switch(chip.thread);
  }
}
