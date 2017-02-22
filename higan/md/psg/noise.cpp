auto PSG::Noise::run() -> void {
  auto latch = clock;

  counter++;
  if(rate == 0) output ^= !counter.bits(0,3);
  if(rate == 1) output ^= !counter.bits(0,4);
  if(rate == 2) output ^= !counter.bits(0,5);
  if(rate == 3) output ^= psg.tone2.clock;

  if(!latch && clock) {
    auto eor = enable ? ~lfsr >> 3 : 0;
    lfsr = (lfsr ^ eor) << 15 | lfsr >> 1;
  }

  output = lfsr.bit(0);
}

auto PSG::Noise::power() -> void {
  volume = ~0;
  counter = 0;
  enable = 0;
  rate = 0;
  lfsr = 0x8000;
  clock = 0;
  output = 0;
}
