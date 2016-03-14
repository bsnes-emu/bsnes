APU::Channel1::Channel1() : Channel(1) {
}

auto APU::Channel1::run() -> void {
  if(r.voice) {
    uint8 volume = r.volumeLeft << 4 | r.volumeRight << 0;
    o.left = r.voiceEnableLeft ? volume : (uint8)0x80;
    o.right = r.voiceEnableRight ? volume : (uint8)0x80;
  } else if(--s.period == r.pitch) {
    s.period = 0;
    auto output = sample(s.sampleOffset++);
    o.left = output * r.volumeLeft;
    o.right = output * r.volumeRight;
  }
}
