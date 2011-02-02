#ifdef APU_CPP

void APU::Master::run() {
}

uint8 APU::Master::read(unsigned r) {
  if(r == 0) {
    return (so2_enable << 7) | (so2_volume << 4) | (so1_enable << 3) | (so1_volume << 0);
  }

  if(r == 1) {
    return (channel4_so2_enable << 7)
         | (channel3_so2_enable << 6)
         | (channel2_so2_enable << 5)
         | (channel1_so2_enable << 4)
         | (channel4_so1_enable << 3)
         | (channel3_so1_enable << 2)
         | (channel2_so1_enable << 1)
         | (channel1_so1_enable << 0);
  }

  if(r == 2) {
    return (enable << 7)
         | (channel4_enable << 3)
         | (channel3_enable << 2)
         | (channel2_enable << 1)
         | (channel1_enable << 0);
  }
}

void APU::Master::write(unsigned r, uint8 data) {
  if(r == 0) {
    so2_enable = data & 0x80;
    so2_volume = (data >> 4) & 7;
    so1_enable = data & 0x08;
    so1_volume = (data >> 0) & 7;
  }

  if(r == 1) {
    channel4_so2_enable = data & 0x80;
    channel3_so2_enable = data & 0x40;
    channel2_so2_enable = data & 0x20;
    channel1_so2_enable = data & 0x10;
    channel4_so1_enable = data & 0x08;
    channel3_so1_enable = data & 0x04;
    channel2_so1_enable = data & 0x02;
    channel1_so1_enable = data & 0x01;
  }

  if(r == 2) {
    enable = data & 0x80;
    channel4_enable = data & 0x08;
    channel3_enable = data & 0x04;
    channel2_enable = data & 0x02;
    channel1_enable = data & 0x01;
  }
}

void APU::Master::power() {
  so2_enable = 0;
  so2_volume = 0;
  so1_enable = 0;
  so1_volume = 0;
  channel4_so2_enable = 0;
  channel3_so2_enable = 0;
  channel2_so2_enable = 0;
  channel1_so2_enable = 0;
  channel4_so1_enable = 0;
  channel3_so1_enable = 0;
  channel2_so1_enable = 0;
  channel1_so1_enable = 0;
  enable = 0;
  channel4_enable = 0;
  channel3_enable = 0;
  channel2_enable = 0;
  channel1_enable = 0;
}

void APU::Master::serialize(serializer &s) {
  s.integer(so2_enable);
  s.integer(so2_volume);
  s.integer(so1_enable);
  s.integer(so1_volume);
  s.integer(channel4_so2_enable);
  s.integer(channel3_so2_enable);
  s.integer(channel2_so2_enable);
  s.integer(channel1_so2_enable);
  s.integer(channel4_so1_enable);
  s.integer(channel3_so1_enable);
  s.integer(channel2_so1_enable);
  s.integer(channel1_so1_enable);
  s.integer(enable);
  s.integer(channel4_enable);
  s.integer(channel3_enable);
  s.integer(channel2_enable);
  s.integer(channel1_enable);
}

#endif
