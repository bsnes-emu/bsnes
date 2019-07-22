struct Program : Lock, Emulator::Platform {
  Application::Namespace tr{"Program"};

  //program.cpp
  auto create() -> void;
  auto main() -> void;
  auto quit() -> void;

  //platform.cpp
  auto open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto load(uint id, string name, string type, vector<string> options = {}) -> Emulator::Platform::Load override;
  auto videoFrame(const uint16* data, uint pitch, uint width, uint height, uint scale) -> void override;
  auto audioFrame(const float* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;

  //game.cpp
  auto load() -> void;
  auto loadFile(string location) -> vector<uint8_t>;
  auto loadSuperFamicom(string location) -> bool;
  auto loadGameBoy(string location) -> bool;
  auto loadBSMemory(string location) -> bool;
  auto loadSufamiTurboA(string location) -> bool;
  auto loadSufamiTurboB(string location) -> bool;
  auto save() -> void;
  auto reset() -> void;
  auto unload() -> void;
  auto verified() const -> bool;

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
  auto screenshotPath() -> string;

  //states.cpp
  struct State {
    string name;
    uint64_t date;
    static const uint Signature;
  };
  auto availableStates(string type) -> vector<State>;
  auto hasState(string filename) -> bool;
  auto loadStateData(string filename) -> vector<uint8_t>;
  auto loadState(string filename) -> bool;
  auto saveState(string filename) -> bool;
  auto saveUndoState() -> bool;
  auto saveRedoState() -> bool;
  auto removeState(string filename) -> bool;
  auto renameState(string from, string to) -> bool;

  //movies.cpp
  struct Movie {
    enum Mode : uint { Inactive, Playing, Recording } mode = Mode::Inactive;
    serializer state;
    vector<int16> input;
  } movie;
  auto movieMode(Movie::Mode) -> void;
  auto moviePlay() -> void;
  auto movieRecord(bool fromBeginning) -> void;
  auto movieStop() -> void;

  //rewind.cpp
  struct Rewind {
    enum Mode : uint { Playing, Rewinding } mode = Mode::Playing;
    vector<serializer> history;
    uint length = 0;
    uint frequency = 0;
    uint counter = 0;  //in frames
  } rewind;
  auto rewindMode(Rewind::Mode) -> void;
  auto rewindReset() -> void;
  auto rewindRun() -> void;

  //video.cpp
  auto updateVideoDriver(Window parent) -> void;
  auto updateVideoExclusive() -> void;
  auto updateVideoBlocking() -> void;
  auto updateVideoFlush() -> void;
  auto updateVideoFormat() -> void;
  auto updateVideoShader() -> void;
  auto updateVideoPalette() -> void;

  //audio.cpp
  auto updateAudioDriver(Window parent) -> void;
  auto updateAudioExclusive() -> void;
  auto updateAudioDevice() -> void;
  auto updateAudioBlocking() -> void;
  auto updateAudioDynamic() -> void;
  auto updateAudioFrequency() -> void;
  auto updateAudioLatency() -> void;
  auto updateAudioEffects() -> void;

  //input.cpp
  auto updateInputDriver(Window parent) -> void;

  //utility.cpp
  auto showMessage(string text) -> void;
  auto showFrameRate(string text) -> void;
  auto updateStatus() -> void;
  auto captureScreenshot() -> bool;
  auto inactive() -> bool;
  auto focused() -> bool;

  //patch.cpp
  auto appliedPatch() const -> bool;
  auto applyPatchIPS(vector<uint8_t>& data, string location) -> bool;
  auto applyPatchBPS(vector<uint8_t>& data, string location) -> bool;

  //hacks.cpp
  auto hackCompatibility() -> void;
  auto hackPatchMemory(vector<uint8_t>& data) -> void;
  auto hackOverclockSuperFX() -> void;

  //filter.cpp
  auto filterSelect(uint& width, uint& height, uint scale) -> Filter::Render;

  //viewport.cpp
  auto viewportSize(uint& width, uint& height, uint scale) -> void;
  auto viewportRefresh() -> void;

public:
  struct Game {
    explicit operator bool() const { return (bool)location; }

    string option;
    string location;
    string manifest;
    Markup::Node document;
    boolean patched;
    boolean verified;
  };

  struct SuperFamicom : Game {
    string title;
    vector<uint8_t> program;
    vector<uint8_t> data;
    vector<uint8_t> expansion;
    vector<uint8_t> firmware;
  } superFamicom;

  struct GameBoy : Game {
    vector<uint8_t> program;
  } gameBoy;

  struct BSMemory : Game {
    vector<uint8_t> program;
  } bsMemory;

  struct SufamiTurbo : Game {
    vector<uint8_t> program;
  } sufamiTurboA, sufamiTurboB;

  vector<string> gameQueue;

  uint32_t palette[32768];
  uint32_t palettePaused[32768];

  struct Screenshot {
    const uint16* data = nullptr;
    uint pitch  = 0;
    uint width  = 0;
    uint height = 0;
    uint scale  = 0;
  } screenshot;

  bool frameAdvance = false;

  uint64 autoSaveTime;

  uint64 statusTime;
  string statusMessage;
  string statusFrameRate;
};

extern Program program;
