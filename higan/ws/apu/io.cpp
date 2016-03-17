auto APU::portRead(uint16 addr) -> uint8 {
  //SDMA_SRC
  if(addr == 0x004a) return dma.s.source.bits( 0, 7);
  if(addr == 0x004b) return dma.s.source.bits( 8,15);
  if(addr == 0x004c) return dma.s.source.bits(16,19);

  //SDMA_LEN
  if(addr == 0x004e) return dma.s.length.bits( 0, 7);
  if(addr == 0x004f) return dma.s.length.bits( 8,15);
  if(addr == 0x0050) return dma.s.length.bits(16,19);

  //SDMA_CTRL
  if(addr == 0x0052) return (
    dma.r.rate      << 0
  | dma.r.unknown   << 2
  | dma.r.loop      << 3
  | dma.r.target    << 4
  | dma.r.direction << 6
  | dma.r.enable    << 7
  );

  //SND_HYPER_CTRL
  if(addr == 0x006a) return (
    channel5.r.volume << 0
  | channel5.r.scale  << 2
  | channel5.r.speed  << 4
  | channel5.r.enable << 7
  );

  //SND_HYPER_CHAN_CTRL
  if(addr == 0x006b) return (
    channel5.r.unknown     << 0
  | channel5.r.leftEnable  << 5
  | channel5.r.rightEnable << 6
  );

  //SND_CH1_PITCH
  if(addr == 0x0080) return channel1.r.pitch.bits(0, 7);
  if(addr == 0x0081) return channel1.r.pitch.bits(8,11);

  //SND_CH2_PITCH
  if(addr == 0x0082) return channel2.r.pitch.bits(0, 7);
  if(addr == 0x0083) return channel2.r.pitch.bits(8,11);

  //SND_CH3_PITCH
  if(addr == 0x0084) return channel3.r.pitch.bits(0, 7);
  if(addr == 0x0085) return channel3.r.pitch.bits(8,11);

  //SND_CH4_PITCH
  if(addr == 0x0086) return channel4.r.pitch.bits(0, 7);
  if(addr == 0x0087) return channel4.r.pitch.bits(8,11);

  //SND_CH1_VOL
  if(addr == 0x0088) return (
    channel1.r.volumeRight << 0
  | channel1.r.volumeLeft  << 4
  );

  //SND_CH2_VOL
  if(addr == 0x0089) return (
    channel2.r.volumeRight << 0
  | channel2.r.volumeLeft  << 4
  );

  //SND_CH3_VOL
  if(addr == 0x008a) return (
    channel3.r.volumeRight << 0
  | channel3.r.volumeLeft  << 4
  );

  //SND_CH4_VOL
  if(addr == 0x008b) return (
    channel4.r.volumeRight << 0
  | channel4.r.volumeLeft  << 4
  );

  //SND_SWEEP_VALUE
  if(addr == 0x008c) return channel3.r.sweepValue;

  //SND_SWEEP_TIME
  if(addr == 0x008d) return channel3.r.sweepTime;

  //SND_NOISE
  //(noiseReset [bit 3] always reads as zero)
  if(addr == 0x008e) return (
    channel4.r.noiseMode   << 0
  | channel4.r.noiseUpdate << 4
  );

  //SND_WAVE_BASE
  if(addr == 0x008f) return r.waveBase;

  //SND_CTRL
  if(addr == 0x0090) return (
    channel1.r.enable << 0
  | channel2.r.enable << 1
  | channel3.r.enable << 2
  | channel4.r.enable << 3
  | channel2.r.voice  << 5
  | channel3.r.sweep  << 6
  | channel4.r.noise  << 7
  );

  //SND_OUTPUT
  if(addr == 0x0091) return (
    r.speakerEnable   << 0
  | r.speakerShift    << 1
  | r.headphoneEnable << 3
  | 1 << 7  //headphone connected
  );

  //SND_RANDOM
  if(addr == 0x0092) return channel4.s.noiseLFSR.bits(0, 7);
  if(addr == 0x0093) return channel4.s.noiseLFSR.bits(8,14);

  //SND_VOICE_CTRL
  if(addr == 0x0094) return (
    channel2.r.voiceEnableRight << 0
  | channel2.r.voiceEnableLeft  << 2
  );

  //SND_HYPERVOICE
  if(addr == 0x0095) return channel5.s.data;

  return 0x00;
}

