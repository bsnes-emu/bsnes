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
  auto loadSuperFamicom(string location) -> void;
  auto loadGameBoy(string location) -> void;
  auto loadBSMemory(string location) -> void;
  auto loadSufamiTurboA(string location) -> void;
  auto loadSufamiTurboB(string location) -> void;
  auto save() -> void;
  auto reset() -> void;
  auto unload() -> void;

  //game-pak.cpp
  auto openPakSuperFamicom(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakGameBoy(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakBSMemory(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakSufamiTurboA(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openPakSufamiTurboB(string name, vfs::file::mode mode) -> vfs::shared::file;

  //game-rom.cpp
  auto openRomSuperFamicom(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomGameBoy(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomBSMemory(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomSufamiTurboA(string name, vfs::file::mode mode) -> vfs::shared::file;
  auto openRomSufamiTurboB(string name, vfs::file::mode mode) -> vfs::shared::file;

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
  auto showMessage(string text) -> void;
  auto showFrameRate(string text) -> void;
  auto updateStatus() -> void;
  auto focused() -> bool;

  //patch.cpp
  auto appliedPatch() const -> bool;
  auto applyPatchIPS(vector<uint8_t>& data, string location) -> bool;
  auto applyPatchBPS(vector<uint8_t>& data, string location) -> bool;

  //hacks.cpp
  auto applyHacks() -> void;
  auto applyHackOverclockSuperFX() -> void;

public:
  struct SuperFamicom {
    string label;
    string location;
    string manifest;
    Markup::Node document;
    vector<uint8_t> program;
    vector<uint8_t> data;
    vector<uint8_t> expansion;
    vector<uint8_t> firmware;
    boolean patched;
  } superFamicom;

  struct GameBoy {
    string location;
    string manifest;
    Markup::Node document;
    vector<uint8_t> program;
    boolean patched;
  } gameBoy;

  struct BSMemory {
    string location;
    string manifest;
    Markup::Node document;
    vector<uint8_t> program;
    boolean patched;
  } bsMemory;

  struct SufamiTurbo {
    string location;
    string manifest;
    Markup::Node document;
    vector<uint8_t> program;
    boolean patched;
  } sufamiTurboA, sufamiTurboB;

  string_vector gameQueue;

  uint64 autoSaveTime;

  uint64 statusTime;
  string statusMessage;
  string statusFrameRate;
};

extern unique_pointer<Program> program;
