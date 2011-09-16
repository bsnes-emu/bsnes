#ifdef NALL_DSP_INTERNAL_HPP

void DSP::resampleCosine() {
  while(resampler.fraction <= 1.0) {
    double channel[settings.channels];

    for(unsigned n = 0; n < settings.channels; n++) {
      double a = buffer.read(n, -1);
      double b = buffer.read(n, -0);

      double mu = resampler.fraction;
      mu = (1.0 - cos(mu * 3.14159265)) / 2.0;

      channel[n] = a * (1.0 - mu) + b * mu;
    }

    resamplerWrite(channel);
    resampler.fraction += resampler.step;
  }

  buffer.rdoffset++;
  resampler.fraction -= 1.0;
}

#endif
