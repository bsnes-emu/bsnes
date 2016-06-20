struct Program : Emulator::Interface::Bind {
  //program.cpp
  Program(lstring args);
  auto load(string) -> void;
  auto main() -> void;
  auto quit() -> void;

  //interface.cpp
  auto path(uint id) -> string override;
  auto open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto loadRequest(uint id, string name, string type, bool required) -> void override;
  auto loadRequest(uint id, string path, bool required) -> void override;
  auto saveRequest(uint id, string path) -> void override;
  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioSample(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;
  auto dipSettings(const Markup::Node& node) -> uint override;
  auto notify(string text) -> void override;

  //medium.cpp
  auto loadMedium(string location) -> void;
  auto loadMedium(Emulator::Interface& interface, Emulator::Interface::Medium& medium, string location) -> void;
  auto unloadMedium() -> void;

  //state.cpp
  auto stateName(uint slot, bool manager = false) -> string;
  auto loadState(uint slot, bool manager = false) -> bool;
  auto saveState(uint slot, bool manager = false) -> bool;

  //utility.cpp
  auto powerCycle() -> void;
  auto softReset() -> void;
  auto connectDevices() -> void;
  auto showMessage(const string& text) -> void;
  auto updateStatusText() -> void;
  auto updateVideoPalette() -> void;
  auto updateVideoShader() -> void;
  auto updateAudioDriver() -> void;
  auto updateAudioEffects() -> void;

  bool pause = false;

  vector<Emulator::Interface*> emulators;

  vector<string> mediumPaths;
  vector<string> folderPaths;

  string statusText;
  string statusMessage;
  time_t statusTime = 0;
};

extern unique_pointer<Program> program;
