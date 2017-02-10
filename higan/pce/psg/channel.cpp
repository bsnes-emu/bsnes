auto PSG::Channel::power(uint id) -> void {
  this->id = id;
  memory::fill(&io, sizeof(IO));
  memory::fill(&output, sizeof(Output));
}

auto PSG::Channel::run() -> void {
  if(!io.enable) return sample(0);

  if(!io.direct && --io.wavePeriod == 0) {
    io.wavePeriod = io.waveFrequency;
    io.waveOffset++;
    io.waveSample = io.waveBuffer[io.waveOffset];
  }

  if(!io.noiseEnable) {
    return sample(io.waveSample);
  }

  if(--io.noisePeriod == 0) {
    io.noisePeriod = ~io.noiseFrequency << 7;
    //todo: this should be a square wave; PRNG algorithm is also unknown
    io.noiseSample = nall::random();
  }

  return sample(io.noiseSample);
}

auto PSG::Channel::loadWavePeriod() -> void {
  io.wavePeriod = io.waveFrequency;
}

auto PSG::Channel::loadWaveSample() -> void {
  io.waveSample = io.waveBuffer[io.waveOffset];
}

auto PSG::Channel::sample(uint5 sample) -> void {
  output.left = sample << 8;  //<< io.volume << io.volumeLeft;
  output.right = sample << 8;  //<< io.volume << io.volumeRight;
}
