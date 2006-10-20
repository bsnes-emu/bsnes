alwaysinline void sSMP::add_clocks(uint clocks) {
  scheduler.addclocks_smp(clocks);

  status.clock_counter += clocks;

//update timers
  while(status.clock_counter >= 24) {
    status.clock_counter -= 24;
    t0.tick();
    t1.tick();
    t2.tick();

  //24 * 32 = 768 clocks/DSP tick
  //1024000 / 768 = 32000 DSP ticks/second
    if(++status.dsp_counter == 32) {
      status.dsp_counter = 0;
      snes->audio_update(r_dsp->run());
    }
  }
}
