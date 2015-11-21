#ifdef NALL_DSP_INTERNAL_HPP

auto DSP::setChannels(uint channels) -> void {
  channels = max(1u, channels);
  buffer.setChannels(channels);
  output.setChannels(channels);
  settings.channels = channels;
}

auto DSP::setPrecision(uint precision) -> void {
  settings.precision = precision;
  settings.intensity = 1 << (settings.precision - 1);
  settings.intensityInverse = 1.0 / settings.intensity;
}

auto DSP::setFrequency(double frequency) -> void {
  settings.frequency = frequency;
  resampler->setFrequency();
}

auto DSP::setVolume(double volume) -> void {
  settings.volume = volume;
}

auto DSP::setBalance(double balance) -> void {
  settings.balance = balance;
}

auto DSP::setResampler(ResampleEngine engine) -> void {
  if(resampler) delete resampler;

  switch(engine) { default:
  case ResampleEngine::Nearest: resampler = new ResampleNearest(*this); return;
  case ResampleEngine::Linear:  resampler = new ResampleLinear (*this); return;
  case ResampleEngine::Cosine:  resampler = new ResampleCosine (*this); return;
  case ResampleEngine::Cubic:   resampler = new ResampleCubic  (*this); return;
  case ResampleEngine::Hermite: resampler = new ResampleHermite(*this); return;
  case ResampleEngine::Average: resampler = new ResampleAverage(*this); return;
  case ResampleEngine::Sinc:    resampler = new ResampleSinc   (*this); return;
  }
}

auto DSP::setResamplerFrequency(double frequency) -> void {
  resampler->frequency = frequency;
  resampler->setFrequency();
}

#endif
