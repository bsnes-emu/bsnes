//DSP clock (~24576khz) / 12 (~2048khz) is fed into the SMP
//from here, the wait states value is really a clock divider of {2, 4, 8, 16}
//because dividers of 8 and 16 are not evenly divislbe into 12, the SMP glitches
//in these two cases, the SMP ends up consuming 10 and 20 cycles instead
//this causes unpredictable behavior on real hardware
//sometimes the SMP will run far slower than expected
//other times (and more likely), the SMP will deadlock until the system is reset
//the timers are not affected by this and advance by their expected values
auto SMP::wait(maybe<uint16> addr) -> void {
  static const uint cycleWaitStates[4] = {2, 4, 10, 20};
  static const uint timerWaitStates[4] = {2, 4,  8, 16};

  uint waitStates = io.externalWaitStates;
  if(!addr) waitStates = io.internalWaitStates;  //idle cycles
  else if((*addr & 0xfff0) == 0x00f0) waitStates = io.internalWaitStates;  //IO registers
  else if(*addr >= 0xffc0 && io.iplromEnable) waitStates = io.internalWaitStates;  //IPLROM

  step(cycleWaitStates[waitStates]);
  stepTimers(timerWaitStates[waitStates]);
}

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

auto SMP::stepTimers(uint clocks) -> void {
  timer0.step(clocks);
  timer1.step(clocks);
  timer2.step(clocks);
}

template<uint Frequency> auto SMP::Timer<Frequency>::step(uint clocks) -> void {
  //stage 0 increment
  stage0 += clocks;
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
