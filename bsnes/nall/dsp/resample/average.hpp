#ifdef NALL_DSP_INTERNAL_HPP

void DSP::resampleAverage() {
  //can only average if input frequency >= output frequency
  if(resampler.step < 1.0) return resampleHermite();

  resampler.fraction += 1.0;

  double scalar = 1.0;
  if(resampler.fraction > resampler.step) scalar = 1.0 - (resampler.fraction - resampler.step);

  output.write(0) += buffer.read(0) * scalar;
  output.write(1) += buffer.read(1) * scalar;

  if(resampler.fraction >= resampler.step) {
    output.write(0) /= resampler.step;
    output.write(1) /= resampler.step;
    output.wroffset++;

    resampler.fraction -= resampler.step;
    output.write(0) = buffer.read(0) * resampler.fraction;
    output.write(1) = buffer.read(1) * resampler.fraction;
  }

  buffer.rdoffset++;
}

#endif
