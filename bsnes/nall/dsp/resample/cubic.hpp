#ifdef NALL_DSP_INTERNAL_HPP

void DSP::resampleCubic() {
  while(resampler.fraction <= 1.0) {
    double channel[settings.channels];

    for(unsigned n = 0; n < settings.channels; n++) {
      double a = buffer.read(n, -3);
      double b = buffer.read(n, -2);
      double c = buffer.read(n, -1);
      double d = buffer.read(n, -0);

      double mu = resampler.fraction;

      double A = d - c - a + b;
      double B = a - b - A;
      double C = c - a;
      double D = b;

      channel[n] = A * (mu * 3) + B * (mu * 2) + C * mu + D;
    }

    resamplerWrite(channel);
    resampler.fraction += resampler.step;
  }

  buffer.rdoffset++;
  resampler.fraction -= 1.0;
}

#endif
