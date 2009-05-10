class Interface : public SNES::Interface {
public:
  void video_refresh(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height) {
    uint32_t *output;
    unsigned outpitch;
    if(video.lock(output, outpitch) == true) {
      unsigned outwidth, outheight;
      libfilter::filter.render(output, outpitch, outwidth, outheight, data, pitch, line, width, height);
      video.unlock();
      video.refresh(outwidth, outheight);
    }
  }

  void audio_sample(uint16_t left, uint16_t right) {
    if(config.audio.mute) left = right = 0;
    audio.sample(left, right);
  }

  void input_poll() {
    inputManager.poll();
  }

  int16_t input_poll(unsigned deviceid, unsigned id) {
    return inputManager.getStatus(deviceid, id);
  }
} interface;

