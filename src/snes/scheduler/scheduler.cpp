Scheduler scheduler;

//

void threadentry_cpu() { r_cpu->enter(); }
void threadentry_smp() { r_smp->enter(); }

//

void Scheduler::enter() {
  switch(clock.active) {
  case THREAD_CPU:
    co_switch(thread_cpu);
    break;
  case THREAD_SMP:
    co_switch(thread_smp);
    break;
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

  clock.active = THREAD_CPU;
  clock.cpusmp = 0;

  if(thread_cpu)co_delete(thread_cpu);
  if(thread_smp)co_delete(thread_smp);

  thread_snes = co_active();
  thread_cpu  = co_create(threadentry_cpu, COTHREAD_STACKSIZE_NORMAL);
  thread_smp  = co_create(threadentry_smp, COTHREAD_STACKSIZE_NORMAL);
}

//

Scheduler::Scheduler() {
  thread_snes = 0;
  thread_cpu  = 0;
  thread_smp  = 0;
}
