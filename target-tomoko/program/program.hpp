struct Program : Emulator::Interface::Bind {
  //program.cpp
  Program();
  auto main() -> void;
  auto quit() -> void;

  //interface.cpp
  auto loadRequest(unsigned id, string name, string type, bool required) -> void override;
  auto loadRequest(unsigned id, string path, bool required) -> void override;
  auto saveRequest(unsigned id, string path) -> void override;
  auto videoColor(unsigned source, uint16 alpha, uint16 red, uint16 green, uint16 blue) -> uint32 override;
  auto videoRefresh(const uint32* palette, const uint32* data, unsigned pitch, unsigned width, unsigned height) -> void override;
  auto audioSample(int16 lsample, int16 rsample) -> void override;
  auto inputPoll(unsigned port, unsigned device, unsigned input) -> int16 override;
  auto inputRumble(unsigned port, unsigned device, unsigned input, bool enable) -> void override;
  auto dipSettings(const Markup::Node& node) -> unsigned override;
  auto path(unsigned group) -> string override;
  auto notify(string text) -> void override;

  //media.cpp
  auto loadMedia(string location) -> void;
  auto loadMedia(Emulator::Interface& interface, Emulator::Interface::Media& media, const string& location) -> void;
  auto unloadMedia() -> void;

  //state.cpp
  auto stateName(unsigned slot, bool manager = false) -> string;
  auto loadState(unsigned slot, bool manager = false) -> bool;
  auto saveState(unsigned slot, bool manager = false) -> bool;

  //utility.cpp
  auto powerCycle() -> void;
  auto softReset() -> void;
  auto showMessage(const string& text) -> void;
  auto updateStatusText() -> void;
  auto updateVideoFilter() -> void;
  auto updateVideoPalette() -> void;
  auto updateAudio() -> void;
  auto updateDSP() -> void;

  DSP dsp;
  bool pause = false;

  vector<Emulator::Interface*> emulators;

  vector<string> mediaPaths;
  vector<string> folderPaths;

  string statusText;
  string statusMessage;
  time_t statusTime = 0;
};

extern Program* program;
