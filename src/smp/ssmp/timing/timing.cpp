#ifdef SSMP_CPP

alwaysinline
void sSMP::add_clocks(uint clocks) {
  scheduler.addclocks_smp(clocks);
}

alwaysinline
void sSMP::tick_timers() {
  t0.tick();
  t1.tick();
  t2.tick();
}

#endif //ifdef SSMP_CPP
