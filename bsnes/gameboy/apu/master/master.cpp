#ifdef APU_CPP

void APU::Master::run() {
  signed sample = 0, channels = 4;
  sample += apu.square1.output;
  sample += apu.square2.output;
  sample +=    apu.wave.output;
  sample +=   apu.noise.output;
  sample /= channels;
  if(sample < -32768) sample = -32768;
  if(sample > +32767) sample = +32767;
  center = sample;

  sample = 0;
  channels = 0;
  if(channel1_left_enable) { sample += apu.square1.output; channels++; }
  if(channel2_left_enable) { sample += apu.square2.output; channels++; }
  if(channel3_left_enable) { sample +=    apu.wave.output; channels++; }
  if(channel4_left_enable) { sample +=   apu.noise.output; channels++; }
  if(channels) sample /= channels;
  left = sample;

  switch(left_volume) {
    case 0: left *= 0.125; break;
    case 1: left *= 0.250; break;
    case 2: left *= 0.375; break;
    case 3: left *= 0.500; break;
    case 4: left *= 0.625; break;
    case 5: left *= 0.750; break;
    case 6: left *= 0.875; break;
    case 7: left *= 1.000; break;
  }
  if(left_enable == false) left = 0;

  sample = 0;
  channels = 0;
  if(channel1_right_enable) { sample += apu.square1.output; channels++; }
  if(channel2_right_enable) { sample += apu.square2.output; channels++; }
  if(channel3_right_enable) { sample +=    apu.wave.output; channels++; }
  if(channel4_right_enable) { sample +=   apu.noise.output; channels++; }
  if(channels) sample /= channels;
  right = sample;

  switch(right_volume) {
    case 0: right *= 0.125; break;
    case 1: right *= 0.250; break;
    case 2: right *= 0.375; break;
    case 3: right *= 0.500; break;
    case 4: right *= 0.625; break;
    case 5: right *= 0.750; break;
    case 6: right *= 0.875; break;
    case 7: right *= 1.000; break;
  }
  if(right_enable == false) right = 0;

  if(left_enable == false && right_enable == false) {
    left  = center;
    right = center;
  }

  if(enable == false) {
    center = 0;
    left   = 0;
    right  = 0;
  }
}

void APU::Master::write(unsigned r, uint8 data) {
  if(r == 0) {
    left_enable  = data & 0x80;
    left_volume  = (data >> 4) & 7;
    right_enable = data & 0x08;
    right_volume = (data >> 0) & 7;
  }

  if(r == 1) {
    channel4_left_enable  = data & 0x80;
    channel3_left_enable  = data & 0x40;
    channel2_left_enable  = data & 0x20;
    channel1_left_enable  = data & 0x10;
    channel4_right_enable = data & 0x08;
    channel3_right_enable = data & 0x04;
    channel2_right_enable = data & 0x02;
    channel1_right_enable = data & 0x01;
  }

  if(r == 2) {
    enable = data & 0x80;
  }
}

void APU::Master::power() {
  left_enable = 0;
  left_volume = 0;
  right_enable = 0;
  right_volume = 0;
  channel4_left_enable  = 0;
  channel3_left_enable  = 0;
  channel2_left_enable  = 0;
  channel1_left_enable  = 0;
  channel4_right_enable = 0;
  channel3_right_enable = 0;
  channel2_right_enable = 0;
  channel1_right_enable = 0;
  enable = 0;

  center = 0;
  left   = 0;
  right  = 0;
}

void APU::Master::serialize(serializer &s) {
  s.integer(left_enable);
  s.integer(left_volume);
  s.integer(right_enable);
  s.integer(right_volume);
  s.integer(channel4_left_enable);
  s.integer(channel3_left_enable);
  s.integer(channel2_left_enable);
  s.integer(channel1_left_enable);
  s.integer(channel4_right_enable);
  s.integer(channel3_right_enable);
  s.integer(channel2_right_enable);
  s.integer(channel1_right_enable);
  s.integer(enable);

  s.integer(center);
  s.integer(left);
  s.integer(right);
}

#endif
