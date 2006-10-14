void bSMP::add_cycles(int cycles) {
  status.clocks_executed += cycles;

  t0.add_cycles(cycles);
  t1.add_cycles(cycles);
  t2.add_cycles(cycles);
}

uint32 bSMP::clocks_executed() {
uint32 r = status.clocks_executed;
  status.clocks_executed = 0;
  return (r << 4) + (r << 3);
}

//cycles should never be greater than 12. since the minimum
//cycle_frequency value is 16, we don't have to worry about
//two ticks occuring in one call to this function.
void bSMP::bSMPTimer::add_cycles(int cycles) {
//stage 1 increment
  stage1_ticks += cycles;
  if(stage1_ticks < cycle_frequency)return;

  stage1_ticks -= cycle_frequency;
  if(enabled == false)return;

//stage 2 increment
  stage2_ticks++;

  if(stage2_ticks != target)return;

//stage 3 increment
  stage2_ticks = 0;
  stage3_ticks++;
  stage3_ticks &= 15;
}
