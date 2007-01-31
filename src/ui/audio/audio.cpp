void Audio::resample_point(
uint32 *output, uint32 *input, uint output_samples, uint input_samples
) {
  for(uint i = 0; i < output_samples; i++) {
    output[i] = input[
      uint32( double(input_samples) / double(output_samples) * double(i) )
    ];
  }
}

void Audio::set_frequency(uint freq) {
  frequency = freq;
  latency   = (double(frequency) / 1000.0) * (double(config::audio.latency) / 3.0);
}

Audio::Audio() {
  frequency = config::audio.frequency;
  latency   = (double(frequency) / 1000.0) * (double(config::audio.latency) / 3.0);
}
