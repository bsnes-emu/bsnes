struct Configuration : public configuration {
  struct Video {
    string driver;
    bool synchronize;
    bool smooth;
    string filter;
    string shader;
    bool region;
    unsigned scale;
    unsigned fullscreenScale;
    bool aspectRatioCorrection;
    unsigned brightness;
    unsigned contrast;
    unsigned gamma;
    bool useGammaRamp;
  } video;

  struct Audio {
    string driver;
    bool synchronize;
    bool mute;
    unsigned volume;
    unsigned latency;
    unsigned inputFrequency;
    unsigned outputFrequency;
  } audio;

  struct Input {
    string driver;
  } input;

  struct Settings {
    unsigned focusPolicy;
  } settings;

  struct Controller {
    unsigned port1;
    unsigned port2;
  } controller;

  void load();
  void save();
  void create();
};

extern Configuration config;
