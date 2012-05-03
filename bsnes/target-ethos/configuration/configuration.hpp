struct Configuration : configuration {
  struct Video {
    unsigned scaleMode;
    bool aspectCorrection;
  } video;

  void load();
  void save();
  Configuration();
};

extern Configuration *config;
