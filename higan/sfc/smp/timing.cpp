auto SMP::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(dsp);

  #if defined(DEBUGGER)
  synchronize(cpu);
  #else
  //forcefully sync S-SMP to S-CPU in case chips are not communicating
  //sync if S-SMP is more than 1ms ahead of S-CPU
  if(clock() - cpu.clock() > Thread::Second / 1'000) synchronize(cpu);
  #endif
}

auto SMP::cycleEdge() -> void {
  timer0.tick();
  timer1.tick();
  timer2.tick();
}

template<uint Frequency> auto SMP::Timer<Frequency>::tick() -> void {
  //stage 0 increment
  stage0 += smp.io.timerStep;
  if(stage0 < Frequency) return;
  stage0 -= Frequency;

  //stage 1 increment
  stage1 ^= 1;
  synchronizeStage1();
}

template<uint Frequency> auto SMP::Timer<Frequency>::synchronizeStage1() -> void {
  bool newLine = stage1;
  if(!smp.io.timersEnable) newLine = false;
  if(smp.io.timersDisable) newLine = false;

  bool oldLine = line;
  line = newLine;
  if(oldLine != 1 || newLine != 0) return;  //only pulse on 1->0 transition

  //stage 2 increment
  if(!enable) return;
  if(++stage2 != target) return;

  //stage 3 increment
  stage2 = 0;
  stage3++;
}
