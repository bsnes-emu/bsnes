struct Settings {
  configuration config;
  string folderpath;
  bool synchronizeAudio;
  bool muteAudio;

  void load();
  void save();
};

extern Settings *settings;
