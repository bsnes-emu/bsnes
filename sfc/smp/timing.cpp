auto SMP::addClocks(uint clocks) -> void {
  step(clocks);
  synchronizeDSP();

  #if defined(DEBUGGER)
  synchronizeCPU();
  #else
  //forcefully sync S-SMP to S-CPU in case chips are not communicating
  //sync if S-SMP is more than 24 samples ahead of S-CPU
  if(clock > +(768 * 24 * (int64)24000000)) synchronizeCPU();
  #endif
}

auto SMP::cycleEdge() -> void {
  timer0.tick();
  timer1.tick();
  timer2.tick();

  //TEST register S-SMP speed control
  //24 clocks have already been added for this cycle at this point
  switch(status.clockSpeed) {
  case 0: break;                      //100% speed
  case 1: addClocks(24); break;       // 50% speed
  case 2: while(true) addClocks(24);  //  0% speed -- locks S-SMP
  case 3: addClocks(24 * 9); break;   // 10% speed
  }
}

template<unsigned Frequency>
auto SMP::Timer<Frequency>::tick() -> void {
  //stage 0 increment
  stage0 += smp.status.timerStep;
  if(stage0 < Frequency) return;
  stage0 -= Frequency;

  //stage 1 increment
  stage1 ^= 1;
  synchronizeStage1();
}

template<unsigned Frequency>
auto SMP::Timer<Frequency>::synchronizeStage1() -> void {
  bool newLine = stage1;
  if(smp.status.timersEnable == false) newLine = false;
  if(smp.status.timersDisable == true) newLine = false;

  bool oldLine = line;
  line = newLine;
  if(oldLine != 1 || newLine != 0) return;  //only pulse on 1->0 transition

  //stage 2 increment
  if(enable == false) return;
  if(++stage2 != target) return;

  //stage 3 increment
  stage2 = 0;
  stage3++;
}
