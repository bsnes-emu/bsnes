void sAPU::add_clocks(int clocks) {
  status.clocks_executed += clocks;
}

uint32 sAPU::clocks_executed() {
uint32 r = status.clocks_executed;
  status.clocks_executed = 0;
  return r;
}

//occurs once every 24 clocks (once every APU opcode cycle)
void sAPU::tick_timers() {
  t0.tick();
  t1.tick();
  t2.tick();
}

void sAPU::sAPUTimer::tick() {
//stage 1 increment
  stage1_ticks++;
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
