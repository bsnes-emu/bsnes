FILE *pcmfp;

//if a filename is not specified, one will be generated
//automatically ("audio%0.3d.wav")
  void    log_audio_enable(const char *fn = 0);
  void    log_audio_disable();

  void    audio_update(uint32 data);
  void    audio_init();
  void    audio_term();

  virtual void sound_run(uint32 data) = 0;
