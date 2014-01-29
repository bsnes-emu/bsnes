struct Interface : Emulator::Interface::Bind {
  Interface();
  bool load(string pathname);
  void unload();
  void inputEvent(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue);

  //bindings
  void loadRequest(unsigned id, string name, string type);
  void loadRequest(unsigned id, string path);
  void saveRequest(unsigned id, string path);
  uint32_t videoColor(unsigned source, uint16_t alpha, uint16_t red, uint16_t green, uint16_t blue);
  void videoRefresh(const uint32_t* palette, const uint32_t* data, unsigned pitch, unsigned width, unsigned height);
  void audioSample(int16_t lsample, int16_t rsample);
  int16_t inputPoll(unsigned port, unsigned device, unsigned input);
  void inputRumble(unsigned port, unsigned device, unsigned input, bool enable);
  unsigned dipSettings(const Markup::Node& node);
  string path(unsigned group);
  string server();
  void notify(string text);

  lstring pathnames;
  struct Gamepad {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool b = false;
    bool a = false;
    bool y = false;
    bool x = false;
    bool l = false;
    bool r = false;
    bool select = false;
    bool start = false;
  } gamepad;
};

extern Interface* interface;
extern SuperFamicom::Interface* emulator;
