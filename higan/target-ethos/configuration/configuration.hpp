struct Configuration : configuration {
  struct Video {
    string driver;
    bool synchronize;
    string shader;
    unsigned scaleMode;
    bool aspectCorrection;
    bool maskOverscan;
    unsigned maskOverscanHorizontal;
    unsigned maskOverscanVertical;
    unsigned saturation;
    unsigned gamma;
    unsigned luminance;
    bool startFullScreen;
  } video;

  struct Audio {
    string driver;
    bool synchronize;
    unsigned frequency;
    unsigned latency;
    unsigned resampler;
    unsigned volume;
    bool mute;
  } audio;

  struct Input {
    string driver;
    bool focusPause;
    bool focusAllow;
  } input;

  struct Timing {
    double video;
    double audio;
  } timing;

  struct HighScores {
    string hostname;
    string username;
    string password;
  } highScores;

  struct Path {
    string game;
  } path;

  void load();
  void save();
  Configuration();
};

extern Configuration *config;
