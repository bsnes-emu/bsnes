auto PSG::Channel::power() -> void {
  memory::fill(&io, sizeof(IO));
  memory::fill(&output, sizeof(Output));
}

auto PSG::Channel::run() -> void {
  if(!io.enable) return sample(0);

  if(io.noiseEnable) {
    if(--io.noisePeriod == 0) {
      io.noisePeriod = ~io.noiseFrequency << 7;
      //todo: this should be a square wave; PRNG algorithm is also unknown
      io.noiseSample = nall::random();
    }
    return sample(io.noiseSample);
  }

  if(io.direct) return sample(io.waveDirect);

  if(--io.period == 0) {
    io.period = io.frequency;
    io.waveOffset++;
  }

  return sample(io.waveData[io.waveOffset]);
}

auto PSG::Channel::sample(uint5 sample) -> void {
  output.left = sample << 8;  //<< io.volume << io.volumeLeft;
  output.right = sample << 8;  //<< io.volume << io.volumeRight;
}
