inline void sSMP::add_clocks(uint clocks) {
  status.clocks_executed += clocks;
  status.clock_counter   += clocks;

//update timers
  while(status.clock_counter >= 24) {
    status.clock_counter -= 24;
    t0.tick();
    t1.tick();
    t2.tick();
  }
}

inline uint32 sSMP::clocks_executed() {
uint32 r = status.clocks_executed;
  status.clocks_executed = 0;
  return r;
}
