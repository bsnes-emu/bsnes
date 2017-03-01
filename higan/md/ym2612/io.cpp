auto YM2612::readStatus() -> uint8 {
  //d7 = busy
  return timerA.line << 0 | timerB.line << 1;
}

auto YM2612::writeAddress(uint9 data) -> void {
  io.address = data;
}

auto YM2612::writeData(uint8 data) -> void {
  switch(io.address) {

  //LFO
  case 0x022: {
    lfo.rate = data.bits(0,2);
    lfo.enable = data.bit(3);
    break;
  }

  //timer A period (high)
  case 0x024: {
    timerA.period.bits(2,9) = data.bits(0,7);
    break;
  }

  //timer A period (low)
  case 0x025: {
    timerA.period.bits(0,1) = data.bits(0,1);
    break;
  }

  //timer B period
  case 0x026: {
    timerB.period.bits(0,7) = data.bits(0,7);
    break;
  }

  //timer control
  case 0x027: {
    //d6,d7 = mode (unimplemented; treated as mode 0 always)

    //reload period on 0->1 transition
    if(!timerA.enable && data.bit(0)) timerA.counter = timerA.period;
    if(!timerB.enable && data.bit(1)) timerB.counter = timerB.period;

    timerA.enable = data.bit(0);
    timerB.enable = data.bit(1);
    timerA.irq = data.bit(2);
    timerB.irq = data.bit(3);

    if(data.bit(4)) timerA.line = 0;
    if(data.bit(5)) timerB.line = 0;

    channels[2].mode = data.bits(6,7);
    for(uint index : range(4)) channels[2].updatePitch(index);

    break;
  }

  //key on/off
  case 0x28: {
    //0,1,2,4,5,6 => 0,1,2,3,4,5
    uint index = data.bits(0,2);
    if(index == 3 || index == 7) break;
    if(index >= 4) index--;
    auto& channel = channels[index];

    channel.trigger(0, data.bit(4));
    channel.trigger(1, data.bit(5));
    channel.trigger(2, data.bit(6));
    channel.trigger(3, data.bit(7));

    break;
  }

  //DAC sample
  case 0x2a: {
    dac.sample = data;
    break;
  }

  //DAC enable
  case 0x2b: {
    dac.enable = data.bit(7);
    break;
  }

  }

  if(io.address % 0x100 < 0x30 || io.address % 0x100 >= 0xb8 || io.address % 4 == 3) return;

  uint voice = io.address % 4 + 3 * (io.address >> 8);
  uint index = (voice * 4 + (io.address >> 3 & 1) + (io.address >> 1 & 2)) % 4;

  auto& channel = channels[voice];
  auto& op = channel.operators[index];

  switch(io.address & 0x0f0) {

  //detune, multiple
  case 0x030: {
    op.multiple = data.bits(0,3);
    op.detune = data.bits(4,6);
    channel.updatePhase(index);
    break;
  }

  //total level
  case 0x040: {
    op.totalLevel = data.bits(0,6);
    channel.updateLevel(index);
    break;
  }

  //key scaling, attack rate
  case 0x050: {
    op.envelope.attackRate = data.bits(0,4);
    op.envelope.keyScale = data.bits(6,7);
    channel.updateEnvelope(index);
    channel.updatePhase(index);
    break;
  }

  //LFO enable, decay rate
  case 0x060: {
    op.envelope.decayRate = data.bits(0,4);
    op.lfoEnable = data.bit(7);
    channel.updateEnvelope(index);
    channel.updateLevel(index);
    break;
  }

  //sustain rate
  case 0x070: {
    op.envelope.sustainRate = data.bits(0,4);
    channel.updateEnvelope(index);
    break;
  }

  //sustain level, release rate
  case 0x080: {
    op.envelope.releaseRate = data.bits(0,3) << 1 | 1;
    op.envelope.sustainLevel = data.bits(4,7);
    channel.updateEnvelope(index);
    break;
  }

  //SSG-EG
  case 0x090: {
    op.ssg.hold = data.bit(0);
    op.ssg.pingPong = data.bit(1);
    op.ssg.attack = data.bit(2);
    op.ssg.enable = data.bit(3);
    break;
  }

  }

  switch(io.address & 0x0fc) {

  //pitch (low)
  case 0x0a0: {
    channel.operators[0].pitch.value = channel.operators[0].pitch.latch | data;
    channel.operators[0].octave.value = channel.operators[0].octave.latch;
    for(auto index : range(4)) channel.updatePitch(index);
    break;
  }

  //pitch (high)
  case 0x0a4: {
    channel.operators[0].pitch.latch = data << 8;
    channel.operators[0].octave.latch = data >> 3;
    break;
  }

  //...
  case 0x0a8: {
    if(voice > 2) break;
    channels[2].operators[1 + voice].pitch.value = channels[2].operators[1 + voice].pitch.latch | data;
    channels[2].operators[1 + voice].octave.value = channels[2].operators[1 + voice].octave.latch;
    channels[2].updatePitch(1 + voice);
    break;
  }

  //...
  case 0x0ac: {
    if(voice > 2) break;
    channels[2].operators[1 + voice].pitch.latch = data << 8;
    channels[2].operators[1 + voice].octave.latch = data >> 3;
    break;
  }

  //algorithm, feedback
  case 0x0b0: {
    channel.algorithm = data.bits(0,2);
    channel.feedback = data.bits(3,5);
    break;
  }

  //panning, tremolo, vibrato
  case 0x0b4: {
    channel.vibrato = data.bits(0,2);
    channel.tremolo = data.bits(4,5);
    channel.rightEnable = data.bit(6);
    channel.leftEnable = data.bit(7);
    for(auto index : range(4)) {
      channel.updateLevel(index);
      channel.updatePhase(index);
    }
    break;
  }

  }
}
