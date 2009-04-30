class AudioResampler {
public:
  //output ofreq samples per input of ifreq samples
  unsigned ifreq;  //input frequency
  unsigned ofreq;  //output frequency

  //intrinsic ring buffer: indices wrap around buffer size automatically
  uint16_t lbuffer[256];
  uint16_t rbuffer[256];
  uint8_t bufferlength;
  uint8_t bufferrd, bufferwr;

  double fraction, step;
  int16_t lhist[4], rhist[4];
  double hermite(double mu, double a, double b, double c, double d);

  void sample(uint16_t lsamp, uint16_t rsamp);
  void reset(unsigned ifreq, unsigned ofreq);
};

class Audio {
public:
  void set_dsp_frequency(unsigned);
  void set_cop_frequency(unsigned);

  void dsp_sample(uint16 l_sample, uint16 r_sample);
  void cop_sample(uint16 l_sample, uint16 r_sample);
  void init();

  friend class System;

private:
  AudioResampler resample;
  unsigned dsp_frequency;
  unsigned cop_frequency;
} audio;
