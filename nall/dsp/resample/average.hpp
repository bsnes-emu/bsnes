#ifdef NALL_DSP_INTERNAL_HPP

struct ResampleAverage : Resampler {
  ResampleAverage(DSP& dsp) : Resampler(dsp) {}

  inline auto setFrequency() -> void;
  inline auto clear() -> void;
  inline auto sample() -> void;
  inline auto sampleLinear() -> void;

private:
  double fraction;
  double step;
};

auto ResampleAverage::setFrequency() -> void {
  fraction = 0.0;
  step = dsp.settings.frequency / frequency;
}

auto ResampleAverage::clear() -> void {
  fraction = 0.0;
}

auto ResampleAverage::sample() -> void {
  //can only average if input frequency >= output frequency
  if(step < 1.0) return sampleLinear();

  fraction += 1.0;

  double scalar = 1.0;
  if(fraction > step) scalar = 1.0 - (fraction - step);

  for(auto c : range(dsp.settings.channels)) {
    dsp.output.write(c) += dsp.buffer.read(c) * scalar;
  }

  if(fraction >= step) {
    for(auto c : range(dsp.settings.channels)) {
      dsp.output.write(c) /= step;
    }
    dsp.output.wroffset++;

    fraction -= step;
    for(auto c : range(dsp.settings.channels)) {
      dsp.output.write(c) = dsp.buffer.read(c) * fraction;
    }
  }

  dsp.buffer.rdoffset++;
}

auto ResampleAverage::sampleLinear() -> void {
  while(fraction <= 1.0) {
    double channel[dsp.settings.channels];

    for(auto n : range(dsp.settings.channels)) {
      double a = dsp.buffer.read(n, -1);
      double b = dsp.buffer.read(n, -0);

      double mu = fraction;

      channel[n] = a * (1.0 - mu) + b * mu;
    }

    dsp.write(channel);
    fraction += step;
  }

  dsp.buffer.rdoffset++;
  fraction -= 1.0;
}

#endif
