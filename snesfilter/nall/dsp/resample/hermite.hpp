#ifdef NALL_DSP_INTERNAL_HPP

void DSP::resampleHermite() {
  while(resampler.fraction <= 1.0) {
    double channel[settings.channels];

    for(unsigned n = 0; n < settings.channels; n++) {
      double a = buffer.read(n, -3);
      double b = buffer.read(n, -2);
      double c = buffer.read(n, -1);
      double d = buffer.read(n, -0);

      const double tension = 0.0;  //-1 = low, 0 = normal, +1 = high
      const double bias = 0.0;  //-1 = left, 0 = even, +1 = right

      double mu1, mu2, mu3, m0, m1, a0, a1, a2, a3;

      mu1 = resampler.fraction;
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

    resamplerWrite(channel);
    resampler.fraction += resampler.step;
  }

  buffer.rdoffset++;
  resampler.fraction -= 1.0;
}

#endif
