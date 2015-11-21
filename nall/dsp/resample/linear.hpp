#ifdef NALL_DSP_INTERNAL_HPP

struct ResampleLinear : Resampler {
  ResampleLinear(DSP& dsp) : Resampler(dsp) {}

  inline auto setFrequency() -> void;
  inline auto clear() -> void;
  inline auto sample() -> void;

private:
  double fraction;
  double step;
};

auto ResampleLinear::setFrequency() -> void {
  fraction = 0.0;
  step = dsp.settings.frequency / frequency;
}

auto ResampleLinear::clear() -> void {
  fraction = 0.0;
}

auto ResampleLinear::sample() -> void {
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
