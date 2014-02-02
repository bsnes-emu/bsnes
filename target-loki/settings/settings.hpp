struct Settings : Configuration::Document {
  struct Video : Configuration::Node {
    string driver;
    bool synchronize;
  } video;

  struct Audio : Configuration::Node {
    string driver;
    bool synchronize;
    bool mute;
  } audio;

  struct Input : Configuration::Node {
    string driver;
  } input;

  Settings();
  void load();
  void save();
  void command(string s, lstring args);
};

extern Settings* settings;
