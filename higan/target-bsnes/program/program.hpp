struct Program : Emulator::Platform {
  //program.cpp
  Program(string_vector arguments);
  auto main() -> void;
  auto quit() -> void;

  //interface.cpp
  auto open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto load(uint id, string name, string type, string_vector options = {}) -> Emulator::Platform::Load override;
  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioSample(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;

  //game.cpp
  auto load() -> void;
  auto loadFile(string location) -> vector<uint8_t>;
  auto loadSuperNintendo(string location) -> void;
  auto loadGameBoy(string location) -> void;
  auto save() -> void;
  auto unload() -> void;

  //game-pak.cpp
  auto openPakSFC(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakGB(string name, vfs::file::mode mode) -> vfs::shared::file;

  //game-rom.cpp
  auto openRomSFC(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomGB(string name, vfs::file::mode mode) -> vfs::shared::file;

  //paths.cpp
  auto path(string type, string location, string extension = "") -> string;
  auto gamePath() -> string;
  auto cheatPath() -> string;
  auto statePath() -> string;

  //states.cpp
  auto loadState(string filename) -> bool;
  auto saveState(string filename) -> bool;
  auto saveRecoveryState() -> bool;

  //utility.cpp
  auto initializeVideoDriver() -> void;
  auto initializeAudioDriver() -> void;
  auto initializeInputDriver() -> void;
  auto updateVideoShader() -> void;
  auto connectDevices() -> void;
  auto applyHacks() -> void;
  auto showMessage(string text) -> void;
  auto showFrameRate(string text) -> void;
  auto updateStatus() -> void;
  auto focused() -> bool;

public:
  struct SuperNintendo {
    string label;
    string location;
    string manifest;
    Markup::Node document;
    vector<uint8_t> program;
    vector<uint8_t> data;
    vector<uint8_t> expansion;
    vector<uint8_t> firmware;
  } superNintendo;

  struct GameBoy {
    string location;
    string manifest;
    Markup::Node document;
    vector<uint8_t> program;
  } gameBoy;

  struct BSMemory {
  } bsMemory;

  struct SufamiTurbo {
  } sufamiTurbo[2];

  string_vector gameQueue;

  uint64 autoSaveTime;

  uint64 statusTime;
  string statusMessage;
  string statusFrameRate;
};

extern unique_pointer<Program> program;
