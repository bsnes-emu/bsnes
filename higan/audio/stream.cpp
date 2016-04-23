//Emulator::Stream implements advanced audio resampling
//First, a lowpass sinc filter is used (with a Blackman window to reduce rippling) in order to remove aliasing
//Second, a decimator is used to reduce the CPU overhead of the sinc function
//Finally, a hermite resampler is used to resample to the exact requested output frequency
//Note: when the cutoff frequency is >= 0.5; only the hermite resampler is used

Stream::Stream(uint channels, double inputFrequency) : channels(channels), inputFrequency(inputFrequency) {
}

Stream::~Stream() {
  reset();
}

auto Stream::reset() -> void {
  if(tap) delete[] tap, tap = nullptr;
  if(input) for(auto c : range(channels)) delete[] input[c];
  delete[] input, input = nullptr;
  if(queue) for(auto c : range(channels)) delete[] queue[c];
  delete[] queue, queue = nullptr;
  if(output) for(auto c : range(channels)) delete[] output[c];
  delete[] output, output = nullptr;
}

auto Stream::setFrequency(double outputFrequency_) -> void {
  reset();

  const double pi = 3.141592;
  auto sinc = [&](double x) -> double {
    if(x == 0) return 1;
    return sin(pi * x) / (pi * x);
  };

  outputFrequency = outputFrequency_;
  cutoffFrequency = outputFrequency / inputFrequency;
  if(cutoffFrequency < 0.5) {
    double transitionBandwidth = 0.008;  //lower = higher quality; more taps (slower)
    taps = (uint)ceil(4.0 / transitionBandwidth) | 1;
    tap = new double[taps];

    double sum = 0.0;
    for(uint t : range(taps)) {
      //sinc filter
      double s = sinc(2.0 * cutoffFrequency * (t - (taps - 1) / 2.0));

      //blackman window
      double b = 0.42 - 0.5 * cos(2.0 * pi * t / (taps - 1)) + 0.08 * cos(4.0 * pi * t / (taps - 1));

      tap[t] = s * b;
      sum += tap[t];
    }

    //normalize so that the sum of all coefficients is 1.0
    for(auto t : range(taps)) tap[t] /= sum;
  } else {
    taps = 1;
    tap = new double[taps];
    tap[0] = 1.0;
  }

  decimationRate = max(1, (uint)floor(inputFrequency / outputFrequency));
  decimationOffset = 0;

  input = new double*[channels];
  for(auto c : range(channels)) input[c] = new double[taps * 2]();
  inputOffset = 0;

  resamplerFrequency = inputFrequency / decimationRate;
  resamplerFraction = 0.0;
  resamplerStep = resamplerFrequency / outputFrequency;
  queue = new double*[channels];
  for(auto c : range(channels)) queue[c] = new double[4]();

  output = new double*[channels];
  outputs = inputFrequency * 0.02;
  for(auto c : range(channels)) output[c] = new double[outputs]();
  outputReadOffset = 0;
  outputWriteOffset = 0;
}

auto Stream::pending() const -> bool {
  return outputReadOffset != outputWriteOffset;
}

auto Stream::read(double* samples) -> void {
  for(auto c : range(channels)) {
    samples[c] = output[c][outputReadOffset];
  }
  if(channels == 1) samples[1] = samples[0];  //monaural->stereo hack
  if(++outputReadOffset >= outputs) outputReadOffset = 0;
}

auto Stream::write(int16* samples) -> void {
  inputOffset = !inputOffset ? taps - 1 : inputOffset - 1;
  for(auto c : range(channels)) {
    auto sample = (samples[c] + 32768.0) / 65535.0;  //normalize
    input[c][inputOffset] = input[c][inputOffset + taps] = sample;
  }

  if(++decimationOffset >= decimationRate) {
    decimationOffset = 0;

    for(auto c : range(channels)) {
      double sample = 0.0;
      for(auto t : range(taps)) sample += input[c][inputOffset + t] * tap[t];

      auto& q = queue[c];
      q[0] = q[1];
      q[1] = q[2];
      q[2] = q[3];
      q[3] = sample;
    }

    //4-tap hermite
    auto& mu = resamplerFraction;
    while(mu <= 1.0) {
      for(auto c : range(channels)) {
        auto& q = queue[c];

        const double tension = 0.0;  //-1 = low, 0 = normal, +1 = high
        const double bias    = 0.0;  //-1 = left, 0 = even, +1 = right

        double mu1 = mu;
        double mu2 = mu * mu;
        double mu3 = mu * mu * mu;

        double m0 = (q[1] - q[0]) * (1.0 + bias) * (1.0 - tension) / 2.0
                  + (q[2] - q[1]) * (1.0 - bias) * (1.0 - tension) / 2.0;
        double m1 = (q[2] - q[1]) * (1.0 + bias) * (1.0 - tension) / 2.0
                  + (q[3] - q[2]) * (1.0 - bias) * (1.0 - tension) / 2.0;

        double a0 = +2 * mu3 - 3 * mu2 + 1;
        double a1 =      mu3 - 2 * mu2 + mu1;
        double a2 =      mu3 -     mu2;
        double a3 = -2 * mu3 + 3 * mu2;

        output[c][outputWriteOffset] = (a0 * q[1]) + (a1 * m0) + (a2 * m1) + (a3 * q[2]);
      }

      if(++outputWriteOffset >= outputs) outputWriteOffset = 0;
      mu += resamplerStep;
      audio.poll();
    }

    mu -= 1.0;
  }
}
