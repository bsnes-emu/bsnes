auto DSP::Audio::coprocessorEnable(bool enable) -> void {
  mixer.clear();
  mixerEnable = enable;
  dsp.read = dsp.write = 0;
  mix.read = mix.write = 0;
}

auto DSP::Audio::coprocessorFrequency(double frequency) -> void {
  mixer.setFrequency(frequency);
  mixer.setResampler(nall::DSP::ResampleEngine::Sinc);
  mixer.setResamplerFrequency(system.apuFrequency() / 768.0);
}

auto DSP::Audio::sample(int16 left, int16 right) -> void {
  if(!mixerEnable) return interface->audioSample(left, right);

  dsp.left[dsp.write] = left;
  dsp.right[dsp.write] = right;
  dsp.write++;
  flush();
}

auto DSP::Audio::coprocessorSample(int16 left, int16 right) -> void {
  int samples[] = {left, right};
  mixer.sample(samples);
  while(mixer.pending()) {
    mixer.read(samples);
    mix.left[mix.write] = samples[0];
    mix.right[mix.write] = samples[1];
    mix.write++;
    flush();
  }
}

auto DSP::Audio::flush() -> void {
  while(dsp.read != dsp.write && mix.read != mix.write) {
    interface->audioSample(
      sclamp<16>(dsp.left[dsp.read] + mix.left[mix.read]),
      sclamp<16>(dsp.right[dsp.read] + mix.right[mix.read])
    );
    dsp.read++;
    mix.read++;
  }
}
