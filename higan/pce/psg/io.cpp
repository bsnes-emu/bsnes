auto PSG::write(uint4 addr, uint8 data) -> void {
  if(addr == 0x00) {
    io.channel = data.bits(0,2);
  }

  if(addr == 0x01) {
    io.volumeRight = data.bits(0,3);
    io.volumeLeft = data.bits(4,7);
  }

  if(addr >= 0x02 && addr <= 0x06 && io.channel <= 5) {
    channel[io.channel].write(addr, data);
  }

  if(addr == 0x07 && io.channel >= 4 && io.channel <= 5) {
    channel[io.channel].write(addr, data);
  }

  if(addr == 0x08) {
    io.lfoFrequency = data;
  }

  if(addr == 0x09) {
    io.lfoControl = data.bits(0,1);
    io.lfoEnable = data.bit(7);
    if(io.lfoEnable) {
      channel[1].io.waveSample = channel[1].io.waveBuffer[channel[1].io.waveOffset = 0];
    }
  }
}

auto PSG::Channel::write(uint4 addr, uint8 data) -> void {
  if(addr == 0x02) {
    io.waveFrequency.bits(0,7) = data.bits(0,7);
    io.wavePeriod = io.waveFrequency;
  }

  if(addr == 0x03) {
    io.waveFrequency.bits(8,11) = data.bits(0,3);
    io.wavePeriod = io.waveFrequency;
  }

  if(addr == 0x04) {
    if(io.direct && !data.bit(6)) {
      io.waveOffset = 0;
      io.waveSample = io.waveBuffer[io.waveOffset];
    }
    if(!io.enable && data.bit(7) && !data.bit(6)) {
      io.waveOffset++;
      io.waveSample = io.waveBuffer[io.waveOffset];
    }
    io.volume = data.bits(0,4);
    io.direct = data.bit(6);
    io.enable = data.bit(7);
  }

  if(addr == 0x05) {
    io.volumeRight = data.bits(0,3);
    io.volumeLeft = data.bits(4,7);
  }

  if(addr == 0x06) {
    if(!io.direct) {
      io.waveBuffer[io.waveOffset] = data.bits(0,4);
      if(!io.enable) io.waveOffset++;
    }
    if(io.enable) {
      io.waveSample = data.bits(0,4);
    }
  }

  //channels 4 and 5 only
  if(addr == 0x07) {
    if(!io.noiseEnable && data.bit(7)) {
      io.noisePeriod = ~data.bits(0,4) << 7;
      io.noiseSample = 0;
    }
    io.noiseFrequency = data.bits(0,4);
    io.noiseEnable = data.bit(7);
  }
}
