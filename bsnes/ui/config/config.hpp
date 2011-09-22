struct Config : public configuration {
  struct Video {
    string driver;
    string filter;
    string shader;
    bool synchronize;
    bool enableOverscan;
    bool correctAspectRatio;

    unsigned brightness;
    unsigned contrast;
    unsigned gamma;
    bool gammaRamp;
    unsigned fullScreenMode;
  } video;

  struct Audio {
    string driver;
    bool synchronize;
    bool mute;
    unsigned volume;

    unsigned frequencyNES;
    unsigned frequencySNES;
    unsigned frequencyGameBoy;
  } audio;

  struct Input {
    string driver;
    unsigned focusPolicy;
  } input;

  struct Path {
    struct BIOS {
      string satellaview;
      string sufamiTurbo;
      string superGameBoy;
    } bios;
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
