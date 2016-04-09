#pragma once

struct ResampleCubic : Resampler {
  ResampleCubic(DSP& dsp) : Resampler(dsp) {}

  inline auto setFrequency() -> void;
  inline auto clear() -> void;
  inline auto sample() -> void;

private:
  double fraction;
  double step;
};

auto ResampleCubic::setFrequency() -> void {
  fraction = 0.0;
  step = dsp.settings.frequency / frequency;
}

auto ResampleCubic::clear() -> void {
  fraction = 0.0;
}

auto ResampleCubic::sample() -> void {
  while(fraction <= 1.0) {
    double channel[dsp.settings.channels];

    for(auto n : range(dsp.settings.channels)) {
      double a = dsp.buffer.read(n, -3);
      double b = dsp.buffer.read(n, -2);
      double c = dsp.buffer.read(n, -1);
      double d = dsp.buffer.read(n, -0);

      double mu = fraction;

      double A = d - c - a + b;
      double B = a - b - A;
      double C = c - a;
      double D = b;

      channel[n] = A * (mu * 3) + B * (mu * 2) + C * mu + D;
    }

    dsp.write(channel);
    fraction += step;
  }

  dsp.buffer.rdoffset++;
  fraction -= 1.0;
}
