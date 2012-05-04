struct Configuration : configuration {
  struct Video {
    bool synchronize;
    unsigned scaleMode;
    bool aspectCorrection;
    bool maskOverscan;
    unsigned maskOverscanHorizontal;
    unsigned maskOverscanVertical;
    unsigned saturation;
    unsigned gamma;
    unsigned luminance;
  } video;

  struct Audio {
    bool synchronize;
    unsigned frequency;
    unsigned latency;
    unsigned resampler;
    unsigned volume;
    bool mute;
  } audio;

  struct Input {
    bool focusPause;
    bool focusAllow;
  } input;

  void load();
  void save();
  Configuration();
};

extern Configuration *config;
