auto APU::Channel2::run() -> void {
  if(r.voice) {
    uint8 volume = r.volumeLeft << 4 | r.volumeRight << 0;
    o.left = r.voiceEnableLeft ? volume : (uint8)0;
    o.right = r.voiceEnableRight ? volume : (uint8)0;
  } else if(--s.period == r.pitch) {
    s.period = 0;
    auto output = apu.sample(2, s.sampleOffset++);
    o.left = output * r.volumeLeft;
    o.right = output * r.volumeRight;
  }
}
