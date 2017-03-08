auto Stream::reset(uint channels_, double inputFrequency, double outputFrequency) -> void {
  this->inputFrequency = inputFrequency;
  this->outputFrequency = outputFrequency;

  channels.reset();
  channels.resize(channels_);

  for(auto& channel : channels) {
    channel.filters.reset();
    channel.resampler.reset(inputFrequency, outputFrequency);
  }
}

auto Stream::addLowPassFilter(double cutoffFrequency, uint passes) -> void {
  for(auto& channel : channels) {
    for(auto pass : range(passes)) {
      double q = DSP::IIR::Biquad::butterworth(passes * 2, pass);
      channel.filters.append(DSP::IIR::Biquad{});
      channel.filters.right().reset(DSP::IIR::Biquad::Type::LowPass, cutoffFrequency, inputFrequency, q);
    }
  }
}

auto Stream::addHighPassFilter(double cutoffFrequency, uint passes) -> void {
  for(auto& channel : channels) {
    for(auto pass : range(passes)) {
      double q = DSP::IIR::Biquad::butterworth(passes * 2, pass);
      channel.filters.append(DSP::IIR::Biquad{});
      channel.filters.right().reset(DSP::IIR::Biquad::Type::HighPass, cutoffFrequency, inputFrequency, q);
    }
  }
}

auto Stream::pending() const -> bool {
  return channels && channels[0].resampler.pending();
}

auto Stream::read(double* samples) -> uint {
  for(auto c : range(channels)) samples[c] = channels[c].resampler.read();
  return channels.size();
}

auto Stream::write(const double* samples) -> void {
  for(auto c : range(channels)) {
    double sample = samples[c] + 1e-25;  //constant offset used to suppress denormals
    for(auto& filter : channels[c].filters) sample = filter.process(sample);
    channels[c].resampler.write(sample);
  }

  audio.process();
}
