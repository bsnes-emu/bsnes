class Audio {
public:
  //if a filename is not specified, one will be generated
  //automatically ("audio%0.3d.wav")
  void log_enable(const char *fn = 0);
  void log_disable();

  void update(uint16 l_sample, uint16 r_sample);
  void init();
  void term();

private:
  FILE *pcmfp;

  friend class SNES;
} audio;
