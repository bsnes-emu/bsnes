struct Interface {
  virtual uint32_t videoColor(uint19_t source, uint16_t red, uint16_t green, uint16_t blue);
  virtual void videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan);
  virtual void audioSample(int16_t lsample, int16_t rsample);
  virtual int16_t inputPoll(bool port, Input::Device device, unsigned index, unsigned id);

  virtual string path(Cartridge::Slot slot, const string &hint) = 0;
  virtual void message(const string &text);
};

extern Interface *interface;
