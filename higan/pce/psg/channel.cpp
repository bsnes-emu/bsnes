auto PSG::Channel::power(uint id) -> void {
  this->id = id;
  io = {};
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
    io.noiseSample = nall::random() & 1 ? ~0 : 0;
  }

  return sample(io.noiseSample);
}

auto PSG::Channel::sample(uint5 sample) -> void {
  io.output = sample;
}
