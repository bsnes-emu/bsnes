auto SMP::serialize(serializer& s) -> void {
  SPC700::serialize(s);
  Thread::serialize(s);

  s.array(apuram);

  s.integer(status.clockCounter);
  s.integer(status.dspCounter);
  s.integer(status.timerStep);

  s.integer(status.clockSpeed);
  s.integer(status.timerSpeed);
  s.integer(status.timersEnable);
  s.integer(status.ramDisable);
  s.integer(status.ramWritable);
  s.integer(status.timersDisable);

  s.integer(status.iplromEnable);

  s.integer(status.dspAddr);

  s.integer(status.ram00f8);
  s.integer(status.ram00f9);

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
