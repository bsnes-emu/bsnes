auto SMP::serialize(serializer& s) -> void {
  SPC700::serialize(s);
  Thread::serialize(s);

  s.array(apuram);

  s.integer(io.clockCounter);
  s.integer(io.dspCounter);
  s.integer(io.timerStep);

  s.integer(io.clockSpeed);
  s.integer(io.timerSpeed);
  s.integer(io.timersEnable);
  s.integer(io.ramDisable);
  s.integer(io.ramWritable);
  s.integer(io.timersDisable);

  s.integer(io.iplromEnable);

  s.integer(io.dspAddr);

  s.array(io.port);

  s.integer(io.ram00f8);
  s.integer(io.ram00f9);

  s.integer(timer0.stage0);
  s.integer(timer0.stage1);
  s.integer(timer0.stage2);
  s.integer(timer0.stage3);
  s.integer(timer0.line);
  s.integer(timer0.enable);
  s.integer(timer0.target);

  s.integer(timer1.stage0);
  s.integer(timer1.stage1);
  s.integer(timer1.stage2);
  s.integer(timer1.stage3);
  s.integer(timer1.line);
  s.integer(timer1.enable);
  s.integer(timer1.target);

  s.integer(timer2.stage0);
  s.integer(timer2.stage1);
  s.integer(timer2.stage2);
  s.integer(timer2.stage3);
  s.integer(timer2.line);
  s.integer(timer2.enable);
  s.integer(timer2.target);
}
