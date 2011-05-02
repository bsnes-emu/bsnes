template<unsigned timer_frequency>
void SMP::Timer<timer_frequency>::tick() {
  //stage 0 increment
  stage0_ticks += smp.status.timer_step;
  if(stage0_ticks < timer_frequency) return;
  stage0_ticks -= timer_frequency;

  //stage 1 increment
  stage1_ticks ^= 1;
  synchronize();
}

template<unsigned timer_frequency>
void SMP::Timer<timer_frequency>::synchronize() {
  bool new_line = stage1_ticks;
  bool old_line = current_line;
  current_line = new_line;
  if(old_line != 1 || new_line != 0) return;  //only pulse on 1->0 transition

  //stage 2 increment
  if(enable == false) return;
  stage2_ticks++;
  if(stage2_ticks != target) return;

  //stage 3 increment
  stage2_ticks = 0;
  stage3_ticks++;
  stage3_ticks &= 15;
}
