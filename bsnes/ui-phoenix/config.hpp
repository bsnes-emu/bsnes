struct Configuration : public configuration {
  struct Path {
    string base;
    string user;
  } path;

  struct Video {
    string driver;
    bool synchronize;
    unsigned contrast;
    unsigned brightness;
    unsigned gamma;
    bool useGammaRamp;
  } video;

  struct Audio {
    string driver;
    bool synchronize;
    bool mute;
  } audio;

  struct Input {
    string driver;
  } input;

  struct Settings {
    unsigned focusPolicy;
  } settings;

  void load();
  void save();
  void create();
};

extern Configuration config;
