auto APU::portRead(uint16 addr) -> uint8 {
  //SND_HYPER_CTRL
  if(addr == 0x006a) return (
    channel4.r.volume << 0
  | channel4.r.scale  << 2
  | channel4.r.speed  << 4
  | channel4.r.enable << 7
  );

  //SND_HYPER_CHAN_CTRL
  if(addr == 0x006b) return (
    channel4.r.unknown     << 0
  | channel4.r.leftEnable  << 5
  | channel4.r.rightEnable << 6
  );

  //SND_CH0_PITCH
  if(addr == 0x0080) return channel0.r.pitch.bits(0, 7);
  if(addr == 0x0081) return channel0.r.pitch.bits(8,11);

  //SND_CH1_PITCH
  if(addr == 0x0082) return channel1.r.pitch.bits(0, 7);
  if(addr == 0x0083) return channel1.r.pitch.bits(8,11);

  //SND_CH2_PITCH
  if(addr == 0x0084) return channel2.r.pitch.bits(0, 7);
  if(addr == 0x0085) return channel2.r.pitch.bits(8,11);

  //SND_CH3_PITCH
  if(addr == 0x0086) return channel3.r.pitch.bits(0, 7);
  if(addr == 0x0087) return channel3.r.pitch.bits(8,11);

  //SND_CH0_VOL
  if(addr == 0x0088) return (
    channel0.r.volumeRight << 0
  | channel0.r.volumeLeft  << 4
  );

  //SND_CH1_VOL
  if(addr == 0x0089) return (
    channel1.r.volumeRight << 0
  | channel1.r.volumeLeft  << 4
  );

  //SND_CH2_VOL
  if(addr == 0x008a) return (
    channel2.r.volumeRight << 0
  | channel2.r.volumeLeft  << 4
  );

  //SND_CH3_VOL
  if(addr == 0x008b) return (
    channel3.r.volumeRight << 0
  | channel3.r.volumeLeft  << 4
  );

  //SND_SWEEP_VALUE
  if(addr == 0x008c) return channel2.r.sweepValue;

  //SND_SWEEP_TIME
  if(addr == 0x008d) return channel2.r.sweepTime;

  //SND_NOISE
  //(noiseReset [bit 3] always reads as zero)
  if(addr == 0x008e) return (
    channel3.r.noiseMode   << 0
  | channel3.r.noiseUpdate << 4
  );

  //SND_WAVE_BASE
  if(addr == 0x008f) return r.waveBase;

  //SND_CTRL
  if(addr == 0x0090) return (
    channel0.r.enable << 0
  | channel1.r.enable << 1
  | channel2.r.enable << 2
  | channel3.r.enable << 3
  | channel1.r.voice  << 5
  | channel2.r.sweep  << 6
  | channel3.r.noise  << 7
  );

  //SND_OUTPUT
  if(addr == 0x0091) return (
    r.speakerEnable   << 0
  | r.speakerShift    << 1
  | r.headphoneEnable << 3
  | 1 << 7  //headphone connected
  );

  //SND_RANDOM
  if(addr == 0x0092) return channel3.s.noiseLFSR.bits(0, 7);
  if(addr == 0x0093) return channel3.s.noiseLFSR.bits(8,14);

  //SND_VOICE_CTRL
  if(addr == 0x0094) return (
    channel1.r.voiceEnableRight << 0
  | channel1.r.voiceEnableLeft  << 2
  );

  return 0x00;
}

auto APU::portWrite(uint16 addr, uint8 data) -> void {
  //SND_HYPER_CTRL
  if(addr == 0x006a) {
    channel4.r.volume = data.bits(0,1);
    channel4.r.scale  = data.bits(2,3);
    channel4.r.speed  = data.bits(4,6);
    channel4.r.enable = data.bit (7);
  }

  //SND_HYPER_CHAN_CTRL
  if(addr == 0x006b) {
    channel4.r.unknown     = data.bits(0,3);
    channel4.r.leftEnable  = data.bit (5);
    channel4.r.rightEnable = data.bit (6);
  }

  //SND_CH0_PITCH
  if(addr == 0x0080) { channel0.r.pitch.bits(0, 7) = data.bits(0,7); return; }
  if(addr == 0x0081) { channel0.r.pitch.bits(8,11) = data.bits(0,3); return; }

  //SND_CH1_PITCH
  if(addr == 0x0082) { channel1.r.pitch.bits(0, 7) = data.bits(0,7); return; }
  if(addr == 0x0083) { channel1.r.pitch.bits(8,11) = data.bits(0,3); return; }

  //SND_CH2_PITCH
  if(addr == 0x0084) { channel2.r.pitch.bits(0, 7) = data.bits(0,7); return; }
  if(addr == 0x0085) { channel2.r.pitch.bits(8,11) = data.bits(0,3); return; }

  //SND_CH3_PITCH
  if(addr == 0x0086) { channel3.r.pitch.bits(0, 7) = data.bits(0,7); return; }
  if(addr == 0x0087) { channel3.r.pitch.bits(8,11) = data.bits(0,3); return; }

  //SND_CH0_VOL
  if(addr == 0x0088) {
    channel0.r.volumeRight = data.bits(0,3);
    channel0.r.volumeLeft  = data.bits(4,7);
    return;
  }

  //SND_CH1_VOL
  if(addr == 0x0089) {
    channel1.r.volumeRight = data.bits(0,3);
    channel1.r.volumeLeft  = data.bits(4,7);
    return;
  }

  //SND_CH2_VOL
  if(addr == 0x008a) {
    channel2.r.volumeRight = data.bits(0,3);
    channel2.r.volumeLeft  = data.bits(4,7);
    return;
  }

  //SND_CH3_VOL
  if(addr == 0x008b) {
    channel3.r.volumeRight = data.bits(0,3);
    channel3.r.volumeLeft  = data.bits(4,7);
    return;
  }

  //SND_SWEEP_VALUE
  if(addr == 0x008c) {
    channel2.r.sweepValue = data;
    return;
  }

  //SND_SWEEP_TIME
  if(addr == 0x008d) {
    channel2.r.sweepTime = data.bits(0,4);
    return;
  }

  //SND_NOISE
  if(addr == 0x008e) {
    channel3.r.noiseMode   = data.bits(0,2);
    channel3.r.noiseReset  = data.bit (3);
    channel3.r.noiseUpdate = data.bit (4);
    return;
  }

  //SND_WAVE_BASE
  if(addr == 0x008f) {
    r.waveBase = data;
    return;
  }

  //SND_CTRL
  if(addr == 0x0090) {
    channel0.r.enable = data.bit(0);
    channel1.r.enable = data.bit(1);
    channel2.r.enable = data.bit(2);
    channel3.r.enable = data.bit(3);
    channel1.r.voice  = data.bit(5);
    channel2.r.sweep  = data.bit(6);
    channel3.r.noise  = data.bit(7);
    return;
  }

  //SND_OUTPUT
  if(addr == 0x0091) {
    r.speakerEnable   = data.bit (0);
    r.speakerShift    = data.bits(1,2);
    r.headphoneEnable = data.bit (3);
    return;
  }

  //SND_VOICE_CTRL
  if(addr == 0x0094) {
    channel1.r.voiceEnableRight = data.bits(0,1);
    channel1.r.voiceEnableLeft  = data.bits(2,3);
    return;
  }
}
