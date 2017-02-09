auto PSG::write(uint4 addr, uint8 data) -> void {
  if(addr == 0x00) {
    io.channel = data.bits(0,2);
    return;
  }

  if(addr == 0x01) {
    io.volumeRight = data.bits(0,3);
    io.volumeLeft = data.bits(4,7);
    return;
  }

  uint3 C = io.channel;

  if(addr == 0x02) {
    if(C == 6 || C == 7) return;
    channel[C].io.frequency.bits(0,7) = data.bits(0,7);
    return;
  }

  if(addr == 0x03) {
    if(C == 6 || C == 7) return;
    channel[C].io.frequency.bits(8,11) = data.bits(0,3);
    return;
  }

  if(addr == 0x04) {
    if(C == 6 || C == 7) return;
    if(channel[C].io.direct && !data.bit(6)) {
      channel[C].io.waveOffset = 0;
    }
    if(!channel[C].io.enable && data.bit(7)) {
      channel[C].io.waveOffset = 0;
      channel[C].io.period = channel[C].io.frequency;
    }
    channel[C].io.volume = data.bits(0,3);
    channel[C].io.direct = data.bit(6);
    channel[C].io.enable = data.bit(7);
    return;
  }

  if(addr == 0x05) {
    if(C == 6 || C == 7) return;
    channel[C].io.volumeRight = data.bits(0,3);
    channel[C].io.volumeLeft = data.bits(4,7);
    return;
  }

  if(addr == 0x06) {
    if(C == 6 || C == 7) return;
    if(channel[C].io.direct) {
      channel[C].io.waveDirect = data.bits(0,4);
    } else if(!channel[C].io.enable) {
      uint5 O = channel[C].io.waveOffset++;
      channel[C].io.waveData[O] = data.bits(0,4);
    }
    return;
  }

  if(addr == 0x07) {
    if(C != 4 && C != 5) return;
    if(!channel[C].io.noiseEnable && data.bit(7)) {
      channel[C].io.noisePeriod = ~data.bits(0,4) << 7;
      channel[C].io.noiseSample = 0;
    }
    channel[C].io.noiseFrequency = data.bits(0,4);
    channel[C].io.noiseEnable = data.bit(7);
    return;
  }

  if(addr == 0x08) {
    io.lfoFrequency = data;
    return;
  }

  if(addr == 0x09) {
    io.lfoControl = data.bits(0,1);
    io.lfoEnable = data.bit(7);
    return;
  }
}
