#ifdef ADSP_CPP

void aDSP::serialize(serializer &s) {
  DSP::serialize(s);

  s.array(dspram);
  s.integer(dsp_counter);

  s.integer(status.MVOLL);
  s.integer(status.MVOLR);
  s.integer(status.EVOLL);
  s.integer(status.EVOLR);
  s.integer(status.KON);
  s.integer(status.KOFF);
  s.integer(status.FLG);
  s.integer(status.ENDX);
  s.integer(status.EFB);
  s.integer(status.PMON);
  s.integer(status.NON);
  s.integer(status.EON);
  s.integer(status.DIR);
  s.integer(status.ESA);
  s.integer(status.EDL);
  s.array(status.FIR);

  s.integer(status.kon);
  s.integer(status.esa);

  s.integer(status.noise_ctr);
  s.integer(status.noise_rate);
  s.integer(status.noise_sample);

  s.integer(status.echo_index);
  s.integer(status.echo_length);
  s.array(status.fir_buffer[0]);
  s.array(status.fir_buffer[1]);
  s.integer(status.fir_buffer_index);

  for(unsigned i = 0; i < 8; i++) {
    s.integer(voice[i].VOLL);
    s.integer(voice[i].VOLR);
    s.integer(voice[i].PITCH);
    s.integer(voice[i].SRCN);
    s.integer(voice[i].ADSR1);
    s.integer(voice[i].ADSR2);
    s.integer(voice[i].GAIN);
    s.integer(voice[i].ENVX);
    s.integer(voice[i].OUTX);

    s.integer(voice[i].pitch_ctr);

    s.integer(voice[i].brr_index);
    s.integer(voice[i].brr_ptr);
    s.integer(voice[i].brr_header);
    s.integer(voice[i].brr_looped);

    s.array(voice[i].brr_data);
    s.integer(voice[i].brr_data_index);

    s.integer(voice[i].envx);
    s.integer(voice[i].env_ctr);
    s.integer(voice[i].env_rate);
    s.integer(voice[i].env_sustain);
    s.integer(voice[i].env_state);
    s.integer(voice[i].env_mode);

    s.integer(voice[i].outx);
  }
}

#endif
