APU::Channel2::Channel2() : Channel(2) {
}

auto APU::Channel2::sweep() -> void {
  if(r.sweep && --s.sweepCounter < 0) {
    s.sweepCounter = r.sweepTime;
    r.pitch += r.sweepTime;
  }
}

auto APU::Channel2::run() -> void {
  if(--s.period == r.pitch) {
    s.period = 0;
    auto output = sample(s.sampleOffset++);
    o.left = output * r.volumeLeft;
    o.right = output * r.volumeRight;
  }
}
