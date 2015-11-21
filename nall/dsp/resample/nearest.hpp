#ifdef NALL_DSP_INTERNAL_HPP

struct ResampleNearest : Resampler {
  ResampleNearest(DSP& dsp) : Resampler(dsp) {}

  inline auto setFrequency() -> void;
  inline auto clear() -> void;
  inline auto sample() -> void;

private:
  double fraction;
  double step;
};

auto ResampleNearest::setFrequency() -> void {
  fraction = 0.0;
  step = dsp.settings.frequency / frequency;
}

auto ResampleNearest::clear() -> void {
  fraction = 0.0;
}

auto ResampleNearest::sample() -> void {
  while(fraction <= 1.0) {
    double channel[dsp.settings.channels];

    for(auto n : range(dsp.settings.channels)) {
      double a = dsp.buffer.read(n, -1);
      double b = dsp.buffer.read(n, -0);

      double mu = fraction;

      channel[n] = mu < 0.5 ? a : b;
    }

    dsp.write(channel);
    fraction += step;
  }

  dsp.buffer.rdoffset++;
  fraction -= 1.0;
}

#endif
