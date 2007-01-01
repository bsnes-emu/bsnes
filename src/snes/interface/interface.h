/*****
 * SNES Interface class
 *
 * Interfaces SNES core with platform-specific functionality
 * (video, audio, input, ...)
 *****/

class SNESInterface {
public:
  bool video_lock(uint16 *&data, uint &pitch);
  void video_unlock();
  void video_refresh();

  void audio_sample(uint16 l_sample, uint16 r_sample);

  void input_poll();
  bool input_poll(uint deviceid, uint button);
};

extern SNESInterface snesinterface;
