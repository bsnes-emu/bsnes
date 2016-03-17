auto APU::DMA::run() -> void {
  if(!r.enable) return;

  if(r.rate == 0 && ++s.clock < 768) return;  // 4000hz
  if(r.rate == 1 && ++s.clock < 512) return;  // 6000hz
  if(r.rate == 2 && ++s.clock < 256) return;  //12000hz
  if(r.rate == 3 && ++s.clock < 128) return;  //24000hz
  s.clock = 0;

  uint8 data = bus.read(s.source);
  if(r.direction == 0) s.source++;
  if(r.direction == 1) s.source--;

  if(r.target == 0) {
    apu.channel2.r.volumeRight = data.bits(0,3);
    apu.channel2.r.volumeLeft  = data.bits(4,7);
  } else {
    apu.channel5.s.data = data;
  }

  if(--s.length) return;

  if(r.loop) {
    s.source = r.source;
    s.length = r.length;
  } else {
    r.enable = false;
  }
}
