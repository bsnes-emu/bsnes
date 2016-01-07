#pragma once

struct ResampleCosine : Resampler {
  ResampleCosine(DSP& dsp) : Resampler(dsp) {}

  inline auto setFrequency() -> void;
  inline auto clear() -> void;
  inline auto sample() -> void;

private:
  double fraction;
  double step;
};

auto ResampleCosine::setFrequency() -> void {
  fraction = 0.0;
  step = dsp.settings.frequency / frequency;
}

auto ResampleCosine::clear() -> void {
  fraction = 0.0;
}

auto ResampleCosine::sample() -> void {
  while(fraction <= 1.0) {
    double channel[dsp.settings.channels];

    for(auto n : range(dsp.settings.channels)) {
      double a = dsp.buffer.read(n, -1);
      double b = dsp.buffer.read(n, -0);

      double mu = fraction;
      mu = (1.0 - cos(mu * 3.14159265)) / 2.0;

      channel[n] = a * (1.0 - mu) + b * mu;
    }

    dsp.write(channel);
    fraction += step;
  }

  dsp.buffer.rdoffset++;
  fraction -= 1.0;
}
