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
    string scale = "Normal";
    bool aspectCorrection = true;
    string filter = "Blur";
    bool colorEmulation = true;

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
  } audio;

  struct Input : Configuration::Node {
    string driver;
  } input;
};

extern ConfigurationManager* configurationManager;
auto config() -> ConfigurationManager&;
