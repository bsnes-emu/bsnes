Stream::Stream(uint channels, double inputFrequency) : channels(channels), inputFrequency(inputFrequency) {
}

auto Stream::reset() -> void {
  iir.reset();
  resampler.reset();
}

auto Stream::setFrequency(double outputFrequency_) -> void {
  reset();

  outputFrequency = outputFrequency_;
  cutoffFrequency = outputFrequency / inputFrequency;
  iir.resize(channels);

  if(cutoffFrequency <= 0.5) {
    for(auto c : range(channels)) {
      iir[c].resize(iirPasses);
      for(auto p : range(iirPasses)) {
        //attenuates frequencies that exceed the limits of human hearing (20KHz) to prevent aliasing
        iir[c][p].reset(DSP::IIR::Biquad::Type::LowPass, 20000.0 / inputFrequency);
      }
    }
  }

  resampler.resize(channels);
  for(auto c : range(channels)) {
    resampler[c].reset(inputFrequency, outputFrequency);
  }
}

auto Stream::pending() const -> bool {
  return resampler && resampler[0].pending();
}

auto Stream::read(double* samples) -> void {
  for(auto c : range(channels)) samples[c] = resampler[c].read();
  if(channels == 1) samples[1] = samples[0];  //monaural->stereo hack
}

auto Stream::write(int16* samples) -> void {
  for(auto c : range(channels)) {
    double sample = (samples[c] + 32768.0) / 65535.0;  //normalize
    for(auto& p : iir[c]) sample = p.process(sample);
    resampler[c].write(sample);
  }

  audio.poll();
}
