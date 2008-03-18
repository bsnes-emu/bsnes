/*****
 * SNES Interface class
 *
 * Interfaces SNES core with platform-specific functionality
 * (video, audio, input, ...)
 *****/

class SNESInterface { public:
  void video_refresh(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height);

  void audio_sample(uint16_t l_sample, uint16_t r_sample);

  function<bool ()> input_ready;
  void input_poll();
  bool input_poll(unsigned deviceid, unsigned button);

  void init();
  void term();
};

extern SNESInterface snesinterface;
