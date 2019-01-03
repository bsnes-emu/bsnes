struct Program : Emulator::Platform {
  //program.cpp
  Program(Arguments arguments);
  auto main() -> void;
  auto quit() -> void;

  //platform.cpp
  auto path(uint id) -> string override;
  auto open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto load(uint id, string name, string type, vector<string> options = {}) -> Emulator::Platform::Load override;
  auto videoFrame(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioFrame(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;
  auto dipSettings(Markup::Node node) -> uint override;
  auto notify(string text) -> void override;

  //game.cpp
  auto load() -> void;
  auto load(Emulator::Interface& interface) -> void;
  auto unload() -> void;

  //state.cpp
  auto stateName(uint slot, bool managed = false) -> string;
  auto loadState(uint slot, bool managed = false) -> bool;
  auto saveState(uint slot, bool managed = false) -> bool;

  //utility.cpp
  auto initializeVideoDriver() -> void;
  auto initializeAudioDriver() -> void;
  auto initializeInputDriver() -> void;

  auto softReset() -> void;
  auto powerCycle() -> void;
  auto togglePause() -> void;
  auto rotateDisplay() -> void;
  auto showMessage(const string& text) -> void;
  auto updateStatusText() -> void;
  auto updateVideoPalette() -> void;
  auto updateVideoShader() -> void;
  auto updateAudioDriver() -> void;
  auto updateAudioEffects() -> void;
  auto focused() -> bool;

  bool hasQuit = false;
  bool pause = false;

  vector<Emulator::Interface*> emulators;

  vector<string> gameQueue;  //for command-line and drag-and-drop loading
  vector<string> gamePaths;  //for keeping track of loaded folder locations

  uint64 autoSaveTime = 0;  //for automatically saving RAM periodically

  uint64 statusTime = 0;  //for status message timeout after two seconds
  string statusMessage;
  string statusInfo;
};

extern unique_pointer<Program> program;
