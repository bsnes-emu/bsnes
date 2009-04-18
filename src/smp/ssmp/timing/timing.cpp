#ifdef SSMP_CPP

void sSMP::add_clocks(unsigned clocks) {
  scheduler.addclocks_smp(clocks);
  #if !defined(USE_STATE_MACHINE)
  scheduler.sync_smpdsp();
  #else
  while(scheduler.clock.smpdsp < 0) dsp.enter();
  #endif
}

void sSMP::tick_timers() {
  t0.tick();
  t1.tick();
  t2.tick();
}

#endif
