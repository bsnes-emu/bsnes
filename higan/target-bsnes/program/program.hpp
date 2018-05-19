struct Program : Emulator::Platform {
  //program.cpp
  Program(string_vector args);
  auto main() -> void;
  auto quit() -> void;

  auto load(string location) -> void;
  auto save() -> void;
  auto unload() -> void;

  //interface.cpp
  auto open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto load(uint id, string name, string type, string_vector options = {}) -> Emulator::Platform::Load override;
  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioSample(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;

  //utility.cpp
  auto initializeVideoDriver() -> void;
  auto initializeAudioDriver() -> void;
  auto initializeInputDriver() -> void;

private:
  struct Context {
    string gameROM;  //program.rom
    string gameRAM;  //save.ram or upd96050.data.ram
  } context;
};

extern unique_pointer<Program> program;
