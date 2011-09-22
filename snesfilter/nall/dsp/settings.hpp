#ifdef NALL_DSP_INTERNAL_HPP

void DSP::setChannels(unsigned channels) {
  assert(channels > 0);
  buffer.setChannels(channels);
  output.setChannels(channels);
  settings.channels = channels;
}

void DSP::setPrecision(unsigned precision) {
  settings.precision = precision;
  settings.intensity = 1 << (settings.precision - 1);
}

void DSP::setFrequency(double frequency) {
  settings.frequency = frequency;
  resampler.fraction = 0;
  resampler.step = settings.frequency / resampler.frequency;
}

void DSP::setVolume(double volume) {
  settings.volume = volume;
}

void DSP::setBalance(double balance) {
  settings.balance = balance;
}

void DSP::setResampler(Resampler engine) {
  resampler.engine = engine;
}

void DSP::setResamplerFrequency(double frequency) {
  resampler.frequency = frequency;
  resampler.fraction = 0;
  resampler.step = settings.frequency / resampler.frequency;
}

#endif
