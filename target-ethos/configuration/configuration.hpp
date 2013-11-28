struct ConfigurationSettings : Configuration::Document {
  struct Video : Configuration::Node {
    string driver;
    bool synchronize;
    string shader;
    unsigned scaleMode;
    bool aspectCorrection;
    struct MaskOverscan : Configuration::Node {
      bool enable;
      unsigned horizontal;
      unsigned vertical;
    } maskOverscan;
    unsigned saturation;
    unsigned gamma;
    unsigned luminance;
    bool startFullScreen;
  } video;

  struct Audio : Configuration::Node {
    string driver;
    bool synchronize;
    unsigned frequency;
    unsigned latency;
    unsigned resampler;
    unsigned volume;
    bool mute;
  } audio;

  struct Input : Configuration::Node {
    string driver;
    struct Focus : Configuration::Node {
      bool pause;
      bool allow;
    } focus;
  } input;

  struct Timing : Configuration::Node {
    double video;
    double audio;
  } timing;

  struct Server : Configuration::Node {
    string hostname;
    string username;
    string password;
  } server;

  struct Library : Configuration::Node {
    unsigned selection;
    bool showOnStartup;
  } library;

  void load();
  void save();
  ConfigurationSettings();
};

extern ConfigurationSettings* config;
