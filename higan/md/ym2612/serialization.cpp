auto YM2612::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(io.address);

  s.integer(lfo.enable);
  s.integer(lfo.rate);
  s.integer(lfo.clock);
  s.integer(lfo.divider);

  s.integer(dac.enable);
  s.integer(dac.sample);

  s.integer(envelope.clock);
  s.integer(envelope.divider);

  s.integer(timerA.enable);
  s.integer(timerA.irq);
  s.integer(timerA.line);
  s.integer(timerA.period);
  s.integer(timerA.counter);

  s.integer(timerB.enable);
  s.integer(timerB.irq);
  s.integer(timerB.line);
  s.integer(timerB.period);
  s.integer(timerB.counter);
  s.integer(timerB.divider);

  for(auto n : range(6)) channels[n].serialize(s);
}

auto YM2612::Channel::serialize(serializer& s) -> void {
  s.integer(leftEnable);
  s.integer(rightEnable);
  s.integer(algorithm);
  s.integer(feedback);
  s.integer(vibrato);
  s.integer(tremolo);
  s.integer(mode);

  for(auto n : range(4)) operators[n].serialize(s);
}

auto YM2612::Channel::Operator::serialize(serializer& s) -> void {
  s.integer(keyOn);
  s.integer(lfoEnable);
  s.integer(detune);
  s.integer(multiple);
  s.integer(totalLevel);
  s.integer(outputLevel);
  s.integer(output);
  s.integer(prior);

  s.integer(pitch.value);
  s.integer(pitch.reload);
  s.integer(pitch.latch);

  s.integer(octave.value);
  s.integer(octave.reload);
  s.integer(octave.latch);

  s.integer(phase.value);
  s.integer(phase.delta);

  s.integer(envelope.state);
  s.integer(envelope.rate);
  s.integer(envelope.divider);
  s.integer(envelope.steps);
  s.integer(envelope.value);
  s.integer(envelope.keyScale);
  s.integer(envelope.attackRate);
  s.integer(envelope.decayRate);
  s.integer(envelope.sustainRate);
  s.integer(envelope.sustainLevel);
  s.integer(envelope.releaseRate);

  s.integer(ssg.enable);
  s.integer(ssg.attack);
  s.integer(ssg.alternate);
  s.integer(ssg.hold);
  s.integer(ssg.invert);
}
