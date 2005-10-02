struct {
  uint16 *data;
  uint32  size, pos;
} dsp_buffer;

FILE *pcmfp;

//buffer_size is in samples
  void    set_playback_buffer_size(uint32 buffer_size);
  uint32  get_playback_buffer_pos();
  uint16 *get_playback_buffer();

//if a filename is not specified, one will be generated
//automatically ("audio%0.3d.wav")
  void    log_audio_enable(const char *fn = 0);
  void    log_audio_disable();

  void    audio_update(uint32 data);
  void    audio_init();
  void    audio_term();

  virtual void sound_run() = 0;
