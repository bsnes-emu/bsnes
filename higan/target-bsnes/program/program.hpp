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

  //game.cpp
  auto load(string location) -> void;
  auto loadGameBoy(string location) -> void;
  auto save() -> void;
  auto unload() -> void;

  //utility.cpp
  auto initializeVideoDriver() -> void;
  auto initializeAudioDriver() -> void;
  auto initializeInputDriver() -> void;

private:
  struct Context {
    struct Game {
      string location;
      string manifest;
      Markup::Node document;
      vector<uint8_t> program;
      vector<uint8_t> data;
      vector<uint8_t> expansion;
      vector<uint8_t> firmware;
    } game;

    struct GameBoy {
      string location;
      string manifest;
      Markup::Node document;
      vector<uint8_t> program;
    } gameBoy;
  } context;
};

extern unique_pointer<Program> program;
