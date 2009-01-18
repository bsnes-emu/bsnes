#ifdef SSMP_CPP

void sSMP::add_clocks(unsigned clocks) {
  scheduler.addclocks_smp(clocks);
}

void sSMP::tick_timers() {
  t0.tick();
  t1.tick();
  t2.tick();
}

#endif
