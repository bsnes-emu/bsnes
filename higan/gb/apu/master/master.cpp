auto APU::Master::run() -> void {
  if(enable == false) {
    center = 0;
    left   = 0;
    right  = 0;

    center_bias = left_bias = right_bias = 0;
    return;
  }

  int sample = 0;
  sample += apu.square1.output;
  sample += apu.square2.output;
  sample +=    apu.wave.output;
  sample +=   apu.noise.output;
  center = (sample * 512) - 16384;

  sample = 0;
  if(channel1_left_enable) sample += apu.square1.output;
  if(channel2_left_enable) sample += apu.square2.output;
  if(channel3_left_enable) sample +=    apu.wave.output;
  if(channel4_left_enable) sample +=   apu.noise.output;
  sample = (sample * 512) - 16384;
  sample = (sample * (left_volume + 1)) / 8;
  left = sample;

  sample = 0;
  if(channel1_right_enable) sample += apu.square1.output;
  if(channel2_right_enable) sample += apu.square2.output;
  if(channel3_right_enable) sample +=    apu.wave.output;
  if(channel4_right_enable) sample +=   apu.noise.output;
  sample = (sample * 512) - 16384;
  sample = (sample * (right_volume + 1)) / 8;
  right = sample;

  //reduce audio volume
  center >>= 1;
  left   >>= 1;
  right  >>= 1;
}

auto APU::Master::write(uint r, uint8 data) -> void {
  if(r == 0) {  //$ff24  NR50
    left_in_enable  = data & 0x80;
    left_volume     = (data >> 4) & 7;
    right_in_enable = data & 0x08;
    right_volume    = (data >> 0) & 7;
  }

  if(r == 1) {  //$ff25  NR51
    channel4_left_enable  = data & 0x80;
    channel3_left_enable  = data & 0x40;
    channel2_left_enable  = data & 0x20;
    channel1_left_enable  = data & 0x10;
    channel4_right_enable = data & 0x08;
    channel3_right_enable = data & 0x04;
    channel2_right_enable = data & 0x02;
    channel1_right_enable = data & 0x01;
  }

  if(r == 2) {  //$ff26  NR52
    enable = data & 0x80;
  }
}

auto APU::Master::power() -> void {
  left_in_enable = 0;
  left_volume = 0;
  right_in_enable = 0;
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

  center_bias = 0;
  left_bias = 0;
  right_bias = 0;
}

auto APU::Master::serialize(serializer& s) -> void {
  s.integer(left_in_enable);
  s.integer(left_volume);
  s.integer(right_in_enable);
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

  s.integer(center_bias);
  s.integer(left_bias);
  s.integer(right_bias);
}
