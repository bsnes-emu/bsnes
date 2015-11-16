auto APU::Square1::runsweep(bool update) -> void {
  if(!sweep.enable) return;

  sweep.negate = sweep.direction;
  uint delta = shadowfrequency >> sweep.shift;
  int updatefrequency = shadowfrequency + (sweep.negate ? -delta : delta);

  if(updatefrequency > 2047) {
    enable = false;
  } else if(sweep.shift && update) {
    shadowfrequency = updatefrequency;
    frequency = updatefrequency;
    period = 2 * (2048 - frequency);
  }
}

auto APU::Square1::clocksweep() -> void {
  if(enable && sweep.frequency && --sweep.period == 0) {
    sweep.period = sweep.frequency;
    runsweep(1);
    runsweep(0);
  }
}

auto APU::Square1::read(uint addr) const -> uint8 {
  switch(addr) {
  case 0: return (sweep.shift << 0) | (sweep.direction << 3) | (sweep.frequency << 4);
  case 1: return (duty << 6);
  case 2: return (envelope.frequency << 0) | (envelope.direction << 3) | (envelope.volume << 4);
  case 3: return 0;
  case 4: return (counter << 6);
  }
}

auto APU::Square1::write(uint addr, uint8 byte) -> void {
  switch(addr) {
  case 0:  //NR10
    if(sweep.negate && sweep.direction && !(byte & 0x08)) enable = false;
    sweep.shift     = byte >> 0;
    sweep.direction = byte >> 3;
    sweep.frequency = byte >> 4;
    break;

  case 1:  //NR11
    length = byte >> 0;
    duty   = byte >> 6;
    break;

  case 2:  //NR12
    envelope.frequency = byte >> 0;
    envelope.direction = byte >> 3;
    envelope.volume    = byte >> 4;
    if(!envelope.dacEnable()) enable = false;
    break;

  case 3:  //NR13
    frequency = (frequency & 0xff00) | (byte << 0);
    break;

  case 4:  //NR14
    frequency = (frequency & 0x00ff) | (byte << 8);
    counter    = byte >> 6;
    initialize = byte >> 7;

    if(initialize) {
      enable = envelope.dacEnable();
      period = 2 * (2048 - frequency);
      envelope.period = envelope.frequency;
      volume = envelope.volume;
      shadowfrequency = frequency;
      sweep.period = sweep.frequency;
      sweep.enable = sweep.period || sweep.shift;
      sweep.negate = false;
      if(sweep.shift) runsweep(0);
    }

    break;
  }
}

auto APU::Square1::power() -> void {
  envelope.frequency = 0;
  envelope.direction = 0;
  envelope.direction = 0;
  envelope.period = 0;
  sweep.shift = 0;
  sweep.direction = 0;
  sweep.frequency = 0;
  sweep.enable = 0;
  sweep.negate = 0;
  sweep.period = 0;
  enable = 0;
  length = 0;
  duty = 0;
  frequency = 0;
  counter = 0;
  initialize = 0;
  shadowfrequency = 0;
  signal = 0;
  output = 0;
  period = 0;
  phase = 0;
  volume = 0;
}
