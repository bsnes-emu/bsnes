struct Interface {
  uint32_t videoColor(unsigned source, uint16_t red, uint16_t green, uint16_t blue);
  void videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(unsigned port, unsigned device, unsigned input);
  void mediaRequest(Emulator::Interface::Media media);
  unsigned dipSettings(const XML::Node &node);
  string path(unsigned group);
};

extern Interface *interface;
