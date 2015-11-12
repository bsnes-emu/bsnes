struct ConfigurationManager : Configuration::Document {
  ConfigurationManager();
  auto quit() -> void;

  struct UserInterface : Configuration::Node {
    bool showStatusBar = true;
  } userInterface;

  struct Library : Configuration::Node {
    string location;
  } library;

  struct Video : Configuration::Node {
    string driver;
    bool synchronize = false;
    string scale = "Small";
    bool aspectCorrection = true;
    string filter = "Blur";
    string shader = "None";
    bool colorEmulation = true;
    unsigned saturation = 100;
    unsigned gamma = 100;
    unsigned luminance = 100;

    struct Overscan : Configuration::Node {
      bool mask = false;
      unsigned horizontal = 8;
      unsigned vertical = 8;
    } overscan;
  } video;

  struct Audio : Configuration::Node {
    string driver;
    string device;
    bool synchronize = true;
    bool mute = false;
    unsigned volume = 100;
    unsigned frequency = 48000;
    unsigned latency = 60;
    string resampler = "Sinc";
  } audio;

  struct Input : Configuration::Node {
    string driver;
  } input;

  struct Timing : Configuration::Node {
    double video = 60.0;
    double audio = 48000.0;
  } timing;
};

struct EmulatorSettings : Markup::Node {
  EmulatorSettings();
  auto quit() -> void;

  auto get(string name) -> string;
  auto set(string name, string value) -> void;
};

extern ConfigurationManager* config;
extern EmulatorSettings* emulatorSettings;