auto APU::portWrite(uint16 addr, uint8 data) -> void {
  //SDMA_SRC
  if(addr == 0x004a) dma.r.source.bits( 0, 7) = data.bits(0,7);
  if(addr == 0x004b) dma.r.source.bits( 8,15) = data.bits(0,7);
  if(addr == 0x004c) dma.r.source.bits(16,19) = data.bits(0,3);

  //SDMA_LEN
  if(addr == 0x004e) dma.r.length.bits( 0, 7) = data.bits(0,7);
  if(addr == 0x004f) dma.r.length.bits( 8,15) = data.bits(0,7);
  if(addr == 0x0050) dma.r.length.bits(16,19) = data.bits(0,3);

  //SDMA_CTRL
  if(addr == 0x0052) {
    bool trigger = !dma.r.enable && data.bit(7);
    dma.r.rate      = data.bits(0,1);
    dma.r.unknown   = data.bit (2);
    dma.r.loop      = data.bit (3);
    dma.r.target    = data.bit (4);
    dma.r.direction = data.bit (6);
    dma.r.enable    = data.bit (7);
    if(trigger) {
      dma.s.source = dma.r.source;
      dma.s.length = dma.r.length;
    }
  }

  //SND_HYPER_CTRL
  if(addr == 0x006a) {
    channel5.r.volume = data.bits(0,1);
    channel5.r.scale  = data.bits(2,3);
    channel5.r.speed  = data.bits(4,6);
    channel5.r.enable = data.bit (7);
  }

  //SND_HYPER_CHAN_CTRL
  if(addr == 0x006b) {
    channel5.r.unknown     = data.bits(0,3);
    channel5.r.leftEnable  = data.bit (5);
    channel5.r.rightEnable = data.bit (6);
  }

  //SND_CH1_PITCH
  if(addr == 0x0080) channel1.r.pitch.bits(0, 7) = data.bits(0,7);
  if(addr == 0x0081) channel1.r.pitch.bits(8,11) = data.bits(0,3);

  //SND_CH2_PITCH
  if(addr == 0x0082) channel2.r.pitch.bits(0, 7) = data.bits(0,7);
  if(addr == 0x0083) channel2.r.pitch.bits(8,11) = data.bits(0,3);

  //SND_CH3_PITCH
  if(addr == 0x0084) channel3.r.pitch.bits(0, 7) = data.bits(0,7);
  if(addr == 0x0085) channel3.r.pitch.bits(8,11) = data.bits(0,3);

  //SND_CH4_PITCH
  if(addr == 0x0086) channel4.r.pitch.bits(0, 7) = data.bits(0,7);
  if(addr == 0x0087) channel4.r.pitch.bits(8,11) = data.bits(0,3);

  //SND_CH1_VOL
  if(addr == 0x0088) {
    channel1.r.volumeRight = data.bits(0,3);
    channel1.r.volumeLeft  = data.bits(4,7);
  }

  //SND_CH2_VOL
  if(addr == 0x0089) {
    channel2.r.volumeRight = data.bits(0,3);
    channel2.r.volumeLeft  = data.bits(4,7);
  }

  //SND_CH3_VOL
  if(addr == 0x008a) {
    channel3.r.volumeRight = data.bits(0,3);
    channel3.r.volumeLeft  = data.bits(4,7);
  }

  //SND_CH4_VOL
  if(addr == 0x008b) {
    channel4.r.volumeRight = data.bits(0,3);
    channel4.r.volumeLeft  = data.bits(4,7);
  }

  //SND_SWEEP_VALUE
  if(addr == 0x008c) channel3.r.sweepValue = data;

  //SND_SWEEP_TIME
  if(addr == 0x008d) channel3.r.sweepTime = data.bits(0,4);

  //SND_NOISE
  if(addr == 0x008e) {
    channel4.r.noiseMode   = data.bits(0,2);
    channel4.r.noiseReset  = data.bit (3);
    channel4.r.noiseUpdate = data.bit (4);
  }

  //SND_WAVE_BASE
  if(addr == 0x008f) r.waveBase = data;

  //SND_CTRL
  if(addr == 0x0090) {
    channel1.r.enable = data.bit(0);
    channel2.r.enable = data.bit(1);
    channel3.r.enable = data.bit(2);
    channel4.r.enable = data.bit(3);
    channel2.r.voice  = data.bit(5);
    channel3.r.sweep  = data.bit(6);
    channel4.r.noise  = data.bit(7);
  }

  //SND_OUTPUT
  if(addr == 0x0091) {
    r.speakerEnable   = data.bit (0);
    r.speakerShift    = data.bits(1,2);
    r.headphoneEnable = data.bit (3);
  }

  //SND_VOICE_CTRL
  if(addr == 0x0094) {
    channel2.r.voiceEnableRight = data.bits(0,1);
    channel2.r.voiceEnableLeft  = data.bits(2,3);
  }
}
