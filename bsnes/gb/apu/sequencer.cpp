auto APU::Sequencer::run() -> void {
  if(enable == false) {
    center = 0;
    left   = 0;
    right  = 0;
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

auto APU::Sequencer::read(uint16 addr) -> uint8 {
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

auto APU::Sequencer::write(uint16 addr, uint8 data) -> void {
  if(addr == 0xff24) {  //NR50
    leftEnable  = bit1(data,7);
    leftVolume  = bits(data,4-6);
    rightEnable = bit1(data,3);
    rightVolume = bits(data,0-2);
  }

  if(addr == 0xff25) {  //NR51
    noise.leftEnable    = bit1(data,7);
    wave.leftEnable     = bit1(data,6);
    square2.leftEnable  = bit1(data,5);
    square1.leftEnable  = bit1(data,4);
    noise.rightEnable   = bit1(data,3);
    wave.rightEnable    = bit1(data,2);
    square2.rightEnable = bit1(data,1);
    square1.rightEnable = bit1(data,0);
  }

  if(addr == 0xff26) {  //NR52
    if(enable != bit1(data,7)) {
      enable = bit1(data,7);

      if(!enable) {
        //power(bool) resets length counters when true (eg for CGB only)
        apu.square1.power(Model::GameBoyColor());
        apu.square2.power(Model::GameBoyColor());
        apu.wave.power(Model::GameBoyColor());
        apu.noise.power(Model::GameBoyColor());
        power();
      } else {
        apu.phase = 0;
      }
    }
  }
}

auto APU::Sequencer::power() -> void {
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
}

auto APU::Sequencer::serialize(serializer& s) -> void {
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
}
