struct Config : public configuration {
  struct Video {
    string driver;

    string filter;
    string shader;

    bool synchronize;
    bool correctAspectRatio;

    bool maskOverscan;
    unsigned maskOverscanHorizontal;
    unsigned maskOverscanVertical;

    unsigned brightness;
    unsigned contrast;
    unsigned gamma;

    unsigned fullScreenMode;
    unsigned compositionMode;

    bool startFullScreen;
  } video;

  struct Audio {
    string driver;
    bool synchronize;
    bool mute;
    unsigned volume;
    unsigned latency;
    string resampler;

    unsigned frequency;
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
