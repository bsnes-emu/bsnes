auto YM2612::Channel::Operator::trigger(bool state) -> void {
  if(keyOn == state) return;  //no change

  keyOn = state;
  envelope.state = Release;
  updateEnvelope();

  if(keyOn) {
    //restart phase and envelope generators
    phase.value = 0;
    ssg.invert = false;
    envelope.state = Attack;
    updateEnvelope();

    if(envelope.rate >= 62) {
      //skip attack and possibly decay stages
      envelope.value = 0;
      envelope.state = envelope.sustainLevel ? Decay : Sustain;
      updateEnvelope();
    }
  } else if(ssg.enable && ssg.attack != ssg.invert) {
    //SSG-EG key-off
    envelope.value = 0x200 - envelope.value;
  }

  updateLevel();
}

auto YM2612::Channel::Operator::runEnvelope() -> void {
  uint sustain = envelope.sustainLevel < 15 ? envelope.sustainLevel << 5 : 0x3f0;
  if(ym2612.envelope.clock & (1 << envelope.divider) - 1) return;

  uint value = ym2612.envelope.clock >> envelope.divider;
  uint step = envelope.steps >> ((~value & 7) << 2) & 0xf;
  if(ssg.enable) step <<= 2;  //SSG results in a 4x faster envelope

  if(envelope.state == Attack) {
    uint next = envelope.value + (~uint16(envelope.value) * step >> 4) & 0x3ff;
    if(next <= envelope.value) {
      envelope.value = next;
    } else {
      envelope.value = 0;
      envelope.state = envelope.value < sustain ? Decay : Sustain;
      updateEnvelope();
    }
  } else if(!ssg.enable || envelope.value < 0x200) {
    envelope.value = min(envelope.value + step, 0x3ff);
    if(envelope.state == Decay && envelope.value >= sustain) {
      envelope.state = Sustain;
      updateEnvelope();
    }
  }

  updateLevel();
}

auto YM2612::Channel::Operator::runPhase() -> void {
  phase.value += phase.delta;  //advance wave position
  if(!(ssg.enable && envelope.value >= 0x200)) return;  //SSG loop check

  if(!ssg.hold && !ssg.alternate) phase.value = 0;
  if(!ssg.hold || ssg.attack == ssg.invert) ssg.invert ^= ssg.alternate;

  if(envelope.state == Attack) {
    //do nothing; SSG is meant to skip the attack phase
  } else if(envelope.state != Release && !ssg.hold) {
    //if still looping, reset the envelope
    envelope.state = Attack;
    if(envelope.attackRate >= 62) {
      envelope.value = 0;
      envelope.state = envelope.sustainLevel ? Decay : Sustain;
    }
    updateEnvelope();
  } else if(envelope.state == Release || (ssg.hold && ssg.attack == ssg.invert)) {
    //clear envelope once finished
    envelope.value = 0x3ff;
  }

  updateLevel();
}

auto YM2612::Channel::Operator::updateEnvelope() -> void {
  uint key = min(max((uint)pitch.value, 0x300), 0x4ff);
  uint ksr = (octave.value << 2) + ((key - 0x300) >> 7);
  uint rate = 0;

  if(envelope.state == Attack)  rate += (envelope.attackRate  << 1);
  if(envelope.state == Decay)   rate += (envelope.decayRate   << 1);
  if(envelope.state == Sustain) rate += (envelope.sustainRate << 1);
  if(envelope.state == Release) rate += (envelope.releaseRate << 1);

  rate += (ksr >> 3 - envelope.keyScale) * (rate > 0);
  rate  = min(rate, 63);

  auto& entry = envelopeRates[rate >> 2];
  envelope.rate    = rate;
  envelope.divider = entry.divider;
  envelope.steps   = entry.steps[rate & 3];
}

auto YM2612::Channel::Operator::updatePitch() -> void {
  //only channel[2] allows per-operator frequencies
  //implemented by forcing mode to zero (single frequency) for other channels
  //in single frequency mode, operator[3] frequency is used for all operators
  pitch.value = channel.mode ? pitch.reload : channel[3].pitch.reload;
  octave.value = channel.mode ? octave.reload : channel[3].octave.reload;

  updatePhase();
  updateEnvelope();  //due to key scaling
}

auto YM2612::Channel::Operator::updatePhase() -> void {
  uint key = min(max((uint)pitch.value, 0x300), 0x4ff);
  uint ksr = (octave.value << 2) + ((key - 0x300) >> 7);
  uint tuning = detune & 3 ? detunes[(detune & 3) - 1][ksr & 7] >> (3 - (ksr >> 3)) : 0;

  uint lfo = ym2612.lfo.clock >> 2 & 0x1f;
  uint pm = 4 * vibratos[channel.vibrato][lfo & 15] * (-lfo >> 4);
  uint msb = 10;
  while(msb > 4 && ~pitch.value & 1 << msb) msb--;

  phase.delta = pitch.value + (pm >> 10 - msb) << 6 >> 7 - octave.value;
  phase.delta = (!detune.bit(2) ? phase.delta + tuning : phase.delta - tuning) & 0x1ffff;
  phase.delta = (multiple ? phase.delta * multiple : phase.delta >> 1) & 0xfffff;
}

auto YM2612::Channel::Operator::updateLevel() -> void {
  uint lfo = ym2612.lfo.clock & 0x40 ? ym2612.lfo.clock & 0x3f : ~ym2612.lfo.clock & 0x3f;
  uint depth = tremolos[channel.tremolo];

  bool invert = ssg.attack != ssg.invert && envelope.state != Release;
  uint10 value = ssg.enable && invert ? 0x200 - envelope.value : 0 + envelope.value;

  outputLevel = ((totalLevel << 3) + value + (lfoEnable ? lfo << 1 >> depth : 0)) << 3;
}

auto YM2612::Channel::power() -> void {
  leftEnable = true;
  rightEnable = true;

  algorithm = 0;
  feedback = 0;
  vibrato = 0;
  tremolo = 0;

  mode = 0;

  for(auto& op : operators) {
    op.keyOn = false;
    op.lfoEnable = false;
    op.detune = 0;
    op.multiple = 0;
    op.totalLevel = 0;

    op.outputLevel = 0x1fff;
    op.output = 0;
    op.prior = 0;

    op.pitch.value = 0;
    op.pitch.reload = 0;
    op.pitch.latch = 0;

    op.octave.value = 0;
    op.octave.reload = 0;
    op.octave.latch = 0;

    op.phase.value = 0;
    op.phase.delta = 0;

    op.envelope.state = Release;
    op.envelope.rate = 0;
    op.envelope.divider = 11;
    op.envelope.steps = 0;
    op.envelope.value = 0x3ff;

    op.envelope.keyScale = 0;
    op.envelope.attackRate = 0;
    op.envelope.decayRate = 0;
    op.envelope.sustainRate = 0;
    op.envelope.sustainLevel = 0;
    op.envelope.releaseRate = 1;

    op.ssg.enable = false;
    op.ssg.attack = false;
    op.ssg.alternate = false;
    op.ssg.hold = false;
    op.ssg.invert = false;

    op.updatePitch();
    op.updateLevel();
  }
}
