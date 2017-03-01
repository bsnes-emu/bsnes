auto YM2612::Channel::runEnvelope(uint2 index) -> void {
  auto& op = operators[index];

  int sustain = op.envelope.sustainLevel < 15 ? op.envelope.sustainLevel << 5 : 0x3f0;
  if(ym2612.envelope.clock & (1 << op.envelope.divider) - 1) return;

  int value = ym2612.envelope.clock >> op.envelope.divider;
  int step = op.envelope.steps >> ~value % 8 * 4 & 0xf;
  if(op.ssg.enable) step <<= 2;  //SSG results in a 4x faster envelope

  if(op.envelope.state == Attack) {
    int next = op.envelope.value + (~uint16(op.envelope.value) * step >> 4) & 0x3ff;
    if(next <= op.envelope.value) {
      op.envelope.value = next;
    } else {
      op.envelope.value = 0;
      op.envelope.state = op.envelope.value < sustain ? Decay : Sustain;
      updateEnvelope(index);
    }
  } else if(!op.ssg.enable || op.envelope.value < 0x200) {
    op.envelope.value = min(op.envelope.value + step, 0x3ff);
    if(op.envelope.state == Decay && op.envelope.value >= sustain) {
      op.envelope.state = Sustain;
      updateEnvelope(index);
    }
  }

  updateLevel(index);
}

auto YM2612::Channel::runPhase(uint2 index) -> void {
  auto& op = operators[index];

  op.phase.value += op.phase.delta;  //advance wave position
  if(!(op.ssg.enable && op.envelope.value >= 0x200)) return;  //SSG loop check

  if(!op.ssg.hold && !op.ssg.pingPong) op.phase.value = 0;

  if(!op.ssg.hold || op.ssg.attack == op.ssg.invert) op.ssg.invert ^= op.ssg.pingPong;

  if(op.envelope.state == Attack) {
    //do nothing; SSG is meant to skip the attack phase
  } else if(op.envelope.state != Release && !op.ssg.hold) {
    //if still looping, reset the envelope
    op.envelope.state = Attack;
    if(op.envelope.attackRate >= 62) {
      op.envelope.value = 0;
      op.envelope.state = op.envelope.sustainLevel ? Decay : Sustain;
    }
    updateEnvelope(index);
  } else if(op.envelope.state == Release || (op.ssg.hold && op.ssg.attack == op.ssg.invert)) {
    //clear envelope once finished
    op.envelope.value = 0x3ff;
  }
  updateLevel(index);
}

auto YM2612::Channel::trigger(uint2 index, bool keyOn) -> void {
  auto& op = operators[index];
  if(op.keyOn == keyOn) return;  //no change

  op.keyOn = keyOn;
  op.envelope.state = Release;
  updateEnvelope(index);

  if(keyOn) {
    //restart phase and envelope generators
    op.phase.value = 0;
    op.envelope.state = Attack;
    updateEnvelope(index);

    if(op.envelope.rate >= 62) {
      //skip attack and possibly decay stages
      op.envelope.value = 0;
      op.envelope.state = op.envelope.sustainLevel ? Decay : Sustain;
      updateEnvelope(index);
    }
  } else if(op.ssg.enable && op.ssg.attack != op.ssg.invert) {
    //SSG-EG key-off
    op.envelope.value = 0x200 - op.envelope.value;
  }

  updateLevel(index);
}

auto YM2612::Channel::updateEnvelope(uint2 index) -> void {
  auto& op = operators[index];

  int key = min(max((uint)op.pitch.value, 0x300), 0x4ff);
  int ksr = op.octave.value * 4 + (key - 0x300) / 0x80;
  int rate = 0;

  if(op.envelope.state == Attack)  rate += (op.envelope.attackRate  << 1);
  if(op.envelope.state == Decay)   rate += (op.envelope.decayRate   << 1);
  if(op.envelope.state == Sustain) rate += (op.envelope.sustainRate << 1);
  if(op.envelope.state == Release) rate += (op.envelope.releaseRate << 1);

  rate += (ksr >> 3 - op.envelope.keyScale) * (rate > 0);
  rate  = min(rate, 63);

  auto& entry = envelopeRates[rate >> 2];
  op.envelope.rate    = rate;
  op.envelope.divider = entry.divider;
  op.envelope.steps   = entry.steps[rate & 3];
}

auto YM2612::Channel::updatePitch(uint2 index) -> void {
  auto& op = operators[index];

  op.pitch.value = mode ? op.pitch.reload : operators[0].pitch.reload;
  op.octave.value = mode ? op.octave.reload : operators[0].octave.reload;

  updatePhase(index);
  updateEnvelope(index);  //due to key scaling
}

auto YM2612::Channel::updatePhase(uint2 index) -> void {
  auto& op = operators[index];

  int ratio = op.multiple ? 2 * op.multiple : 1;

  int detune = op.detune & 3;
  int key = min(max((uint)op.pitch.value, 0x300), 0x4ff);
  int ksr = op.octave.value * 4 + (key - 0x300) / 0x80;
  int tuning = detune ? detunes[detune - 1][ksr & 7] >> (3 - (ksr >> 3)) : 0;

  int lfo = ym2612.lfo.clock >> 2 & 0x1f;
  int pm = 4 * vibratos[vibrato][lfo & 15] * (-lfo >> 4);
  int msb = 10;

  while(msb > 4 && ~op.pitch.value & 1 << msb) msb--;

  op.phase.delta = op.pitch.value + (pm >> 10 - msb) << 6 >> 7 - op.octave.value;

  if(op.detune < 4) op.phase.delta += tuning;
  else              op.phase.delta -= tuning;

  op.phase.delta  &= 0x1ffff;
  op.phase.delta  *= ratio;
  op.phase.delta >>= 1;
  op.phase.delta  &= 0xfffff;
}

auto YM2612::Channel::updateLevel(uint2 index) -> void {
  auto& op = operators[index];

  int lfo = ym2612.lfo.clock & 0x40 ? ym2612.lfo.clock & 0x3f : ~ym2612.lfo.clock & 0x3f;
  int depth = tremolos[tremolo];

  bool invert = op.ssg.attack != op.ssg.invert && op.envelope.state != Release;
  uint10 envelope = op.ssg.enable && invert ? 0x200 - op.envelope.value : 0 + op.envelope.value;

  op.outputLevel   = op.totalLevel << 3;
  op.outputLevel  += envelope;
  op.outputLevel  += (2 * lfo >> depth) * ym2612.lfo.enable;
  op.outputLevel <<= 2 + 1;  //sign bit
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
    op.ssg.pingPong = false;
    op.ssg.hold = false;
    op.ssg.invert = false;
  }

  for(auto index : range(4)) {
    updatePitch(index);
    updateLevel(index);
  }
}
