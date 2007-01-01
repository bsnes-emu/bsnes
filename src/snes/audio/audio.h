FILE *pcmfp;

//if a filename is not specified, one will be generated
//automatically ("audio%0.3d.wav")
  void log_audio_enable(const char *fn = 0);
  void log_audio_disable();

  void audio_update(uint16 l_sample, uint16 r_sample);
  void audio_init();
  void audio_term();
