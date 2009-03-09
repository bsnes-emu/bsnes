//====================
//SNES interface class
//====================
//Interfaces SNES core with platform-specific functionality (video, audio, input, ...)

class SNESInterface {
public:
  void video_refresh(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height);
  void audio_sample(uint16_t l_sample, uint16_t r_sample);
  void input_poll();
  int16_t input_poll(unsigned deviceid, unsigned id);

  void init();
  void term();
};

extern SNESInterface snesinterface;
