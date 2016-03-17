auto APU::Channel1::run() -> void {
  if(--s.period == r.pitch) {
    s.period = 0;
    auto output = apu.sample(1, s.sampleOffset++);
    o.left = output * r.volumeLeft;
    o.right = output * r.volumeRight;
  }
}
