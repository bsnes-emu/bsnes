auto PSG::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(io.channel);
  s.integer(io.volumeLeft);
  s.integer(io.volumeRight);
  s.integer(io.lfoFrequency);
  s.integer(io.lfoControl);
  s.integer(io.lfoEnable);

  for(auto C : range(6)) {
    s.integer(channel[C].io.waveFrequency);
    s.integer(channel[C].io.volume);
    s.integer(channel[C].io.direct);
    s.integer(channel[C].io.enable);
    s.integer(channel[C].io.volumeLeft);
    s.integer(channel[C].io.volumeRight);
    s.array(channel[C].io.waveBuffer);
    s.integer(channel[C].io.noiseFrequency);
    s.integer(channel[C].io.noiseEnable);
    s.integer(channel[C].io.wavePeriod);
    s.integer(channel[C].io.waveSample);
    s.integer(channel[C].io.waveOffset);
    s.integer(channel[C].io.noisePeriod);
    s.integer(channel[C].io.noiseSample);
    s.integer(channel[C].io.output);
  }
}
