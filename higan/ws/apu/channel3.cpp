auto APU::Channel3::sweep() -> void {
  if(r.sweep && --s.sweepCounter < 0) {
    s.sweepCounter = r.sweepTime;
    r.pitch += r.sweepValue;
  }
}

auto APU::Channel3::run() -> void {
  if(--s.period == r.pitch) {
    s.period = 0;
    auto output = apu.sample(3, s.sampleOffset++);
    o.left = output * r.volumeLeft;
    o.right = output * r.volumeRight;
  }
}
