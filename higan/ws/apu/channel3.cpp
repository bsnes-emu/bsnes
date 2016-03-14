APU::Channel3::Channel3() : Channel(3) {
}

auto APU::Channel3::noiseSample() -> uint4 {
  return s.noiseOutput ? 0xf : 0x0;
}

auto APU::Channel3::run() -> void {
  if(--s.period == r.pitch) {
    s.period = 0;

    auto output = r.noise ? noiseSample() : sample(s.sampleOffset++);
    o.left = output * r.volumeLeft;
    o.right = output * r.volumeRight;

    if(r.noiseReset) {
      r.noiseReset = 0;
      s.noiseLFSR = 0;
      s.noiseOutput = 0;
    }

    if(r.noiseUpdate) {
      static const int taps[8] = {14, 10, 13, 4, 8, 6, 9, 11};
      auto tap = taps[r.noiseMode];

      s.noiseOutput = (1 ^ (s.noiseLFSR >> 7) ^ (s.noiseLFSR >> tap)) & 1;
      s.noiseLFSR = s.noiseLFSR << 1 | s.noiseOutput;
    }
  }
}
