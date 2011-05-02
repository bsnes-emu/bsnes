#ifdef SMP_CPP

void SMP::serialize(serializer &s) {
  Processor::serialize(s);
  SMPcore::core_serialize(s);

  s.array(apuram);

  s.integer(status.clock_counter);
  s.integer(status.dsp_counter);
  s.integer(status.timer_step);

  s.integer(status.clock_speed);
  s.integer(status.timer_speed);
  s.integer(status.timers_enable);
  s.integer(status.ram_disable);
  s.integer(status.ram_writable);
  s.integer(status.timers_disable);

  s.integer(status.iplrom_enable);

  s.integer(status.dsp_addr);

  s.integer(status.ram00f8);
  s.integer(status.ram00f9);

  s.integer(t0.stage0_ticks);
  s.integer(t0.stage1_ticks);
  s.integer(t0.stage2_ticks);
  s.integer(t0.stage3_ticks);
  s.integer(t0.current_line);
  s.integer(t0.enabled);
  s.integer(t0.target);

  s.integer(t1.stage0_ticks);
  s.integer(t1.stage1_ticks);
  s.integer(t1.stage2_ticks);
  s.integer(t1.stage3_ticks);
  s.integer(t1.current_line);
  s.integer(t1.enabled);
  s.integer(t1.target);

  s.integer(t2.stage0_ticks);
  s.integer(t2.stage1_ticks);
  s.integer(t2.stage2_ticks);
  s.integer(t2.stage3_ticks);
  s.integer(t2.current_line);
  s.integer(t2.enabled);
  s.integer(t2.target);
}

#endif
