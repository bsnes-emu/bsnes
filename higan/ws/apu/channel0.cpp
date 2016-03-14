APU::Channel0::Channel0() : Channel(0) {
}

auto APU::Channel0::run() -> void {
  if(--s.period == r.pitch) {
    s.period = 0;
    auto output = sample(s.sampleOffset++);
    o.left = output * r.volumeLeft;
    o.right = output * r.volumeRight;
  }
}
