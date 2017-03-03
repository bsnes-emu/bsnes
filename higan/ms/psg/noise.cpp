auto PSG::Noise::run() -> void {
  if(counter--) return;

  if(rate == 0) counter = 0x10;
  if(rate == 1) counter = 0x20;
  if(rate == 2) counter = 0x40;
  if(rate == 3) counter = pitch;  //shared with tone2

  if(clock ^= 1) {  //0->1 transition
    output = !lfsr.bit(0);
    lfsr = (lfsr.bit(0) ^ (lfsr.bit(3) & enable)) << 15 | lfsr >> 1;
  }
}

auto PSG::Noise::power() -> void {
  volume = ~0;
  counter = 0;
  pitch = 0;
  enable = 0;
  rate = 0;
  lfsr = 0x8000;
  clock = 0;
  output = 0;

  left = 1;
  right = 1;
}
