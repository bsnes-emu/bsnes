struct Config : public configuration {
  struct Video {
    string driver;
    string shader;
    bool synchronize;
    bool smooth;
  } video;

  struct Audio {
    string driver;
    bool synchronize;
    bool mute;
  } audio;

  struct Input {
    string driver;
    unsigned focusPolicy;
  } input;

  struct Path {
    string last;
  } path;

  struct NES {
    unsigned controllerPort1Device;
    unsigned controllerPort2Device;
  } nes;

  struct SNES {
    unsigned controllerPort1Device;
    unsigned controllerPort2Device;
  } snes;

  Config();
  ~Config();
};

extern Config *config;
