auto APU::Master::run() -> void {
  if(enable == false) {
    center = 0;
    left   = 0;
    right  = 0;

    centerBias = leftBias = rightBias = 0;
    return;
  }

  int sample = 0;
  sample += apu.square1.output;
  sample += apu.square2.output;
  sample +=    apu.wave.output;
  sample +=   apu.noise.output;
  center = (sample * 512) - 16384;

  sample = 0;
  if(square1.leftEnable) sample += apu.square1.output;
  if(square2.leftEnable) sample += apu.square2.output;
  if(   wave.leftEnable) sample +=    apu.wave.output;
  if(  noise.leftEnable) sample +=   apu.noise.output;
  sample = (sample * 512) - 16384;
  sample = (sample * (leftVolume + 1)) / 8;
  left = sample;

  sample = 0;
  if(square1.rightEnable) sample += apu.square1.output;
  if(square2.rightEnable) sample += apu.square2.output;
  if(   wave.rightEnable) sample +=    apu.wave.output;
  if(  noise.rightEnable) sample +=   apu.noise.output;
  sample = (sample * 512) - 16384;
  sample = (sample * (rightVolume + 1)) / 8;
  right = sample;

  //reduce audio volume
  center >>= 1;
  left   >>= 1;
  right  >>= 1;
}

auto APU::Master::read(uint16 addr) -> uint8 {
  if(addr == 0xff24) {  //NR50
    return leftEnable << 7 | leftVolume << 4 | rightEnable << 3 | rightVolume;
  }

  if(addr == 0xff25) {  //NR51
    return noise.leftEnable    << 7
         | wave.leftEnable     << 6
         | square2.leftEnable  << 5
         | square1.leftEnable  << 4
         | noise.rightEnable   << 3
         | wave.rightEnable    << 2
         | square2.rightEnable << 1
         | square1.rightEnable << 0;
  }

  if(addr == 0xff26) {  //NR52
    return enable << 7 | 0x70
         | apu.noise.enable   << 3
         | apu.wave.enable    << 2
         | apu.square2.enable << 1
         | apu.square1.enable << 0;
  }

  return 0xff;
}

auto APU::Master::write(uint16 addr, uint8 data) -> void {
  if(addr == 0xff24) {  //NR50
    leftEnable  = data & 0x80;
    leftVolume  = (data >> 4) & 7;
    rightEnable = data & 0x08;
    rightVolume = (data >> 0) & 7;
  }

  if(addr == 0xff25) {  //NR51
    noise.leftEnable    = data & 0x80;
    wave.leftEnable     = data & 0x40;
    square2.leftEnable  = data & 0x20;
    square1.leftEnable  = data & 0x10;
    noise.rightEnable   = data & 0x08;
    wave.rightEnable    = data & 0x04;
    square2.rightEnable = data & 0x02;
    square1.rightEnable = data & 0x01;
  }

  if(addr == 0xff26) {  //NR52
    enable = data & 0x80;
    if(!enable) {
      //power(bool) resets length counters when true (eg for CGB only)
      apu.square1.power(system.cgb());
      apu.square2.power(system.cgb());
      apu.wave.power(system.cgb());
      apu.noise.power(system.cgb());
      power();
    }
  }
}

auto APU::Master::power() -> void {
  leftEnable = 0;
  leftVolume = 0;
  rightEnable = 0;
  rightVolume = 0;
  noise.leftEnable = 0;
  wave.leftEnable = 0;
  square2.leftEnable = 0;
  square1.leftEnable = 0;
  noise.rightEnable = 0;
  wave.rightEnable = 0;
  square2.rightEnable = 0;
  square1.rightEnable = 0;
  enable = 0;

  center = 0;
  left   = 0;
  right  = 0;

  centerBias = 0;
  leftBias = 0;
  rightBias = 0;
}

auto APU::Master::serialize(serializer& s) -> void {
  s.integer(leftEnable);
  s.integer(leftVolume);
  s.integer(rightEnable);
  s.integer(rightVolume);
  s.integer(noise.leftEnable);
  s.integer(wave.leftEnable);
  s.integer(square2.leftEnable);
  s.integer(square1.leftEnable);
  s.integer(noise.rightEnable);
  s.integer(wave.rightEnable);
  s.integer(square2.rightEnable);
  s.integer(square1.rightEnable);
  s.integer(enable);

  s.integer(center);
  s.integer(left);
  s.integer(right);

  s.integer(centerBias);
  s.integer(leftBias);
  s.integer(rightBias);
}
