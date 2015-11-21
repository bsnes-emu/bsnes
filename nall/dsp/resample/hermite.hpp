#ifdef NALL_DSP_INTERNAL_HPP

struct ResampleHermite : Resampler {
  ResampleHermite(DSP& dsp) : Resampler(dsp) {}

  inline auto setFrequency() -> void;
  inline auto clear() -> void;
  inline auto sample() -> void;

private:
  double fraction;
  double step;
};

auto ResampleHermite::setFrequency() -> void {
  fraction = 0.0;
  step = dsp.settings.frequency / frequency;
}

auto ResampleHermite::clear() -> void {
  fraction = 0.0;
}

auto ResampleHermite::sample() -> void {
  while(fraction <= 1.0) {
    double channel[dsp.settings.channels];

    for(auto n : range(dsp.settings.channels)) {
      double a = dsp.buffer.read(n, -3);
      double b = dsp.buffer.read(n, -2);
      double c = dsp.buffer.read(n, -1);
      double d = dsp.buffer.read(n, -0);

      const double tension = 0.0;  //-1 = low, 0 = normal, +1 = high
      const double bias = 0.0;  //-1 = left, 0 = even, +1 = right

      double mu1, mu2, mu3, m0, m1, a0, a1, a2, a3;

      mu1 = fraction;
      mu2 = mu1 * mu1;
      mu3 = mu2 * mu1;

      m0  = (b - a) * (1.0 + bias) * (1.0 - tension) / 2.0;
      m0 += (c - b) * (1.0 - bias) * (1.0 - tension) / 2.0;
      m1  = (c - b) * (1.0 + bias) * (1.0 - tension) / 2.0;
      m1 += (d - c) * (1.0 - bias) * (1.0 - tension) / 2.0;

      a0 = +2 * mu3 - 3 * mu2 + 1;
      a1 =      mu3 - 2 * mu2 + mu1;
      a2 =      mu3 -     mu2;
      a3 = -2 * mu3 + 3 * mu2;

      channel[n] = (a0 * b) + (a1 * m0) + (a2 * m1) + (a3 * c);
    }

    dsp.write(channel);
    fraction += step;
  }

  dsp.buffer.rdoffset++;
  fraction -= 1.0;
}

#endif
