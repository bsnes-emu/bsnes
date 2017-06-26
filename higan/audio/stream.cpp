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

auto Stream::addFilter(Filter::Order order, Filter::Type type, double cutoffFrequency, uint passes) -> void {
  for(auto& channel : channels) {
    for(auto pass : range(passes)) {
      Filter filter{order};

      if(order == Filter::Order::First) {
        DSP::IIR::OnePole::Type _type;
        if(type == Filter::Type::LowPass) _type = DSP::IIR::OnePole::Type::LowPass;
        if(type == Filter::Type::HighPass) _type = DSP::IIR::OnePole::Type::HighPass;
        filter.onePole.reset(_type, cutoffFrequency, inputFrequency);
      }

      if(order == Filter::Order::Second) {
        DSP::IIR::Biquad::Type _type;
        if(type == Filter::Type::LowPass) _type = DSP::IIR::Biquad::Type::LowPass;
        if(type == Filter::Type::HighPass) _type = DSP::IIR::Biquad::Type::HighPass;
        double q = DSP::IIR::Biquad::butterworth(passes * 2, pass);
        filter.biquad.reset(_type, cutoffFrequency, inputFrequency, q);
      }

      channel.filters.append(filter);
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
    for(auto& filter : channels[c].filters) {
      switch(filter.order) {
      case Filter::Order::First: sample = filter.onePole.process(sample); break;
      case Filter::Order::Second: sample = filter.biquad.process(sample); break;
      }
    }
    channels[c].resampler.write(sample);
  }

  audio.process();
}
