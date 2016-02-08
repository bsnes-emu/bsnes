struct Program : Emulator::Interface::Bind {
  //program.cpp
  Program(lstring args);
  auto load(string) -> void;
  auto main() -> void;
  auto quit() -> void;

  //interface.cpp
  auto loadRequest(uint id, string name, string type, bool required) -> void override;
  auto loadRequest(uint id, string path, bool required) -> void override;
  auto saveRequest(uint id, string path) -> void override;
  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioSample(int16 lsample, int16 rsample) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;
  auto dipSettings(const Markup::Node& node) -> uint override;
  auto path(uint group) -> string override;
  auto notify(string text) -> void override;

  //media.cpp
  auto loadMedia(string location) -> void;
  auto loadMedia(Emulator::Interface& interface, Emulator::Interface::Media& media, const string& location) -> void;
  auto unloadMedia() -> void;

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
  auto updateVideoShader() -> void;
  auto updateAudio() -> void;
  auto updateAudioVolume() -> void;
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

extern unique_pointer<Program> program;
