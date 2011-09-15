#ifdef SYSTEM_CPP

Audio audio;

void Audio::coprocessor_enable(bool state) {
  coprocessor = state;
  dspaudio.clear();

  dsp_rdoffset = cop_rdoffset = 0;
  dsp_wroffset = cop_wroffset = 0;
  dsp_length = cop_length = 0;
}

void Audio::coprocessor_frequency(double input_frequency) {
  dspaudio.setFrequency(input_frequency);
  dspaudio.setResampler(nall::DSP::Resampler::Average);
  dspaudio.setResamplerFrequency(system.apu_frequency() / 768.0);
}

void Audio::sample(int16 left, int16 right) {
  if(coprocessor == false) {
    interface->audioSample(left, right);
  } else {
    dsp_buffer[dsp_wroffset] = ((uint16)left << 0) + ((uint16)right << 16);
    dsp_wroffset = (dsp_wroffset + 1) & buffer_mask;
    dsp_length = (dsp_length + 1) & buffer_mask;
    flush();
  }
}

void Audio::coprocessor_sample(int16 left, int16 right) {
  dspaudio.sample(left, right);
  while(dspaudio.pending()) {
    signed left, right;
    dspaudio.read(left, right);

    cop_buffer[cop_wroffset] = ((uint16)left << 0) + ((uint16)right << 16);
    cop_wroffset = (cop_wroffset + 1) & buffer_mask;
    cop_length = (cop_length + 1) & buffer_mask;
    flush();
  }
}

void Audio::init() {
}

void Audio::flush() {
  while(dsp_length > 0 && cop_length > 0) {
    uint32 dsp_sample = dsp_buffer[dsp_rdoffset];
    uint32 cop_sample = cop_buffer[cop_rdoffset];

    dsp_rdoffset = (dsp_rdoffset + 1) & buffer_mask;
    cop_rdoffset = (cop_rdoffset + 1) & buffer_mask;

    dsp_length--;
    cop_length--;

    int dsp_left  = (int16)(dsp_sample >>  0);
    int dsp_right = (int16)(dsp_sample >> 16);

    int cop_left  = (int16)(cop_sample >>  0);
    int cop_right = (int16)(cop_sample >> 16);

    interface->audioSample(
      sclamp<16>((dsp_left  + cop_left ) / 2),
      sclamp<16>((dsp_right + cop_right) / 2)
    );
  }
}

#endif
