Scheduler scheduler;

//

void cocall threadentry_cpu(void*) { r_cpu->enter(); }
void cocall threadentry_smp(void*) { r_smp->enter(); }
void cocall threadentry_ppu(void*) {} //currently unused
void cocall threadentry_dsp(void*) { r_dsp->enter(); }

//

void Scheduler::enter() {
  switch(clock.active) {
  case THREAD_CPU: co_switch(thread_cpu); break;
  case THREAD_SMP: co_switch(thread_smp); break;
  case THREAD_PPU: co_switch(thread_ppu); break;
  case THREAD_DSP: co_switch(thread_dsp); break;
  }
}

void Scheduler::exit() {
  co_switch(thread_snes);
}

void Scheduler::init() {
  clock.cpu_freq = snes.region() == SNES::NTSC ?
                   config::cpu.ntsc_clock_rate :
                   config::cpu.pal_clock_rate;
  clock.smp_freq = snes.region() == SNES::NTSC ?
                   config::smp.ntsc_clock_rate :
                   config::smp.pal_clock_rate;
  clock.dsp_freq = clock.smp_freq >> 3;

  clock.active = THREAD_CPU;
  clock.cpusmp = 0;
  clock.smpdsp = 0;

  if(thread_cpu)co_delete(thread_cpu);
  if(thread_smp)co_delete(thread_smp);
  if(thread_ppu)co_delete(thread_ppu);
  if(thread_dsp)co_delete(thread_dsp);

  thread_snes = co_active();
  thread_cpu  = co_create(sizeof(void*) * 64 * 1024, threadentry_cpu, 0);
  thread_smp  = co_create(sizeof(void*) * 64 * 1024, threadentry_smp, 0);
  thread_ppu  = co_create(sizeof(void*) * 64 * 1024, threadentry_ppu, 0);
  thread_dsp  = co_create(sizeof(void*) * 64 * 1024, threadentry_dsp, 0);
}

//

Scheduler::Scheduler() {
  thread_snes = 0;
  thread_cpu  = 0;
  thread_smp  = 0;
  thread_ppu  = 0;
  thread_dsp  = 0;
}
