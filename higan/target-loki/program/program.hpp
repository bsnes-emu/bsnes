struct Program : Emulator::Interface::Bind {
  //program.cpp
  Program(lstring args);
  auto main() -> void;
  auto quit() -> void;

  //media.cpp
  auto loadMedia(string location) -> void;
  auto loadMedia(Emulator::Interface::Media& media, string location) -> void;
  auto unloadMedia() -> void;

  //interface.cpp
  auto loadRequest(uint id, string name, string type, bool required) -> void override;
  auto loadRequest(uint id, string path, bool required) -> void override;
  auto saveRequest(uint id, string path) -> void override;
  auto videoRefresh(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioSample(int16 left, int16 right) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;
  auto dipSettings(const Markup::Node& node) -> uint override;
  auto path(uint group) -> string override;
  auto notify(string text) -> void override;

  vector<shared_pointer<HID::Device>> devices;
  vector<string> mediaPaths;
  vector<string> folderPaths;
};

extern unique_pointer<Program> program;
