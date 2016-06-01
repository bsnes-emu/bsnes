auto Stream::reset(uint channels_, double inputFrequency, double outputFrequency) -> void {
  channels.reset();
  channels.resize(channels_);

  for(auto& channel : channels) {
    if(outputFrequency / inputFrequency <= 0.5) {
      channel.iir.resize(order / 2);
      for(auto phase : range(order / 2)) {
        double q = DSP::IIR::Biquad::butterworth(order, phase);
        channel.iir[phase].reset(DSP::IIR::Biquad::Type::LowPass, 20000.0 / inputFrequency, q);
      }
    }

    channel.resampler.reset(inputFrequency, outputFrequency);
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
    for(auto& iir : channels[c].iir) sample = iir.process(sample);
    channels[c].resampler.write(sample);
  }

  audio.process();
}
