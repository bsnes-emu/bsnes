#ifdef NALL_DSP_INTERNAL_HPP

void DSP::resampleAverage() {
  //can only average if input frequency >= output frequency
  if(resampler.step < 1.0) return resampleHermite();

  resampler.fraction += 1.0;

  double scalar = 1.0;
  if(resampler.fraction > resampler.step) scalar = 1.0 - (resampler.fraction - resampler.step);

  for(unsigned c = 0; c < settings.channels; c++) {
    output.write(c) += buffer.read(c) * scalar;
  }

  if(resampler.fraction >= resampler.step) {
    for(unsigned c = 0; c < settings.channels; c++) {
      output.write(c) /= resampler.step;
    }
    output.wroffset++;

    resampler.fraction -= resampler.step;
    for(unsigned c = 0; c < settings.channels; c++) {
      output.write(c) = buffer.read(c) * resampler.fraction;
    }
  }

  buffer.rdoffset++;
}

#endif
